-- --------------------------------------------------------------------------------
-- Configuration: Can be update considering physical pion capacity
-- --------------------------------------------------------------------------------
local pointCircleRadius = 100 -- meters 
local pointRecceSpeed   = 2
local pointSearchSpeed  = 1
local buSearchSpeed     = 0.5
local areaRecceSpeed    = 2
local areaSearchSpeed   = 1
local objectRecceSpeed  = 5
local objectSearchSpeed   = 1
local pointNBCSpeed  = 2
local urbanBlockNBCSpeed  = 2
local areaNBCSpeed  = 2
local objectNBCSpeed  = 2

-- --------------------------------------------------------------------------------
-- Recce BU
-- --------------------------------------------------------------------------------
integration.getUrbanBlockReconnaissanceState = function( urbanBlock )
    return integration.getUrbanBlockCurrentReconnaissanceState( urbanBlock.source ) * 100
end
integration.startRecceUrbanBlock = function( urbanBlock )
    urbanBlock.area = urbanBlock.area or DEC_PolygoneBlocUrbain( urbanBlock.source )
    urbanBlock.recceAction = 
        DEC_Perception_ActiverReconnaissanceDansBlocUrbain( urbanBlock.source )
    urbanBlock.recceObj = DEC_Perception_ActiverDetectionObjetLocalisation( urbanBlock.area, urbanBlock:getPosition(), buSearchSpeed )
    meKnowledge:RC( eRC_DebutReconnaissanceBlocUrbain )
    return true
end
integration.stopRecceUrbanBlock = function( urbanBlock )
    DEC_Perception_DesactiverReconnaissanceDansBlocUrbain( urbanBlock.recceAction )
    DEC_Perception_DesactiverDetectionObjetLocalisation( urbanBlock.recceObj )
    meKnowledge:RC( eRC_FinReconnaissanceBlocUrbain )
    return true
end

-- --------------------------------------------------------------------------------
-- Search BU
-- --------------------------------------------------------------------------------
integration.getUrbanBlockSearchState = function( urbanBlock )
    return ( urbanBlock.bActionSearchFinished and 100 ) or 0
end
integration.startSearchUrbanBlock = function( urbanBlock )
    urbanBlock.area = urbanBlock.area or DEC_PolygoneBlocUrbain( urbanBlock.source )
    urbanBlock.actionSearch = 
        DEC_Perception_ActivateLocationProgressiveRecce( urbanBlock.area, buSearchSpeed )
    urbanBlock.recceObj = DEC_Perception_ActiverDetectionObjetLocalisation( urbanBlock.area, urbanBlock:getPosition(), buSearchSpeed )
    urbanBlock.bActionSearchFinished = false
    urbanBlock.recoFinished = false
    perceptionReconnaissanceCallbacks[ urbanBlock.actionSearch ] = function( arg )
        urbanBlock.recoFinished = true
    end
    meKnowledge:RC( eRC_DebutFouilleBlocUrbain )
    return true
end

integration.startedSearchUrbanBlock = function( urbanBlock )
    if  urbanBlock.recoFinished then
        DEC_Connaissances_IdentifierToutesUnitesDansZone( urbanBlock.area )     
        local terroristsInUB = integration.getTerroristsInObjective( urbanBlock )     
        if integration.capture(terroristsInUB, eRC_TerroristCaptured ) then
            urbanBlock.bActionSearchFinished = true
        end
    end
end

integration.stopSearchUrbanBlock = function( urbanBlock )
    perceptionReconnaissanceCallbacks[ urbanBlock.actionSearch ] = nil
    DEC_Perception_DesactiverReconnaissanceLocalisation( urbanBlock.actionSearch )
    DEC_Perception_DesactiverDetectionObjetLocalisation( urbanBlock.recceObj )
    meKnowledge:RC( eRC_FinFouilleBlocUrbain )
    urbanBlock.area = nil
    return true
end

