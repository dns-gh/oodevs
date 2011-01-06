CREATE SCHEMA sword;

-- Using ArcSDE SDELAYER command requires that the role accessing the schema be exactly the same
-- string than the schema, so we create a user and a schema called "sword", and at the 
-- end of this script, we will grant all tables of sword's tablespace to sword's role.
--
-- This is a bug that seems to be related to the buggy SQL command sde.st_register_spatial_column
-- which seems to switch the schema and database argument.


-- TODO: remove 


-- Users --------------------------------

CREATE TABLE sword.users
(
  id         INTEGER PRIMARY KEY,
  name       VARCHAR(50),
  password   VARCHAR(100),
  mail       VARCHAR(100),
  crypto     VARCHAR(64),
  "role"     VARCHAR(10), -- TODO: use an enum instead. requires droping ESRI impl which can't support enums.
  client_id  INTEGER
);

CREATE TABLE sword.clients
(
  id         INTEGER PRIMARY KEY,
  name       VARCHAR(50)
);

CREATE TABLE sword.user_sessions
(
    user_id       INTEGER,
    session_id    INTEGER
);

CREATE TABLE sword.user_exercises
(
    user_id       INTEGER,
    exercise_id   INTEGER
);

-- Exercises ----------------------------

CREATE TABLE sword.exercises
(
    id           SERIAL PRIMARY KEY,
    name         VARCHAR(50),
    description  VARCHAR(255)
);

CREATE TABLE sword.extents
(
    exercise_id   INTEGER,
    exercise_xmin REAL DEFAULT -180,
    exercise_ymin REAL DEFAULT -90,
    exercise_xmax REAL DEFAULT 180,
    exercise_ymax REAL DEFAULT 90
);

CREATE TYPE simulation_state AS ENUM ( 'stopped', 'initializing', 'running', 'paused' );

CREATE TABLE sword.sessions
(
    id          SERIAL PRIMARY KEY,
    name        VARCHAR(255),
    status      simulation_state,
    exercise_id INTEGER
);

CREATE TABLE sword.simulationproperties
(
    id          SERIAL PRIMARY KEY,
    property    CHARACTER VARYING(25),
    "value"     CHARACTER VARYING(25),
    checked     INTEGER DEFAULT (-1),
    session_id  INTEGER
);

CREATE TABLE sword.simulationclock
(
    id          SERIAL PRIMARY KEY,
    clock       CHARACTER VARYING(15),
    session_id  INTEGER
);



-- Population ----------------------------

CREATE TABLE sword.population
(
  id            SERIAL PRIMARY KEY,
  shape         st_geometry,  
  link_id       INTEGER NOT NULL,
  individuals   INTEGER,
  road          INTEGER,
  pavement      INTEGER,
  office        INTEGER,
  residential   INTEGER,
  shop          INTEGER,
  cell_id       INTEGER,
  session_id    INTEGER
);

CREATE TABLE sword.population_attributes
(
    population_id   INTEGER,
    activity        INTEGER,
    profile         INTEGER,
    quantity        INTEGER,
    cell_id         INTEGER,
    session_id      INTEGER
);

CREATE TABLE sword.activities
(
  id            INTEGER PRIMARY KEY,
  name          VARCHAR(255),  
  session_id    INTEGER
);

CREATE TABLE sword.profiles
(
  id            INTEGER PRIMARY KEY,
  name          VARCHAR(255),  
  session_id    INTEGER
);

-- Organization (i.e. orbat) ----------------------------

CREATE TABLE sword.teams
(
  id            SERIAL PRIMARY KEY,
  public_oid    INTEGER NOT NULL,
  type             INTEGER,
  name          VARCHAR(255),
  session_id    INTEGER
);

CREATE TABLE sword.formations
(
  id            SERIAL PRIMARY KEY,
  public_oid    INTEGER NOT NULL,
  parent_oid    INTEGER NOT NULL,
  type          INTEGER,
  engaged       INTEGER,
  name          VARCHAR(255),
  symbol_id     CHARACTER(15),
  session_id    INTEGER
);

