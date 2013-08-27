#include "CAEN_V1718.h"
#include <CAENVMElib.h>
#include <iostream>

using namespace std;

CAEN_V1718 *CAEN_V1718::_instance = 0;

int CAEN_V1718::Handle( const int index)
{
  if ( ! _instance)
    {
      _instance = new CAEN_V1718();
    }
  return _instance->getHandle();
}

int CAEN_V1718::Finish( const int index)
{

  if ( ! _instance) return 0;

  delete _instance;
  return 0;
}




CAEN_V1718::CAEN_V1718()
{
  int handle;
  CAENVME_API ret = CAENVME_Init( cvV1718,
				  0, 
				  0, 
				  &handle );
  
  if (ret) 
    {
      cout << __FILE__ << " " << __LINE__ <<  CAENVME_DecodeError (ret) << endl;
      _handle = -1;
      return;
    }

  cout << __FILE__ << " " << __LINE__ << " Sucessfully initialized the V1718, handle = " << handle << endl;

  _handle = handle; 

}

CAEN_V1718::~CAEN_V1718()
{

  if (_handle)   CAENVME_End(_handle);
  _handle = 0;
}

int CAEN_V1718::VMEReset( const int index)
{
  if (_handle)   
    {
      CAENVME_API ret = CAENVME_SystemReset( _handle);
      if (ret) 
	{
	  cout << __FILE__ << " " << __LINE__ <<  CAENVME_DecodeError (ret) << endl;
	}
      return -1;
    }
  return 0;
}

