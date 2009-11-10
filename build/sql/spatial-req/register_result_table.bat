@echo off
REM set /p x=Entrez la valeur de x:

@echo register-sword-db.bat [db] 

SET DB=sword_bruxelles_db
IF NOT "%1"=="" SET DB=%1

SET SPACE=sword
SET USR=sword
SET PWD=sword

call set_sword_var.bat

@echo on
@echo When prompt for a password, enter %PWD%


SET SERVICE_NAME=esri_sde_sword_bruxelles_db_4
%COMMAND_SDE_LAYER% -o register -l %DB%.%SPACE%.population_group_1,shape -C id,SDE -e s -t ST_GEOMETRY -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%
