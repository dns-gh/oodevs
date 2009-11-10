--http://www.faqs.org/docs/ppbook/x19832.htm
CREATE or REPLACE function sword.createTestData( sessionid integer) RETURNS integer AS
$BODY$ 
BEGIN

DELETE FROM sword.exercises where name='test_case_exercise'; -- assume all constraints have been properly define

-- Exercise, session, and demo user
INSERT INTO sword.exercises (id, name, description) VALUES (0, 'test_case_exercise', 'an exercise for test cases');
INSERT INTO sword.sessions (id, name, exercise_id) VALUES (sessionid, 'test_case_session', 0);
INSERT INTO sword.user_sessions (user_id, session_id) VALUES (5, sessionid); -- user 5 has 'demo' crypto


-- Unit forces - ID is a serial, autoincremented
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (30, 23, 50, 'Chirurgicale section (modular)', 'SFGPUSM----D---', NULL, 100, '280000000100000001000400000000000C0000000100000080D7E1AFC417A0D2C2FC9E1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (31, 23, 53, 'Chirurgicale section HMC', 'SFGPUSM----D---', NULL, 100, '280000000100000001000400000000000C0000000100000090E1FDA8C417A0F5E6FA9E1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (32, 23, 52, 'Casualty assessment section', 'SFGPUSM----D---', NULL, 100, '280000000100000001000400000000000C0000000100000090CF98A2C417A0D2C2FC9E1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (34, 33, 25, 'Supply CP (division)', 'SFGPUSS---AD---', NULL, 100, '280000000100000001000400000000000C0000000100000080CBD0B6C41780B7DCA79F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (35, 33, 38, 'Ammo supply group SOFF TT - D', 'SFGPUSS5---C---', NULL, 100, '280000000100000001000400000000000C0000000100000090F9B6BDC417A08EB7A39F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (36, 33, 37, 'Ammo supply group OFF TT- D', 'SFGPUSS5---C---', NULL, 100, '280000000100000001000400000000000C0000000100000090E7D1B6C41780E6919F9F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (37, 33, 39, 'Water supply group', 'SFGPUSS2---C---', NULL, 0, '280000000100000001000400000000000C0000000100000080B9EBAFC417A08EB7A39F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (38, 33, 40, 'Ammo supply group', 'SFGPUSS5---C---', NULL, 100, '280000000100000001000400000000000C0000000100000090F9B6BDC41780B7DCA79F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (39, 33, 41, 'Ammo supply group SOFF TD', 'SFGPUSS5---C---', NULL, 100, '280000000100000001000400000000000C00000001000000B0EA9DC0C417A08EB7A39F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (40, 33, 45, 'Supply Transportation platoon (PT)', 'SFGPUST----D---', NULL, 100, '280000000100000001000400000000000C0000000100000090F9B6BDC41780E6919F9F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (41, 33, 44, 'Supply platoon (PR)', 'SFGPUST----D---', NULL, 100, '280000000100000001000400000000000C0000000100000090E7D1B6C4178089B69D9F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (42, 33, 43, 'Supply group 60m3', 'SFGPUST----C---', NULL, 100, '280000000100000001000400000000000C0000000100000090D5ECAFC41780E6919F9F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (43, 33, 42, 'Magazin section CAP', 'SFGPUSS2---C---', NULL, 100, '280000000100000001000400000000000C00000001000000A0C784ADC417A08EB7A39F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (44, 33, 62, 'Supply Transportation platoon', 'SFGPUST----D---', NULL, 100, '280000000100000001000400000000000C0000000100000080B9EBAFC41780B7DCA79F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (8, 7, 70, 'Armored squadron CP', 'SFGPUCAW--AD---', NULL, 100, '280000000100000001000400000000000C00000001000000A0FCCAF1C417A0BEB0A09F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (9, 7, 69, 'Light armored platoon', 'SFGPUCAWM--D---', NULL, 100, '280000000100000001000400000000000C00000001000000A08EB0F8C41780968B9C9F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (10, 7, 69, 'Light armored platoon', 'SFGPUCAWM--D---', NULL, 100, '280000000100000001000400000000000C0000000100000090E0C9F1C417A0EDE5979F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (11, 7, 69, 'Light armored platoon', 'SFGPUCAWM--D---', NULL, 100, '280000000100000001000400000000000C00000001000000B086E7EAC41780968B9C9F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (12, 7, 69, 'Light armored platoon', 'SFGPUCAWM--D---', NULL, 100, '280000000100000001000400000000000C00000001000000B0AAB1F8C417A0BEB0A09F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (16, 15, 36, 'Maintenance CP  (division)', 'SFGPUSX---AD---', NULL, 100, '280000000100000001000400000000000C0000000100000080BE83DFC41780C6CF809F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (17, 15, 56, 'Maintenance evacuation group', 'SFGPUSX----D---', NULL, 100, '280000000100000001000400000000000C00000001000000B0B3E7E5C417A09DAAFC9E1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (18, 15, 61, 'Repairs section (corps)', 'SFGPUSX----D---', NULL, 100, '280000000100000001000400000000000C0000000100000090DA84DFC41780F584F89E1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (19, 15, 60, 'Mobility repairs section (corps)', 'SFGPUSX----D---', NULL, 100, '280000000100000001000400000000000C00000001000000A0E4A0D8C417A09DAAFC9E1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (20, 15, 59, 'Firing mobility repairs section (corps)', 'SFGPUSX----D---', NULL, 100, '280000000100000001000400000000000C0000000100000080D0E8E5C41780C6CF809F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (22, 21, 28, 'Logistic CT', 'SFGPUSS---AD---', NULL, 100, '280000000100000001000400000000000C00000001000000B0A3C6D6C41780ED83959F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (24, 23, 26, 'Medical CP (division)', 'SFGPUSM---AD---', NULL, 100, '280000000100000001000400000000000C0000000100000080C5FCA8C417A0A38D859F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (25, 23, 46, 'Medical group VAB', 'SFGPUSM----C---', NULL, 100, '280000000100000001000400000000000C0000000100000080D7E1AFC41780FBE7809F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (26, 23, 47, 'Casualty evacuation group', 'SFGPUSM----C---', NULL, 100, '280000000100000001000400000000000C0000000100000090E1FDA8C417A0D2C2FC9E1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (27, 23, 51, 'Logistic decontamination section', 'SFGPUUAD---D---', NULL, 100, '280000000100000001000400000000000C0000000100000090CF98A2C41780FBE7809F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (28, 23, 49, 'Psychological section', 'SFGPUSMP---D---', NULL, 100, '280000000100000001000400000000000C0000000100000080D7E1AFC417A0A38D859F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (29, 23, 48, 'Medical section', 'SFGPUSM----D---', NULL, 100, '280000000100000001000400000000000C00000001000000A0C8C8B2C41780FBE7809F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (45, 33, 63, 'Supply group 35m3', 'SFGPUST----C---', NULL, 100, '280000000100000001000400000000000C0000000100000080CBD0B6C417A0AEC4A99F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (46, 33, 64, 'Supply group180m3', 'SFGPUST----C---', NULL, 100, '280000000100000001000400000000000C00000001000000A0CE9CC0C417A0AEC4A99F1A00000000', 360, 0, sessionid);
INSERT INTO sword.unitforces (public_oid, parent_oid, type, name, symbol_id, valid, op_state, shape, direction, speed, session_id) VALUES (14, 13, 18, 'Refugee', 'SFGPU-----A----', NULL, 100, '280000000100000001000400000000000C00000001000000808A98C2C41780A395B39F1A00000000', 360, 0, sessionid);

-- Formation
INSERT INTO sword.formations (public_oid, parent_oid, type, engaged, name, symbol_id, session_id) VALUES (5, 0, -1, 0, 'Formation [5]', '-F---------H---', sessionid);
INSERT INTO sword.formations (public_oid, parent_oid, type, engaged, name, symbol_id, session_id) VALUES (6, 0, -1, 0, 'Formation [6]', '-F---------H---', sessionid);
INSERT INTO sword.formations (public_oid, parent_oid, type, engaged, name, symbol_id, session_id) VALUES (7, 5, 138, 0, 'Light armored squadron [7]', '-F---------A---', sessionid);
INSERT INTO sword.formations (public_oid, parent_oid, type, engaged, name, symbol_id, session_id) VALUES (15, 5, 130, 0, 'Maintenance logistic area (corps) [15]', '-F---------A---', sessionid);
INSERT INTO sword.formations (public_oid, parent_oid, type, engaged, name, symbol_id, session_id) VALUES (21, 5, 127, 0, 'Logistic combat train [21]', '-F---------A---', sessionid);
INSERT INTO sword.formations (public_oid, parent_oid, type, engaged, name, symbol_id, session_id) VALUES (23, 5, 134, 0, 'Medical logistic area (corps) [23]', '-F---------A---', sessionid);
INSERT INTO sword.formations (public_oid, parent_oid, type, engaged, name, symbol_id, session_id) VALUES (33, 5, 132, 0, 'Supply logistic area  (corps) [33]', '-F---------A---', sessionid);
INSERT INTO sword.formations (public_oid, parent_oid, type, engaged, name, symbol_id, session_id) VALUES (13, 6, 136, 0, 'Refugees [13]', '-F---------A---', sessionid);

-- Objects
INSERT INTO sword.tacticalobject_area (id, symbol_id, public_oid, shape, session_id, name, type, state) VALUES (nextval('sword.tacticalobject_area_id_seq'::regclass), 'GFSPAH--------X', 47, sde.st_polygon('POLYGON ((4.36507 50.8529, 4.36979 50.8472, 4.38109 50.8517, 4.37545 50.855, 4.36507 50.8529))'), sessionid, 'ttttttttttttt', 'refugees camp', 0);
INSERT INTO sword.tacticalobject_area (id, symbol_id, public_oid, shape, session_id, name, type, state) VALUES (nextval('sword.tacticalobject_area_id_seq'::regclass), 'GFSPAH--------X', 47, sde.st_polygon('POLYGON ((4.36507 50.7529, 4.36979 50.7472, 4.38109 50.7517, 4.37545 50.755, 4.36507 50.7529))'), sessionid, 'ttttttttttttt', 'refugees camp', 0);
INSERT INTO sword.tacticalobject_area (id, symbol_id, public_oid, shape, session_id, name, type, state) VALUES (nextval('sword.tacticalobject_area_id_seq'::regclass), 'GFSPAH--------X', 47, sde.st_polygon('POLYGON ((4.36507 50.6529, 4.36979 50.6472, 4.38109 50.6517, 4.37545 50.655, 4.36507 50.6529))'), sessionid, 'ttttttttttttt', 'refugees camp', 0);

-- Tactical lines
INSERT INTO sword.tacticallines (public_oid, symbol_id, shape, session_id, name) VALUES (1500, 'TCTCL------', sde.st_linestring('LINESTRING (4.3 50.8, 4.31 50.84, 4.29 50.85)'), sessionid, NULL);

-- Boundary limits
INSERT INTO sword.boundarylimits (public_oid, symbol_id, shape, session_id, name) VALUES (1600, 'TCTCL------', sde.st_linestring('LINESTRING (6.32 51.87, 4.33 50.87, 3.34 50.8745)'), sessionid, NULL);
INSERT INTO sword.boundarylimits (public_oid, symbol_id, shape, session_id, name) VALUES (1601, 'TCTCL------', sde.st_linestring('LINESTRING (5.4 53.9, 5.5 51.99, 4.55 50.99)'), sessionid, NULL);

-------------------------------
-- Object Knowledges
INSERT INTO sword.knowledgeobjects (public_oid, symbol_id, team_id, session_id, name, type, observer_affiliation, state) VALUES (34, 'GUSPAH--------X', 3, sessionid, 'ttttttttttttt', 'refugees camp', NULL, 0);
INSERT INTO sword.knowledgeobjects_area (shape, public_oid, session_id) VALUES (sde.st_polygon('POLYGON ((4.36507 50.8529, 4.36979 50.8472, 4.38109 50.8517, 4.37545 50.855, 4.36507 50.8529))'), 34, sessionid);
-- refugee camp is part of side1 in orbat, but a knowledge for side3 (refugees)


-- Unit Knowledges
INSERT INTO sword.knowledgeunits (symbol_id, public_oid, group_oid, unit_oid, speed, name, parent, effective, quantity, signature, direction, identification_level, observer_affiliation, shape, session_id, dead, observer_oid) VALUES ('SUZP-----------', 1000, 3, 14, 0, NULL, NULL, NULL, NULL, NULL, 0, 2, 'H', '280000000100000001000400000000000C00000001000000808A98C2C41780A395B39F1A00000000', sessionid, false, NULL);
-- refugee knowledge is supposed to be held by knowledge group 3 in side 1
INSERT INTO sword.knowledgeunits (symbol_id, public_oid, group_oid, unit_oid, speed, name, parent, effective, quantity, signature, direction, identification_level, observer_affiliation, shape, session_id, dead, observer_oid) VALUES ('SFGPUST----C---', 1001, 4, 46, 0, NULL, NULL, NULL, NULL, NULL, 0, 2, 'H', '280000000100000001000400000000000C00000001000000A0CE9CC0C417A0AEC4A99F1A00000000', sessionid, false, NULL);
-- unit knowledge is supposed to be held by knowledge group 4 in side 3 (refugee sides)

  
RETURN 1;
END;
$BODY$ LANGUAGE 'plpgsql' VOLATILE;

-- Execute addition of test data
select * from sword.createTestData( 2000 );
