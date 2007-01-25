#!/bin/sh

# Usage:
# ./make.sh master
# ./make.sh simulation
#

cd $1
mkdir tmp

for file in *.asn; do
../asn1c.exe $file -c++ -per -trace -print `basename ${file} .asn`Print.cpp -o tmp;
done

# get different files ignoring header (generation date) and retrieve it has a list of files
files=`diff -qrN --ignore-file-name-case -x "*.svn" -I "(http://www.obj-sys.com)" tmp generated | sed -r "s/^.*(tmp\\/\w*(\.cpp|\.h)).*$/\\1/"`

if [ ! -z "${files}" ]; then
    echo "Changes found in : " ${files}
    cp ${files} generated/
fi
rm -rf tmp
cd ..