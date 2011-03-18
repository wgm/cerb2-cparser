
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

OBJECTS = cfile.o cfile_buffer.o cfile_cleanup.o cfile_close.o cfile_free.o cfile_getline.o cfile_getline_buffer.o cfile_getline_next.o cfile_init.o cfile_lseek.o cfile_mkstemp.o cfile_new.o cfile_open.o cfile_open_create.o cfile_pipe.o cfile_read.o cfile_sync.o cfile_testsuite.o cfile_unlink.o cfile_write.o

TARGET = libcfile.a

.SUFFIXES: .c

.c.o:
	$(CC) -c $(CFLAGS) -I../cdict -I../cstring $(INCPATH) -o $@ $<

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



