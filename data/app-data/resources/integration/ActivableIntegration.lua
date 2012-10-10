--Default Activable Implementation

--- Return if the unit has the capacity to activate the selected object
-- @param knowledge on an object
-- @author MGD
-- @release 2010-02-23
integration.canActivateIt = function( object )
    if DEC_Agent_PeutActiverObjet( object.source ) then
        meKnowledge:RC( eRC_ActivationObstacleDeManoeuvre )
        return true
    else
        DEC_Trace( "object cannot be activated" )
        return false
    end
end

integration.activationLevel = function( object )
    return DEC_ConnaissanceObjet_EstObstacleDeManoeuvreActif( object.source ) and 100 or 0
end

integration.activateIt = function( object )
    if DEC_ActiverObjet( object.source ) then
        meKnowledge:RC( eRC_ActivationObstacleDeManoeuvre, object.source )
        return true
    end
    return false
end

integration.activateItSecu = function( object )
    DEC_ActiverObjet( object.source )
    return true
end

integration.isManeuverObstacle = function ( object )
    return DEC_ConnaissanceObjet_EstObstacleDeManoeuvre( object.source )
end

integration.isActifManeuverObstacle = function ( object )
    return DEC_ConnaissanceObjet_EstObstacleDeManoeuvreActif( object.source )
end

integration.delaiAvantActivationForGenObject = function ( genObject )
    return DEC_GenObject_DateActivationMines( genObject.source )
end

integration.delaiAvantActivationForObject = function ( object )
    return DEC_ConnaissanceObjet_DateActivationObstacle( object.source )
end
