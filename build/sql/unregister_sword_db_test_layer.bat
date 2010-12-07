@echo off
REM set /p x=Entrez la valeur de x:

@echo unregister-sword-db.bat [db] 

SET DB=sword_crossbow_db
IF NOT "%1"=="" SET DB=%1

SET PORT=6789
IF NOT "%2"=="" SET PORT=%2

SET SPACE=sword
SET USR=sword
SET PWD=sword

call set_sword_var.bat

@echo on
@echo When prompt for a password, enter %PWD%

%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.unitforces_test,shape -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.orderparameters_area_test,shape -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.orderparameters_line_test,shape -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.orderparameters_point_test,shape -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N

%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.tacticalobject_area_test,shape -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.tacticalobject_line_test,shape -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N


%COMMAND_PSQL% -f drop-sword-schema-test.sql --username=%USR% --password=%PWD% %DB%

@echo done.

pause