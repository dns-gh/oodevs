-------------------------------------------------------------------
---- ACTIVABLE INTERFACE IMPLEMENTATION
-------------------------------------------------------------------

--- Returns true if unit has the capacity to activate the provided object, false otherwise
-- @param object an object knowledge
-- @return Boolean, whether or not this entity can activate the object knowledge
integration.canActivateIt = function( object )
    if DEC_Agent_PeutActiverObjet( object.source ) then
        meKnowledge:RC( eRC_ActivationObstacleDeManoeuvre )
        return true
    else
        DEC_Trace( "object cannot be activated" )
        return false
    end
end

--- Returns the activation level of the provided object knowledge
-- @param object an object knowledge
-- @return Integer (between 0 and 100)
integration.activationLevel = function( object )
    return DEC_ConnaissanceObjet_EstObstacleDeManoeuvreActif( object.source ) and 100 or 0
end

--- Allows the unit to activate the provided object knowledge (e.g. mines are operationnals)
-- if object is activated, a report is displayed
-- @param object an object knowledge
-- @return Boolean, whether or not object knowledge is activated
integration.activateIt = function( object )
    if DEC_ActiverObjet( object.source ) then
        meKnowledge:RC( eRC_ActivationObstacleDeManoeuvre, object.source )
        return true
    end
    return false
end

--- Allows the unit to activate the provided object knowledge (e.g. mines are operationnals) for safety unit
-- @param object an object knowledge
-- @return Boolean, always true even if object knowledge is not activated
integration.activateItSecu = function( object )
    DEC_ActiverObjet( object.source )
    return true
end

--- Returns true if object knowledge is a maneuver obstacle, false otherwise
-- A maneuver obstacle is a mined object which could be activated (e.g. mines are operationnals) 
-- @param object an object knowledge
-- @return Boolean, whether or not this knowledge object is a maneuver obstacle
integration.isManeuverObstacle = function ( object )
    return DEC_ConnaissanceObjet_EstObstacleDeManoeuvre( object.source )
end

--- Returns true if object knowledge is an activated maneuver obstacle, false otherwise
-- An activating maneuver obstacle is an activated mined object (e.g. mines are operationnals) 
-- @param object an object knowledge
-- @return Boolean, whether or not this knowledge object is activated
integration.isActifManeuverObstacle = function ( object )
    return DEC_ConnaissanceObjet_EstObstacleDeManoeuvreActif( object.source )
end

--- Returns the time before activation for the provided plannedWork knowledge
-- @param genObject a plannedWork knowledge
-- @return Integer, the delay before activation in second
integration.delaiAvantActivationForGenObject = function ( genObject )
    return DEC_GenObject_DateActivationMines( genObject.source )
end

--- Returns the time before activation for the provided object knowledge
-- @param object an object knowledge
-- @return Integer, the delay before activation in second
integration.delaiAvantActivationForObject = function ( object )
    return DEC_ConnaissanceObjet_DateActivationObstacle( object.source )
end
