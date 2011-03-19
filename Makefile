#Makefile for Cerberus by [BGH] of WGM

### CONFIGURATION AREA ###

BUILDNUMBER = 649
OS-NAME = $(shell uname -s)
OS-VER = $(shell uname -r)
OS = ${OS-NAME}-${OS-VER}

##----------------------------------
# Ubuntu/Debian 
##----------------------------------
#LIBS = -Wl,-Bdynami -ldl -lc
#CFLAGS = -O -Wall -pipe -DBUILDNUMBER=\"${BUILDNUMBER}\"
#INCPATH = -I/usr/local/include -I../opt/include -I../cutest/
#LIBPATH = -L../opt/lib -L/usr/local/lib
#LDFLAGS =
#LD = gcc
#RANLIB = ranlib

##LIBS = -lmemwatch -lefence
##LIBS = -lefence -Wl,-Bdynami -ldl -lc
##LIBS = -Wl,-Bdynami -lc -licuuc
##LIBS = -Wl,-Bdynami -lc
##CFLAGS = -g3 -Wall -pipe -DBUILDNUMBER=\"${BUILDNUMBER}\"
##CFLAGS = -Wall -pipe -DBUILDNUMBER=\"${BUILDNUMBER}\"
##CFLAGS = -g3 -pg -Wall -DMEMWATCH=1 -DBUILDNUMBER=\"${BUILDNUMBER}\"
##CFLAGS = -g3 -pipe -Wall -DBUILDNUMBER=\"${BUILDNUMBER}\"

##----------------------------------
# BSD RH9
##----------------------------------
#INCPATH = -I/usr/local/include -I../opt/include -I../cutest/
#LIBPATH = -L../opt/lib -L/usr/local/lib 
#LIBS = -lmemwatch -lefence
#LIBS = -lefence -Wl,-Bdynami -ldl -lc
#LIBS = -Wl,-Bdynami -ldl -lc
#LIBS = -Wl,-Bdynami -lc -licuuc
#LIBS = -Wl,-Bdynami -lc 
#CFLAGS = -O -Wall -pipe -DBUILDNUMBER=\"${BUILDNUMBER}\"
#CFLAGS = -g3 -Wall -pipe -DBUILDNUMBER=\"${BUILDNUMBER}\"
#CFLAGS = -Wall -pipe -DBUILDNUMBER=\"${BUILDNUMBER}\"
#CFLAGS = -g3 -pg -Wall -DMEMWATCH=1 -DBUILDNUMBER=\"${BUILDNUMBER}\"
#CFLAGS = -g3 -pipe -Wall -DBUILDNUMBER=\"${BUILDNUMBER}\"
#LDFLAGS = 
#LD = gcc
#RANLIB = ranlib

##----------------------------------
# RH6
##----------------------------------
#INCPATH = -I/usr/local/include -I/home/bhalsted/opt/include
#LIBPATH = -L/usr/local/lib
#LIBS = -lc
#CFLAGS = -O -Wall -DBUILDNUMBER=\"${BUILDNUMBER}\"
#LDFLAGS = --export-dynamic
#LD = gcc
#RANLIB = ranlib

##LIBS = -lmemwatch -lefence
##LIBS = -lefence
##CFLAGS = -g3 -pg -DMEMWATCH=1 -DBUILDNUMBER=\"${BUILDNUMBER}\"
##CFLAGS = -g3 -DBUILDNUMBER=\"${BUILDNUMBER}\"
##LDFLAGS = -static -S

##----------------------------------
# OSX
##----------------------------------
#INCPATH = -I/usr/include -I/opt/local/include -I../cutest
#LIBPATH = -L/opt/local/lib
#LIBS = -lc
#CFLAGS = -O -Wall -DBUILDNUMBER=\"${BUILDNUMBER}\" -DMAC=1
#LD = gcc
#RANLIB = ranlib

##CFLAGS = -g3 -Wall -DBUILDNUMBER=\"${BUILDNUMBER}\" -DMAC=1
##LDFLAGS = -static
##LDFLAGS = --export-dynamic

##----------------------------------
# Win32
##----------------------------------
#OS=Win32
#INCPATH = -I/usr/include -I../opt/include -I../cutest
#LIBPATH = -L/usr/lib -L../opt/lib
#LIBS = -lcurldll -lws2_32
#CFLAGS = -O -DBUILDNUMBER=\"${BUILDNUMBER}\" -DWIN32=1
#LDFLAGS = -mwindows 
#LD = gcc
#RANLIB = /mingw/bin/ranlib.exe

##CFLAGS = -g3 -DBUILDNUMBER=\"${BUILDNUMBER}\" -DWIN32=1
##LDFLAGS = -static -S
##LDFLAGS = 

