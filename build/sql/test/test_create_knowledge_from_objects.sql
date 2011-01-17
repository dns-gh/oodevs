--http://www.faqs.org/docs/ppbook/x19832.htm
CREATE or REPLACE function sword.createObjectKnowledgeFromObjects( sessionid integer) RETURNS integer AS
$BODY$ 
BEGIN

-- Clean knowledges
DELETE FROM sword.knowledgeobjects where session_id = sessionid;

--SELECT (public_oid, symbol_id, name, type, state, session_id) FROM sword.tacticalobject_area WHERE session_id = sessionid;

-- Copy Point Object as Knowledges
INSERT INTO sword.knowledgeobjects ( public_oid, symbol_id, name, type, state, session_id ) -- observer_affiliation
       SELECT public_oid, symbol_id, name, type, state, session_id FROM sword.tacticalobject_area WHERE session_id = sessionid; -- observer_affiliation
INSERT INTO sword.knowledgeobjects_area ( public_oid, session_id, shape )
       SELECT public_oid, session_id, shape FROM sword.tacticalobject_area WHERE session_id = sessionid;


INSERT INTO sword.knowledgeobjects ( public_oid, symbol_id, name, type, state, session_id ) -- observer_affiliation
       SELECT public_oid, symbol_id, name, type, state, session_id FROM sword.tacticalobject_point WHERE session_id = sessionid; -- observer_affiliation
INSERT INTO sword.knowledgeobjects_point ( public_oid, session_id, shape )
       SELECT public_oid, session_id, shape FROM sword.tacticalobject_point WHERE session_id = sessionid;


INSERT INTO sword.knowledgeobjects ( public_oid, symbol_id, name, type, state, session_id ) -- observer_affiliation
       SELECT public_oid, symbol_id, name, type, state, session_id FROM sword.tacticalobject_line WHERE session_id = sessionid; -- observer_affiliation
INSERT INTO sword.knowledgeobjects_line ( public_oid, session_id, shape )
       SELECT public_oid, session_id, shape FROM sword.tacticalobject_line WHERE session_id = sessionid;


-- Unit Knowledges
--INSERT INTO sword.knowledgeunits (symbol_id, public_oid, group_oid, unit_oid, speed, name, parent, effective, quantity, signature, direction, identification_level, observer_affiliation, shape, session_id, dead, observer_oid) VALUES ('SUZP-----------', 1000, 13, 14, 0, NULL, NULL, NULL, NULL, NULL, 0, 2, 'H', '280000000100000001000400000000000C00000001000000808A98C2C41780A395B39F1A00000000', sessionid, false, NULL);
--INSERT INTO sword.knowledgeunits (symbol_id, public_oid, group_oid, unit_oid, speed, name, parent, effective, quantity, signature, direction, identification_level, observer_affiliation, shape, session_id, dead, observer_oid) VALUES ('SFGPUST----C---', 1001, 13, 46, 0, NULL, NULL, NULL, NULL, NULL, 0, 2, 'H', '280000000100000001000400000000000C00000001000000A0CE9CC0C417A0AEC4A99F1A00000000', sessionid, false, NULL);

-- Objects
--SELECT (public_oid, symbol_id, name, type, state, shape, session_id) FROM sword.tacticalobject_area;

  
RETURN 1;
END;
$BODY$ LANGUAGE 'plpgsql' VOLATILE;

-- Execute addition of test data
select * from sword.createObjectKnowledgeFromObjects( 2 );
