ALTER TABLE sword.users                        ADD CONSTRAINT users_client_id_ctrt FOREIGN KEY(client_id) REFERENCES sword.clients(id) ON DELETE CASCADE;
ALTER TABLE sword.extents                      ADD CONSTRAINT extents_exercise_id_ctrt FOREIGN KEY(exercise_id) REFERENCES sword.exercises(id) ON DELETE CASCADE;
ALTER TABLE sword.user_sessions                ADD CONSTRAINT usersessions_user_id_ctrt FOREIGN KEY(user_id) REFERENCES sword.users(id) ON DELETE CASCADE;
ALTER TABLE sword.user_sessions                ADD CONSTRAINT usersessions_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;

ALTER TABLE sword.sessions                     ADD CONSTRAINT sessions_exercise_id_ctrt FOREIGN KEY(exercise_id) REFERENCES sword.exercises(id) ON DELETE CASCADE;

ALTER TABLE sword.emergencies                  ADD CONSTRAINT emergencies_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;

----------------------
ALTER TABLE sword.population                   ADD CONSTRAINT population_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.population_attributes        ADD CONSTRAINT population_id_ctrt FOREIGN KEY(population_id) REFERENCES sword.population(id) ON DELETE CASCADE;
ALTER TABLE sword.activities					         ADD CONSTRAINT activities_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.profiles  					         ADD CONSTRAINT profiles_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;

ALTER TABLE sword.terrain_grid  				       ADD CONSTRAINT terrain_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;

----------------------
ALTER TABLE sword.formations                   ADD CONSTRAINT formations_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.unitforces                   ADD CONSTRAINT unitforces_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.unitforces                   ADD CONSTRAINT unitforces_op_state_ctrt CHECK (op_state >= 0 AND op_state <= 100);
ALTER TABLE sword.knowledgeunits               ADD CONSTRAINT knowledgeunits_exercise_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;

ALTER TABLE sword.orders                       ADD CONSTRAINT orders_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.orderparameters              ADD CONSTRAINT orderparameters_order_id_ctrt FOREIGN KEY(order_id) REFERENCES sword.orders(id) ON DELETE CASCADE;
ALTER TABLE sword.orderparameters_area         ADD CONSTRAINT orderparameters_area_order_id_ctrt FOREIGN KEY(parameter_id) REFERENCES sword.orderparameters(id) ON DELETE CASCADE;
ALTER TABLE sword.orderparameters_line         ADD CONSTRAINT orderparameters_line_order_id_ctrt FOREIGN KEY(parameter_id) REFERENCES sword.orderparameters(id) ON DELETE CASCADE;
ALTER TABLE sword.orderparameters_point        ADD CONSTRAINT orderparameters_point_order_id_ctrt FOREIGN KEY(parameter_id) REFERENCES sword.orderparameters(id) ON DELETE CASCADE;

ALTER TABLE sword.reports                      ADD CONSTRAINT reports_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;

----------------------
ALTER TABLE sword.boundarylimits  			       ADD CONSTRAINT boundarylimits_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.boundarylimits_creation      ADD CONSTRAINT boundarylimits_creation_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.tacticallines    			       ADD CONSTRAINT tacticallines_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;

ALTER TABLE sword.create_tacticalobject_area   ADD CONSTRAINT create_tacticalobject_area_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.create_tacticalobject_line   ADD CONSTRAINT create_tacticalobject_line_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.create_tacticalobject_point  ADD CONSTRAINT create_tacticalobject_point_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;

ALTER TABLE sword.tacticalobject_area          ADD CONSTRAINT create_tacticalobject_area_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.tacticalobject_line          ADD CONSTRAINT create_tacticalobject_line_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;
ALTER TABLE sword.tacticalobject_point         ADD CONSTRAINT create_tacticalobject_point_session_id_ctrt FOREIGN KEY(session_id) REFERENCES sword.sessions(id) ON DELETE CASCADE;

-- ALTER TABLE sword.tacticalobject_attribute_activity_time TO sword;
-- ALTER TABLE sword.tacticalobject_attribute_bypass TO sword;
-- ALTER TABLE sword.tacticalobject_attribute_construction TO sword;
-- ALTER TABLE sword.tacticalobject_attribute_crossing_site TO sword;
-- ALTER TABLE sword.tacticalobject_attribute_fire TO sword;
-- ALTER TABLE sword.tacticalobject_attribute_interaction_height TO sword;
-- ALTER TABLE sword.tacticalobject_attribute_logistic TO sword;
-- ALTER TABLE sword.tacticalobject_attribute_medical_treatment TO sword;
-- ALTER TABLE sword.tacticalobject_attribute_mine TO sword;
-- ALTER TABLE sword.tacticalobject_attribute_nbc TO sword;
-- ALTER TABLE sword.tacticalobject_attribute_nbc_type TO sword;
-- ALTER TABLE sword.tacticalobject_attribute_obstacle TO sword;
-- ALTER TABLE sword.tacticalobject_attribute_stock TO sword;
-- ALTER TABLE sword.tacticalobject_attribute_supplyroute TO sword;
-- ALTER TABLE sword.tacticalobject_attribute_toxic_cloud TO sword;

ALTER TABLE sword.knowledgeobjects ADD CONSTRAINT knowledge_unique_session_id_public_oid UNIQUE (public_oid, session_id);
ALTER TABLE sword.knowledgeobjects ADD CONSTRAINT knowledgeobjects_session_id_ctrt FOREIGN KEY (session_id) REFERENCES sword.sessions (id) MATCH SIMPLE ON UPDATE NO ACTION ON DELETE CASCADE;
ALTER TABLE sword.knowledgeobjects_point ADD CONSTRAINT knowledgeobjects_point_ctrt FOREIGN KEY (public_oid, session_id) REFERENCES sword.knowledgeobjects (public_oid, session_id) MATCH SIMPLE ON UPDATE NO ACTION ON DELETE CASCADE;
ALTER TABLE sword.knowledgeobjects_area ADD CONSTRAINT knowledgeobjects_area_ctrt FOREIGN KEY (public_oid, session_id) REFERENCES sword.knowledgeobjects (public_oid, session_id) MATCH SIMPLE ON UPDATE NO ACTION ON DELETE CASCADE;
ALTER TABLE sword.knowledgeobjects_line ADD CONSTRAINT knowledgeobjects_line_ctrt FOREIGN KEY (public_oid, session_id) REFERENCES sword.knowledgeobjects (public_oid, session_id) MATCH SIMPLE ON UPDATE NO ACTION ON DELETE CASCADE;
ALTER TABLE sword.knowledgeobjects_area ADD CONSTRAINT knowledgeobjects_area_pk PRIMARY KEY (public_oid, session_id);
ALTER TABLE sword.knowledgeobjects_line ADD CONSTRAINT knowledgeobjects_line_pk PRIMARY KEY (public_oid, session_id);
ALTER TABLE sword.knowledgeobjects_point ADD CONSTRAINT knowledgeobjects_point_pk PRIMARY KEY (public_oid, session_id);