-- --------------------------------------------------------------------------------
-- NBC Reco Urban Block
-- --------------------------------------------------------------------------------
integration.getUrbanBlockNBC = function( urbanBlock )
    return ( urbanBlock.bActionNBCFinished and 100 ) or 0
end
integration.startNBCUrbanBlock = function( urbanBlock )
    urbanBlock.area = urbanBlock.area or DEC_PolygoneBlocUrbain( urbanBlock.source )
    local  lstObjects = {}
    urbanBlock.actionNBC = DEC_Perception_ActivateLocationProgressiveRecce( urbanBlock.area, urbanBlockNBCSpeed )
    urbanBlock.recceObjNBC = DEC_Perception_ActiverDetectionObjetLocalisation( urbanBlock.area, urbanBlock:getPosition(), urbanBlockNBCSpeed )
    urbanBlock.bActionNBCFinished = false
    perceptionReconnaissanceCallbacks[ urbanBlock.actionNBC ] = function( arg )
        urbanBlock.bActionNBCFinished = true
        lstObjects = DEC_ObjectKnowledgesIntersectingInZone( urbanBlock.area, { eTypeObjectNBCZone, eTypeObjectNBCCloud, eTypeObjectRota } )
        for _, object in pairs ( lstObjects ) do
            DEC_ConnaissanceObjet_Reconnaitre( object )
        end
    end
    meKnowledge:RC( eRC_DebutMesure )
    return true
end

integration.stopNBCUrbanBlock = function( urbanBlock )
    DEC_Perception_DesactiverReconnaissanceLocalisation( urbanBlock.actionNBC )
    DEC_Perception_DesactiverDetectionObjetLocalisation( urbanBlock.recceObjNBC )
    meKnowledge:RC( eRC_FinMesure )
    perceptionReconnaissanceCallbacks[ urbanBlock.actionNBC ] = nil
    urbanBlock.area = nil
    return true
end

-- --------------------------------------------------------------------------------
-- Recce Point
-- --------------------------------------------------------------------------------
integration.getPointReconnaissanceState = function( point )
    return ( point.bActionRecceFinished and 100 ) or 0
end
integration.startReccePoint = function( point, partially )
    if partially then
        pointCircleRadius = 30
    end
    point.reconnaissanceAction = DEC_Perception_ActiverReconnaissancePoint( 
                                 point.source, pointCircleRadius, pointRecceSpeed )
    point.recceObj = DEC_Perception_ActiverDetectionObjetLocalisation( DEC_Geometrie_ConvertirPointEnLocalisation( point.source ), point.source, pointRecceSpeed )
    point.bActionRecceFinished = false
    perceptionReconnaissanceCallbacks[ point.reconnaissanceAction ] = function( arg )
        point.bActionRecceFinished = true
    end
    meKnowledge:RC( eRC_DebutReconnaissancePoint )
    if partially then
        point:elementIsReconnoitered()
    end
    return false
end
integration.stopReccePoint = function( point )
    DEC_Perception_DesactiverReconnaissancePoint( point.reconnaissanceAction )
    DEC_Perception_DesactiverDetectionObjetLocalisation( point.recceObj )
    meKnowledge:RC( eRC_FinReconnaissancePoint )
    perceptionReconnaissanceCallbacks[ point.reconnaissanceAction ] = nil
    return true
end

-- --------------------------------------------------------------------------------
-- Search Point
-- --------------------------------------------------------------------------------
integration.getPointSearchState = function( point )
    return ( point.bActionSearchFinished and 100 ) or 0
