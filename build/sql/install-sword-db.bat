@echo off
@echo install-sword-db.bat [db] 

SET DB=sword_crossbow_db
IF NOT "%1"=="" SET DB=%1

call set_sword_var.bat

@echo on

@echo Create sword user, database, generate sword tables
%COMMAND_PSQL% -f %SQL_USER_FILE% --username=%PG_USR% %PG_DB%

%COMMAND_CREATE_DB% --template=%TEMPLATE_DB% --username=%PG_USR% %DB% 
@echo%COMMAND_CREATE_LANGAGE% --dbname %DB% --username=%PG_USR% plpgsql

%COMMAND_PSQL% -f %SQL_SCHEMA_FILE% --username=%PG_USR% %DB%
%COMMAND_PSQL% -f %SQL_CONSTRAINT_FILE% --username=%PG_USR% %DB%
