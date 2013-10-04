#include "daq_device_V1718pulse.h"

#include "CAEN_V1718TriggerHandler.h"
#include "CAEN_V1718.h"

#include <unistd.h>
#include <string.h>

using namespace std;

daq_device_V1718pulse::daq_device_V1718pulse (const int eventtype
		  , const int subeventid
		  , const int pattern)
{

  m_eventType  = eventtype;
  m_subeventid = subeventid;

  _pattern - pattern;
  _broken = 0;
  

  _handle = CAEN_V1718::Handle();

  if (  _handle == -1) 
    {
      _broken =1;
      return;
    }

}

daq_device_V1718pulse::~daq_device_V1718pulse()
{
}



// the put_data function

int daq_device_V1718pulse::put_data(const int etype, int * adr, const int length )
{

  if ( _broken ) 
    {
      cout << __LINE__ << "  " << __FILE__ << " broken ";
      identify();
      return 0; //  we had a catastrophic failure
    }

  if (etype != m_eventType )  // not our id
    {
      return 0;
    }

  CAENVME_API ret = CAENVME_PulseOutputRegister(_handle, (cvOut0Bit | cvOut1Bit) );
  return  0;

}


void daq_device_V1718pulse::identify(std::ostream& os) const
{

  os  << "Device V1718Pulse" << " Event Type: " << m_eventType << " Subevent id: " << m_subeventid 
      << " pattern " << _pattern;
  if ( _broken) os << " ** not functional ** ";
  os << endl;
  
}


int  daq_device_V1718pulse::init()
{

  CAENVME_API ret;


  // output 0 is the busy latch reset
  ret = CAENVME_SetOutputConf ( _handle, cvOutput0, cvDirect, cvActiveLow, cvManualSW );
  ret = CAENVME_SetOutputConf ( _handle, cvOutput1, cvDirect, cvActiveLow, cvManualSW );
  ret = CAENVME_SetOutputConf ( _handle, cvOutput2, cvDirect, cvActiveHigh, cvManualSW );
  ret = CAENVME_SetOutputConf ( _handle, cvOutput3, cvDirect, cvActiveHigh, cvManualSW );
  ret = CAENVME_SetOutputConf ( _handle, cvOutput4, cvDirect, cvActiveHigh, cvManualSW );

  return 0;

}

