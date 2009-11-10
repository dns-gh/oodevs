ALTER TABLE sword.emergencies RENAME COLUMN exercise_id TO session_id;
ALTER TABLE sword.formations RENAME COLUMN exercise_id TO session_id;
ALTER TABLE sword.unitforces RENAME COLUMN exercise_id TO session_id;
ALTER TABLE sword.knowledgeunits RENAME COLUMN exercise_id TO session_id;
ALTER TABLE sword.orders RENAME COLUMN exercise_id TO session_id;
ALTER TABLE sword.population RENAME COLUMN exercise_id TO session_id;
ALTER TABLE sword.reports RENAME COLUMN exercise_id TO session_id;

ALTER TABLE sword.activities ADD COLUMN session_id INTEGER;
ALTER TABLE sword.profiles ADD COLUMN session_id INTEGER;

