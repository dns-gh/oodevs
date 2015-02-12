-------------------------------------------------------------------
---- BYPASSABLE INTERFACE IMPLEMENTATION
-------------------------------------------------------------------


--- Returns 'true' if this agent has the physical ability to bypass the given object, 'false' otherwise.
-- This method can only be called by an agent.
-- @param object Object knowledge
-- @return Boolean, whether or not unit has the capacity to bypass this object
integration.canBypassIt = function( object )
    return _DEC_Agent_PeutConstruireContournementObjet( myself, object.source )
end

--- Returns 'true' if this agent (including mounted equipments) has the physical ability to bypass the given object, 'false' otherwise.
-- This method can only be called by an agent.
-- @param object a DirectIA object knowledge
-- @param agent a Simulation agent (optional)
-- @return Boolean
integration.canBypassItWithMountedEquipments = function( object, agent )
    return _DEC_Agent_PeutConstruireContournementObjetAvecComposantesEmbarquees( agent or myself, object.source )
end

--- Returns true if the object knowledge can be bypassed, false otherwise
-- @param object Object knowledge
-- @return Boolean, the capacity as defined in the physical database
integration.canBeBypassed = function( object )
    return DEC_ConnaissanceObjet_PeutEtreContourne( object.source )
end

--- Returns the bypass level for the selected object knowledge
-- @param object Object knowledge
-- @return the bypass level between 0 and 100 (in fact 0 or 100: if value is 100 the object is bypassed, if value is 0 the object is not bypassed)
integration.bypassLevel = function( object )
    return DEC_ConnaissanceObjet_EstContourne( object.source ) == eTristate_True and 100 or 0
end

--- Allows the unit to begin work to permit bypassing the selected object
-- It will take time depending on the unit ability as defined in the physical database. Two reports are sent
-- @see integration.updateBypassIt
-- @see integration.stopBypassIt
-- @param object Object knowledge
integration.startBypassIt = function( object )
    reportFunction(eRC_EnContournementObstacle )
    object[myself] = object[myself] or {}
    object[myself].actionBypass = _DEC_StartCreerContournementObjet( myself, object.source )
    actionCallbacks[ object[myself].actionBypass ] = function( arg ) object[myself].actionBypassState = arg end
    
    reportFunction(eRC_DebutTravaux )
end

--- Continue work to permit bypassing the selected object knowledge
-- @see integration.startBypassIt
-- @see integration.stopBypassIt
-- @param object Object knowledge
-- @return Boolean, true if work is in progress, false if work cannot be finished
integration.updateBypassIt = function( object )
    if object[myself].actionBypassState == eActionObjetImpossible then
        return false 
    elseif object[myself].actionBypassState == eActionObjetManqueDotation then
        return false 
    elseif object[myself].actionBypassState == eActionObjetPasDeCapacite then
        return false 
    end
    return true
end

--- Allows the unit to stop work to permit bypassing the selected object knowledge
-- A report is sent when work is finished
-- @integration.startBypassIt
-- @see integration.updateBypassIt
-- @param object Object knowledge
-- @return Boolean, true if work is completed or impossible, false if work is paused (it could be resumed later)
integration.stopBypassIt = function( object )
    object[myself] = object[myself] or {}
    reportFunction(eRC_FinTravaux )
    if object[myself].actionBypassState == eActionObjetTerminee then
        object[myself].actionBypass = _DEC__StopAction( myself, object[myself].actionBypass )
        object[myself].actionBypassState = nil
        return true
    else
        object[myself].actionBypass = _DEC__StopAction( myself, object[myself].actionBypass )
        object[myself].actionBypassState = nil
        _DEC_Trace( myself, "pause work bypass" )
        return false
    end
end

--- Returns true if the unit has the ability to do work to permit bypassing the selected object, false otherwise
-- @param entity Simulation agent
-- @param objectType String, the type of the object see ObjectNames.xml and Objects.xml in physical database
-- @return Boolean, the availability as defined in the physical database
integration.canBuildBypassingObject = function( entity, objectType )
    return DEC_Agent_AgentPeutConstruireContournementObjet( entity, objectType )
end
