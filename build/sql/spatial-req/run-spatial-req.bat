@echo off
@echo install-sword-db.bat [db] 

SET SCRIPT=nada!
SET DB=sword_bruxelles_db

IF NOT "%1"=="" SET SCRIPT=%1

SET SPACE=sde
SET USR=sde
SET PWD=sde

call set_sword_var.bat

%COMMAND_PSQL% -f %SCRIPT% --username=%USR% --password=%PWD% %DB%
