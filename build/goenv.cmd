@echo off
for /F %%i in ("%~dp0..") do set ROOT=%%~fi
set PATH=%ROOT%/bin/golang_amd64/bin;%PATH%
set GOPATH=%ROOT%/lib/vc100_x64;%ROOT%
set GOROOT=%ROOT%/bin/golang_amd64
