ALTER TABLE sword.users                        DROP CONSTRAINT users_client_id_ctrt CASCADE;
ALTER TABLE sword.extents                      DROP CONSTRAINT extents_exercise_id_ctrt             CASCADE;
ALTER TABLE sword.user_sessions                DROP CONSTRAINT usersessions_user_id_ctrt             CASCADE;
ALTER TABLE sword.user_sessions                DROP CONSTRAINT usersessions_session_id_ctrt     CASCADE;
ALTER TABLE sword.user_exercises               DROP CONSTRAINT userexercises_user_id_ctrt         CASCADE;
ALTER TABLE sword.user_exercises               DROP CONSTRAINT userexercises_exercise_id_ctrt CASCADE;

ALTER TABLE sword.sessions                     DROP CONSTRAINT sessions_exercise_id_ctrt CASCADE;

----------------------
ALTER TABLE sword.activities                   DROP CONSTRAINT activities_session_id_ctrt CASCADE;
ALTER TABLE sword.profiles                     DROP CONSTRAINT profiles_session_id_ctrt CASCADE;

ALTER TABLE sword.serial_grid                  DROP CONSTRAINT terrain_exercise_id_ctrt CASCADE;

----------------------
ALTER TABLE sword.teams                        DROP CONSTRAINT teams_session_id_ctrt CASCADE;
ALTER TABLE sword.formations                   DROP CONSTRAINT formations_session_id_ctrt CASCADE;
ALTER TABLE sword.unitforces                   DROP CONSTRAINT unitforces_session_id_ctrt CASCADE;
ALTER TABLE sword.unitforces                   DROP CONSTRAINT unitforces_op_state_ctrt CASCADE; 
ALTER TABLE sword.knowledgeunits               DROP CONSTRAINT knowledgeunits_exercise_id_ctrt CASCADE;

ALTER TABLE sword.create_order                 DROP CONSTRAINT create_order_session_id_ctrt CASCADE;
ALTER TABLE sword.create_object                DROP CONSTRAINT create_object_session_id_ctrt CASCADE;

ALTER TABLE sword.objects                      DROP CONSTRAINT objects_session_id_ctrt;
ALTER TABLE sword.objectparameters             DROP CONSTRAINT objectparameters_object_id_ctrt;

ALTER TABLE sword.reports                      DROP CONSTRAINT reports_session_id_ctrt CASCADE;

----------------------
ALTER TABLE sword.boundarylimits               DROP CONSTRAINT boundarylimits_session_id_ctrt CASCADE;
ALTER TABLE sword.boundarylimits_creation      DROP CONSTRAINT boundarylimits_creation_session_id_ctrt CASCADE;
ALTER TABLE sword.tacticallines                DROP CONSTRAINT tacticallines_session_id_ctrt CASCADE;

ALTER TABLE sword.knowledgeobjects             DROP CONSTRAINT knowledgeobjects_session_id_ctrt CASCADE;

ALTER TABLE sword.layers                       DROP CONSTRAINT layers_exercise_id CASCADE;
ALTER TABLE sword.layers_properties            DROP CONSTRAINT layers_properties_id CASCADE;

ALTER TABLE sword.magic_action                 DROP CONSTRAINT magic_action_session_id_ctrt CASCADE;
