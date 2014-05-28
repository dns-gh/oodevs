@echo off
set SWORD=e:/git/sword
.\bin\gotester.exe %* . -- ./src/... -race -test.v -gocheck.v --application "%SWORD%/out/vc100_x64/release/simulation_app.exe" --run-dir "%SWORD%/run/vc100_x64"