##----------------------------------
# Solaris 2.8
##----------------------------------
#CC = gcc
#INCPATH = -I/export/home/benhal/opt/include
#LIBPATH = -L/export/home/benhal/opt/lib
#LIBS = -lc -ldl -lsocket -lnsl -lz
#CFLAGS = -O -DBUILDNUMBER=\"${BUILDNUMBER}\" -DSOLARIS
#LDFLAGS = 
#LD = gcc
#RANLIB = /usr/ccs/bin/ranlib

##CFLAGS = -g3 -pg -DBUILDNUMBER=\"${BUILDNUMBER}\"
##LDFLAGS = -static -S

### END CONFIGURATION AREA ###

export BUILDNUMBER CC INCPATH LIBPATH CFLAGS LDFLAGS LIBS LD OS RANLIB ;

objects = \
	sub-ccrypt \
	sub-cdata \
	sub-cdict \
	sub-cfile \
	sub-clog \
	sub-cmime \
	sub-cpop3 \
	sub-cscheduler \
	sub-csocket \
	sub-cstring \
	sub-cutest \
	sub-cxml \
	sub-cerberus

all: $(objects)

test: 
	rm -f cerberus/*.o
	rm -f ./build/cerberus-2.${BUILDNUMBER}.${OS}
	$(MAKE) all
	./build/cerberus-2.${BUILDNUMBER}.${OS}

stats: sub-cdata sub-cdict sub-cfile sub-clog sub-cstring sub-cxml

package: all
	cp build/cerberus-2.${BUILDNUMBER}.${OS} .
	strip build/cerberus-2.${BUILDNUMBER}.${OS}
	cp extras/config.xml config.xml.txt
	cp extras/curl-ca-bundle.crt .
	tar cvf cerberus-2.${BUILDNUMBER}.${OS}.tar cerberus-2.${BUILDNUMBER}.${OS} curl-ca-bundle.crt config.xml.txt libcurl.so.3
	gzip --best cerberus-2.${BUILDNUMBER}.${OS}.tar
	scp cerberus-2.${BUILDNUMBER}.${OS}.tar.gz bhalsted@xev.webgroupmedia.com:

packages:
	

sub-ccrypt: 
	cd ccrypt && $(MAKE)

sub-cdata: 
	cd cdata && $(MAKE)

sub-cdict:
	cd cdict && $(MAKE)

sub-cerberus: 
	cd cerberus && $(MAKE)

sub-cfile: 
	cd cfile && $(MAKE)

sub-clog: 
	cd clog && $(MAKE)

sub-cmime: 
	cd cmime && $(MAKE)

sub-cpop3: 
	cd cpop3 && $(MAKE)

sub-cscheduler:
	cd cscheduler && $(MAKE)

sub-csocket: 
	cd csocket && $(MAKE)

sub-cstring: 
	cd cstring && $(MAKE)

sub-cxml: 
	cd cxml && $(MAKE)

sub-cutest: 
	cd cutest && $(MAKE)

changelog:
	cvs2cl --distributed -I ChangeLog --no-wrap -S
	cvs2cl -I ChangeLog --no-wrap -S

install:
	install build/libcdata.a build/libcdict.a build/libcfile.a build/libclog.a build/libcpop3.a build/libcsocket.a build/libcstring.a build/libcxml.a /usr/local/lib
	install cfile/cfile.h cpop3/cpop3.h cxml/cxml.h cdata/cdata.h clog/clog.h csocket/csocket.h cdict/cdict.h cstring/cstring.h /usr/local/include


clean: FORCE
	( [ -d ccrypt ] && cd ccrypt ; $(MAKE) $@; ) || true
	( [ -d cdata ] && cd cdata ; $(MAKE) $@; ) || true
	( [ -d cdict ] && cd cdict ; $(MAKE) $@; ) || true
	( [ -d cerberus ] && cd cerberus ; $(MAKE) $@; ) || true
	( [ -d cfile ] && cd cfile ; $(MAKE) $@; ) || true
	( [ -d clog ] && cd clog ; $(MAKE) $@; ) || true
	( [ -d cmime ] && cd cmime ; $(MAKE) $@; ) || true
	( [ -d cpop3 ] && cd cpop3 ; $(MAKE) $@; ) || true
	( [ -d cscheduler ] && cd cscheduler ; $(MAKE) $@; ) || true
	( [ -d csocket ] && cd csocket ; $(MAKE) $@; ) || true
	( [ -d cstring ] && cd cstring ; $(MAKE) $@; ) || true
	( [ -d cutest ] && cd cutest ; $(MAKE) $@; ) || true
	( [ -d cxml ] && cd cxml ; $(MAKE) $@; ) || true

FORCE:

.PHONY : $(objects)

