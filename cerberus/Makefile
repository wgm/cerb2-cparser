
#If running by self
ifndef RANLIB
  RANLIB=ranlib
endif
ifndef CC
  CC=gcc
endif
ifndef AR
  AR=ar
endif

OBJECTS = cerberus.o cer_add_sub_files.o cer_check_domain.o cer_curl_error.o cer_curl_init.o cer_curl_write.o cer_del_sub_files.o cer_key_info.o cer_save_input.o cer_strdom.o cer_stremail.o cer_load_config.o cer_parse_files.o

TARGET   = ../build/cerberus-2.${BUILDNUMBER}.${OS}

.SUFFIXES: .c

.c.o:
	$(CC) -c $(CFLAGS) -I../ccrypt -I../cstring -I../cdata -I../cdict -I../cfile -I../clog -I../csocket -I../cpop3 -I../cxml -I../cmime -I../cutest $(INCPATH) -o $@ $<

all: $(TARGET)

$(TARGET):  $(OBJECTS)
	test -d ../build/ || mkdir -p ../build/
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBPATH) -Wl,-Bstatic ../build/libcmime.a ../build/libcxml.a ../build/libcpop3.a ../build/libcsocket.a ../build/libclog.a ../build/libcfile.a ../build/libcdict.a ../build/libcdata.a ../build/libcstring.a ../build/librsa.a ../build/libcutest.a -lexpat $(LIBS)

clean:
	-rm -f $(OBJECTS) 
	-rm -f *~ core *.core
	-rm -f $(TARGET)

