#include "daq_device_sis3300.h"

#include "CAEN_V1718TriggerHandler.h"
#include "CAEN_V1718.h"

#include <unistd.h>
#include <string.h>

using namespace std;

daq_device_sis3300::daq_device_sis3300 (const int eventtype
		  , const int subeventid
		  , const int device_nr
		  , const int samples
		  , const int look_back
		  , const int stop_delay
		  , const int trigger)
{

  m_eventType  = eventtype;
  m_subeventid = subeventid;

  _device_nr = device_nr;
  _samples = samples;
  _look_back = look_back;
  _stop_delay = stop_delay;
  _trigger = trigger;

  _broken = 0;
  

  _handle = CAEN_V1718::Handle();

  if (  _handle == -1) 
    {
      _broken =1;
      return;
    }

  _baseaddress = 0x30000000 
                + 0x1000000 * _device_nr;

  cout<< __FILE__ << " " << __LINE__ <<  " base Address is " << hex << _baseaddress << dec << " device nr " << _device_nr << endl;

  sample_cnt =  ( _baseaddress +  SIS3300_BANK1_ADDR_CNT_ADC12 );

  bank[0] = ( _baseaddress + SIS3300_MEMBASE_BANK1_ADC12 );
  bank[1] = ( _baseaddress + SIS3300_MEMBASE_BANK1_ADC34 );
  bank[2] = ( _baseaddress + SIS3300_MEMBASE_BANK1_ADC56 );
  bank[3] = ( _baseaddress + SIS3300_MEMBASE_BANK1_ADC78 );


  if ( _trigger) 
    {
      //  cout << __LINE__ << "  " << __FILE__ << " registering triggerhandler " << endl;
      _th  = new CAEN_V1718TriggerHandler ( _handle);
      registerTriggerHandler ( _th);
    }

}

daq_device_sis3300::~daq_device_sis3300()
{
  if ( _th) 
    {
      clearTriggerHandler();
      delete _th;
      _th = 0;
    }

}



// the put_data function

int daq_device_sis3300::put_data(const int etype, int * adr, const int length )
{
  //  cout << __LINE__ << "  " << __FILE__ << " etype= " << etype << " adr= " << adr << " length= " << length << endl;

  if ( _broken ) 
    {
      cout << __LINE__ << "  " << __FILE__ << " broken ";
      identify();
      return 0; //  we had a catastrophic failure
    }

  int len = 0;

  if (etype != m_eventType )  // not our id
    {
      return 0;
    }

  sevt =  (subevtdata_ptr) adr;
  // set the initial subevent length
  sevt->sub_length =  SEVTHEADERLENGTH;

  // update id's etc
  sevt->sub_id =  m_subeventid;
  sevt->sub_type=4;
  sevt->sub_decoding = 65;  // 65 is "IDSIS3300R"
  sevt->reserved[0] = 0;
  sevt->reserved[1] = 0;

  if ( length < max_length(etype) ) 
    {
      cout << __LINE__ << "  " << __FILE__ << " length " << length <<endl;
      return 0;
    }

  int samplelength = 128*1024;
  unsigned int data;

  int position;

  int *d = (int *) &sevt->data;

  *d++ = _samples;  // set the first data words -> samples;
  len++;

  CAENVME_API ret = CAENVME_ReadCycle (_handle, sample_cnt , &position, cvA32_U_DATA, cvD32);
  if ( ret) cout << __FILE__ << " " << __LINE__ << " return code " << ret << endl;

  int start_adr = (position + samplelength - _look_back) % samplelength;
  int count;
  int i;

  if ( ( start_adr + _samples) < samplelength )
    {  
      
      for ( i = 0; i < 4; i++)
	{
	  unsigned int adr = bank[i];
	  ret = CAENVME_BLTReadCycle( _handle, adr, (void *)d,
				      4*_samples, cvA32_U_MBLT , cvD64, &count);
	
	  //	  cout << __LINE__ << "  " << __FILE__ <<  " got " << count << " values" << endl;

	  // int l1, c1;
	  // for ( l1 = 0; l1 < _samples; l1 +=8 )
	  //   {
	  //     cout << setw(6) << l1 << " | " ;
	  //     for ( c1 = 0; c1 < 8; c1++ )
	  // 	{
	  // 	  cout << hex << setw(10) << d[l1+c1] << dec;
	  // 	}
	  //     cout << endl;
	  //   }
	  // cout << " -------------------- " << endl;

	  d+= _samples;
	  len += _samples;
	}
    }
  else
    {
      int n = samplelength - start_adr; // so many samples until high end
      for ( i = 0; i < 4; i++)
	{
	  unsigned int adr = bank[i] + start_adr*4;
	  ret = CAENVME_BLTReadCycle( _handle, adr, (void *)d,
				      4*n, cvA32_U_MBLT , cvD64, &count);
	  
	  d+= n;
	  len += n;
	  
	  adr = bank[i];
	  ret = CAENVME_BLTReadCycle( _handle, adr, (void *)d,
				      4*(_samples-n), cvA32_U_MBLT , cvD64, &count);
	  
	  d+= (_samples -n);
	  len += (_samples -n);	  
	  
	}
    }


  sevt->sub_padding = len%2;
  len = len + (len%2);
  sevt->sub_length += len;
  //cout << __LINE__ << "  " << __FILE__ << " returning "  << sevt->sub_length << endl;
  return  sevt->sub_length;
}


