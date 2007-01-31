#!/bin/sh

# Usage:
# ./make.sh game_asn
# ./make.sh master_asn
#

inputDir=../src/libraries/$1
tmpDir=$inputDir/tmp
generatedDir=$inputDir/generated

mkdir $tmpDir
for file in $inputDir/*.asn; do
    ../bin/asn1c.exe $file -c++ -per -trace -print `basename ${file} .asn`Print.cpp -o $tmpDir -I $inputDir
done

# get different files ignoring header (generation date) and retrieve it has a list of files
escapedTmpDir=`echo "$tmpDir" | sed "s|/|\\\\\\/|g"`
files=`diff -qrN --ignore-file-name-case -x "*.svn" -I "(http://www.obj-sys.com)" $tmpDir $generatedDir | sed -r "s/^.*($escapedTmpDir\\/\w*(\.cpp|\.h)).*$/\\1/"`

if [ ! -z "${files}" ]; then
    echo "Changes found in : " ${files}
    mkdir -p $generatedDir
    cp -r ${files} $generatedDir
fi
rm -rf $tmpDir
