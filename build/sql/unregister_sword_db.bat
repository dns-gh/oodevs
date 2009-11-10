@echo off
REM set /p x=Entrez la valeur de x:

@echo unregister-sword-db.bat [db] 

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

%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.unitforces,shape -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.emergencies,shape  -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.knowledgeunits,shape  -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N

%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.objectknowledges_area,shape  -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.objectknowledges_line,shape  -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.objectknowledges_point,shape  -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N


%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.orderparameters_area,shape  -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.orderparameters_line,shape  -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.orderparameters_point,shape -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N

%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.boundarylimits,shape  -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.tacticallines,shape   -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N

%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.create_tacticalobject_area,shape  -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.create_tacticalobject_line,shape  -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.create_tacticalobject_point,shape -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N

%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.tacticalobject_area,shape  -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.tacticalobject_line,shape  -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.tacticalobject_point,shape -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N

%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.population,shape -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_LAYER% -o delete -l %DB%.%SPACE%.terrain_grid,shape -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N

%COMMAND_SDE_TABLE% -o unregister -t %DB%.%SPACE%.exercises -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N
%COMMAND_SDE_TABLE% -o unregister -t %DB%.%SPACE%.sessions -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB% -N

@echo done.
