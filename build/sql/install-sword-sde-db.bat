@echo off
@echo install-sword-db.bat [db] 

SET DB=sword_crossbow_db
IF NOT "%1"=="" SET DB=%1

SET PORT=6789
IF NOT "%2"=="" SET PORT=%2

SET SPACE=sde
SET USR=sde
SET PWD=sde

call set_sword_var.bat

@echo on
@echo When prompt for a password, enter %PWD%
@echo Documentation for SDE commands: file:///C:/Program Files/ArcGIS/ArcSDE/Documentation/Admin_Cmd_Ref/Support_files/admincmdref.htm

@echo %COMMAND_SDE_SERVC%

@echo Create database, copy SDE system tables, generate sword tables
%COMMAND_CREATE_DB% --username=%USR% --password=%PWD% %DB%
%COMMAND_SDE_SETUP% -o install -d POSTGRESQL -u %USR% -p %PWD% -D %DB% -l %ARC_SDE_LICENSE_FILE%

@echo %COMMAND_CREATE_LANGAGE% --dbname %DB% --username=%USR% --password=%PWD% plpgsql

%COMMAND_PSQL% -f %SQL_USER_FILE% --username=%USR% --password=%PWD% %DB%
%COMMAND_PSQL% -f %SQL_SCHEMA_FILE% --username=%USR% --password=%PWD% %DB%
%COMMAND_PSQL% -f %SQL_CONSTRAINT_FILE% --username=%USR% --password=%PWD% %DB%

@echo Create SDE service
%COMMAND_SDE_SERVC% -o create -p %PWD% -d POSTGRESQL,%PGSERVICE% -i %SERVICE_NAME%
%COMMAND_SDE_SERVC% -o register -r ADMIN_DATABASE -v %DB% -u %USR% -p %PWD% -d POSTGRESQL,%PGSERVICE% -i %SERVICE_NAME%
echo.%SERVICE_NAME%  %SERVICE_PORT%/%SERVICE_PROTOCOL%  #ArcSDE Service for Sword Database:%DB% >> %WINDOWS_SERVC_FILE%
echo.%SERVICE_NAME%  %SERVICE_PORT%/%SERVICE_PROTOCOL%  #ArcSDE Service for Sword Database:%DB% >> %ARC_SDE_SERVC_FILE%
%COMMAND_SRV_START% %SERVICE_NAME%
