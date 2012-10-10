-- Controlable Implementation
local pointCircleSize = 250 -- meters 
local pointRecceSpeed   = 3
-- --------------------------------------------------------------------------------
-- Control area
-- --------------------------------------------------------------------------------
integration.startControlArea = function( area )
    area.perceptionID = DEC_Perception_ActivateLocationProgressiveRecce( area.source, 3 )
    area.bActionFinished = false
    perceptionReconnaissanceCallbacks[ area.perceptionID ] = function( arg )
        area.bActionFinished = true
    end
    meKnowledge:RC( eRC_DebutControleZone )
    return true
end
integration.stopControlArea  = function( area )
    DEC_Perception_DesactiverReconnaissanceLocalisation( area.perceptionID )
    meKnowledge:RC( eRC_FinControleZone )
    perceptionReconnaissanceCallbacks[ area.perceptionID ] = nil
    return true
end

-- --------------------------------------------------------------------------------
-- Control BU
-- The control effect of an urban bloc is implemented with de recce effect.
-- --------------------------------------------------------------------------------
integration.startControlUrbanBlock = function( urbanBlock )
    urbanBlock.recceAction = DEC_Perception_ActiverReconnaissanceDansBlocUrbain( urbanBlock.source )
    meKnowledge:RC( eRC_DebutControleBlocUrbain )
    return true
end
integration.stopControlUrbanBlock  = function( urbanBlock )
    DEC_Perception_DesactiverReconnaissanceDansBlocUrbain( urbanBlock.recceAction )
    meKnowledge:RC( eRC_FinControleBlocUrbain )
    return true
end

-- --------------------------------------------------------------------------------
-- Control Point
-- Le control d'un  point est impl�ment� avec les capacit�s de reconnaissance 
-- d'un point
-- --------------------------------------------------------------------------------
integration.startControlPoint = function( point )
    point.perceptionID = DEC_Perception_ActiverReconnaissancePoint( point.source, pointCircleSize, pointRecceSpeed )
    point.bActionFinished = false
    perceptionReconnaissanceCallbacks[ point.perceptionID ] = function( arg )
        point.bActionFinished = true
    end
    meKnowledge:RC( eRC_DebutControlPoint )
    return true
end
integration.stopControlPoint = function( point )
    DEC_Perception_DesactiverReconnaissancePoint( point.perceptionID )
    meKnowledge:RC( eRC_FinControlPoint )
    perceptionReconnaissanceCallbacks[ point.perceptionID ] = nil
    return true
end