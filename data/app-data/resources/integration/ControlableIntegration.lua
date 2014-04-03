-- Controlable Implementation
local defaultPointCircleSize = 250 -- meters 
local defaultPointRecceSpeed = 3

--- Start controlling a location
-- Activate 'magical' progressive recce inside an area or an object
-- The agent will gradually acquire knwoledges inside an area in the given location.
-- This area will grow over time depending of the pointRecceSpeed parameter
-- This method can only be called by an agent
-- @param location The DirectIA location or object to control
-- @param pointRecceSpeed Float, the reconnaissance speed in meters/tick (optional, default value defaultPointRecceSpeed = 3)
-- @return true
integration.startControlArea = function( location, pointRecceSpeed )
    location.perceptionID = DEC_Perception_ActivateLocationProgressiveRecce( location:getLocalisation(), pointRecceSpeed or defaultPointRecceSpeed )
    location.bActionFinished = false
    perceptionReconnaissanceCallbacks[ location.perceptionID ] = function( arg )
        location.bActionFinished = true
    end
    reportFunction(eRC_DebutControleZone )
    return true
end

--- Stop controlling a location
-- Deactivate 'magical' progressive recce inside an area or an object
-- @see integration.startControlArea
-- @see integration.startLocationProgressiveRecce
-- The action in the simulation is stopped
-- This method can only be called by an agent
-- @param location, the DirectIA location or object to stop controlling
-- @return true
integration.stopControlArea = function( location )
    DEC_Perception_DesactiverReconnaissanceLocalisation( location.perceptionID )
    reportFunction( eRC_FinControleZone )
    perceptionReconnaissanceCallbacks[ location.perceptionID ] = nil
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