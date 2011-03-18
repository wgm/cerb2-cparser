
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

OBJECTS = cpop3_connect.o cpop3_dele.o cpop3_free.o cpop3_disconnect.o cpop3_init.o cpop3_iserror.o cpop3_nextline.o cpop3_pass.o cpop3_quit.o cpop3_retr.o cpop3_stat.o cpop3_user.o

TARGET = libcpop3.a

.SUFFIXES: .c

.c.o:
	$(CC) -c $(CFLAGS) -I../cdict -I../cdata -I../cfile -I../clog -I../csocket -I../cstring $(INCPATH) -o $@ $<

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