end
integration.startSearchPoint = function( point )
    pointCircleRadius = 30
    point.actionSearch = DEC_Perception_ActiverReconnaissancePoint( 
                                        point.source, pointCircleRadius, pointSearchSpeed )
    point.recceObj = DEC_Perception_ActiverDetectionObjetLocalisation( DEC_Geometrie_ConvertirPointEnLocalisation( point.source ), point.source, pointSearchSpeed )
    point.bActionSearchFinished = false
    perceptionReconnaissanceCallbacks[ point.actionSearch ] = function( arg )
        point.bActionSearchFinished = true
        local pointArea = DEC_Geometrie_CreerLocalisationCercle( point.source, pointCircleRadius )
        DEC_Connaissances_IdentifierToutesUnitesDansZone( pointArea )
    end
    meKnowledge:RC( eRC_DebutFouillePoint )
    return true
end

integration.stopSearchPoint = function( point )
    DEC_Perception_DesactiverReconnaissancePoint( point.actionSearch )
    DEC_Perception_DesactiverDetectionObjetLocalisation( point.recceObj )
    meKnowledge:RC( eRC_FinFouillePoint )
    perceptionReconnaissanceCallbacks[ point.actionSearch ] = nil
    return true
end

-- --------------------------------------------------------------------------------
-- NBC Reco Point
-- --------------------------------------------------------------------------------
integration.getPointNBC = function( point )
    return ( point.bActionNBCFinished and 100 ) or 0
end
integration.startNBCPoint = function( point )
    local  lstObjects = {}
    pointCircleRadius = 100
    point.actionNBC = DEC_Perception_ActiverReconnaissancePoint( 
                                        point.source, pointCircleRadius, pointNBCSpeed )
    point.recceObjNBC = DEC_Perception_ActiverDetectionObjetLocalisation( DEC_Geometrie_ConvertirPointEnLocalisation( point.source ), point.source, pointNBCSpeed )
    point.bActionNBCFinished = false
    perceptionReconnaissanceCallbacks[ point.actionNBC ] = function( arg )
        point.bActionNBCFinished = true
        local pointArea = DEC_Geometrie_CreerLocalisationCercle( point.source, pointCircleRadius )
        lstObjects = DEC_ObjectKnowledgesIntersectingInZone( pointArea, { eTypeObjectNBCZone, eTypeObjectNBCCloud, eTypeObjectRota } )
        for _, object in pairs ( lstObjects ) do
            DEC_ConnaissanceObjet_Reconnaitre( object )
        end
    end
    meKnowledge:RC( eRC_DebutMesure )
    return true
end

integration.stopNBCPoint = function( point )
    DEC_Perception_DesactiverReconnaissancePoint( point.actionNBC )
    DEC_Perception_DesactiverDetectionObjetLocalisation( point.recceObjNBC )
    meKnowledge:RC( eRC_FinMesure )
    perceptionReconnaissanceCallbacks[ point.actionNBC ] = nil
    return true
end

-- --------------------------------------------------------------------------------
-- Recce Area
-- --------------------------------------------------------------------------------
integration.getAreaReconnaissanceState = function( area )
    return ( area.bActionRecceFinished and 100 ) or 0
end
integration.startRecceArea = function( area )
    area.reconnaissanceAction = 
        DEC_Perception_ActivateLocationProgressiveRecce( area.source, areaRecceSpeed )
    area.recceObj = DEC_Perception_ActiverDetectionObjetLocalisation( area.source, area:getPosition(), areaRecceSpeed )
    area.bActionRecceFinished = false
    perceptionReconnaissanceCallbacks[ area.reconnaissanceAction ] = function( arg )
        area.bActionRecceFinished = true
    end
    meKnowledge:RC( eRC_DebutReconnaissanceZone )
    return true
end
integration.stopRecceArea  = function( area )
    DEC_Perception_DesactiverReconnaissanceLocalisation( area.reconnaissanceAction )
    DEC_Perception_DesactiverDetectionObjetLocalisation( area.recceObj )
    meKnowledge:RC( eRC_FinReconnaissanceZone )
    perceptionReconnaissanceCallbacks[ area.reconnaissanceAction ] = nil
    return true
end

-- --------------------------------------------------------------------------------
-- Search Area
-- --------------------------------------------------------------------------------
integration.getAreaSearchState = function( area )
    return ( area.bActionSearchFinished and 100 ) or 0
