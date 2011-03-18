
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

OBJECTS = cmime_body_parse.o cmime_file_parse.o cmime_header_parse.o cmime_parse_822.o cmime_parse_b64.o cmime_parse_qptext.o cmime_parse_subject.o cmime_parse_text.o cmime_parse_tnef.o cmime_parse_uu.o cmime_testsuite.o

TARGET = libcmime.a

.SUFFIXES: .c

.c.o:
	$(CC) -c $(CFLAGS) -I../cfile -I../cxml -I../clog -I../cstring -I../cdict $(INCPATH) -o $@ $<

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



