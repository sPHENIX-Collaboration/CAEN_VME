#include "CAEN_VME_plugin.h"

#include "CAEN_V1718.h"

#include <daq_device_sis3300.h>

#include <strings.h>
#include <iostream>

using namespace std;

CAEN_VME_plugin::CAEN_VME_plugin()
{
  _broken = 0;

  int handle = CAEN_V1718::Handle();
  cout << __FILE__ << " " << __LINE__ <<  " handle = " << handle << endl;

  if ( handle == -1)
    {  
      cout << __FILE__ << " " << __LINE__ << "error initializing the V1718"  << endl;
      _broken = 1;
    }
  else
    {
      cout << __FILE__ << " " << __LINE__ << "established connection to the V1718"  << endl;
    }
}

CAEN_VME_plugin::~CAEN_VME_plugin()
{
  cout << __FILE__ << " " << __LINE__ << " ending access to the VME1718" << endl;
  CAEN_V1718::Finish();
}


int CAEN_VME_plugin::create_device(deviceblock *db)
{

  if (_broken)
    {
      cout << __LINE__ << "  " << __FILE__ << "  " << "defunct plugin "  << std::endl;
      return -1;
    }

  if ( strcasecmp(db->argv0,"device_sis3300") == 0 ) 
    {

      // params:
      // 1 - event type
      // 2 - packet id
      // 3 - base address
      // 4 - number of channels to read
      // 5 - how many channels back
      // 6 - additional trigger delay
      // 7 - (optional) we have the trigger

      // we need at least 6 params
      if ( db->npar < 6 ) return 1; // indicate wrong params
      
      int eventtype  = atoi ( db->argv1); // event type
      int subid = atoi ( db->argv2); // subevent id
      if ( db->npar == 6 )
	{
	  daq_device *x = new daq_device_sis3300( eventtype,
						  subid,
						  atoi (db->argv3),
						  atoi (db->argv4),
						  atoi (db->argv5),
						  atoi (db->argv6));

	  add_readoutdevice (x);
	  return 0;  // say "we handled this request" 
	}  
      else
	{
	  daq_device *x = new daq_device_sis3300( eventtype,
						  subid,
						  atoi (db->argv3),
						  atoi (db->argv4),
						  atoi (db->argv5),
						  atoi (db->argv6),
						  atoi (db->argv7));

	  add_readoutdevice (x);
	  return 0;  // say "we handled this request" 
	}	  
    } 
  return -1; // say " this is not our device"
}

void CAEN_VME_plugin::identify(std::ostream& os, const int flag) const
{

  if ( flag <=2 )
    {
      os << " - CAEN VME1718 Plugin";
      if (_broken) os << " **** defunct ****";
      os<< std::endl;
    }
  else
    {
      os << " - CAEN VME1718 Plugin";
      if (_broken) os << " **** defunct ****";

      os << ", provides - " << std::endl;
      os << " -     device_sis3300 (evttype, subid, device nr, samples, look_back, trigger_delay [, trigger]) - readout a SIS 3300 Flash ADC " << std::endl;

    }
      
}

CAEN_VME_plugin _sp;

