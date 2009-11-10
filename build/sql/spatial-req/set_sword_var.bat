@echo off

SET SERVICE_NAME=esri_sde_%DB%
SET SERVICE_PORT=6161
SET SERVICE_PROTOCOL=tcp

SET COMMAND_PSQL="C:\Program Files\PostgreSQL\8.3\bin\psql.exe"
SET COMMAND_CREATE_DB="C:\Program Files\PostgreSQL\8.3\bin\createdb.exe"
SET COMMAND_CREATE_LANGAGE="C:\Program Files\PostgreSQL\8.3\bin\createlang.exe"
SET COMMAND_DROP_DB="C:\Program Files\PostgreSQL\8.3\bin\dropdb.exe"

SET SDEHOME=C:\Program Files\ArcGIS\ArcSDE\pgexe
SET COMMAND_SDE_LAYER="%SDEHOME%\bin\sdelayer.exe"
SET COMMAND_SDE_SETUP="%SDEHOME%\bin\sdesetup.exe"
SET COMMAND_SDE_SERVC="%SDEHOME%\bin\sdeservice.exe"
SET COMMAND_SRV_START=net start
SET COMMAND_SRV_STOP=net stop
SET COMMAND_SRV_DELETE=sc delete

SET WINDOWS_SERVC_FILE="C:\WINDOWS\system32\drivers\etc\services"
SET ARC_SDE_SERVC_FILE="%SDEHOME%\etc\services.sde"
SET ARC_SDE_LICENSE_FILE="EDN431189245 - ArcGIS Server.ecp"
SET SQL_DB_FILE="create-sword-schema.sql"

SET COORD_SYSTEM=4326
SET XY_TOLERANCE=0.0000008
SET XY_SCALE=1000

@echo sde:postgresql:localhost
@echo esri_sde_%DB%
