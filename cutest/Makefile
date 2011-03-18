
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

OBJECTS = CuTest.o CuTestTest.o

TARGET = libcutest.a

.SUFFIXES: .c

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

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



