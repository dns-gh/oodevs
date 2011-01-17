@echo off

SET SPACE=sword
SET USR=sword
SET PWD=sword

SET PG_USR=postgres
SET PG_PWD=postgres
SET PG_DB=postgres
SET TEMPLATE_DB=template_postgis

SET PGHOME=D:\postgres\bin
SET COMMAND_PSQL="%PGHOME%\psql.exe"
SET COMMAND_CREATE_DB="%PGHOME%\createdb.exe"
SET COMMAND_CREATE_LANGAGE="%PGHOME%\createlang.exe"
SET COMMAND_DROP_DB="%PGHOME%\dropdb.exe"

SET SQL_SCHEMA_FILE="create-sword-schema.sql"
SET SQL_CONSTRAINT_FILE="create-sword-constraints.sql"
SET SQL_USER_FILE="create-sword-user.sql"

