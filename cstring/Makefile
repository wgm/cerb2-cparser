
#If running by self
ifndef RANLIB
  RANLIB=ranlib
endif
ifndef INCPATH
  INCPATH=-I../cutest
endif
ifndef CC
  CC=gcc
endif
ifndef AR
  AR=ar
endif

OBJECTS = cstring_free.o cstring_import.o cstring_ltrim.o cstring_new.o cstring_rtrim.o cstring_strcat.o cstring_strcat_imp.o cstring_strchr.o cstring_strcmp.o cstring_strdup.o cstring_stricmp.o cstring_strlen.o cstring_strlower.o cstring_strncat.o cstring_strncmp.o cstring_strndup.o cstring_strnicmp.o cstring_strrchr.o cstring_strstr.o cstring_strupper.o cstring_trim.o cstring_testsuite.o

TARGET = libcstring.a

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

test: ../build/$(TARGET) main.o
	$(CC) -o ../build/test_cstring main.o ../build/$(TARGET)

clean:
	-rm -f $(OBJECTS) 
	-rm -f main.o ../build/test_cstring
	-rm -f *~ core *.core
	-rm -f ../build/$(TARGET)



