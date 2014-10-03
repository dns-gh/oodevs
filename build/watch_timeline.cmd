@echo off
echo running ant timeline.web every second...
pushd %~dp0
:loop
call ant timeline.web timeline.web.test.build
ping -n 2 127.0.0.1 >NUL
goto :loop
pause
