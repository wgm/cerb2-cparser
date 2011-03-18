#!/bin/sh

echo "Copying current directory to ../package"
cp -R . ../package
echo "Going to ../package directory"
cd ../package

date=$(date '+%G%m%d')

echo "Removing CVS directories"
find . -type d -name ".svn" -exec rm -rf {} \;

for project in cdata cdict cfile clog cpop3 csocket cstring cxml
do
	tar cfz ${project}_${date}.tar.gz $project/
	mv ${project}_${date}.tar.gz ../
done

cd ../

 rm -rf package

exit 0

