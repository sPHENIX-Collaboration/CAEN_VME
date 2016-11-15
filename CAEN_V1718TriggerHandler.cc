
#include "CAEN_V1718TriggerHandler.h"
#include <iostream>
#include <CAENVMElib.h>

using namespace std;


CAEN_V1718TriggerHandler::CAEN_V1718TriggerHandler(int handle)
{
  _go = 0;
  _handle = handle;
  _reset_on_next = 0;
  _trigger_register = 0;
  _old_trigger_register = 0;
  
  CAENVME_API ret;

  // set up inputs 0 and 1 
  ret = CAENVME_SetInputConf(_handle, cvInput0 , cvDirect, cvActiveHigh);
  ret = CAENVME_SetInputConf(_handle, cvInput1 , cvDirect, cvActiveHigh);

  // output 0 is the busy latch reset
  ret = CAENVME_SetOutputConf ( _handle, cvOutput0, cvDirect, cvActiveLow, cvManualSW );
  ret = CAENVME_SetOutputConf ( _handle, cvOutput1, cvDirect, cvActiveLow, cvManualSW );
  ret = CAENVME_SetOutputConf ( _handle, cvOutput2, cvDirect, cvActiveHigh, cvManualSW );
  ret = CAENVME_SetOutputConf ( _handle, cvOutput3, cvDirect, cvActiveHigh, cvManualSW );
  ret = CAENVME_SetOutputConf ( _handle, cvOutput4, cvDirect, cvActiveHigh, cvManualSW );

}


int CAEN_V1718TriggerHandler::wait_for_trigger( const int moreinfo)
{

  CAENVME_API ret;
  //  CAENVME_API ret = CAENVME_ReadRegister(_handle,cvInputReg, &olddata);

  while ( _go )
    {
      if ( _reset_on_next )
      	{
      	  ret = CAENVME_PulseOutputRegister(_handle, (cvOut0Bit | cvOut1Bit) );
      	  _reset_on_next = 0;
      	  //    	  usleep (30);
      	}
  
      ret = CAENVME_ReadRegister(_handle,cvInputReg, &_trigger_register);
      //      cout << __FILE__ << "  " << __LINE__ << "  " << data <<  "  " << olddata << endl; 

      if (_trigger_register != _old_trigger_register) 
      	{
	  //cout << hex << _trigger_register << endl;
	  if ( _old_trigger_register == 0 && _trigger_register == 6 )  // I have no idea why, but input 0 yields this value
	    {
	      static int c = 0;
	      //	      cout << __FILE__ << "  " << __LINE__ << " data trigger " << _old_trigger_register << "  " <<  _trigger_register << "  trig nr " << c++ << endl; 
	      _old_trigger_register =  _trigger_register;
	      return 1;
	    }
	  if ( _old_trigger_register == 0 && _trigger_register == 5 )  // input 1 yields this value
	    {
	      //cout << __FILE__ << "  " << __LINE__ << " Spill-off trigger " << data << endl; 
	      _old_trigger_register =  _trigger_register;
	      return 16;   // 16 is a spill-off
	    }
	  _old_trigger_register =  _trigger_register;
	}
    }
  return 0;
}

int CAEN_V1718TriggerHandler::rearm()
{
  //  cout << __FILE__ << "  " << __LINE__ << " rearming " << endl; 
  _reset_on_next = 1;
  //  CAENVME_API ret = CAENVME_PulseOutputRegister(_handle, (cvOut0Bit | cvOut1Bit) );
  return 0;
}
