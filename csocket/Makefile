
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

OBJECTS = csocket_close.o csocket_connect.o csocket_free.o csocket_gethostbyname.o csocket_get_iplist.o csocket_new.o csocket_read.o csocket_set_hptosa.o csocket_set_port.o csocket_set_timeout.o csocket_set_type.o csocket_socket.o csocket_winsock_start.o csocket_winsock_stop.o csocket_write.o

TARGET = libcsocket.a

.SUFFIXES: .c

.c.o:
	$(CC) -c $(CFLAGS) -I../clog $(INCPATH) -o $@ $<

all: Makefile ../build/$(TARGET) 

../build/$(TARGET):  $(OBJECTS)
	test -d ../build/ || mkdir -p ../build/
	-rm -f $(TARGET)
	$(AR) cq $(TARGET) $(OBJECTS) 
	-$(RANLIB) $(TARGET)
	-rm -f ../build/$(TARGET)
	-mv $(TARGET) ../build/

test: ../build/$(TARGET) main.o
	$(CC) -o ../build/test_csocket main.o ../build/$(TARGET) ../build/libcstring.a ../build/libclog.a

clean:
	-rm -f $(OBJECTS) 
	-rm -f main.o ../build/test_csocket
	-rm -f *~ core *.core
	-rm -f ../build/$(TARGET)



