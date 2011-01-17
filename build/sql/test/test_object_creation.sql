
DELETE FROM sword.create_objects;
DELETE FROM sword.actionparameters;
DELETE FROM sword.actionparameters_point;
DELETE FROM sword.actionparameters_line;
DELETE FROM sword.actionparameters_area;

DELETE FROM sword.sessions WHERE id=2000;
DELETE FROM sword.exercises WHERE id=4242;

-- INSERT INTO sword.exercises (id, "name" ) VALUES (4242, 'test' );
-- INSERT INTO sword.sessions (id, "name", exercise_id) VALUES (2000, 'test-session', 4242 );

INSERT INTO sword.create_objects   (id, team_id, name, type, session_id) VALUES ( 331, 3, 'Checkpoint 1', 'checkpoint', 1 );
INSERT INTO sword.actionparameters (reference_id, type, name, value) VALUES ( 331, 'String', 'Type', 'checkpoint' );
INSERT INTO sword.actionparameters (id, reference_id, type, name, value) VALUES ( 33201, 331, 'Location', 'Location', 'actionparameters_point' );
INSERT INTO sword.actionparameters_point (parameter_id, shape) VALUES ( 
    33201, st_point( 0.13297806372070312, 49.5225, 0 ) );
INSERT INTO sword.actionparameters (reference_id, type, name, value) VALUES ( 331, 'String', 'Name', 'Checkpoint 1' );
INSERT INTO sword.actionparameters (reference_id, type, name, value) VALUES ( 331, 'Army', 'Camp', '3' );
INSERT INTO sword.actionparameters (reference_id, type, name, value) VALUES ( 331, 'List', 'Attributes', '' );

INSERT INTO sword.create_objects   (id, team_id, name, type, session_id) VALUES ( 321, 3, 'Abattis', 'abattis', 1 );
INSERT INTO sword.actionparameters (reference_id, type, name, value) VALUES ( 321, 'String', 'Type', 'abattis' );
INSERT INTO sword.actionparameters (id, reference_id, type, name, value) VALUES ( 30201, 321, 'Location', 'Location', 'actionparameters_area' );
INSERT INTO sword.actionparameters_area (parameter_id, shape) VALUES ( 
    30201, st_polygon( 'polygon(( 0.1283868466796875 49.52336174621582,
                                      0.11705719580078125 49.52353340759277,
                                      0.12169205297851562 49.51735359802246, 0.1283868466796875 49.52336174621582 ))', 0 ) );
INSERT INTO sword.actionparameters (reference_id, type, name, value) VALUES ( 321, 'String', 'Name', 'Abattis 1' );
INSERT INTO sword.actionparameters (reference_id, type, name, value) VALUES ( 321, 'Army', 'Camp', '3' );
INSERT INTO sword.actionparameters (id, reference_id, type, name, value) VALUES ( 30222, 321, 'List', 'Attributes', '' );
INSERT INTO sword.actionparameters (id, reference_id, type, name, value) VALUES ( 300222, 30222, 'List', 'Obstacle', '' );
INSERT INTO sword.actionparameters (reference_id, type, name, value) VALUES ( 300222, 'Identifier', 'AttributeId',  '1' );
INSERT INTO sword.actionparameters (reference_id, type, name, value) VALUES ( 300222, 'Identifier', 'TargetType',  '1' );
INSERT INTO sword.actionparameters (reference_id, type, name, value) VALUES ( 300222, 'Bool', 'Activation',  '0' );
INSERT INTO sword.actionparameters (reference_id, type, name, value) VALUES ( 300222, 'Quantity', 'ActivationTime',  '43200' );

