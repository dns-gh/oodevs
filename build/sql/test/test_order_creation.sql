
DELETE FROM sword.create_orders;
DELETE FROM sword.actionparameters;
DELETE FROM sword.actionparameters_point;
DELETE FROM sword.actionparameters_line;
DELETE FROM sword.actionparameters_area;

-- DELETE FROM sword.sessions WHERE id=1;
-- DELETE FROM sword.exercises WHERE id=1;

-- INSERT INTO sword.exercises (id, "name" ) VALUES (1, 'Indiana' );
-- INSERT INTO sword.sessions (id, "name", exercise_id) VALUES (1, 'default', 1 );

-- run on lehavre

INSERT INTO sword.create_orders (id, type_id, name, unit_id, checked, session_id) VALUES (321, 133, 'Move', 340, -1, 1 );
INSERT INTO sword.actionparameters (id, reference_id, type, name, value) VALUES (3201, 321, 'Heading', 'direction dangereuse', 0 );
INSERT INTO sword.actionparameters (id, reference_id, type, name, value) VALUES (3202, 321, 'Phaseline', 'Limas', 'actionparameters_line' );
INSERT INTO sword.actionparameters (id, reference_id, type, name, value) VALUES (3203, 321, 'Limit', 'Limit 1', 'actionparameters_line' );
INSERT INTO sword.actionparameters (id, reference_id, type, name, value) VALUES (3204, 321, 'Limit', 'Limit 2', 'actionparameters_line' );
-- INSERT INTO sword.actionparameters (id, reference_id, type, name, value) VALUES (3205, 321, 'Intelligence', 'Renseignements', null );
INSERT INTO sword.actionparameters (id, reference_id, type, name, value) VALUES (3206, 321, 'Path', 'Route', 'actionparameters_point' );


INSERT INTO sword.actionparameters_point (parameter_id, shape) VALUES ( 3206, ST_SetSRID(st_point( 0.14297806372070312, 49.53863960876465 ), 4326) );

INSERT INTO sword.actionparameters_line (parameter_id, shape) VALUES ( 3203, 
             ST_SetSRID( st_linefromtext( 'linestring( 0.1283868466796875 49.72336174621582, 0.10705719580078125 49.92353340759277 )' ), 4326 ) );

INSERT INTO sword.actionparameters_line (parameter_id, shape) VALUES ( 3204,
             ST_SetSRID( st_linefromtext( 'linestring( 0.1283868466796875 49.52336174621582, 0.11705719580078125 49.52353340759277 )' ), 4326 ) );


INSERT INTO sword.create_orders (id, type_id, name, unit_id, checked, session_id) VALUES (322, 184, 'Emergency - provide first aid', 339, -1, 1 );
INSERT INTO sword.actionparameters (id, reference_id, type, name, value) VALUES (3221, 322, 'Heading', 'direction dangereuse', 0 );
INSERT INTO sword.actionparameters (id, reference_id, type, name, value) VALUES (3222, 322, 'Phaseline', 'Limas', 'actionparameters_line' );
INSERT INTO sword.actionparameters (id, reference_id, type, name, value) VALUES (3223, 322, 'Limit', 'Limit 1', 'actionparameters_line' );
INSERT INTO sword.actionparameters (id, reference_id, type, name, value) VALUES (3224, 322, 'Limit', 'Limit 2', 'actionparameters_line' );
-- INSERT INTO sword.actionparameters (id, reference_id, type, name, value) VALUES (3225, 322, 'Intelligence', 'Renseignements', null );
INSERT INTO sword.actionparameters (id, reference_id, type, name, value) VALUES (3226, 322, 'Point', 'Position', 'actionparameters_point' );

INSERT INTO sword.actionparameters_point (parameter_id, shape) VALUES ( 3226, 
             ST_SetSRID(st_point( 0.14297806372070312, 49.53863960876465 ), 4326) );
INSERT INTO sword.actionparameters_line(parameter_id, shape) VALUES ( 3223, 
             ST_SetSRID( st_linefromtext('linestring( 0.1283868466796875 49.72336174621582, 0.10705719580078125 49.92353340759277 )' ), 4326 ) );
INSERT INTO sword.actionparameters_line (parameter_id, shape) VALUES ( 3224,
             sT_SetSRID( st_linefromtext( 'linestring( 0.1283868466796875 49.52336174621582, 0.11705719580078125 49.52353340759277 )' ), 4326 ) );