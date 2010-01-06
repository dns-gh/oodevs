@echo off

SET SERVICE_NAME=esri_sde_%DB%
SET SERVICE_PORT=%PORT%
SET SERVICE_PROTOCOL=tcp

SET PGSERVICE=postgresql-8.3
SET PGHOME=C:\Program Files\PostgreSQL\8.3\bin
SET COMMAND_PSQL="%PGHOME%\psql.exe"
SET COMMAND_CREATE_DB="%PGHOME%\createdb.exe"
SET COMMAND_CREATE_LANGAGE="%PGHOME%\createlang.exe"
SET COMMAND_DROP_DB="%PGHOME%\dropdb.exe"

SET SDEHOME=C:\ArcGIS\ArcSDE\pgexe
SET COMMAND_SDE_LAYER="%SDEHOME%\bin\sdelayer.exe"
SET COMMAND_SDE_TABLE="%SDEHOME%\bin\sdetable.exe"
SET COMMAND_SDE_SETUP="%SDEHOME%\bin\sdesetup.exe"
SET COMMAND_SDE_SERVC="%SDEHOME%\bin\sdeservice.exe"
SET COMMAND_SRV_START=net start
SET COMMAND_SRV_STOP=net stop
SET COMMAND_SRV_DELETE=sc delete

SET WINDOWS_SERVC_FILE="C:\WINDOWS\system32\drivers\etc\services"
SET ARC_SDE_SERVC_FILE="%SDEHOME%\etc\services.sde"
SET ARC_SDE_LICENSE_FILE="EDN431307235_ArcGisServer.ecp"
SET SQL_SCHEMA_FILE="create-sword-schema.sql"
SET SQL_CONSTRAINT_FILE="create-sword-constraints.sql"
SET SQL_USER_FILE="create-sword-user.sql"

SET COORD_SYSTEM=4326
SET XY_TOLERANCE=0.0000008
SET XY_SCALE=1000

@echo sde:postgresql:localhost
@echo esri_sde_%DB%
