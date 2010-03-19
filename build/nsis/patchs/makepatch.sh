#!/bin/sh

productName=""
installDir="C:/Program Files"
documentDir="C:/Documents and Settings/sbo/Mes Documents"
fromVersion="1.0.2"
toVersion="1.0.3"

patchApp="../../bin/nsis/nsisPatchGen.exe"

${patchApp} --prefix patch_app "${installDir}/${productName}_${fromVersion}" "${installDir}/${productName}_${toVersion}" patch_app patch_app.nsi
${patchApp} --prefix patch_data "${documentDir}/${productName}_${fromVersion}" "${documentDir}/${productName}_${toVersion}" patch_data patch_data.nsi

function prepare_patch {
    tempFile="$1.tmp"
    # add macro definition
    echo "!ifndef NSISPGFile" > ${tempFile}
    echo "!define NSISPGFile" >> ${tempFile}
    echo "!macro NSISPGFile inputFile" >> ${tempFile}
    echo "   !insertmacro UNINSTALL.LOG_OPEN_INSTALL" >> ${tempFile}
    echo "   File \"\${inputFile}\"" >> ${tempFile}
    echo "   !insertmacro UNINSTALL.LOG_CLOSE_INSTALL" >> ${tempFile}
    echo "!macroend" >> ${tempFile}
    echo "!endif" >> ${tempFile}
    # remove Uninstall.exe and Uninstall.dat
    # change "File" calls to "!insertmacro NSISPGFile"
    cat $1 | grep -vE "Uninstall\.(dat|exe)" | sed -r "s/^(\s+)File \"\\$/\\1\!insertmacro NSISPGFile \"\\$/" >> ${tempFile}
    mv -f "${tempFile}" "$1"
}

prepare_patch patch_app.nsi
prepare_patch patch_data.nsi