void daq_device_sis3300::identify(std::ostream& os) const
{
  unsigned int x1;
  uint32_t address = _baseaddress + SIS3300_ID;
  cout <<  __FILE__ << " " << __LINE__ << " Address is " << hex << address << dec << endl;

  
  CAENVME_API ret = CAENVME_ReadCycle (_handle, address , &x1, cvA32_U_DATA, cvD32);
  if ( ret) cout << __FILE__ << " " << __LINE__ << " return code " << ret << endl;

  int id = (x1 >> 16 ) &0xffff;
  int revision = x1  &0xffff;

  os  << "Device SIS" << hex << id << dec << " Rev. " << revision << " Event Type: " << m_eventType << " Subevent id: " << m_subeventid 
      << " VME id " << _device_nr 
      << " samples " << _samples 
      << " look_back " << _look_back 
      << " stop_delay " << _stop_delay; 
  if ( _th) os << " trigger ";
  if ( _broken) os << " ** not functional ** ";
  os << endl;
  
}

int daq_device_sis3300::max_length(const int etype) const
{
  if (etype != m_eventType) return 0;
  return  (_samples*8 + 1 + SEVTHEADERLENGTH + 10);
}

int  daq_device_sis3300::init()
{

  set_3300_value (SIS3300_RESET , 1);  //reset the thing

  set_3300_value (SIS3300_ACQUISITION_CONTROL , 0xffff0000);  // clear everything
  set_3300_value (SIS3300_ACQUISITION_CONTROL , 0x180 );  // enable front panel stop + stop delay

  return 0;

}
int  daq_device_sis3300::endrun()
{

  return 0;
}

// the rearm() function
int  daq_device_sis3300::rearm(const int etype)
{
  if (etype != m_eventType) return 0;

  set_3300_value (SIS3300_KEY_BANK1_FULL_FLAG , 1);  // enable memory bank 1

  // enable memory bank 1; enable stop mode
  set_3300_value (SIS3300_ACQUISITION_CONTROL , 0x181);

  // set the stop delay, if any
  set_3300_value (SIS3300_STOP_DELAY , _stop_delay);

  // set bit 3, wrap around mode
  set_3300_value (SIS3300_EVENT_CONFIG_ALL_ADC, 0x8);

  // and start sampling, we use the common stop.
  set_3300_value (SIS3300_KEY_START, 1);
  return 0;
}



int daq_device_sis3300::set_3300_value( const int what, const int value)
{
  unsigned int d = ( _baseaddress  + what );
  int x = value;

  CAENVME_API ret = CAENVME_WriteCycle (_handle, d , &x, cvA32_U_DATA, cvD32);
  if ( ret) cout << __FILE__ << " " << __LINE__ << " return code " << ret << endl;

}
