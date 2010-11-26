ALTER TABLE sword.emergencies           	DROP CONSTRAINT emergencies_exercise_id_ctrt;
ALTER TABLE sword.formations            	DROP CONSTRAINT formations_exercise_id_ctrt;
ALTER TABLE sword.unitforces            	DROP CONSTRAINT unitforces_exercise_id_ctrt;
ALTER TABLE sword.unitforces            	DROP CONSTRAINT unitforces_op_state_ctrt;
ALTER TABLE sword.knowledgeunits        	DROP CONSTRAINT knowledgeunits_exercise_id_ctrt;

ALTER TABLE sword.orders                	DROP CONSTRAINT orders_exercise_id_ctrt;
ALTER TABLE sword.orderparameters       	DROP CONSTRAINT orderparameters_order_id_ctrt;
ALTER TABLE sword.orderparameters_area  	DROP CONSTRAINT orderparameters_area_order_id_ctrt;
ALTER TABLE sword.orderparameters_line  	DROP CONSTRAINT orderparameters_line_order_id_ctrt;
ALTER TABLE sword.orderparameters_point 	DROP CONSTRAINT orderparameters_point_order_id_ctrt;

ALTER TABLE sword.reports               	DROP CONSTRAINT reports_exercise_id_ctrt;

ALTER TABLE sword.knowledgeobjects 				DROP CONSTRAINT knowledge_unique_public_oid;
ALTER TABLE sword.knowledgeobjects 				DROP CONSTRAINT knowledgeobjects_session_id_ctrt;
ALTER TABLE sword.knowledgeobjects_point	DROP CONSTRAINT knowledgeobjects_point_ctrt;
ALTER TABLE sword.knowledgeobjects_area 	DROP CONSTRAINT knowledgeobjects_area_ctrt;
ALTER TABLE sword.knowledgeobjects_line 	DROP CONSTRAINT knowledgeobjects_line_ctrt;
