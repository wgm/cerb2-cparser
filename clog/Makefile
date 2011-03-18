
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

OBJECTS = clog.o clog_close.o clog_fileclose.o clog_fileopen.o clog_getlevel.o clog_open.o clog_perror.o clog_setcallback.o clog_setcallbacklevel.o clog_setfile.o clog_setfilelevel.o clog_stderr.o clog_stdout.o

TARGET = libclog.a

.SUFFIXES: .c

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

all: ../build/$(TARGET)

../build/$(TARGET):	$(OBJECTS)
	test -d ../build/ || mkdir -p ../build/
	-rm -f $(TARGET)
	$(AR) cq $(TARGET) $(OBJECTS)
	-$(RANLIB) $(TARGET)
	-rm -f ../build/$(TARGET)
	-mv $(TARGET) ../build/

test: main.o ../build/$(TARGET)
	$(CC) -o ../build/test_clog main.o ../build/$(TARGET)

clean:
	-rm -f $(OBJECTS)
	-rm -f main.o
	-rm -f ../build/test_clog
	-rm -f *~ core *.core
	-rm -f ../build/$(TARGET)

