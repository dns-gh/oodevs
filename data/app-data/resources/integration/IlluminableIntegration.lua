--Default Illuminable Implementation

local eActionIllumination_Impossible = 0
local eActionIllumination_EnemyDestroyed = 1
local eActionIllumination_NoCapacity = 2
local eActionIllumination_NoAmmo = 3
local eActionIllumination_Running = 4
local eActionIllumination_Finished = 5
local eActionIllumination_None = 6

--- Return if the unit has illumination capacity regarding target distance
-- @param knowledge on an agent 
-- @author MGD
-- @release 2010-02-15
integration.canIlluminateIt = function( unit )
  return DEC_ConnaissanceAgent_PeutEtreIllumine( unit.source )
end

--- Return if the unit has illumination capacity
-- @param knowledge on an agent 
-- @author MGD
-- @release 2010-12-03
integration.canIlluminate = function()
    return DEC_ConnaissanceAgent_PeutIlluminer()
end

integration.isDefinitelyIlluminated = function( target )
    return DEC_ConnaissanceAgent_EstDefinitivementIllumine( target.source )
end

integration.illuminationLevel = function( self )
    return DEC_ConnaissanceAgent_EstIllumine( self.source ) and 100 or 0
end

integration.startIlluminateIt = function( target, allies )
    target[myself] = target[myself] or {}
    target[myself].actionIlluminate = DEC_StartIlluminer( target.source, allies[1].source  )
    integration.pionRC( eRC_IllumineCible )
    actionCallbacks[ target[myself].actionIlluminate ] = function( arg ) target[myself].actionState = arg end
    return false
end

integration.startedIlluminateIt = function( target ) 
    if target[myself].actionState == eActionIllumination_Finished then -- IndirectFire hit target
        return true
    end
end

integration.stopIlluminateIt = function( target )
    target[myself] = target[myself] or {}
    target[myself].actionIlluminate = DEC__StopAction( target[myself].actionIlluminate )
    return false
end

integration.canAgentIlluminate = function( agent )
    return DEC_Agent_PeutIllumine( agent )
end

