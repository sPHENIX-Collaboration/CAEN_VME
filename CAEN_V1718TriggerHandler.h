#ifndef __CAEN_V1718TRIGGERHANDLER_H__
#define __CAEN_V1718TRIGGERHANDLER_H__


#include "TriggerHandler.h"


class CAEN_V1718TriggerHandler : public TriggerHandler {

public:

  CAEN_V1718TriggerHandler( int handle);

  int rearm();
  int enable()  { _go = 1;};
  int disable() { _go = 0;};

  ~CAEN_V1718TriggerHandler() {};

  // this is the virtual worker routine
  int wait_for_trigger( const int moreinfo=0);

 protected:
  int _handle;

  int _go;
  int _reset_on_next;

};

#endif
