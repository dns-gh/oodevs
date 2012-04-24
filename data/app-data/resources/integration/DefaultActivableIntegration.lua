--Default Activable Implementation

--- Return if the unit has the capacity to activate the selected object
-- @param knowledge on an object
-- @author MGD
-- @release 2010-02-23
integration.canActivateIt = function( object )
  return DEC_Agent_PeutActiverObjet( object.source )
end

integration.activationLevel = function( object )
    return DEC_ConnaissanceObjet_EstObstacleDeManoeuvreActif( object.source ) and 100 or 0
end

integration.activateIt = function( object )
    if DEC_ActiverObjet( object.source ) then
        meKnowledge:RC( eRC_ActivationObstacleDeManoeuvre )
        return true
    end
    return false
end

integration.isManeuverObstacle = function ( object )
    return DEC_ConnaissanceObjet_EstObstacleDeManoeuvre( object.source )
end
