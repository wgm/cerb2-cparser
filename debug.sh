#!/bin/sh

for file in $(ls testemails/*)
do
	echo $file
	echo $file >> memwatch.log
	build/cerberus-2.597.Linux-2.4.21-215-smp4G build/config.test.xml FATAL log.txt < $file
done

exit 0
