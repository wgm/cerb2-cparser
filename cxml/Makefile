
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

OBJECTS = cxml_fn_data.o cxml_fn_end.o cxml_fn_start.o cxml_node_addattribute.o cxml_node_adddata.o cxml_node_addnode.o cxml_node_create.o cxml_node_delete.o cxml_node_free.o cxml_node_get.o cxml_node_getattribute.o cxml_node_iterate.o cxml_node_new.o cxml_node_next.o cxml_node_tostring.o cxml_read_buf.o cxml_read_file.o cxml_root_free.o cxml_root_new.o cxml_testsuite.o

TARGET = libcxml.a

.SUFFIXES: .c

.c.o:
	$(CC) -c $(CFLAGS) -I../cutest -I../cdict -I../cstring -I../clog -I../cfile -I../cdata $(INCPATH) -o $@ $<

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



