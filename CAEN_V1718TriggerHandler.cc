
#include "CAEN_V1718TriggerHandler.h"
#include <iostream>
#include <CAENVMElib.h>

using namespace std;


CAEN_V1718TriggerHandler::CAEN_V1718TriggerHandler(int handle)
{
  _go = 0;
  _handle = handle;

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
  unsigned int olddata =0 ;
  unsigned int data;

  //  CAENVME_API ret = CAENVME_ReadRegister(_handle,cvInputReg, &olddata);

  while ( _go )
    {
      CAENVME_API ret = CAENVME_ReadRegister(_handle,cvInputReg, &data);

      if (data != olddata) 
      	{
	  //  cout << hex << data << endl;
	  if ( olddata == 0 && data != 0 )
	    {
	      //	      cout << __FILE__ << "  " << __LINE__ << " trigger " << endl; 
	      return 0;
	    }
	  olddata = data;
	}
    }
  return 1;
}

int CAEN_V1718TriggerHandler::rearm()
{
  //  cout << __FILE__ << "  " << __LINE__ << " rearming " << endl; 
  CAENVME_API ret = CAENVME_PulseOutputRegister(_handle, (cvOut0Bit) );
  return 0;
}
