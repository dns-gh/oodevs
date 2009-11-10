@echo off
REM set /p x=Entrez la valeur de x:

@echo uninstall-sword-db.bat [db] 

SET DB=sword_crossbow_db
IF NOT "%1"=="" SET DB=%1

SET PORT=5151
IF NOT "%2"=="" SET PORT=%2

SET SPACE=sword
SET USR=sword
SET PWD=sword

call set_sword_var.bat

@echo on
@echo When prompt for a password, enter %PWD%

%COMMAND_SRV_STOP% %SERVICE_NAME%
REM TODO: %COMMAND_SDE_SERVC% -o unregister -r ADMIN_DATABASE -v %DB% -p %PWD% -d POSTGRESQL,%PGSERVICE% -i %SERVICE_NAME%
%COMMAND_SDE_SERVC% -o delete -d POSTGRESQL,%PGSERVICE% -i %SERVICE_NAME%
%COMMAND_SRV_DELETE% %SERVICE_NAME%
%COMMAND_DROP_DB% --username=%USR% --password=%PWD% %DB%

@echo
@echo 
@echo ----------------------------------------------
@echo You should manually delete services from files 
@echo - %WINDOWS_SERVC_FILE% 
@echo - %ARC_SDE_SERVC_FILE%
