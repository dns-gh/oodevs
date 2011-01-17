@echo off
REM set /p x=Entrez la valeur de x:

@echo uninstall-sword-db.bat [db] 

SET DB=sword_crossbow_db
IF NOT "%1"=="" SET DB=%1

call set_sword_var.bat

@echo on
@echo When prompt for a password, enter %PWD%
%COMMAND_DROP_DB% --username=%USR% --password=%PWD% %DB%

