#ifndef __DAQ_DEVICE_SRS_H__
#define __DAQ_DEVICE_SRS_H__

#include <daq_device.h>
#include <CAENVMElib.h>

#include <iostream>

class TriggerHandler;


class daq_device_sis3300 : public  daq_device {

public:

  daq_device_sis3300 (const int eventtype
		  , const int subeventid
		  , const int device_nr
		  , const int samples
		  , const int look_back
		  , const int stop_delay
		  , const int trigger = 0);

  ~daq_device_sis3300();


  void identify(std::ostream& os = std::cout) const;

  int max_length(const int etype) const;

  // functions to do the work

  int put_data(const int etype, int * adr, const int length);

  int init();
  int endrun();

  int rearm( const int etype);


protected:
  subevtdata_ptr sevt;
  unsigned int number_of_words;

  virtual int set_3300_value( const int what, const int value);

  int _device_nr;
  int _samples;
  int _look_back;
  int _stop_delay;
  int _trigger;
  int _broken;

  int _handle;
  uint32_t _baseaddress;

  int bank[4];
  int sample_cnt;

  int has_been_overridden;

  TriggerHandler *_th;

};

#define __MAX_SAMPLES (8*1024)


#define SIS3300_CONTROL_STATUS            0x00
#define SIS3300_ID                        0x04
#define SIS3300_IRQ_CONFIG                0x08
#define SIS3300_IRQ_CONTROL               0x0c
#define SIS3300_ACQUISITION_CONTROL       0x10
#define SIS3300_START_DELAY               0x14
#define SIS3300_STOP_DELAY                0x18
#define SIS3300_RESET                     0x20
#define SIS3300_KEY_START                 0x30
#define SIS3300_KEY_STOP                  0x34

#define SIS3300_KEY_BANK1_FULL_FLAG       0x48
#define SIS3300_KEY_BANK2_FULL_FLAG       0x4C

#define SIS3300_EVENT_CONFIG_ALL_ADC      0x100000

#define SIS3300_ACTUAL_SAMPLE_VALUE_ADC12 0x200018
#define SIS3300_ACTUAL_SAMPLE_VALUE_ADC34 0x280018
#define SIS3300_ACTUAL_SAMPLE_VALUE_ADC56 0x300018
#define SIS3300_ACTUAL_SAMPLE_VALUE_ADC78 0x380018

#define SIS3300_BANK1_ADDR_CNT_ADC12       0x200008
#define SIS3300_BANK1_ADDR_CNT_ADC34       0x280008
#define SIS3300_BANK1_ADDR_CNT_ADC56       0x300008
#define SIS3300_BANK1_ADDR_CNT_ADC78       0x380008

#define SIS3300_MEMBASE_BANK1_ADC12       0x400000
#define SIS3300_MEMBASE_BANK1_ADC34       0x480000
#define SIS3300_MEMBASE_BANK1_ADC56       0x500000
#define SIS3300_MEMBASE_BANK1_ADC78       0x580000

#define SIS3300_MEMBASE_BANK2_ADC12       0x600000
#define SIS3300_MEMBASE_BANK2_ADC34       0x680000
#define SIS3300_MEMBASE_BANK2_ADC56       0x700000
#define SIS3300_MEMBASE_BANK2_ADC78       0x780000

#endif

