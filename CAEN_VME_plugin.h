#ifndef __CAEN_VME_PLUGIN_H__
#define __CAEN_VME_PLUGIN_H__

#include <rcdaq_plugin.h>

#include <daq_device_sis3300.h>
#include <iostream>


class CAEN_VME_plugin : public RCDAQPlugin {

 public:
  CAEN_VME_plugin();
  ~CAEN_VME_plugin();

  int  create_device(deviceblock *db);

  void identify(std::ostream& os = std::cout, const int flag=0) const;

 protected:
  int _broken;

};


#endif