CREATE TABLE sword.unitforces
(
  id          SERIAL PRIMARY KEY,
  public_oid  INTEGER NOT NULL,
  parent_oid  INTEGER,
  type        INTEGER,
  name        VARCHAR(255),
  symbol_id   CHARACTER(15),  
  valid       INTEGER,
  op_state    INTEGER,
  shape       st_geometry,
  direction   DOUBLE PRECISION DEFAULT 0,
  speed       INTEGER, 
  lastupdate  timestamp with time zone,
  session_id  INTEGER
);

CREATE TABLE sword.knowledgeunits
(
  id                     SERIAL PRIMARY KEY,
  symbol_id              VARCHAR(15),
  public_oid             INTEGER NOT NULL,
  group_oid              INTEGER,
  unit_oid               INTEGER,
  speed                  INTEGER,
  dead                   INTEGER DEFAULT -1,
  name                   VARCHAR(255),
  parent                 VARCHAR(255),
  effective              VARCHAR(255),
  quantity               VARCHAR(255), 
  signature              VARCHAR(255),
  direction              DOUBLE PRECISION DEFAULT 0,
  identification_level   INTEGER,
  observer_affiliation   VARCHAR(255),
  observer_oid           INTEGER,
  shape                  st_geometry,
  lastupdate             timestamp with time zone,
  session_id             INTEGER
);

CREATE TABLE sword.knowledgeobjects
(
  id            SERIAL NOT NULL PRIMARY KEY,
  public_oid    INTEGER NOT NULL,
  symbol_id     CHARACTER varying(20),
  session_id    INTEGER,
  "name"        CHARACTER varying(100),
  "type"        CHARACTER varying(100),
  state         INTEGER DEFAULT 0,
  team_id       INTEGER DEFAULT -1,
  lastupdate    timestamp with time zone,
  observer_affiliation CHARACTER varying(255)
 );
 
CREATE TABLE sword.knowledgeobjects_point
(
  id            SERIAL NOT NULL, -- dummy column to have SDE+GEOTOOLS working
  public_oid    INTEGER NOT NULL,
  session_id    INTEGER NOT NULL,
  shape         st_geometry
 );
 
CREATE TABLE sword.knowledgeobjects_area
(
  id            SERIAL NOT NULL, -- dummy column to have SDE+GEOTOOLS working
  public_oid    INTEGER NOT NULL ,
  session_id    INTEGER NOT NULL,
  shape         st_geometry
 );
 
CREATE TABLE sword.knowledgeobjects_line
(
  id            SERIAL NOT NULL, -- dummy column to have SDE+GEOTOOLS working
  public_oid    INTEGER NOT NULL ,
  session_id    INTEGER NOT NULL,
  shape         st_geometry
 );

-- Terrain -------------------------------------------

CREATE TABLE sword.serial_grid
(
  id            SERIAL PRIMARY KEY,
  shape         st_geometry,
  exercise_id   INTEGER  -- one terrain per exercise, not per session
);

-- Mission Orders & parameters -----------------------

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

-- Tactical objects creation -----------------------

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


CREATE TABLE sword.actionparameters
(
  id            SERIAL PRIMARY KEY,
  type          VARCHAR(255),
  name          VARCHAR(255),
  value         VARCHAR(255),
  reference_id  INTEGER NOT NULL -- REFERENCES sword.create_orders(id) or sword.create_objects(id) ON DELETE CASCADE
);

CREATE TABLE sword.actionparameters_area
(
  id            SERIAL PRIMARY KEY,
  parameter_id  INTEGER NOT NULL,-- REFERENCES sword.actionparameters( id ) ON DELETE CASCADE,
  shape         st_geometry
);

CREATE TABLE sword.actionparameters_line
(
  id            SERIAL PRIMARY KEY,
  parameter_id  INTEGER NOT NULL,-- REFERENCES sword.actionparameters( id ) ON DELETE CASCADE,
  shape         st_geometry
);