end
integration.startSearchArea = function( area )
    area.actionSearch = 
        DEC_Perception_ActivateLocationProgressiveRecce( area.source, areaSearchSpeed )
    area.recceObj = DEC_Perception_ActiverDetectionObjetLocalisation( area.source, area:getPosition(), areaSearchSpeed )
    area.bActionSearchFinished = false
    perceptionReconnaissanceCallbacks[ area.actionSearch ] = function( arg )
        area.bActionSearchFinished = true
        DEC_Connaissances_IdentifierToutesUnitesDansZone( area.source )
    end
    meKnowledge:RC( eRC_DebutFouilleZone )
    return true
end
integration.stopSearchArea = function( area )
    DEC_Perception_DesactiverReconnaissanceLocalisation( area.actionSearch )
    DEC_Perception_DesactiverDetectionObjetLocalisation( area.recceObj )
    meKnowledge:RC( eRC_FinFouilleZone )
    perceptionReconnaissanceCallbacks[ area.actionSearch ] = nil
    return true
end
integration.startRensRecceArea = function( area )
    area.recoAction = 
        DEC_Perception_ActiverReconnaissanceLocalisation( area.source )
    perceptionReconnaissanceCallbacks[ area.recoAction ] = function( arg )
    end
    meKnowledge:RC( eRC_DebutSurveillance )
    return true
end
integration.stopRensRecceArea  = function( area )
    DEC_Perception_DesactiverReconnaissanceLocalisation( area.recoAction )
    meKnowledge:RC( eRC_FinSurveillance )
    perceptionReconnaissanceCallbacks[ area.recoAction ] = nil
    return true
end

integration.startRecoEo = function()
    DEC_Perception_ActiverObserveurTir()
end

integration.stopRecoEo = function()
    DEC_Perception_DesactiverObserveurTir()
end

-- --------------------------------------------------------------------------------
-- NBC Reco Area
-- --------------------------------------------------------------------------------
integration.getAreaNBC = function( area )
    return ( area.bActionNBCFinished and 100 ) or 0
end
integration.startNBCArea = function( area )
    local  lstObjects = {}
    area.actionNBC = DEC_Perception_ActivateLocationProgressiveRecce( area.source, areaNBCSpeed )
    area.recceObjNBC = DEC_Perception_ActiverDetectionObjetLocalisation( area.source, area:getPosition(), areaNBCSpeed )
    area.bActionNBCFinished = false
    perceptionReconnaissanceCallbacks[ area.actionNBC ] = function( arg )
        area.bActionNBCFinished = true
        lstObjects = DEC_ObjectKnowledgesIntersectingInZone( area.source, { eTypeObjectNBCZone, eTypeObjectNBCCloud, eTypeObjectRota } )
        for _, object in pairs ( lstObjects ) do
            DEC_ConnaissanceObjet_Reconnaitre( object )
        end
    end
    meKnowledge:RC( eRC_DebutMesure )
    return true
end

integration.stopNBCArea = function( area )
    DEC_Perception_DesactiverReconnaissanceLocalisation( area.actionNBC )
    DEC_Perception_DesactiverDetectionObjetLocalisation( area.recceObjNBC )
    meKnowledge:RC( eRC_FinMesure )
    perceptionReconnaissanceCallbacks[ area.actionNBC ] = nil
    return true
end

-- --------------------------------------------------------------------------------
-- Recce Objet
-- --------------------------------------------------------------------------------
integration.getObjectReconnaissanceState = function( object )
    return DEC_ConnaissanceObjet_EstReconnu( object.source ) and 100 or 0
