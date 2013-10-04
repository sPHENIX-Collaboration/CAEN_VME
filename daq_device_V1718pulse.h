#ifndef __DAQ_DEVICE_V1718PULSE_H__
#define __DAQ_DEVICE_V1718PULSE_H__

#include <daq_device.h>
#include <CAENVMElib.h>

#include <iostream>

class daq_device_V1718pulse : public  daq_device {

public:

  daq_device_V1718pulse (const int eventtype
		  , const int subeventid
		  , const int pattern);

  ~daq_device_V1718pulse();


  void identify(std::ostream& os = std::cout) const;

  int max_length(const int etype) const { return 0;};

  // functions to do the work

  int put_data(const int etype, int * adr, const int length);

  int init();

protected:
  subevtdata_ptr sevt;

  int _pattern;
  int _handle;
  int _broken;

};


#endif

