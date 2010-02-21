#!/usr/bin/bash

# set -x

# Usage:
# ./make.sh protocol
#

binDir=../bin
inputDir=../src/libraries/$1
tmpDir=$inputDir/tmp
generatedDir=$inputDir/generated

mkdir -p $tmpDir
${binDir}/protoc -I=${inputDir}/proto --cpp_out=${tmpDir} ${inputDir}/proto/*.proto

# get different files and retrieve it has a list of files
escapedTmpDir=`echo "$tmpDir" | sed "s|/|\\\\\\/|g"`
files=`diff -qrN --ignore-file-name-case -x "*.svn" $tmpDir $generatedDir | sed -r "s/^.*($escapedTmpDir\\/\w*\.pb(\.cc|\.h)).*$/\\1/"`

if [ ! -z "${files}" ]; then
    echo "Changes found in : " ${files}
    mkdir -p $generatedDir
    cp -r ${files} $generatedDir
fi
# rm -rf $tmpDir
