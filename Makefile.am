AUTOMAKE_OPTIONS = foreign

allsources = \
	CAEN_V1718TriggerHandler.cc \
	daq_device_sis3300.cc \
	CAEN_VME_plugin.cc \
	CAEN_V1718.cc 


AM_LDFLAGS = -L$(libdir) -L$(ONLINE_MAIN)/lib


INCLUDES = -I$(includedir) -I$(ONLINE_MAIN)/include 


lib_LTLIBRARIES =  librcdaqplugin_caen_vme.la 

librcdaqplugin_caen_vme_la_SOURCES = \
	CAEN_V1718TriggerHandler.cc \
	daq_device_sis3300.cc \
	CAEN_VME_plugin.cc \
	CAEN_V1718.cc 


librcdaqplugin_caen_vme_la_LIBADD =  -lrcdaqutils -lCAENVME


clean-local:
	rm -f $(BUILT_SOURCES)