CREATE TABLE sword.actionparameters_point
(
  id            SERIAL PRIMARY KEY,
  parameter_id  INTEGER NOT NULL,-- REFERENCES sword.actionparameters( id ) ON DELETE CASCADE,
  shape         st_geometry
);

-----------------------------------------
CREATE TABLE sword.phase_lines
(
  id            INTEGER PRIMARY KEY,
  parameter_id  INTEGER NOT NULL,
  type          VARCHAR(6),
  shape         st_geometry
);

CREATE TABLE sword.reports
(
  id            SERIAL PRIMARY KEY,
  message       VARCHAR(255),            --SHOULD IT BE LONGER THAN 255? SHOULD WE HAVE DATE FIELD?
  unit_id       INTEGER,
  lastupdate    timestamp with time zone,
  session_id    INTEGER
);


-- Boundary Limits & tactical lines ------------------
-- tactical lines are used to set up lines where units have to wait for other units.
-- whereas boundary limits are used to define coridors.

CREATE TABLE sword.boundarylimits
(
  id            SERIAL PRIMARY KEY,
  public_oid    INTEGER NOT NULL,
  symbol_id     VARCHAR(15) NOT NULL,    -- WHAT IS IT?
  name          VARCHAR(255),
--  location   VARCHAR(20),            -- WHAT IS IT?
--  direction  DOUBLE, --NUMERIC(38,8),
--  type       VARCHAR(20),              -- WHAT IS IT?
-- ADD diffusion 
  shape         st_geometry,
  lastupdate    timestamp with time zone,
  session_id    INTEGER
);

CREATE TABLE sword.boundarylimits_creation
(
  id            SERIAL PRIMARY KEY,
  public_oid    INTEGER NOT NULL,
  symbol_id     VARCHAR(15) NOT NULL,    -- WHAT IS IT?
  name          VARCHAR(255),
  location      VARCHAR(20),            -- WHAT IS IT?
  direction     DOUBLE PRECISION DEFAULT 0,
  type          VARCHAR(20),              -- WHAT IS IT?
  shape         st_geometry,
  session_id    INTEGER
);

CREATE TABLE sword.tacticallines
(
  id            SERIAL PRIMARY KEY,
  public_oid    INTEGER NOT NULL,
  symbol_id     VARCHAR(15),
  name          VARCHAR(255),
  shape         st_geometry,
-- ADD diffusion              -- type donn� au moment de la mission
  session_id    INTEGER
);

-- Tactical objects instances -----------------------

CREATE TABLE sword.tacticalobject_area
(
  id            SERIAL PRIMARY KEY,
  name          VARCHAR(100),            
  type          VARCHAR(100),    
  symbol_id     VARCHAR(255),            
  public_oid    INTEGER NOT NULL,
  shape         st_geometry,
  state         INTEGER DEFAULT 0,
  session_id    INTEGER
);

CREATE TABLE sword.tacticalobject_line
(
  id            SERIAL PRIMARY KEY,
  name          VARCHAR(100),            
  type          VARCHAR(100),
  public_oid    INTEGER NOT NULL,
  symbol_id     VARCHAR(255),      -- SHOUD ADD DIRECTION? WHY ISN'T IT IN CREATION TABLE
  shape         st_geometry,
  state         INTEGER DEFAULT 0,
  session_id    INTEGER
);

CREATE TABLE sword.tacticalobject_point
(
  id            SERIAL PRIMARY KEY,
  name          VARCHAR(100),
  type          VARCHAR(100),
  public_oid    INTEGER NOT NULL,
  symbol_id     VARCHAR(20),
  shape         st_geometry,
  state         INTEGER DEFAULT 0,
  session_id    INTEGER
);

-- Tactical Object Attributes ---------------

CREATE TABLE sword.tacticalobject_attribute_activity_time
(
  id            SERIAL PRIMARY KEY,
  activity_time INTEGER,
  object_id     INTEGER,
  session_id    INTEGER
);

CREATE TABLE sword.tacticalobject_attribute_bypass
(
  id         SERIAL PRIMARY KEY,
  percentage INTEGER,
  object_id  INTEGER,
  session_id INTEGER
);

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

