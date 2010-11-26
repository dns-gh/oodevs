-- Table: sde.exercise_Emergencies
-- DROP TABLE sde.exercise_Emergencies;

DELETE FROM sde."exercise_orders";
DELETE FROM sde."exercise_order_parameters";
DELETE FROM sde."exercise_order_parameter_area";
DELETE FROM sde."exercise_order_parameter_line";
DELETE FROM sde."exercise_order_parameter_point";
DELETE FROM sde."exercise_orders_validation";

DELETE FROM sde."exercise_tactical_object_area_request";
DELETE FROM sde."exercise_tactical_object_line_request";
DELETE FROM sde."exercise_tactical_object_point_request";

DELETE FROM sde."exercise_formations";
DELETE FROM sde."exercise_unit_forces";
DELETE FROM sde."exercise_knowledge_units";
DELETE FROM sde."exercise_reports";
DELETE FROM sde."exercise_emergencies";

DELETE FROM sde."exercise_boundary_limits";
DELETE FROM sde."exercise_tactical_lines";

DELETE FROM sde."exercise_tactical_object_area";
DELETE FROM sde."exercise_tactical_object_line";
DELETE FROM sde."exercise_tactical_object_point";

DELETE FROM sde."exercise_simulation_properties";

