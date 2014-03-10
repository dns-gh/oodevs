-- Controlable Implementation
local pointCircleSize = 250 -- meters 
local pointRecceSpeed = 3

--- Start controlling an area
-- An action in the simulation is started
-- This method can only be called by an agent
-- @param area The DirectIA area to control
-- @return true
integration.startControlArea = function( area )
    area.perceptionID = DEC_Perception_ActivateLocationProgressiveRecce( area.source, 3 )
    area.bActionFinished = false
    perceptionReconnaissanceCallbacks[ area.perceptionID ] = function( arg )
        area.bActionFinished = true
    end
    reportFunction(eRC_DebutControleZone )
    return true
end

--- Stop controlling an area
-- The action in the simulation is stopped
-- This method can only be called by an agent
-- @param area, the DirectIA area to stop controlling
-- @return true
integration.stopControlArea  = function( area )
    DEC_Perception_DesactiverReconnaissanceLocalisation( area.perceptionID )
    reportFunction(eRC_FinControleZone )
    perceptionReconnaissanceCallbacks[ area.perceptionID ] = nil
    return true
end

--- Start controlling an urbanBlock
-- An action in the simulation is started
-- This method can only be called by an agent
-- @param urbanBlock The DirectIA urbanBlock to control
-- @return true
integration.startControlUrbanBlock = function( urbanBlock )
    urbanBlock.recceAction = DEC_Perception_ActiverReconnaissanceDansBlocUrbain( urbanBlock.source )
    reportFunction(eRC_DebutControleBlocUrbain )
    return true
end

--- Stop controlling an urbanBlock
-- The action in the simulation is stopped
-- This method can only be called by an agent
-- @param urbanBlock, the DirectIA urbanBlock to stop controlling
-- @return true
integration.stopControlUrbanBlock  = function( urbanBlock )
    DEC_Perception_DesactiverReconnaissanceDansBlocUrbain( urbanBlock.recceAction )
    reportFunction(eRC_FinControleBlocUrbain )
    return true
end

--- Start controlling a point
-- An action in the simulation is started
-- This method can only be called by an agent
-- @param point The DirectIA point to control
-- @return true
integration.startControlPoint = function( point )
    point.perceptionID = DEC_Perception_ActiverReconnaissancePoint( point.source, pointCircleSize, pointRecceSpeed )
    point.bActionFinished = false
    perceptionReconnaissanceCallbacks[ point.perceptionID ] = function( arg )
        point.bActionFinished = true
    end
    reportFunction(eRC_DebutControlPoint )
    return true
end

--- Stop controlling a point
-- The action in the simulation is stopped
-- This method can only be called by an agent
-- @param point, the DirectIA point to stop controlling
-- @return true
integration.stopControlPoint = function( point )
    DEC_Perception_DesactiverReconnaissancePoint( point.perceptionID )
    reportFunction(eRC_FinControlPoint )
    perceptionReconnaissanceCallbacks[ point.perceptionID ] = nil
    return true
end