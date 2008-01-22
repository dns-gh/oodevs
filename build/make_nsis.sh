#!/bin/zsh

C:/WINDOWS/Microsoft.NET/Framework/v2.0.50727/RegAsm.exe ../out/applications/crossbow_app/vc80/Release/Sword.Crossbow.dll /codebase /regfile:SwordCrossbow.reg
version=`cat SwordCrossbow.reg | grep "Version=" | tail -n 1 | cut -d , -f 2 | cut -d = -f 2`
echo $version
cat nsis/crossbow.nsi.template | sed s/\"\$\$VERSION\$\$\"/\"$version\"/ > nsis/crossbow.nsi
rm SwordCrossbow.reg