ALTER TABLE sword.urban_blocks                ADD CONSTRAINT urban_blocks_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.resource_network            ADD CONSTRAINT resource_network_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.resource_network_link       ADD CONSTRAINT resource_network_id_ctrt FOREIGN KEY(network_id) REFERENCES sword.resource_network(id) ON DELETE CASCADE;

ALTER TABLE sword.users                       ADD CONSTRAINT users_client_id_ctrt FOREIGN KEY(client_id) REFERENCES sword.clients(id) ON DELETE CASCADE;
ALTER TABLE sword.user_sessions               ADD CONSTRAINT usersessions_user_id_ctrt FOREIGN KEY(user_id) REFERENCES sword.users(id) ON DELETE CASCADE;
ALTER TABLE sword.user_sessions               ADD CONSTRAINT usersessions_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.user_exercises              ADD CONSTRAINT userexercises_user_id_ctrt FOREIGN KEY(user_id) REFERENCES sword.users(id) ON DELETE CASCADE;
ALTER TABLE sword.user_exercises              ADD CONSTRAINT userexercises_exercise_id_ctrt FOREIGN KEY(exercise_id) REFERENCES sword.exercises(id) ON DELETE CASCADE;

ALTER TABLE sword.sessions                    ADD CONSTRAINT sessions_exercise_id_ctrt FOREIGN KEY(exercise_id) REFERENCES sword.exercises(id) ON DELETE CASCADE;
ALTER TABLE sword.layers                      ADD CONSTRAINT layers_exercise_id FOREIGN KEY (exercise_id) REFERENCES sword.exercises (id) ON DELETE CASCADE;
ALTER TABLE sword.layers_properties           ADD CONSTRAINT layers_properties_id FOREIGN KEY (id) REFERENCES sword.layers (id) ON DELETE CASCADE;

----------------------
ALTER TABLE sword.activities                  ADD CONSTRAINT activities_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.profiles                    ADD CONSTRAINT profiles_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;

----------------------
ALTER TABLE sword.teams                       ADD CONSTRAINT teams_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.formations                  ADD CONSTRAINT formations_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.unitforces                  ADD CONSTRAINT unitforces_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.unitforces                  ADD CONSTRAINT unitforces_op_state_ctrt CHECK (op_state >= 0 AND op_state <= 100);
ALTER TABLE sword.knowledgeunits              ADD CONSTRAINT knowledgeunits_exercise_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;

ALTER TABLE sword.create_order                ADD CONSTRAINT create_order_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.create_object               ADD CONSTRAINT create_object_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;

ALTER TABLE sword.reports                     ADD CONSTRAINT reports_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;

----------------------
ALTER TABLE sword.boundarylimits              ADD CONSTRAINT boundarylimits_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.boundarylimits_creation     ADD CONSTRAINT boundarylimits_creation_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.tacticallines               ADD CONSTRAINT tacticallines_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;

ALTER TABLE sword.objects                     ADD CONSTRAINT objects_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.objectparameters            ADD CONSTRAINT objectparameters_object_id_ctrt FOREIGN KEY(object_id) REFERENCES sword.objects(id) ON DELETE CASCADE;

ALTER TABLE sword.knowledgeobjects            ADD CONSTRAINT knowledgeobjects_session_id_ctrt FOREIGN KEY (session_id) REFERENCES sword.sessions (id) MATCH SIMPLE ON UPDATE NO ACTION ON DELETE CASCADE;
