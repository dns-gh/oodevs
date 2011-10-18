@echo off

regedit /S "%~dp0\Scipio-Sword_post_install.reg"

if not exist "d:\MASA\SWORDROOT" mkdir "d:\MASA\SWORDROOT"

if exist "%~dp0\vcredist_vc100.exe" goto x86
if exist "%~dp0\vcredist_vc100_x64.exe" goto x64
goto end

:x86
"%~dp0\vcredist_vc100.exe" /S /NCRC

:x64
"%~dp0\vcredist_vc100_x64.exe" /S /NCRC

%~dp0../applications/launcher_app.exe -i 

:end
