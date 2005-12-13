#!/bin/sh

cd asn
mkdir tmp

cd tmp
../../asn1c.exe ../*.asn -c++ -per -trace -print ScipioAsnPrint.cpp
cd ..

# get different files ignoring header (generation date) and retrieve it has a list of files
files=`diff -qr --ignore-file-name-case -x "*.svn" -x "license.dat" -x "asn1c.exe" -I "(http://www.obj-sys.com)" tmp generated | sed -r "s/^.*(tmp\\/\w*(\.cpp|\.h)).*$/\\1/"`

if [ ! -z "${files}" ]; then
    echo "Changes found in : " ${files}
    cp ${files} generated/
fi

rm -rf tmp
