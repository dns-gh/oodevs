CREATE SCHEMA sword;

CREATE TYPE simulation_state AS ENUM ( 'stopped', 'initializing', 'running', 'paused' );
CREATE TABLE sword.actionparameters

CREATE TABLE sword.activities
(
  id          INTEGER PRIMARY KEY,
  "name"      CHARACTER VARYING(255),
  session_id  INTEGER
);
ALTER TABLE sword.activities OWNER TO sword;
GRANT ALL ON TABLE sword.activities TO sword;

CREATE TABLE sword.boundarylimits
(
  id SERIAL   PRIMARY KEY,
  public_oid  INTEGER NOT NULL,
  symbol_id   CHARACTER VARYING(15) NOT NULL,
  "name"      CHARACTER VARYING(255),
  lastupdate  TIMESTAMP WITH TIME ZONE,
  session_id  INTEGER
 );
 SELECT AddGeometryColumn('sword', 'boundarylimits', 'shape', 4326, 'GEOMETRY', 2);

ALTER TABLE sword.boundarylimits OWNER TO sword;
GRANT ALL ON TABLE sword.boundarylimits TO sword;

CREATE TABLE sword.boundarylimits_creation
(
  id serial   PRIMARY KEY,
  public_oid  INTEGER NOT NULL,
  symbol_id   CHARACTER VARYING(15) NOT NULL,
  "name"      CHARACTER VARYING(255),
  "location"  CHARACTER VARYING(20),
  direction   DOUBLE PRECISION DEFAULT 0,
  "type"      CHARACTER VARYING(20),
  session_id  INTEGER
);
SELECT AddGeometryColumn('sword', 'boundarylimits_creation', 'shape', 4326, 'GEOMETRY', 2);

ALTER TABLE sword.boundarylimits_creation OWNER TO sword;
GRANT ALL ON TABLE sword.boundarylimits_creation TO sword;

CREATE TABLE sword.clients
(
  id      INTEGER PRIMARY KEY,
  "name"  CHARACTER VARYING(50)
);
ALTER TABLE sword.clients OWNER TO sword;
GRANT ALL ON TABLE sword.clients TO sword;

CREATE TABLE sword.create_object
(
  id                         SERIAL NOT NULL PRIMARY KEY,
  data                       XML,
  "date"      TIMESTAMP WITHOUT TIME ZONE,
  checked_xbow         INTEGER DEFAULT 0,
  checked_sim         CREATE_STATE,
  session_id            INTEGER NOT NULL
);
ALTER TABLE sword.create_object OWNER TO sword;
GRANT ALL ON TABLE sword.create_object TO sword;

CREATE TABLE sword.create_order
(
  id                             SERIAL NOT NULL PRIMARY KEY,
  data                       XML,
  "date"     TIMESTAMP WITHOUT TIME ZONE,
  checked_xbow         INTEGER DEFAULT 0,
  checked_sim         CREATE_STATE,
  session_id            INTEGER NOT NULL
);
ALTER TABLE sword.create_order OWNER TO sword;
GRANT ALL ON TABLE sword.create_order TO sword;

CREATE TABLE sword.exercises
(
  id          SERIAL PRIMARY KEY,
  "name"      CHARACTER VARYING(50),
  description CHARACTER VARYING(255),
  extent_xmin double precision,
  extent_ymin double precision,
  extent_xmax double precision,
  extent_ymax double precision  
);
ALTER TABLE sword.exercises OWNER TO sword;
GRANT ALL ON TABLE sword.exercises TO sword;

CREATE TABLE sword.formations
(
  id          SERIAL PRIMARY KEY,
  public_oid  INTEGER NOT NULL,
  parent_oid  INTEGER NOT NULL,
  "type"      INTEGER,
  engaged     SMALLINT,
  "name"      CHARACTER VARYING(255),
  symbol_id   CHARACTER(15),
  session_id  INTEGER
);

ALTER TABLE sword.formations OWNER TO sword;
GRANT ALL ON TABLE sword.formations TO sword;

