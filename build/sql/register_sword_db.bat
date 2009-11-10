@echo off
REM set /p x=Entrez la valeur de x:

@echo register-sword-db.bat [db] 

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

%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.unitforces,shape -C id,SDE -e p -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.emergencies,shape -C id,SDE -e a -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.knowledgeunits,shape -C id,SDE -e a -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%

%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.orderparameters_area,shape -C id,SDE -e a -x 0,0,%XY_SCALE%,%XY_TOLERANCE% -G %COORD_SYSTEM% -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.orderparameters_line,shape -C id,SDE -e s -x 0,0,1000,%XY_TOLERANCE% -G %COORD_SYSTEM% -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.orderparameters_point,shape -C id,SDE -e p -x 0,0,1000,%XY_TOLERANCE% -G %COORD_SYSTEM% -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%

%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.boundarylimits,shape -C id,SDE -e s -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.tacticallines,shape -C id,SDE -e s -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%

%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.create_tacticalobject_area,shape -C id,SDE -e a -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.create_tacticalobject_line,shape -C id,SDE -e s -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.create_tacticalobject_point,shape -C id,SDE -e p -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%

%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.tacticalobject_area,shape -C id,SDE -e a -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.tacticalobject_line,shape -C id,SDE -e s -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.tacticalobject_point,shape -C id,SDE -e p -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%

%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.population,shape -C id,SDE -e s -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.terrain_grid,shape -C id,SDE -e a -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%


%COMMAND_SDE_TABLE% -o register -t %DB%.%SPACE%.exercises -c id -C USER -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
%COMMAND_SDE_TABLE% -o register -t %DB%.%SPACE%.sessions -c id -C USER -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%

%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.objectknowledges_area,shape -C public_oid,USER -e a -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.objectknowledges_line,shape -C public_oid,USER -e s -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.objectknowledges_point,shape -C public_oid,USER -e p -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%


