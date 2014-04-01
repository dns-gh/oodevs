-- Controlable Implementation
local defaultPointCircleSize = 250 -- meters 
local defaultPointRecceSpeed = 3

--- Start controlling an area
-- An action in the simulation is started
-- This method can only be called by an agent
-- @param area The DirectIA area to control
-- @param pointRecceSpeed Float, the reconnaissance speed in meters/tick (optional, default value defaultPointRecceSpeed = 3)
-- @return true
integration.startControlArea = function( area, pointRecceSpeed )
    area.perceptionID = DEC_Perception_ActivateLocationProgressiveRecce( area.source, pointRecceSpeed or defaultPointRecceSpeed )
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

--- Start controlling a circle around the given point.
-- An action in the simulation is started
-- This method can only be called by an agent
-- @param point The DirectIA point to control
-- @param pointCircleSize Float, the radius of the circle to control (optional, default value is defaultPointCircleSize = 250)
-- @param pointRecceSpeed Float, the reconnaissance speed in km/h (optional, default value is defaultPointRecceSpeed = 3)
-- @return true
integration.startControlPoint = function( point, pointCircleSize, pointRecceSpeed )
    point.perceptionID = DEC_Perception_ActiverReconnaissancePoint( point.source, pointCircleSize or defaultPointCircleSize, pointRecceSpeed or defaultPointRecceSpeed )
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