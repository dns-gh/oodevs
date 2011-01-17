@echo off
@echo install-sword-db.bat [db] 

SET DB=sword_crossbow_db
IF NOT "%1"=="" SET DB=%1

call set_sword_var.bat

@echo on
@echo When prompt for a password, enter %PWD%

@echo Create database, copy SDE system tables, generate sword tables
%COMMAND_PSQL% -f %SQL_USER_FILE% --username=%PG_USR% --password=%PG_PWD% %PG_DB%

%COMMAND_CREATE_DB% --template=%TEMPLATE_DB% --username=%USR% --password=%PWD% %DB% 
@echo%COMMAND_CREATE_LANGAGE% --dbname %DB% --username=%USR% --password=%PWD% plpgsql

%COMMAND_PSQL% -f %SQL_SCHEMA_FILE% --username=%USR% --password=%PWD% %DB%
%COMMAND_PSQL% -f %SQL_CONSTRAINT_FILE% --username=%USR% --password=%PWD% %DB%