CREATE TABLE sword.tacticalobject_attribute_fire
(
  id            SERIAL PRIMARY KEY,
  class_id      INTEGER,
  heat          INTEGER,
  object_id     INTEGER,
  session_id    INTEGER
);

CREATE TABLE sword.tacticalobject_attribute_interaction_height
(
  id         SERIAL PRIMARY KEY,
  height     NUMERIC(38,8),
  object_id  INTEGER,
  session_id INTEGER
);

CREATE TABLE sword.tacticalobject_attribute_logistic
(
  id         SERIAL PRIMARY KEY,
  tc2        INTEGER,  
  object_id  INTEGER,
  session_id INTEGER
);

CREATE TABLE sword.tacticalobject_attribute_medical_treatment
(
  id                SERIAL PRIMARY KEY,
  doctors           INTEGER,
  beds              INTEGER,
  available_doctors INTEGER,
  available_beds    INTEGER,
  object_id         INTEGER,
  session_id        INTEGER
    --TODO: (list) type_id  INTEGER,
);

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

CREATE TABLE sword.tacticalobject_attribute_nbc
(
  id           SERIAL PRIMARY KEY,
  danger_level INTEGER,  
  --TODO: nbc_agents (list)
  object_id    INTEGER,
  session_id   INTEGER
);

CREATE TABLE sword.tacticalobject_attribute_nbc_type
(
    id                   SERIAL PRIMARY KEY,
    agent_id             INTEGER,  
    concentration        INTEGER,
    source_life_duration INTEGER,
    object_id            INTEGER,
    session_id           INTEGER
);

CREATE TABLE sword.tacticalobject_attribute_obstacle
(
  id            SERIAL PRIMARY KEY,
  activated     BOOLEAN,
  type          VARCHAR(20),
  object_id     INTEGER,
  session_id    INTEGER
);

CREATE TABLE sword.tacticalobject_attribute_stock
(
  id            SERIAL PRIMARY KEY,
  object_id     INTEGER,
  session_id    INTEGER
  --TODO:stock.resources (list)
);

CREATE TABLE sword.tacticalobject_attribute_supplyroute
(
  id            SERIAL PRIMARY KEY,
  equipped      BOOLEAN,
  flow_rate     INTEGER,
  length        INTEGER,
  max_weight    INTEGER,
  width         INTEGER,
  object_id     INTEGER,
  session_id    INTEGER
);

CREATE TABLE sword.tacticalobject_attribute_toxic_cloud
(
  id            SERIAL PRIMARY KEY,
  object_id     INTEGER,
  session_id    INTEGER
  --TODO:quantities (list)
);

---------------------------------------------

CREATE TABLE sword.emergencies
(
  id          SERIAL PRIMARY KEY,
  type        VARCHAR(255),            -- WHAT IS IT?
  symbol_id   VARCHAR(15),
  valid       INTEGER,
  shape       st_geometry,
  
  session_id INTEGER
);

------------------------------------------------------
ALTER SCHEMA sword OWNER TO sword;

