#!/bin/sh

gcc -DMAC=1 -o ../build/cerberus-2.592.Darwin-7.2.0 cerberus.o -L/Users/bhalsted/opt/lib -L/opt/apache2/lib/ ../build/libcmime.a ../build/libcxml.a ../build/libcpop3.a ../build/libcsocket.a ../build/libclog.a ../build/libcfile.a ../build/libcdict.a ../build/libcdata.a ../build/libcstring.a ../build/librsa.a /Users/bhalsted/opt/lib/libexpat.a  -lc

exit 0
