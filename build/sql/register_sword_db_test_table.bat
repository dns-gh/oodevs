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

%COMMAND_SDE_TABLE% -o register -t %DB%.%SPACE%.orders_test -i %SERVICE_NAME% -u %USR% -p %PWD% -D %DB%


pause