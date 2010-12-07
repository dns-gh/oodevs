
DELETE FROM sword.orders;
DELETE FROM sword.orderparameters;
DELETE FROM sword.orderparameters_point_test;
DELETE FROM sword.orderparameters_line_test;
DELETE FROM sword.orderparameters_area_test;

DELETE FROM sword.sessions WHERE id=2000;
DELETE FROM sword.exercises WHERE id=4242;

INSERT INTO sword.exercises (id, "name" ) VALUES (4242, 'test' );
INSERT INTO sword.sessions (id, "name", exercise_id) VALUES (2000, 'test-session', 4242 );

INSERT INTO sword.orders (id, unit_id, session_id) VALUES (321, 42, 2000 );

INSERT INTO sword.orderparameters (id, order_id, value) VALUES (321, 321, 'orderparameters_point_test' );
INSERT INTO sword.orderparameters_point_test (id, parameter_id, shape) VALUES
	(nextval('sword.orderparameters_point_test_id_seq'::regclass), 321, sde.st_point( 0.14297806372070312, 49.53863960876465, 0 ) );

INSERT INTO sword.orderparameters (id, order_id, value) VALUES (322, 321, 'orderparameters_line_test' );
INSERT INTO sword.orderparameters_line_test (id, parameter_id, shape) VALUES
	(nextval('sword.orderparameters_line_test_id_seq'::regclass), 322,
		sde.st_linestring( 'linestring( 0.1283868466796875 49.52336174621582, 0.11705719580078125 49.52353340759277 )', 0 ) );

INSERT INTO sword.orderparameters (id, order_id, value) VALUES (323, 321, 'orderparameters_area_test' );
INSERT INTO sword.orderparameters_area_test (id, parameter_id, shape) VALUES
	(nextval('sword.orderparameters_area_test_id_seq'::regclass), 323,
		sde.st_polygon( 'polygon(( 0.1283868466796875 49.52336174621582,
					  0.11705719580078125 49.52353340759277,
					  0.12169205297851562 49.51735359802246, 0.1283868466796875 49.52336174621582 ))', 0 ) );

INSERT INTO sword.orderparameters (id, order_id, value) VALUES (324, 321, 'orderparameters_point_test' );
INSERT INTO sword.orderparameters_point_test (id, parameter_id, shape) VALUES
	(nextval('sword.orderparameters_point_test_id_seq'::regclass), 324, sde.st_point( 0.14297806372070312, 49.53863960876465, 0 ) );
INSERT INTO sword.orderparameters_point_test (id, parameter_id, shape) VALUES
	(nextval('sword.orderparameters_point_test_id_seq'::regclass), 324, sde.st_point( 0.14297806372070312, 49.53863960876465, 0 ) );
INSERT INTO sword.orderparameters_point_test (id, parameter_id, shape) VALUES
	(nextval('sword.orderparameters_point_test_id_seq'::regclass), 324, sde.st_point( 0.14297806372070312, 49.53863960876465, 0 ) );

INSERT INTO sword.orderparameters (id, order_id, value) VALUES (325, 321, 'orderparameters_point_test' );
INSERT INTO sword.orderparameters_point_test (id, parameter_id, shape) VALUES
	(nextval('sword.orderparameters_point_test_id_seq'::regclass), 325, sde.st_point(-121.815033,36.645834, 0 ) );
