#!/bin/sh

gcc -o ../build/cerberus-2.576.SunOS-5.8 cerberus.o -L/export/home/benhal/opt/lib ../build/libcmime.a ../build/libcxml.a ../build/libcpop3.a ../build/libcsocket.a ../build/libclog.a ../build/libcfile.a ../build/libcdict.a ../build/libcdata.a ../build/libcstring.a ../build/librsa.a /export/home/benhal/opt/lib/libexpat.a /export/home/benhal/opt/lib/libcurl.a /export/home/benhal/opt/lib/libssl.a /export/home/benhal/opt/lib/libcrypto.a -ldl -lsocket -lnsl -lz

exit 0
