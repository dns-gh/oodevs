@echo off
net use \\file1 /USER:<USER> <PASSWORD> 1>NUL
set FILE=sword_cloud_vc100_x64_setup.exe
set BIN=sword_cloud_vc100_x64_bin.zip
set HOME=c:\Users\Admin
copy /B /V /Y "%HOME%\%FILE%" "%HOME%\%FILE%.backup" 1>NUL
copy /B /V /Y "%HOME%\%BIN%"  "%HOME%\%BIN%.backup" 1>NUL
copy /B /V /Y "\\file1\Masa\common\dev\delivery\internal\cloud\current\%FILE%" "%HOME%" 1>NUL
copy /B /V /Y "\\file1\Masa\common\dev\delivery\internal\cloud\current\%BIN%"  "%HOME%" 1>NUL
net stop "sword cloud"
"%HOME%\%FILE%" /S /SKIP_SERVICE_CONTROL /D=%HOME%\sword_cloud
copy /B /V /Y "%HOME%\cloud_server.config" "%HOME%\sword_cloud\bin"
copy /B /V /Y "%HOME%\license.lic" "%HOME%\sword_cloud\bin"
net start "sword cloud"
exit