ALTER TABLE sword.simulationproperties OWNER TO sword;
ALTER TABLE sword.simulationclock OWNER TO sword;
ALTER TABLE sword.users OWNER TO sword;
ALTER TABLE sword.clients OWNER TO sword;
ALTER TABLE sword.user_sessions OWNER TO sword;
ALTER TABLE sword.user_exercises OWNER TO sword;
ALTER TABLE sword.exercises OWNER TO sword;
ALTER TABLE sword.extents OWNER TO sword;
ALTER TABLE sword.sessions OWNER TO sword;
ALTER TABLE sword.activities OWNER TO sword;
ALTER TABLE sword.profiles OWNER TO sword;
ALTER TABLE sword.teams OWNER TO sword;
ALTER TABLE sword.formations OWNER TO sword;
ALTER TABLE sword.unitforces OWNER TO sword;
ALTER TABLE sword.knowledgeunits OWNER TO sword;
ALTER TABLE sword.emergencies OWNER TO sword;
ALTER TABLE sword.population OWNER TO sword;
ALTER TABLE sword.population_attributes OWNER TO sword;
ALTER TABLE sword.terrain_grid OWNER TO sword;
ALTER TABLE sword.create_orders OWNER TO sword;
ALTER TABLE sword.create_objects OWNER TO sword;
ALTER TABLE sword.actionparameters OWNER TO sword;
ALTER TABLE sword.actionparameters_area OWNER TO sword;
ALTER TABLE sword.actionparameters_line OWNER TO sword;
ALTER TABLE sword.actionparameters_point OWNER TO sword;
ALTER TABLE sword.phase_lines OWNER TO sword;
ALTER TABLE sword.reports OWNER TO sword;
ALTER TABLE sword.boundarylimits OWNER TO sword;
ALTER TABLE sword.boundarylimits_creation OWNER TO sword;
ALTER TABLE sword.tacticallines OWNER TO sword;
ALTER TABLE sword.tacticalobject_area OWNER TO sword;
ALTER TABLE sword.tacticalobject_line OWNER TO sword;
ALTER TABLE sword.tacticalobject_point OWNER TO sword;
ALTER TABLE sword.tacticalobject_attribute_activity_time OWNER TO sword;
ALTER TABLE sword.tacticalobject_attribute_bypass OWNER TO sword;
ALTER TABLE sword.tacticalobject_attribute_construction OWNER TO sword;
ALTER TABLE sword.tacticalobject_attribute_crossing_site OWNER TO sword;
ALTER TABLE sword.tacticalobject_attribute_fire OWNER TO sword;
ALTER TABLE sword.tacticalobject_attribute_interaction_height OWNER TO sword;
ALTER TABLE sword.tacticalobject_attribute_logistic OWNER TO sword;
ALTER TABLE sword.tacticalobject_attribute_medical_treatment OWNER TO sword;
ALTER TABLE sword.tacticalobject_attribute_mine OWNER TO sword;
ALTER TABLE sword.tacticalobject_attribute_nbc OWNER TO sword;
ALTER TABLE sword.tacticalobject_attribute_nbc_type OWNER TO sword;
ALTER TABLE sword.tacticalobject_attribute_obstacle OWNER TO sword;
ALTER TABLE sword.tacticalobject_attribute_stock OWNER TO sword;
ALTER TABLE sword.tacticalobject_attribute_supplyroute OWNER TO sword;
ALTER TABLE sword.tacticalobject_attribute_toxic_cloud OWNER TO sword;
ALTER TABLE sword.population OWNER TO sword;
ALTER TABLE sword.terrain_grid OWNER TO sword;
ALTER TABLE sword.knowledgeobjects OWNER TO sword;
ALTER TABLE sword.knowledgeobjects_point OWNER TO sword;
ALTER TABLE sword.knowledgeobjects_line OWNER TO sword;
ALTER TABLE sword.knowledgeobjects_area OWNER TO sword;

------------------------------------------------------
GRANT ALL ON SCHEMA sword TO sword;

