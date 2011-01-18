CREATE SCHEMA sword;

CREATE TYPE simulation_state AS ENUM ( 'stopped', 'initializing', 'running', 'paused' );

CREATE TABLE sword.actionparameters
(
  id            SERIAL PRIMARY KEY,
  type          VARCHAR(255),
  name          VARCHAR(255),
  value         VARCHAR(255),
  reference_id  INTEGER NOT NULL
);
ALTER TABLE sword.actionparameters OWNER TO sword;
GRANT ALL ON TABLE sword.actionparameters TO sword;

CREATE TABLE sword.actionparameters_area
(
  id            SERIAL PRIMARY KEY,
  parameter_id  INTEGER NOT NULL 
);
SELECT AddGeometryColumn('sword', 'actionparameters_area', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.actionparameters_area OWNER TO sword;
GRANT ALL ON TABLE sword.actionparameters_area TO sword;

CREATE TABLE sword.actionparameters_line
(
  id            SERIAL PRIMARY KEY,
  parameter_id  INTEGER NOT NULL
);
SELECT AddGeometryColumn('sword', 'actionparameters_line', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.actionparameters_line OWNER TO sword;
GRANT ALL ON TABLE sword.actionparameters_line TO sword;

CREATE TABLE sword.actionparameters_point
(
  id            SERIAL PRIMARY KEY,
  parameter_id  INTEGER NOT NULL
);
SELECT AddGeometryColumn('sword', 'actionparameters_point', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.actionparameters_point OWNER TO sword;
GRANT ALL ON TABLE sword.actionparameters_point TO sword;

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


CREATE TABLE sword.create_orders
(
  id          SERIAL PRIMARY KEY,
  name        VARCHAR(255),
  type_id     INTEGER NOT NULL,
  unit_id     INTEGER NOT NULL,
  "date"      TIMESTAMP WITHOUT TIME ZONE,
  checked     INTEGER,                         --TODO: set as boolean
  session_id  INTEGER 
);
ALTER TABLE sword.create_orders OWNER TO sword;
GRANT ALL ON TABLE sword.create_orders TO sword;

CREATE TABLE sword.create_objects
(
  id         SERIAL PRIMARY KEY,
  team_id    INTEGER,
  name       VARCHAR(255),
  type       VARCHAR(75),
  "date"     TIMESTAMP WITHOUT TIME ZONE,
  checked    INTEGER,                         --TODO: set as boolean
  session_id INTEGER
);
ALTER TABLE sword.create_objects OWNER TO sword;
GRANT ALL ON TABLE sword.create_objects TO sword;


CREATE TABLE sword.create_tacticalobject_area
(
  id          SERIAL PRIMARY KEY,
  team_id     INTEGER NOT NULL,
  session_id  INTEGER,
  "name"      CHARACTER VARYING(255),
  "type"      CHARACTER VARYING(75)
);
SELECT AddGeometryColumn('sword', 'create_tacticalobject_area', 'shape', 4326, 'GEOMETRY', 2);

ALTER TABLE sword.create_tacticalobject_area OWNER TO sword;
GRANT ALL ON TABLE sword.create_tacticalobject_area TO sword;

CREATE TABLE sword.create_tacticalobject_line
(
  id          SERIAL PRIMARY KEY,
  team_id     INTEGER NOT NULL,
  session_id  INTEGER,
  "name"      CHARACTER VARYING(255),
  "type"      CHARACTER VARYING(75) 
);
SELECT AddGeometryColumn('sword', 'create_tacticalobject_line', 'shape', 4326, 'GEOMETRY', 2);

ALTER TABLE sword.create_tacticalobject_line OWNER TO sword;
GRANT ALL ON TABLE sword.create_tacticalobject_line TO sword;

CREATE TABLE sword.create_tacticalobject_point
(
  id          SERIAL PRIMARY KEY,
  team_id     INTEGER NOT NULL,
  session_id  INTEGER,
  "name"      CHARACTER VARYING(255),
  "type"      CHARACTER VARYING(75)
);
SELECT AddGeometryColumn('sword', 'create_tacticalobject_point', 'shape', 4326, 'GEOMETRY', 2);

ALTER TABLE sword.create_tacticalobject_point OWNER TO sword;
GRANT ALL ON TABLE sword.create_tacticalobject_point TO sword;

CREATE TABLE sword.emergencies
(
  id          SERIAL PRIMARY KEY,
  "type"      CHARACTER VARYING(255),
  symbol_id   CHARACTER VARYING(15),
  "valid"     SMALLINT,
  session_id  INTEGER
);
SELECT AddGeometryColumn('sword', 'emergencies', 'shape', 4326, 'GEOMETRY', 2);

ALTER TABLE sword.emergencies OWNER TO sword;
GRANT ALL ON TABLE sword.emergencies TO sword;

CREATE TABLE sword.exercises
(
  id          SERIAL PRIMARY KEY,
  "name"      CHARACTER VARYING(50),
  description CHARACTER VARYING(255)
);
ALTER TABLE sword.exercises OWNER TO sword;
GRANT ALL ON TABLE sword.exercises TO sword;

CREATE TABLE sword.extents
(
  exercise_id   INTEGER,
  exercise_xmin REAL DEFAULT (-180),
  exercise_ymin REAL DEFAULT (-90),
  exercise_xmax REAL DEFAULT 180,
  exercise_ymax REAL DEFAULT 90
);
ALTER TABLE sword.extents OWNER TO sword;
GRANT ALL ON TABLE sword.extents TO sword;

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


CREATE TABLE sword.knowledgeobjects
(
  id                    SERIAL PRIMARY KEY,                         
  public_oid            INTEGER NOT NULL,                         
  symbol_id             CHARACTER VARYING(20),                          
  session_id            INTEGER,                          
  "name"                CHARACTER VARYING(100),                         
  "type"                CHARACTER VARYING(100),                         
  state                 INTEGER DEFAULT 0,                          
  team_id               INTEGER DEFAULT (-1),
  lastupdate 						TIMESTAMP WITH TIME ZONE,                         
  observer_affiliation  CHARACTER VARYING(255)
);
ALTER TABLE sword.knowledgeobjects OWNER TO sword;
GRANT ALL ON TABLE sword.knowledgeobjects TO sword;


CREATE TABLE sword.knowledgeobjects_point
(
  id            SERIAL NOT NULL, -- dummy column to have SDE+GEOTOOLS working
  public_oid    INTEGER NOT NULL,
  session_id    INTEGER NOT NULL
 );
SELECT AddGeometryColumn('sword', 'knowledgeobjects_point', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.knowledgeobjects_point OWNER TO sword;
GRANT ALL ON TABLE sword.knowledgeobjects_point TO sword;

CREATE TABLE sword.knowledgeobjects_area
(
  id            SERIAL NOT NULL, -- dummy column to have SDE+GEOTOOLS working
  public_oid    INTEGER NOT NULL ,
  session_id    INTEGER NOT NULL
 );
SELECT AddGeometryColumn('sword', 'knowledgeobjects_area', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.knowledgeobjects_area OWNER TO sword;
GRANT ALL ON TABLE sword.knowledgeobjects_area TO sword;
 
CREATE TABLE sword.knowledgeobjects_line
(
  id            SERIAL NOT NULL, -- dummy column to have SDE+GEOTOOLS working
  public_oid    INTEGER NOT NULL ,
  session_id    INTEGER NOT NULL
 );
SELECT AddGeometryColumn('sword', 'knowledgeobjects_line', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.knowledgeobjects_line OWNER TO sword;
GRANT ALL ON TABLE sword.knowledgeobjects_line TO sword;

CREATE TABLE sword.knowledgeunits
(
  id                    SERIAL PRIMARY KEY,
  symbol_id             CHARACTER VARYING(255),
  public_oid            INTEGER NOT NULL,
  group_oid             INTEGER,
  unit_oid              INTEGER,
  speed                 INTEGER,
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
  lastupdate            TIMESTAMP WITH TIME ZONE --used?
);    
SELECT AddGeometryColumn('sword', 'knowledgeunits', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.knowledgeunits OWNER TO sword;
GRANT ALL ON TABLE sword.knowledgeunits TO sword;

CREATE TABLE sword.phase_lines
(
  id            INTEGER PRIMARY KEY,
  parameter_id  INTEGER NOT NULL,
  "type"        CHARACTER VARYING(6)
);
SELECT AddGeometryColumn('sword', 'phase_lines', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.phase_lines OWNER TO sword;
GRANT ALL ON TABLE sword.phase_lines TO sword;

CREATE TABLE sword.population
(
  id          INTEGER PRIMARY KEY,
  link_id     INTEGER NOT NULL,
  individuals INTEGER,
  road        INTEGER,
  pavement    INTEGER,
  office      INTEGER,
  residential INTEGER,
  shop        INTEGER,
  cell_id     INTEGER,
  session_id  INTEGER
);
SELECT AddGeometryColumn('sword', 'population', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.population OWNER TO sword;
GRANT ALL ON TABLE sword.population TO sword;

CREATE TABLE sword.population_attributes
(
  population_id   INTEGER,
  activity        INTEGER,
  profile         INTEGER,
  quantity        INTEGER,
  cell_id         INTEGER,
  session_id      INTEGER
);
ALTER TABLE sword.population_attributes OWNER TO sword;
GRANT ALL ON TABLE sword.population_attributes TO sword;

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


CREATE TABLE sword.serial_grid
(
  id            SERIAL PRIMARY KEY,
  exercise_id   INTEGER  -- one terrain per exercise, not per session
);
SELECT AddGeometryColumn('sword', 'serial_grid', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.serial_grid OWNER TO sword;
GRANT ALL ON TABLE sword.serial_grid TO sword;


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

CREATE 	SEQUENCE sword.tacticalobject_id_seq
				INCREMENT 1
  			MINVALUE 1
  			MAXVALUE 9223372036854775807
  			START 1
  			CACHE 1;
ALTER TABLE sword.tacticalobject_id_seq OWNER TO sword;

CREATE TABLE sword.tacticalobject_area
(
  id            INTEGER DEFAULT nextval('sword.tacticalobject_id_seq') PRIMARY KEY,
  "name"        CHARACTER VARYING(100),
  "type"        CHARACTER VARYING(100),
  symbol_id     CHARACTER VARYING(255),
  public_oid    INTEGER NOT NULL,
  state         INTEGER DEFAULT 0,
  session_id    INTEGER
);          
SELECT AddGeometryColumn('sword', 'tacticalobject_area', 'shape', 4326, 'GEOMETRY', 2);
ALTER TABLE sword.tacticalobject_area OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_area TO sword;

CREATE TABLE sword.tacticalobject_line
(
  id            INTEGER DEFAULT nextval('sword.tacticalobject_id_seq') PRIMARY KEY,                                              
  "name"        CHARACTER VARYING(100),                                                
  "type"        CHARACTER VARYING(100),                                                
  symbol_id     CHARACTER VARYING(255),                                                
  public_oid    INTEGER NOT NULL,                                                      
  state         INTEGER DEFAULT 0,                                                     
  session_id    INTEGER                                                                
);                                                                                     
SELECT AddGeometryColumn('sword', 'tacticalobject_line', 'shape', 4326, 'GEOMETRY', 2);

ALTER TABLE sword.tacticalobject_line OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_line TO sword;

CREATE TABLE sword.tacticalobject_point
(
  id            INTEGER DEFAULT nextval('sword.tacticalobject_id_seq') PRIMARY KEY,                                                
  "name"        CHARACTER VARYING(100),                                                  
  "type"        CHARACTER VARYING(100),                                                  
  symbol_id     CHARACTER VARYING(255),                                                  
  public_oid    INTEGER NOT NULL,                                                        
  state         INTEGER DEFAULT 0,                                                       
  session_id    INTEGER                                                                  
);                                                                                       
SELECT AddGeometryColumn('sword', 'tacticalobject_point', 'shape', 4326, 'GEOMETRY', 2);  
ALTER TABLE sword.tacticalobject_point OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_point TO sword;

CREATE TABLE sword.tacticalobject_attribute_activity_time                
(                                                                        
  id            SERIAL PRIMARY KEY,                                                        
  activity_time INTEGER,                                                 
  object_id     INTEGER,                                                     
  session_id    INTEGER                                                  
);                                                       
ALTER TABLE sword.tacticalobject_attribute_activity_time OWNER TO sword; 
GRANT ALL ON TABLE sword.tacticalobject_attribute_activity_time TO sword;

CREATE TABLE sword.tacticalobject_attribute_bypass
(
  id            SERIAL PRIMARY KEY,
  percentage    INTEGER,
  object_id     INTEGER,
  session_id    INTEGER
);
ALTER TABLE sword.tacticalobject_attribute_bypass OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_bypass TO sword;

CREATE TABLE sword.tacticalobject_attribute_construction
(
  id            SERIAL PRIMARY KEY,
  dotation_nbr  INTEGER,
  dotation_type INTEGER,
  percentage    INTEGER,
  density       NUMERIC(38,8),
  object_id     INTEGER,
  session_id    INTEGER
);
ALTER TABLE sword.tacticalobject_attribute_construction OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_construction TO sword;

CREATE TABLE sword.tacticalobject_attribute_crossing_site
(
  id                    SERIAL PRIMARY KEY,
  banks_require_fitting BOOLEAN,
  depth                 INTEGER,
  flow_rate             INTEGER,
  width                 INTEGER,
  object_id             INTEGER,
  session_id            INTEGER
);
ALTER TABLE sword.tacticalobject_attribute_crossing_site OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_crossing_site TO sword;

CREATE TABLE sword.tacticalobject_attribute_fire
(
  id          SERIAL PRIMARY KEY,
  class_id    INTEGER,
  heat        INTEGER,
  object_id   INTEGER,
  session_id  INTEGER
);
ALTER TABLE sword.tacticalobject_attribute_fire OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_fire TO sword;

CREATE TABLE sword.tacticalobject_attribute_interaction_height
(
  id          SERIAL PRIMARY KEY ,
  height      NUMERIC(38,8),
  object_id   INTEGER,
  session_id  INTEGER
);
ALTER TABLE sword.tacticalobject_attribute_interaction_height OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_interaction_height TO sword;

CREATE TABLE sword.tacticalobject_attribute_logistic
(
  id          SERIAL PRIMARY KEY,
  tc2         INTEGER,
  object_id   INTEGER,
  session_id  INTEGER
);
ALTER TABLE sword.tacticalobject_attribute_logistic OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_logistic TO sword;

CREATE TABLE sword.tacticalobject_attribute_medical_treatment
(
  id                  SERIAL PRIMARY KEY,
  doctors             INTEGER,
  beds                INTEGER,
  available_doctors   INTEGER,
  available_beds      INTEGER,
  object_id           INTEGER,
  session_id          INTEGER
);
ALTER TABLE sword.tacticalobject_attribute_medical_treatment OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_medical_treatment TO sword;

CREATE TABLE sword.tacticalobject_attribute_mine
(
  id            SERIAL PRIMARY KEY,
  dotation_nbr  INTEGER,
  dotation_type INTEGER,
  percentage    INTEGER,
  density       NUMERIC(38,8),
  object_id     INTEGER,
  session_id    INTEGER
);
ALTER TABLE sword.tacticalobject_attribute_mine OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_mine TO sword;

CREATE TABLE sword.tacticalobject_attribute_nbc
(
  id            SERIAL PRIMARY KEY,
  danger_level  INTEGER,
  object_id     INTEGER,
  session_id    INTEGER
);
ALTER TABLE sword.tacticalobject_attribute_nbc OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_nbc TO sword;

CREATE TABLE sword.tacticalobject_attribute_nbc_type
(
  id                    SERIAL PRIMARY KEY,
  agent_id              INTEGER,
  concentration         INTEGER,
  source_life_duration  INTEGER,
  object_id             INTEGER,
  session_id            INTEGER
);
ALTER TABLE sword.tacticalobject_attribute_nbc_type OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_nbc_type TO sword;

CREATE TABLE sword.tacticalobject_attribute_obstacle
(
  id          SERIAL PRIMARY KEY,
  activated   BOOLEAN,
  "type"      CHARACTER VARYING(20),
  object_id   INTEGER,
  session_id  INTEGER
);
ALTER TABLE sword.tacticalobject_attribute_obstacle OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_obstacle TO sword;

CREATE TABLE sword.tacticalobject_attribute_stock
(
  id          SERIAL PRIMARY KEY,
  object_id   INTEGER,
  session_id  INTEGER
);
ALTER TABLE sword.tacticalobject_attribute_stock OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_stock TO sword;

CREATE TABLE sword.tacticalobject_attribute_supplyroute
(
  id          SERIAL PRIMARY KEY,
  equipped    BOOLEAN,
  flow_rate   INTEGER,
  length      INTEGER,
  max_weight  INTEGER,
  width       INTEGER,
  object_id   INTEGER,
  session_id  INTEGER
);
ALTER TABLE sword.tacticalobject_attribute_supplyroute OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_supplyroute TO sword;

CREATE TABLE sword.tacticalobject_attribute_toxic_cloud
(
  id          SERIAL NOT NULL,
  object_id   INTEGER,
  session_id  INTEGER
);
ALTER TABLE sword.tacticalobject_attribute_toxic_cloud OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_toxic_cloud TO sword;

CREATE TABLE sword.teams(
  id          SERIAL PRIMARY KEY,
  public_oid  INTEGER NOT NULL,
  "type"      INTEGER,
  "name"      CHARACTER VARYING(255),
  session_id  INTEGER
);
ALTER TABLE sword.tacticalobject_attribute_toxic_cloud OWNER TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_toxic_cloud TO sword;

CREATE TABLE sword.unitforces
(
  id          SERIAL PRIMARY KEY,
  public_oid  INTEGER NOT NULL,
  parent_oid  INTEGER,
  "type"      INTEGER,
  "name"      CHARACTER VARYING(255),
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

----------------------------------------------------------
-- Function                             
----------------------------------------------------------
CREATE OR REPLACE FUNCTION sword.get_objectknowledges(IN geotype character varying, IN sessionid integer, OUT id integer, OUT public_oid integer, OUT symbol_id character varying, OUT session_id integer, OUT "name" character varying, OUT "type" character varying, OUT state integer, OUT observer_affiliation character varying, OUT team_id integer, OUT shape character varying)
  RETURNS SETOF record AS               
$BODY$                                  
BEGIN                                   
    IF ( geotype = 'Point' ) THEN       
        RETURN QUERY SELECT a.id, a.public_oid, a.symbol_id, a.session_id, a."name", a."type", a.state, a.observer_affiliation, a.team_id, cast( public.st_astext(b.shape) as varchar ) 
        FROM sword.knowledgeobjects a, sword.knowledgeobjects_point b
        WHERE a.public_oid=b.public_oid and a.session_id = sessionid and a.session_id = b.session_id;
    ELSIF ( geotype = 'Line' ) THEN
        RETURN QUERY SELECT a.id, a.public_oid, a.symbol_id, a.session_id, a."name", a."type", a.state, a.observer_affiliation, a.team_id, cast( public.st_astext(b.shape) as varchar ) 
        FROM sword.knowledgeobjects a, sword.knowledgeobjects_line b
        WHERE a.public_oid=b.public_oid and a.session_id = sessionid and a.session_id = b.session_id;
    ELSIF ( geotype = 'Area' ) THEN
        RETURN QUERY SELECT a.id, a.public_oid, a.symbol_id, a.session_id, a."name", a."type", a.state, a.observer_affiliation, a.team_id, cast( public.st_astext(b.shape) as varchar ) 
        FROM sword.knowledgeobjects a, sword.knowledgeobjects_area b
        WHERE a.public_oid=b.public_oid and a.session_id = sessionid and a.session_id = b.session_id;
    END IF;
 END;  
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100
  ROWS 1000;
ALTER FUNCTION sword.get_objectknowledges(character varying, integer) OWNER TO sword;