#!/bin/sh

if [ $# -lt 1 ]; then
    echo "Usage:" `basename $0` "input-directory [output-directory]"
    exit
fi

inputDir=$1
outputDir=$inputDir

if [ $# -gt 1 ]; then
    outputDir=$2
    mkdir -p $outputDir
fi

for file in $inputDir/*.xml; do
    xsltFile=`basename ${file} .xml`_old_to_new.xslt
    if [ -e "${xsltFile}" ]; then
        echo "Processing '"`basename ${file}`"' using '${xsltFile}'"
        xsltproc -o ${outputDir}/`basename ${file}` ${xsltFile} ${file}
    fi
done

