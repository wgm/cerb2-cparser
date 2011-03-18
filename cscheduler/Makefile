
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

OBJECTS = cscheduler.o 

TARGET = libcscheduler.a

.SUFFIXES: .c

.c.o:
	$(CC) -c $(CFLAGS) -I../cxml -I../cdict -I../cstring -I../clog -I../cfile -I../cdata $(INCPATH) -o $@ $<

all: Makefile ../build/$(TARGET) 

../build/$(TARGET):  $(OBJECTS)
	test -d ../build/ || mkdir -p ../build/
	-rm -f $(TARGET)
	$(AR) cq $(TARGET) $(OBJECTS) 
	-$(RANLIB) $(TARGET)
	-rm -f ../build/$(TARGET)
	-mv $(TARGET) ../build/


clean:
	-rm -f $(OBJECTS) 
	-rm -f *~ core *.core
	-rm -f ../build/$(TARGET)