GRANT ALL ON TABLE sword.simulationproperties TO sword;
GRANT ALL ON TABLE sword.simulationclock TO sword;
GRANT ALL ON TABLE sword.users TO sword;
GRANT ALL ON TABLE sword.clients TO sword;
GRANT ALL ON TABLE sword.user_sessions TO sword;
GRANT ALL ON TABLE sword.user_exercises TO sword;
GRANT ALL ON TABLE sword.exercises TO sword;
GRANT ALL ON TABLE sword.extents TO sword;
GRANT ALL ON TABLE sword.sessions TO sword;
GRANT ALL ON TABLE sword.population TO sword;
GRANT ALL ON TABLE sword.population_attributes TO sword;
GRANT ALL ON TABLE sword.terrain_grid TO sword;
GRANT ALL ON TABLE sword.activities TO sword;
GRANT ALL ON TABLE sword.profiles TO sword;
GRANT ALL ON TABLE sword.teams TO sword;
GRANT ALL ON TABLE sword.formations TO sword;
GRANT ALL ON TABLE sword.unitforces TO sword;
GRANT ALL ON TABLE sword.emergencies TO sword;
GRANT ALL ON TABLE sword.knowledgeunits TO sword;
GRANT ALL ON TABLE sword.create_orders TO sword;
GRANT ALL ON TABLE sword.create_objects TO sword;
GRANT ALL ON TABLE sword.actionparameters TO sword;
GRANT ALL ON TABLE sword.phase_lines TO sword;
GRANT ALL ON TABLE sword.actionparameters_area TO sword;
GRANT ALL ON TABLE sword.actionparameters_line TO sword;
GRANT ALL ON TABLE sword.actionparameters_point TO sword;
GRANT ALL ON TABLE sword.reports TO sword;
GRANT ALL ON TABLE sword.boundarylimits TO sword;
GRANT ALL ON TABLE sword.boundarylimits_creation TO sword;
GRANT ALL ON TABLE sword.tacticallines TO sword;
GRANT ALL ON TABLE sword.tacticalobject_area TO sword;
GRANT ALL ON TABLE sword.tacticalobject_line TO sword;
GRANT ALL ON TABLE sword.tacticalobject_point TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_activity_time TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_bypass TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_construction TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_crossing_site TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_fire TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_interaction_height TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_logistic TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_medical_treatment TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_mine TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_nbc TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_nbc_type TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_obstacle TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_stock TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_supplyroute TO sword;
GRANT ALL ON TABLE sword.tacticalobject_attribute_toxic_cloud TO sword;
GRANT ALL ON TABLE sword.population TO sword;
GRANT ALL ON TABLE sword.terrain_grid TO sword;
GRANT ALL ON TABLE sword.knowledgeobjects TO sword;
GRANT ALL ON TABLE sword.knowledgeobjects_point TO sword;
GRANT ALL ON TABLE sword.knowledgeobjects_line TO sword;
GRANT ALL ON TABLE sword.knowledgeobjects_area TO sword;


-------------------- FUNCTIONS AND PROCEDURES ---------------------------------
CREATE OR REPLACE FUNCTION sword.get_objectknowledges(IN geotype character varying, IN sessionid integer, OUT id integer, OUT public_oid integer, OUT symbol_id character varying, OUT session_id integer, OUT "name" character varying, OUT "type" character varying, OUT state integer, OUT observer_affiliation character varying, OUT team_id integer, OUT shape character varying)
  RETURNS SETOF record AS
$BODY$ 
BEGIN
    IF ( geotype = 'Point' ) THEN
        RETURN QUERY SELECT a.id, a.public_oid, a.symbol_id, a.session_id, a."name", a."type", a.state, a.observer_affiliation, a.team_id, cast( sde.st_AsText(b.shape) as varchar ) 
        FROM sword.knowledgeobjects a, sword.knowledgeobjects_point b
        WHERE a.public_oid=b.public_oid and a.session_id = sessionid and a.session_id = b.session_id;
    ELSIF ( geotype = 'Line' ) THEN
        RETURN QUERY SELECT a.id, a.public_oid, a.symbol_id, a.session_id, a."name", a."type", a.state, a.observer_affiliation, a.team_id, cast( sde.st_AsText(b.shape) as varchar ) 
        FROM sword.knowledgeobjects a, sword.knowledgeobjects_line b
        WHERE a.public_oid=b.public_oid and a.session_id = sessionid and a.session_id = b.session_id;
    ELSIF ( geotype = 'Area' ) THEN
        RETURN QUERY SELECT a.id, a.public_oid, a.symbol_id, a.session_id, a."name", a."type", a.state, a.observer_affiliation, a.team_id, cast( sde.st_AsText(b.shape) as varchar ) 
        FROM sword.knowledgeobjects a, sword.knowledgeobjects_area b
        WHERE a.public_oid=b.public_oid and a.session_id = sessionid and a.session_id = b.session_id;
    END IF;
 END;  
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100
  ROWS 1000;
ALTER FUNCTION sword.get_objectknowledges(character varying, integer) OWNER TO sword;

