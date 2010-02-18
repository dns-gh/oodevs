--Default Illuminable Implementation

local eActionIllumination_Impossible = 0
local eActionIllumination_EnemyDestroyed = 1
local eActionIllumination_NoCapacity = 2
local eActionIllumination_NoAmmo = 3
local eActionIllumination_Running = 4
local eActionIllumination_Finished = 5
local eActionIllumination_None = 6

--- Return if the unit has illumination capacity
-- @param knowledge on an agent 
-- @author MGD
-- @release 2010-02-15
integration.canIlluminateIt = function( unit )
  return DEC_ConnaissanceAgent_PeutEtreIllumine( unit.source )
end

integration.isDefinitelyIlluminated = function( target )
    return DEC_ConnaissanceAgent_EstDefinitivementIllumine( target.source )
end

integration.illuminationLevel = function( self )
    return DEC_ConnaissanceAgent_EstIllumine() and 100 or 0
end

integration.startIlluminateIt = function( target )
    target.actionIlluminate = DEC_StartIlluminer( target.source )
    actionCallbacks[ target.actionIlluminate ] = function( arg ) target.actionState = arg end
    return false
end

integration.startedIlluminateIt = function( target ) 
    if target.actionState == eActionIllumination_Finished then -- IndirectFire hit target
        return true
    end
end

integration.stopIlluminateIt = function( target )
    DEC_StopAction( target.actionIlluminate )
    return false
end

