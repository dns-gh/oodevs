@echo off
echo running ant web every second...
pushd %~dp0
:loop
call ant web web.test.build
ping -n 2 127.0.0.1 >NUL
goto :loop
pause