end
integration.startRecceObject = function( object )
    object.perceptionID = DEC_Perception_ActivateLocationProgressiveRecce( 
            DEC_ConnaissanceObjet_Localisation( object.source ), objectRecceSpeed )
    object.bActionFinished = false
    perceptionReconnaissanceCallbacks[ object.perceptionID ] = function( arg )
        object.bActionFinished = true
        DEC_ConnaissanceObjet_Reconnaitre( object.source )
    end
    meKnowledge:RC( eRC_ReconnaissancePoint )
    return true
end
integration.stopRecceObject  = function( object )
    DEC_Perception_DesactiverReconnaissanceLocalisation( object.perceptionID )
    meKnowledge:RC( eRC_ReconnaissanceTerminee )
    perceptionReconnaissanceCallbacks[ object.perceptionID ] = nil
    return true
end
integration.AddReconnoiteredPointBy = function( point )
    DEC_Perception_AjouterPointReconnuAutomateParPion( meKnowledge.source, point.source )
end
integration.pointIsReconnoitedByMeOrNoOne = function( point )
    return DEC_Perception_EstPointReconnuParPionOuPersonne( meKnowledge.source, point.source )
end

-- --------------------------------------------------------------------------------
-- Search Object
-- --------------------------------------------------------------------------------
integration.getObjectSearchState = function( object )
    return ( object.bActionSearchFinished and 100 ) or 0
end
integration.startSearchObject = function( object )
    area = CreateKnowledge( integration.ontology.types.area, object:getLocalisation() )
    object.actionSearch = 
        DEC_Perception_ActivateLocationProgressiveRecce( area.source, objectSearchSpeed )
    object.recceObj = DEC_Perception_ActiverDetectionObjetLocalisation( area.source, area:getPosition(), objectSearchSpeed )
    object.bActionSearchFinished = false
    perceptionReconnaissanceCallbacks[ object.actionSearch ] = function( arg )
        object.bActionSearchFinished = true
        DEC_Connaissances_IdentifierToutesUnitesDansZone( area.source )
    end
    meKnowledge:RC( eRC_DebutFouilleObjet )
    return true
end
integration.stopSearchObject = function( object )
    DEC_Perception_DesactiverReconnaissanceLocalisation( object.actionSearch )
    DEC_Perception_DesactiverDetectionObjetLocalisation( object.recceObj )
    meKnowledge:RC( eRC_FinFouilleObjet )
    perceptionReconnaissanceCallbacks[ object.actionSearch ] = nil
    return true
end

-- --------------------------------------------------------------------------------
-- NBC Reco Object
-- --------------------------------------------------------------------------------
integration.getObjectNBC = function( object )
    return ( object.bActionNBCFinished and 100 ) or 0
end
integration.startNBCObject = function( object )
    local  lstObjects = {}
    object.actionNBC = DEC_Perception_ActivateLocationProgressiveRecce( DEC_ConnaissanceObjet_Localisation( object.source ), objectNBCSpeed )
    object.recceObjNBC = DEC_Perception_ActiverDetectionObjetLocalisation( DEC_ConnaissanceObjet_Localisation( object.source ), object:getPosition(), objectNBCSpeed )
    object.bActionNBCFinished = false
    perceptionReconnaissanceCallbacks[ object.actionNBC ] = function( arg )
        object.bActionNBCFinished = true
        lstObjects = DEC_ObjectKnowledgesIntersectingInZone( DEC_ConnaissanceObjet_Localisation( object.source ), { eTypeObjectNBCZone, eTypeObjectNBCCloud, eTypeObjectRota } )
        for _, object in pairs ( lstObjects ) do
            DEC_ConnaissanceObjet_Reconnaitre( object )
        end
    end
    meKnowledge:RC( eRC_DebutMesure )
    return true
end

integration.stopNBCObject = function( object )
    DEC_Perception_DesactiverReconnaissancePoint( object.actionNBC )
    DEC_Perception_DesactiverDetectionObjetLocalisation( object.recceObjNBC )
    meKnowledge:RC( eRC_FinMesure )
    perceptionReconnaissanceCallbacks[ object.actionNBC ] = nil
    return true
end
