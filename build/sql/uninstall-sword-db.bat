@echo off
REM set /p x=Entrez la valeur de x:

@echo uninstall-sword-db.bat [db] 

SET DB=sword_crossbow_db
IF NOT "%1"=="" SET DB=%1

call set_sword_var.bat

@echo on
%COMMAND_DROP_DB% --username=%PG_USR%  %DB%
%COMMAND_PSQL% -f %SQL_DROP_USER_FILE% --username=%PG_USR% %PG_DB%