CREATE TABLE sword.objects
(
  id            SERIAL PRIMARY KEY,
  "name"        CHARACTER VARYING(100),
  "type"        CHARACTER VARYING(100),
  symbol_id     CHARACTER VARYING(15),
  public_oid    INTEGER NOT NULL,
  state         INTEGER DEFAULT 0,
  lastupdate      TIMESTAMP WITH TIME ZONE,
  session_id    INTEGER
);          
SELECT AddGeometryColumn('sword', 'objects', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.objects OWNER TO sword;
GRANT ALL ON TABLE sword.objects TO sword;


CREATE TABLE sword.objectparameters
(
  id            SERIAL PRIMARY KEY,
  type          VARCHAR(255),
  name          VARCHAR(255),
  value         VARCHAR(255),
  object_id          INTEGER NOT NULL,
  parameter_id    INTEGER
);
SELECT AddGeometryColumn('sword', 'objectparameters', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.objectparameters OWNER TO sword;
GRANT ALL ON TABLE sword.objectparameters TO sword;

CREATE TABLE sword.knowledgeobjects
(
  id                    SERIAL PRIMARY KEY,                         
  public_oid            INTEGER NOT NULL,                         
  symbol_id             CHARACTER VARYING(15),
  session_id            INTEGER,                          
  "name"                CHARACTER VARYING(100),                         
  "type"                CHARACTER VARYING(100),                         
  state                 INTEGER DEFAULT 0,                          
  team_id               INTEGER DEFAULT (-1),
  lastupdate 						TIMESTAMP WITH TIME ZONE,                         
  observer_affiliation  CHARACTER VARYING(255)
);
SELECT AddGeometryColumn('sword', 'knowledgeobjects', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.knowledgeobjects OWNER TO sword;
GRANT ALL ON TABLE sword.knowledgeobjects TO sword;

CREATE TABLE sword.knowledgeunits
(
  id                    SERIAL PRIMARY KEY,
  symbol_id             CHARACTER VARYING(15),
  public_oid            INTEGER NOT NULL,
  group_oid             INTEGER,
  unit_oid              INTEGER,
  speed                 INTEGER,
  "type"                INTEGER,
  dead                  INTEGER DEFAULT (-1),
  "name"                CHARACTER VARYING(255),
  parent                CHARACTER VARYING(255),
  effective             CHARACTER VARYING(255),
  quantity              CHARACTER VARYING(255),
  signature             CHARACTER VARYING(255),
  direction             REAL DEFAULT 0,
  identification_level  INTEGER,
  observer_affiliation  CHARACTER VARYING(255),
  observer_oid          INTEGER,
  session_id            INTEGER,
  lastupdate            TIMESTAMP WITH TIME ZONE 
);    
SELECT AddGeometryColumn('sword', 'knowledgeunits', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.knowledgeunits OWNER TO sword;
GRANT ALL ON TABLE sword.knowledgeunits TO sword;

CREATE TABLE sword.layers
(
  id serial NOT NULL PRIMARY KEY,
  exercise_id integer NOT NULL,
  "level" integer NOT NULL DEFAULT 0,
  "type" character varying(50),
  "name" character varying(50),
  url character varying(500)
);
ALTER TABLE sword.layers OWNER TO sword;
GRANT ALL ON TABLE sword.layers TO sword;

CREATE TABLE sword.layers_properties
(
  id integer NOT NULL,
  property character varying(50),
  "value" character varying(500)
);
ALTER TABLE sword.layers_properties OWNER TO sword;
GRANT ALL ON TABLE sword.layers_properties TO sword;

CREATE TABLE sword.phase_lines
(
  id            INTEGER PRIMARY KEY,
  parameter_id  INTEGER NOT NULL,
  "type"        CHARACTER VARYING(6)
);
SELECT AddGeometryColumn('sword', 'phase_lines', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.phase_lines OWNER TO sword;
GRANT ALL ON TABLE sword.phase_lines TO sword;

CREATE TABLE sword.profiles
(
  id          INTEGER PRIMARY KEY,
  "name"      CHARACTER VARYING(255),
  session_id  INTEGER
);
ALTER TABLE sword.profiles OWNER TO sword;
GRANT ALL ON TABLE sword.profiles TO sword;

CREATE TABLE sword.reports
(
  id          SERIAL PRIMARY KEY,
  message     CHARACTER VARYING(255),
  unit_id     INTEGER,
  session_id  INTEGER,
  date        TIMESTAMP WITHOUT TIME ZONE
);
ALTER TABLE sword.reports OWNER TO sword;
GRANT ALL ON TABLE sword.reports TO sword;

CREATE TABLE sword.sessions
(
  id          SERIAL PRIMARY KEY,
  "name"      CHARACTER VARYING(255),
  status      SIMULATION_STATE,
  exercise_id INTEGER
);
ALTER TABLE sword.sessions OWNER TO sword;
GRANT ALL ON TABLE sword.sessions TO sword;  

CREATE TABLE sword.simulationclock
(
  id          SERIAL PRIMARY KEY ,
  clock       CHARACTER VARYING(15),
  session_id  INTEGER
);
ALTER TABLE sword.simulationclock OWNER TO sword;
GRANT ALL ON TABLE sword.simulationclock TO sword;  

CREATE TABLE sword.simulationproperties
(
  id          SERIAL PRIMARY KEY,
  property    CHARACTER VARYING(25),
  "value"     CHARACTER VARYING(25),
  checked     INTEGER DEFAULT (-1),
  session_id  INTEGER
);
ALTER TABLE sword.simulationproperties OWNER TO sword;
GRANT ALL ON TABLE sword.simulationproperties TO sword;

CREATE TABLE sword.tacticallines
(
  id          SERIAL PRIMARY KEY,
  public_oid  INTEGER NOT NULL,
  symbol_id   CHARACTER VARYING(15),
  "name"      CHARACTER VARYING(255),
  session_id  INTEGER
);
SELECT AddGeometryColumn('sword', 'tacticallines', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.tacticallines OWNER TO sword;
GRANT ALL ON TABLE sword.tacticallines TO sword;

CREATE TABLE sword.teams(
  id          SERIAL PRIMARY KEY,
  public_oid  INTEGER NOT NULL,
  "type"      INTEGER,
  "name"      CHARACTER VARYING(255),
  session_id  INTEGER
);
ALTER TABLE sword.teams OWNER TO sword;
GRANT ALL ON TABLE sword.teams TO sword;

CREATE TABLE sword.unitforces
(
  id          SERIAL PRIMARY KEY,
  public_oid  INTEGER NOT NULL,
  parent_oid  INTEGER,
  "type"      INTEGER,
  "name"      CHARACTER VARYING(15),
  symbol_id   CHARACTER(15),
  "valid"     SMALLINT,
  op_state    SMALLINT,
  direction   REAL DEFAULT 0,
  speed       INTEGER,
  session_id  INTEGER
);
SELECT AddGeometryColumn('sword', 'unitforces', 'shape', 4326, 'GEOMETRY', 2); 
ALTER TABLE sword.unitforces OWNER TO sword;
GRANT ALL ON TABLE sword.unitforces TO sword;

CREATE TABLE sword.user_exercises
(
  user_id     INTEGER,
  exercise_id INTEGER
);
ALTER TABLE sword.user_exercises OWNER TO sword;
GRANT ALL ON TABLE sword.user_exercises TO sword;

CREATE TABLE sword.user_sessions
(
  user_id     INTEGER,
  session_id  INTEGER
);
ALTER TABLE sword.user_sessions OWNER TO sword;
GRANT ALL ON TABLE sword.user_sessions TO sword;

CREATE TABLE sword.users
(
  id          INTEGER PRIMARY KEY,
  "name"      CHARACTER VARYING(50),
  "password"  CHARACTER VARYING(100),
  mail        CHARACTER VARYING(100),
  crypto      CHARACTER VARYING(100),
  "role"      CHARACTER VARYING(10),
  client_id   INTEGER
);
ALTER TABLE sword.users OWNER TO sword;
GRANT ALL ON TABLE sword.users TO sword;


CREATE TABLE sword.urban_blocks
(
  id                      SERIAL NOT NULL PRIMARY KEY,
  public_oid              INTEGER NOT NULL,
  parent_oid              INTEGER,
  session_id              INTEGER NOT NULL,
  "name"                  CHARACTER VARYING(50),
  structure_state         INTEGER,
  archi_height            REAL,
  archi_floor_number      INTEGER, 
  archi_roof_shape        CHARACTER VARYING(50),
  archi_material          CHARACTER VARYING(50),
  archi_occupation        REAL,
  archi_trafficability    REAL,
  archi_parking_available INTEGER,  
  infra_type              CHARACTER VARYING(50),
  infra_active            INTEGER,
  infra_threshold         REAL,
  usage_role              CHARACTER VARYING(50),
  usage_percentage        INTEGER,
  color                   CHARACTER VARYING(25) -- ( r, g, b, a )
);
SELECT AddGeometryColumn('sword', 'urban_blocks', 'shape', 4326, 'GEOMETRY', 2); 
ALTER TABLE sword.urban_blocks OWNER TO sword;
GRANT ALL ON TABLE sword.urban_blocks TO sword;

CREATE TYPE link_type AS ENUM ( 'urban', 'object' );

CREATE TABLE sword.resource_network_link
(
    network_id    INTEGER NOT NULL,
    target_oid    INTEGER NOT NULL,
    target_type   link_type,
    capacity      INTEGER,
    flow          INTEGER
);
ALTER TABLE sword.resource_network_link OWNER TO sword;
GRANT ALL ON TABLE sword.resource_network_link TO sword;

CREATE TABLE sword.resource_network
(
  id                SERIAL NOT NULL PRIMARY KEY,
  object_id         INTEGER,
  session_id        INTEGER NOT NULL,
  resource_type     CHARACTER VARYING(50),
  enabled           INTEGER,
  intial_stock      INTEGER,
  max_stock         INTEGER,
  stock             INTEGER,
  production        INTEGER,
  max_production    INTEGER,
  consumption       INTEGER,
  max_consumption   INTEGER,
  critical          INTEGER
);
ALTER TABLE sword.resource_network OWNER TO sword;
GRANT ALL ON TABLE sword.resource_network TO sword;

CREATE TABLE sword.inhabitants
(
  id                      SERIAL NOT NULL PRIMARY KEY,
  public_oid              INTEGER NOT NULL,
  session_id              INTEGER NOT NULL,
  "name"                  CHARACTER VARYING(50),
  team_id                 INTEGER NOT NULL
);
ALTER TABLE sword.inhabitants OWNER TO sword;
GRANT ALL ON TABLE sword.inhabitants TO sword;

CREATE OR REPLACE VIEW sword.inhabitants_by_team AS 
 SELECT teams.session_id AS session_id, teams.id AS team_id, teams.name AS team_name, inhabitants.name AS name
   FROM sword.inhabitants
   RIGHT JOIN sword.teams ON inhabitants.team_id = teams.public_oid AND inhabitants.session_id = teams.session_id
  ORDER BY teams.name, inhabitants.name;

ALTER TABLE sword.inhabitants_by_team OWNER TO sword;
GRANT ALL ON TABLE sword.inhabitants_by_team TO sword;

CREATE TABLE sword.inhabitants_urban_blocks_occupation
(
  inhabitant_id           INTEGER NOT NULL,
  urban_block_id          INTEGER NOT NULL,
  session_id              INTEGER NOT NULL,
  alerted                 INTEGER NOT NULL,
  confined                INTEGER NOT NULL,
  evacuated               INTEGER NOT NULL,
  angriness               REAL NOT NULL
);
ALTER TABLE sword.inhabitants_urban_blocks_occupation OWNER TO sword;
GRANT ALL ON TABLE sword.inhabitants_urban_blocks_occupation TO sword;

CREATE OR REPLACE VIEW sword.urbanblocks_inhabitants AS 
 SELECT inhabitants_urban_blocks_occupation.urban_block_id AS urban_block_id, inhabitants_urban_blocks_occupation.session_id AS session_id, inhabitants.name AS name, inhabitants_urban_blocks_occupation.alerted AS alerted, inhabitants_urban_blocks_occupation.confined AS confined, inhabitants_urban_blocks_occupation.evacuated AS evacuated, inhabitants_urban_blocks_occupation.angriness AS angriness
   FROM sword.inhabitants_urban_blocks_occupation
   JOIN sword.inhabitants ON inhabitants_urban_blocks_occupation.inhabitant_id = inhabitants.public_oid AND inhabitants_urban_blocks_occupation.session_id = inhabitants.session_id
  ORDER BY inhabitants.name;

ALTER TABLE sword.urbanblocks_inhabitants OWNER TO sword;
GRANT ALL ON TABLE sword.urbanblocks_inhabitants TO sword;

