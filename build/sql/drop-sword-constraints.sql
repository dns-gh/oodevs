ALTER TABLE sword.users                        DROP CONSTRAINT users_client_id_ctrt CASCADE;
ALTER TABLE sword.extents                      DROP CONSTRAINT extents_exercise_id_ctrt 			CASCADE;
ALTER TABLE sword.user_sessions                DROP CONSTRAINT usersessions_user_id_ctrt 			CASCADE;
ALTER TABLE sword.user_sessions                DROP CONSTRAINT usersessions_session_id_ctrt 	CASCADE;
ALTER TABLE sword.user_exercises               DROP CONSTRAINT userexercises_user_id_ctrt 		CASCADE;
ALTER TABLE sword.user_exercises               DROP CONSTRAINT userexercises_exercise_id_ctrt CASCADE;

ALTER TABLE sword.sessions                     DROP CONSTRAINT sessions_exercise_id_ctrt CASCADE;

ALTER TABLE sword.emergencies                  DROP CONSTRAINT emergencies_session_id_ctrt CASCADE;

----------------------
ALTER TABLE sword.population                   DROP CONSTRAINT population_session_id_ctrt CASCADE;
ALTER TABLE sword.population_attributes        DROP CONSTRAINT population_id_ctrt CASCADE;
ALTER TABLE sword.activities                   DROP CONSTRAINT activities_session_id_ctrt CASCADE;
ALTER TABLE sword.profiles                     DROP CONSTRAINT profiles_session_id_ctrt CASCADE;

ALTER TABLE sword.serial_grid                 DROP CONSTRAINT terrain_exercise_id_ctrt CASCADE;

----------------------
ALTER TABLE sword.teams                        DROP CONSTRAINT teams_session_id_ctrt CASCADE;
ALTER TABLE sword.formations                   DROP CONSTRAINT formations_session_id_ctrt CASCADE;
ALTER TABLE sword.unitforces                   DROP CONSTRAINT unitforces_session_id_ctrt CASCADE;
ALTER TABLE sword.unitforces                   DROP CONSTRAINT unitforces_op_state_ctrt CASCADE; 
ALTER TABLE sword.knowledgeunits               DROP CONSTRAINT knowledgeunits_exercise_id_ctrt CASCADE;

ALTER TABLE sword.create_orders                DROP CONSTRAINT orders_session_id_ctrt CASCADE;
ALTER TABLE sword.create_objects               DROP CONSTRAINT create_tacticalobject_area_session_id_ctrt CASCADE;

ALTER TABLE sword.actionparameters_area         DROP CONSTRAINT actionparameters_area_order_id_ctrt  CASCADE;
ALTER TABLE sword.actionparameters_line         DROP CONSTRAINT actionparameters_line_order_id_ctrt  CASCADE;
ALTER TABLE sword.actionparameters_point        DROP CONSTRAINT actionparameters_point_order_id_ctrt CASCADE;

ALTER TABLE sword.reports                      DROP CONSTRAINT reports_session_id_ctrt CASCADE;

----------------------
ALTER TABLE sword.boundarylimits               DROP CONSTRAINT boundarylimits_session_id_ctrt CASCADE;
ALTER TABLE sword.boundarylimits_creation      DROP CONSTRAINT boundarylimits_creation_session_id_ctrt CASCADE;
ALTER TABLE sword.tacticallines                DROP CONSTRAINT tacticallines_session_id_ctrt CASCADE;



ALTER TABLE sword.create_tacticalobject_area   DROP CONSTRAINT create_tacticalobject_area_session_id_ctrt CASCADE;
ALTER TABLE sword.create_tacticalobject_line   DROP CONSTRAINT create_tacticalobject_line_session_id_ctrt CASCADE;
ALTER TABLE sword.create_tacticalobject_point  DROP CONSTRAINT create_tacticalobject_point_session_id_ctrt CASCADE;

ALTER TABLE sword.tacticalobject_area          DROP CONSTRAINT create_tacticalobject_area_session_id_ctrt CASCADE;
ALTER TABLE sword.tacticalobject_line          DROP CONSTRAINT create_tacticalobject_line_session_id_ctrt CASCADE;
ALTER TABLE sword.tacticalobject_point         DROP CONSTRAINT create_tacticalobject_point_session_id_ctrt CASCADE;


ALTER TABLE sword.knowledgeobjects            DROP CONSTRAINT knowledge_unique_session_id_public_oid CASCADE;
ALTER TABLE sword.knowledgeobjects            DROP CONSTRAINT knowledgeobjects_session_id_ctrt CASCADE;

ALTER TABLE sword.knowledgeobjects_area       DROP CONSTRAINT knowledgeobjects_area_ctrt CASCADE;
ALTER TABLE sword.knowledgeobjects_line       DROP CONSTRAINT knowledgeobjects_line_ctrt CASCADE;
ALTER TABLE sword.knowledgeobjects_point      DROP CONSTRAINT knowledgeobjects_point_ctrt CASCADE;

ALTER TABLE sword.tacticalobject_attribute_activity_time    	DROP CONSTRAINT tacticalobject_attribute_activity_time_session_id_ctrt CASCADE;
ALTER TABLE sword.tacticalobject_attribute_bypass         		DROP CONSTRAINT tacticalobject_attribute_bypass_session_id_ctrt CASCADE;
ALTER TABLE sword.tacticalobject_attribute_construction     	DROP CONSTRAINT tacticalobject_attribute_construction_session_id_ctrt CASCADE;
ALTER TABLE sword.tacticalobject_attribute_crossing_site    	DROP CONSTRAINT tacticalobject_attribute_crossing_site_session_id_ctrt CASCADE;
ALTER TABLE sword.tacticalobject_attribute_fire         			DROP CONSTRAINT tacticalobject_attribute_fire_session_id_ctrt CASCADE;
ALTER TABLE sword.tacticalobject_attribute_interaction_height DROP CONSTRAINT tacticalobject_attribute_interaction_height_session_id_ctrt CASCADE;
ALTER TABLE sword.tacticalobject_attribute_logistic       		DROP CONSTRAINT tacticalobject_attribute_logistic_session_id_ctrt CASCADE;
ALTER TABLE sword.tacticalobject_attribute_medical_treatment  DROP CONSTRAINT tacticalobject_attribute_medical_treatment_session_id_ctrt CASCADE; 	
ALTER TABLE sword.tacticalobject_attribute_mine         			DROP CONSTRAINT tacticalobject_attribute_mine_session_id_ctrt CASCADE;
ALTER TABLE sword.tacticalobject_attribute_nbc          			DROP CONSTRAINT tacticalobject_attribute_nbc_session_id_ctrt 	CASCADE;						
ALTER TABLE sword.tacticalobject_attribute_nbc_type       		DROP CONSTRAINT tacticalobject_attribute_nbc_type_session_id_ctrt CASCADE; 					
ALTER TABLE sword.tacticalobject_attribute_obstacle       		DROP CONSTRAINT tacticalobject_attribute_obstacle_session_id_ctrt CASCADE;					
ALTER TABLE sword.tacticalobject_attribute_stock        			DROP CONSTRAINT tacticalobject_attribute_stock_session_id_ctrt CASCADE;							
ALTER TABLE sword.tacticalobject_attribute_supplyroute      	DROP CONSTRAINT tacticalobject_attribute_supplyroute_session_id_ctrt CASCADE;				
ALTER TABLE sword.tacticalobject_attribute_toxic_cloud      	DROP CONSTRAINT tacticalobject_attribute_toxic_cloud_session_id_ctrt CASCADE; 				
