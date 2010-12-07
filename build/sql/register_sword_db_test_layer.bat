@echo off
REM set /p x=Entrez la valeur de x:

@echo register-sword-db.bat [db] 

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

%COMMAND_PSQL% -f create-sword-schema-test.sql --username=%USR% --password=%PWD% %DB%

%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.unitforces_test,shape -C id,SDE -e p -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%

%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.orderparameters_area_test,shape -C id,SDE -e a -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.orderparameters_line_test,shape -C id,SDE -e s -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.orderparameters_point_test,shape -C id,SDE -e p -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%

%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.tacticalobject_area_test,shape -C id,SDE -e a -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.tacticalobject_line_test,shape -C id,SDE -e s -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%


pause