select * from sword.exercises where changedate > '2009-12-11 11:27:30'

select now()


-- PRE UPDATE TRIGGER ----
CREATE OR REPLACE FUNCTION sword.trialExerciseChangestampPreUpdate() RETURNS TRIGGER AS 
'
 BEGIN 
  OLD.changedate:=now();
  RETURN OLD;
 END; 
' 
LANGUAGE 'plpgsql';

DROP TRIGGER IF EXISTS trialExercisePreUpdateTrigger ON sword.exercises;

CREATE TRIGGER trialExercisePreUpdateTrigger 
BEFORE UPDATE ON sword.exercises
FOR EACH ROW EXECUTE PROCEDURE sword.trialExerciseChangestampPreUpdate();
-------------------------

-- PRE DELETE TRIGGER ----

-- copy of exercises
CREATE TABLE sword.exercises_delete_history
(
  id serial NOT NULL,
  "name" character varying(50),
  description character varying(255),
  changedate timestamp without time zone DEFAULT now(),
  CONSTRAINT exercises_delete_history_pkey PRIMARY KEY (id)
)
WITH (OIDS=FALSE);
ALTER TABLE sword.exercises OWNER TO sword;
GRANT ALL ON TABLE sword.exercises TO sword;



CREATE OR REPLACE FUNCTION sword.trialExerciseChangestampPreDelete() RETURNS TRIGGER AS 
'
 BEGIN 
  insert into sword.exercises_delete_history (id, changedate) values (OLD.id, now() );
  return null;
 END; 
' 
LANGUAGE 'plpgsql';


DROP TRIGGER IF EXISTS trialExercisePreDeleteTrigger ON sword.exercises;

CREATE TRIGGER trialExercisePreDeleteTrigger 
BEFORE DELETE ON sword.exercises
FOR EACH ROW EXECUTE PROCEDURE sword.trialExerciseChangestampPreDelete();
-------------------------



-- POST UPDATE TRIGGER ----
DROP TRIGGER IF EXISTS trialExercisePostUpdateTrigger ON sword.exercises;

CREATE OR REPLACE FUNCTION sword.trialExerciseChangestampPostUpdate() RETURNS TRIGGER AS 
'
 BEGIN 
  NEW.changedate:=now();
  RETURN NEW;
 END; 
' 
LANGUAGE 'plpgsql';

CREATE TRIGGER trialExercisePostUpdateTrigger 
AFTER UPDATE ON sword.exercises
FOR EACH ROW EXECUTE PROCEDURE sword.trialExerciseChangestampPostUpdate();
-------------------------


