@echo off
echo running ant admin.web every second...
pushd %~dp0
:loop
call ant admin.web
ping -n 2 127.0.0.1 >NUL
goto :loop
pause
