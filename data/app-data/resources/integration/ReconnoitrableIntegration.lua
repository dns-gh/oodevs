local objectRecceSpeed = 5
local objectSearchSpeed = 1
local pointNBCSpeed = 2
local urbanBlockNBCSpeed = 2
local areaNBCSpeed = 2
local objectNBCSpeed = 2

-- --------------------------------------------------------------------------------
-- Recce BU
-- --------------------------------------------------------------------------------
--- Return reconnaissance state of an urban block knowledge
-- @param urbanBlock Urban block knowledge
-- @return reconnaissance level between 0 and 100
integration.getUrbanBlockReconnaissanceState = function( urbanBlock )
    return integration.getUrbanBlockCurrentReconnaissanceState( urbanBlock.source ) * 100
end
--- Start reconnoitering an urban block knowledge and emits a report
-- @param urbanBlock Urban block knowledge
-- @param recceSpeed Optional. Speed of the reconnaissance in meters/tick. By default, use the value defined in authoring tool
-- @return true
integration.startRecceUrbanBlock = function( urbanBlock, recceSpeed )
    urbanBlock.area = urbanBlock.area or DEC_PolygoneBlocUrbain( urbanBlock.source )
    urbanBlock.recceAction = 
        _DEC_Perception_ActiverReconnaissanceDansBlocUrbain( myself, urbanBlock.source )
    urbanBlock.recceObj = _DEC_Perception_ActiverDetectionObjetLocalisation( myself, urbanBlock.area, urbanBlock:getPosition(), recceSpeed or DEC_GetUrbanRecoSpeed( urbanBlock.source ) )
    reportFunction(eRC_DebutReconnaissanceBlocUrbain )
    return true
end
--- Stop reconnoitering an urban block knowledge and emits a report
-- @param urbanBlock Urban block knowledge
-- @param noReport Boolean - if set to 'true', no report will be displayed.
-- @return true
integration.stopRecceUrbanBlock = function( urbanBlock, noReport )
    _DEC_Perception_DesactiverReconnaissanceDansBlocUrbain( myself, urbanBlock.recceAction )
    _DEC_Perception_DesactiverDetectionObjetLocalisation( myself, urbanBlock.recceObj )
    if not noReport then
        reportFunction( eRC_FinReconnaissanceBlocUrbain )
    end
    return true
end

-- --------------------------------------------------------------------------------
-- Search BU
-- --------------------------------------------------------------------------------
--- Return search state of an urban block
-- @param urbanBlock The urban block knowledge
-- @return 100 if the block is searched, 0 otherwise.
integration.getUrbanBlockSearchState = function( urbanBlock )
    return ( urbanBlock.bActionSearchFinished and 100 ) or 0
end

--- Start searching an urban block
-- @param urbanBlock Urban block knowledge
-- @param recceSpeed Optional. Speed of the reconnaissance in meters/tick. By default, use the value defined in authoring tool
-- @return true
integration.startSearchUrbanBlock = function( urbanBlock, recceSpeed )
    urbanBlock.area = urbanBlock.area or DEC_PolygoneBlocUrbain( urbanBlock.source )
    local buSearchSpeed = recceSpeed or DEC_GetUrbanSearchSpeed( urbanBlock.source )
    urbanBlock.actionSearch = 
        _DEC_Perception_ActivateLocationProgressiveRecce( myself, urbanBlock.area, buSearchSpeed )
    urbanBlock.recceObj = _DEC_Perception_ActiverDetectionObjetLocalisation( myself, urbanBlock.area, urbanBlock:getPosition(), buSearchSpeed )
    urbanBlock.bActionSearchFinished = false
    urbanBlock.recoFinished = false
    perceptionReconnaissanceCallbacks[ urbanBlock.actionSearch ] = function( arg )
        urbanBlock.recoFinished = true
    end
    reportFunction(eRC_DebutFouilleBlocUrbain )
    return true
end

--- Continue searching an urban block
-- @param urbanBlock Urban block knowledge
-- @param doNotCaptureTerrorists true, if terrorists should not be captured; false, otherwise
-- @return Boolean, whether the searching action is finished or not
integration.startedSearchUrbanBlock = function( urbanBlock, doNotCaptureTerrorists )
    if  urbanBlock.recoFinished then
        _DEC_Connaissances_IdentifierToutesUnitesDansZone( myself, urbanBlock.area )     
        if not doNotCaptureTerrorists then
            integration.capture( integration.getTerroristsInObjective( urbanBlock ), eRC_TerroristCaptured ) 
        end
        urbanBlock.bActionSearchFinished = true
    end
    return urbanBlock.bActionSearchFinished
end

--- Stop searching an urban block
-- @param urbanBlock Urban block knowledge
-- @param noReport Boolean - if set to 'true', no report will be displayed.
-- @param captureTerrorists Boolean, true if terrorists should be captured, false otherwise.
-- @return true
integration.stopSearchUrbanBlock = function( urbanBlock, noReport, captureTerrorists )
    perceptionReconnaissanceCallbacks[ urbanBlock.actionSearch ] = nil
    if captureTerrorists then
        integration.capture( integration.getTerroristsInObjective( urbanBlock ), eRC_TerroristCaptured ) 
    end
    _DEC_Perception_DesactiverReconnaissanceLocalisation( myself, urbanBlock.actionSearch )
    _DEC_Perception_DesactiverDetectionObjetLocalisation( myself, urbanBlock.recceObj )
    if not noReport then
        reportFunction( eRC_FinFouilleBlocUrbain )
    end
    urbanBlock.area = nil
    return true
end

-- --------------------------------------------------------------------------------
-- NBC Reco Urban Block
-- --------------------------------------------------------------------------------
--- Return the nbc reconnaissance level of an urban block
-- @param urbanBlock Urban block knowledge
-- @return 100 if the urban block has been reconnoitered for nbc, 0 otherwise
integration.getUrbanBlockNBC = function( urbanBlock )
    return ( urbanBlock.bActionNBCFinished and 100 ) or 0
end

--- Start reconnoitering an urban block for nbc
-- The reconnaisance will fill the attributes of nbc object knowledges in the area when completed.
-- @param urbanBlock Urban block knowledge
-- @param recceSpeed Optional. Speed of the reconnaissance in meters/tick. By default, urbanBlockNBCSpeed = 2
-- @return true
integration.startNBCUrbanBlock = function( urbanBlock, recceSpeed )
    urbanBlock.area = urbanBlock.area or DEC_PolygoneBlocUrbain( urbanBlock.source )
    local lstObjects = {}
    urbanBlock.actionNBC = _DEC_Perception_ActivateLocationProgressiveRecce( myself, urbanBlock.area, recceSpeed or urbanBlockNBCSpeed )
    urbanBlock.recceObjNBC = _DEC_Perception_ActiverDetectionObjetLocalisation( myself, urbanBlock.area, urbanBlock:getPosition(), recceSpeed or urbanBlockNBCSpeed )
    urbanBlock.bActionNBCFinished = false
    perceptionReconnaissanceCallbacks[ urbanBlock.actionNBC ] = function( arg )
        urbanBlock.bActionNBCFinished = true
        lstObjects = _DEC_ObjectKnowledgesIntersectingInZone( myself, urbanBlock.area, { eTypeObjectNBCZone, eTypeObjectNBCCloud, eTypeObjectRota } )
        for _, object in pairs ( lstObjects ) do
            _DEC_ConnaissanceObjet_Reconnaitre( myself, object )
        end
    end
    reportFunction(eRC_DebutMesure )
    return true
end

--- Stop reconnoitering an urban block for nbc
-- @param urbanBlock Urban block knowledge
-- @return true
integration.stopNBCUrbanBlock = function( urbanBlock )
    _DEC_Perception_DesactiverReconnaissanceLocalisation( myself, urbanBlock.actionNBC )
    _DEC_Perception_DesactiverDetectionObjetLocalisation( myself, urbanBlock.recceObjNBC )
    reportFunction(eRC_FinMesure )
    perceptionReconnaissanceCallbacks[ urbanBlock.actionNBC ] = nil
    urbanBlock.area = nil
    return true
end

-- --------------------------------------------------------------------------------
-- Recce Point
-- --------------------------------------------------------------------------------
--- Return the reconnaissance level of a point
-- @param point Point knowledge
-- @return 100 if the point has been reconnoitered, 0 otherwise
integration.getPointReconnaissanceState = function( point )
    return ( point.bActionRecceFinished and 100 ) or 0
end

--- Start reconnoitering a point
-- @param point Point knowledge
-- @param partially If false or nil, reconnoiter 100 meters around the point. Otherwise (true), reconnoiter 30 meters around the point.
-- @param radius Optional. Override radius computed with partially parameter
-- @param recceSpeed Optional. Speed of the reconnaissance in meters/tick. By default, use the value defined in authoring tool
-- @return true
integration.startReccePoint = function( point, partially, radius, recceSpeed )
    local pointCircleRadius = radius or 100
    if not radius and partially then
        pointCircleRadius = 30
    end
    local actualRecceSpeed = recceSpeed or DEC_GetOpenRecoSpeed()
    point.reconnaissanceAction = _DEC_Perception_ActiverReconnaissancePoint( myself,
                                 point.source, pointCircleRadius, actualRecceSpeed )
    point.recceObj = _DEC_Perception_ActiverDetectionObjetLocalisation( myself, DEC_Geometrie_ConvertirPointEnLocalisation( point.source ), point.source, actualRecceSpeed )
    point.bActionRecceFinished = false
    perceptionReconnaissanceCallbacks[ point.reconnaissanceAction ] = function( arg )
        point.bActionRecceFinished = true
    end
    point.bActionRecceObjFinished = false
    perceptionReconnaissanceCallbacks[ point.recceObj ] = function( arg )
        point.bActionRecceObjFinished = true
    end
    reportFunction(eRC_DebutReconnaissancePoint )
    if partially then
        point:elementIsReconnoitered()
    end
    return false
end

--- Stop reconnoitering a point
-- @param point Point knowledge
-- @param noReport Boolean - if set to 'true', no report will be displayed.
-- @return true
integration.stopReccePoint = function( point, noReport )
    _DEC_Perception_DesactiverReconnaissancePoint( myself, point.reconnaissanceAction )
    _DEC_Perception_DesactiverDetectionObjetLocalisation( myself, point.recceObj )
    if not noReport then
        reportFunction( eRC_FinReconnaissancePoint )
    end
    perceptionReconnaissanceCallbacks[ point.reconnaissanceAction ] = nil
    return true
end

-- --------------------------------------------------------------------------------
-- Search Point
-- --------------------------------------------------------------------------------
--- Return the search level of a point
-- @param point Point knowledge
-- @return 100 if the point has been searched, 0 otherwise
integration.getPointSearchState = function( point )
    return ( point.bActionSearchFinished and 100 ) or 0
end

--- Start searching a point
-- @param point Point knowledge
-- @param radius Radius of search. Optional. 30 meters by default.
-- @param searchSpeed Optional. Speed of the search in meters/tick. By default, use the value defined in authoring tool
-- @return true
integration.startSearchPoint = function( point, radius, searchSpeed )
    local pointCircleRadius = radius or 30
    local actualSearchSpeed = searchSpeed or DEC_GetOpenSearchSpeed()
    point.actionSearch = _DEC_Perception_ActiverReconnaissancePoint( myself,
                                        point.source, pointCircleRadius, actualSearchSpeed )
    point.recceObj = _DEC_Perception_ActiverDetectionObjetLocalisation( myself, DEC_Geometrie_ConvertirPointEnLocalisation( point.source ), point.source, actualSearchSpeed )
    point.bActionSearchFinished = false
    perceptionReconnaissanceCallbacks[ point.actionSearch ] = function( arg )
        point.bActionSearchFinished = true
        local pointArea = DEC_Geometrie_CreerLocalisationCercle( point.source, pointCircleRadius )
        _DEC_Connaissances_IdentifierToutesUnitesDansZone( myself, pointArea )
    end
    reportFunction( eRC_DebutFouillePoint )
    return true
end

--- Stop searching a point
-- @param point Point knowledge
-- @param noReport Boolean - if set to 'true', no report will be displayed.
-- @return true
integration.stopSearchPoint = function( point, noReport )
    _DEC_Perception_DesactiverReconnaissancePoint( myself, point.actionSearch )
    _DEC_Perception_DesactiverDetectionObjetLocalisation( myself, point.recceObj )
    if not noReport then
        reportFunction( eRC_FinFouillePoint )
    end
    perceptionReconnaissanceCallbacks[ point.actionSearch ] = nil
    return true
end

-- --------------------------------------------------------------------------------
-- NBC Reco Point
-- --------------------------------------------------------------------------------
--- Return the nbc reconnaissance level of a point
-- @param point Point knowledge
-- @return 100 if the point has been reconnoitered for nbc, 0 otherwise
integration.getPointNBC = function( point )
    return ( point.bActionNBCFinished and 100 ) or 0
end
--
--- Start reconnoitering a point for nbc
-- The reconnaisance will fill the attributes of nbc object knowledges around the point when completed.
-- @param point Point knowledge
-- @param circleRadius Optional. The radius of the circle to reconnoiter around the point. 100 meters by default.
-- @param recceSpeed Optional. Speed of the reconnaissance in meters/tick. By default, pointNBCSpeed = 2
-- @return true
integration.startNBCPoint = function( point, circleRadius, recceSpeed )
    local  lstObjects = {}
    local pointCircleRadius = circleRadius or 100
    point.actionNBC = _DEC_Perception_ActiverReconnaissancePoint( myself,
                                        point.source, pointCircleRadius, recceSpeed or pointNBCSpeed )
    point.recceObjNBC = _DEC_Perception_ActiverDetectionObjetLocalisation( myself, DEC_Geometrie_ConvertirPointEnLocalisation( point.source ), point.source, recceSpeed or pointNBCSpeed )
    point.bActionNBCFinished = false
    perceptionReconnaissanceCallbacks[ point.actionNBC ] = function( arg )
        point.bActionNBCFinished = true
        local pointArea = DEC_Geometrie_CreerLocalisationCercle( point.source, pointCircleRadius )
        lstObjects = _DEC_ObjectKnowledgesIntersectingInZone( myself, pointArea, { eTypeObjectNBCZone, eTypeObjectNBCCloud, eTypeObjectRota } )
        for _, object in pairs ( lstObjects ) do
            _DEC_ConnaissanceObjet_Reconnaitre( myself, object )
        end
    end
    reportFunction(eRC_DebutMesure )
    return true
end

--- Stop reconnoitering a point for nbc
-- @param point Point knowledge
-- @return true
integration.stopNBCPoint = function( point )
    _DEC_Perception_DesactiverReconnaissancePoint( myself, point.actionNBC )
    _DEC_Perception_DesactiverDetectionObjetLocalisation( myself, point.recceObjNBC )
    reportFunction(eRC_FinMesure )
    perceptionReconnaissanceCallbacks[ point.actionNBC ] = nil
    return true
end

-- --------------------------------------------------------------------------------
-- Recce Area
-- --------------------------------------------------------------------------------
--- Return reconnaissance level of an area
-- @param area Area knowledge
-- @return 100 if the area is reconnoitered, 0 otherwise
integration.getAreaReconnaissanceState = function( area )
    return ( area.bActionRecceFinished and 100 ) or 0
end

--- Start reconnoitering an area
-- @param area Area knowledge
-- @param recceSpeed Optional. Speed of the reconnaissance in meters/tick. By default, use the value defined in authoring tool
-- @return true
integration.startRecceArea = function( area, recceSpeed )
    local areaRecceSpeed = recceSpeed or DEC_GetOpenRecoSpeed()
    area.reconnaissanceAction = 
        _DEC_Perception_ActivateLocationProgressiveRecce( myself, area.source, areaRecceSpeed )
    area.recceObj = _DEC_Perception_ActiverDetectionObjetLocalisation( myself, area.source, area:getPosition(), areaRecceSpeed )
    area.bActionRecceFinished = false
    perceptionReconnaissanceCallbacks[ area.reconnaissanceAction ] = function( arg )
        area.bActionRecceFinished = true
    end
    area.bActionRecceObjFinished = false
    perceptionReconnaissanceCallbacks[ area.recceObj ] = function( arg )
        area.bActionRecceObjFinished = true
    end
    reportFunction(eRC_DebutReconnaissanceZone )
    return true
end

--- Stop reconnoitering an area
-- @param area Area knowledge
-- @param noReport Boolean - if set to 'true', no report will be displayed.
-- @return true
integration.stopRecceArea  = function( area, noReport )
    _DEC_Perception_DesactiverReconnaissanceLocalisation( myself, area.reconnaissanceAction )
    _DEC_Perception_DesactiverDetectionObjetLocalisation( myself, area.recceObj )
    if not noReport then
        reportFunction( eRC_FinReconnaissanceZone )
    end
    perceptionReconnaissanceCallbacks[ area.reconnaissanceAction ] = nil
    return true
end

-- --------------------------------------------------------------------------------
-- Search Area
-- --------------------------------------------------------------------------------
--- Return search level of an area
-- @param area Area knowledge
-- @return 100 if the area is searched, 0 otherwise
integration.getAreaSearchState = function( area )
    return ( area.bActionSearchFinished and 100 ) or 0
end

--- Start searching an area
-- @param area Area knowledge
-- @param recceSpeed Optional. Speed of the reconnaissance in meters/tick. By default, use the value defined in authoring tool
-- @return true
integration.startSearchArea = function( area, recceSpeed )
    local areaSearchSpeed = recceSpeed or DEC_GetOpenSearchSpeed()
    area.actionSearch = 
        _DEC_Perception_ActivateLocationProgressiveRecce( myself, area.source, areaSearchSpeed )
    area.recceObj = _DEC_Perception_ActiverDetectionObjetLocalisation( myself, area.source, area:getPosition(), areaSearchSpeed )
    area.bActionSearchFinished = false
    perceptionReconnaissanceCallbacks[ area.actionSearch ] = function( arg )
        area.bActionSearchFinished = true
        _DEC_Connaissances_IdentifierToutesUnitesDansZone( myself, area.source )
    end
    reportFunction(eRC_DebutFouilleZone )
    return true
end

--- Stop searching an area
-- @param area Area knowledge
-- @param noReport Boolean - if set to 'true', no report will be displayed.
-- @return true
integration.stopSearchArea = function( area, noReport )
    _DEC_Perception_DesactiverReconnaissanceLocalisation( myself, area.actionSearch )
    _DEC_Perception_DesactiverDetectionObjetLocalisation( myself, area.recceObj )
    if not noReport then
        reportFunction( eRC_FinFouilleZone )
    end
    perceptionReconnaissanceCallbacks[ area.actionSearch ] = nil
    return true
end

--- Start reconnoitering an area
-- Reconnaissance of the area is instantaneous inside the agent sensors range
-- @param area Area knowledge
-- @return true
integration.startRensRecceArea = function( area )
    area.recoAction = 
        _DEC_Perception_ActiverReconnaissanceLocalisation( myself, area.source )
    perceptionReconnaissanceCallbacks[ area.recoAction ] = function( arg )
    end
    reportFunction(eRC_DebutSurveillance )
    return true
end

--- Stop reconnoitering an area instantaneously
-- @param area Area knowledge
-- @return true
integration.stopRensRecceArea  = function( area )
    _DEC_Perception_DesactiverReconnaissanceLocalisation( myself, area.recoAction )
    reportFunction(eRC_FinSurveillance )
    perceptionReconnaissanceCallbacks[ area.recoAction ] = nil
    return true
end

--- Report indirect fires detections
integration.startRecoEo = function()
    _DEC_Perception_ActiverObserveurTir( myself )
end

--- Stop reporting indirect fires detections
integration.stopRecoEo = function()
    _DEC_Perception_DesactiverObserveurTir( myself )
end

-- --------------------------------------------------------------------------------
-- NBC Reco Area
-- --------------------------------------------------------------------------------
--- Return the nbc reconnaissance level of an area
-- @param area Knowledge of an area
-- @return 100 if the area has been reconnoitered for nbc, 0 otherwise
integration.getAreaNBC = function( area )
    return ( area.bActionNBCFinished and 100 ) or 0
end

--- Start reconnoitering an area for nbc
-- The reconnaisance will fill the attributes of nbc object knowledges in the area when completed.
-- @param area Area knowledge
-- @param recceSpeed Optional. Speed of the reconnaissance in meters/tick. By default, areaNBCSpeed = 2
-- @return true
integration.startNBCArea = function( area, recceSpeed )
    local  lstObjects = {}
    area.actionNBC = _DEC_Perception_ActivateLocationProgressiveRecce( myself, area.source, recceSpeed or areaNBCSpeed )
    area.recceObjNBC = _DEC_Perception_ActiverDetectionObjetLocalisation( myself, area.source, area:getPosition(), recceSpeed or areaNBCSpeed )
    area.bActionNBCFinished = false
    perceptionReconnaissanceCallbacks[ area.actionNBC ] = function( arg )
        area.bActionNBCFinished = true
        lstObjects = _DEC_ObjectKnowledgesIntersectingInZone( myself, area.source, { eTypeObjectNBCZone, eTypeObjectNBCCloud, eTypeObjectRota } )
        for _, object in pairs ( lstObjects ) do
            _DEC_ConnaissanceObjet_Reconnaitre( myself, object )
        end
    end
    reportFunction(eRC_DebutMesure )
    return true
end

--- Stop reconnoitering an area for nbc
-- @param area Area knowledge
-- @return true
integration.stopNBCArea = function( area )
    _DEC_Perception_DesactiverReconnaissanceLocalisation( myself, area.actionNBC )
    _DEC_Perception_DesactiverDetectionObjetLocalisation( myself, area.recceObjNBC )
    reportFunction(eRC_FinMesure )
    perceptionReconnaissanceCallbacks[ area.actionNBC ] = nil
    return true
end

-- --------------------------------------------------------------------------------
-- Recce Objet
-- --------------------------------------------------------------------------------
--- Return object reconnaissance level
-- @param object Object knowledge
-- @return 100 if object knowledge is reconnoitered, 0 otherwise
integration.getObjectReconnaissanceState = function( object )
    return DEC_ConnaissanceObjet_EstReconnu( object.source ) and 100 or 0
end

--- Start reconnoitering an object
-- Reconnoiters the object itself. Other objects inside the object will not be reconnoitered.
-- @param object Object knowledge
-- @param recceSpeed Optional. Speed of the reconnaissance in meters/tick. By default, objectRecceSpeed = 5
-- @return true
integration.startRecceObject = function( object, recceSpeed )
    object.perceptionID = _DEC_Perception_ActivateLocationProgressiveRecce( myself,
            DEC_ConnaissanceObjet_Localisation( object.source ), recceSpeed or objectRecceSpeed )
    object.bActionFinished = false
    perceptionReconnaissanceCallbacks[ object.perceptionID ] = function( arg )
        object.bActionFinished = true
        _DEC_ConnaissanceObjet_Reconnaitre( myself, object.source )
    end
    reportFunction( eRC_ReconnaissancePoint )
    return true
end

--- Returns true if the ongoing reconnoitering action on the given object is finished, false otherwise.
-- @see integration.startRecceObject
-- @see integration.stopRecceObject
-- @param target Object knowledge
-- @return Boolean
integration.startedRecceObject = function( object )
    return object.bActionFinished
end

--- Stop reconnoitering an object
-- @param object Object knowledge
-- @param noReport Boolean - if set to 'true', no report will be displayed.
-- @return true
integration.stopRecceObject  = function( object, noReport )
    _DEC_Perception_DesactiverReconnaissanceLocalisation( myself, object.perceptionID )
    if not noReport then
        reportFunction( eRC_ReconnaissanceTerminee )
    end
    perceptionReconnaissanceCallbacks[ object.perceptionID ] = nil
    return true
end

--- Add a point to list of points reconnoitered by one's automaton
-- This function expects meKnowledge to be an agent
-- @param point Point knowledge
integration.AddReconnoiteredPointBy = function( point )
    DEC_Perception_AjouterPointReconnuAutomateParPion( meKnowledge.source, point.source )
end

--- Return whether the point has been reconnoitered by some agent of the same automaton
-- @param point Point knowledge
integration.pointIsReconnoitedByMeOrNoOne = function( point )
    return DEC_Perception_EstPointReconnuParPionOuPersonne( meKnowledge.source, point.source )
end

-- --------------------------------------------------------------------------------
-- Search Object
-- --------------------------------------------------------------------------------
--- Return object search level
-- @param object Object knowledge
-- @return 100 if object knowledge has been searched, 0 otherwise
integration.getObjectSearchState = function( object )
    return ( object.bActionSearchFinished and 100 ) or 0
end

--- Start searching an object
-- Performs a search inside the area of the object.
-- @param object Object knowledge
-- @param recceSpeed Optional. Speed of the search in meters/tick. By default, objectSearchSpeed = 1
-- @return true
integration.startSearchObject = function( object, recceSpeed )
    area = CreateKnowledge( integration.ontology.types.area, object:getLocalisation() )
    object.actionSearch = 
        _DEC_Perception_ActivateLocationProgressiveRecce( myself, area.source, objectSearchSpeed )
    object.recceObj = _DEC_Perception_ActiverDetectionObjetLocalisation( myself, area.source, area:getPosition(), recceSpeed or objectSearchSpeed )
    object.bActionSearchFinished = false
    perceptionReconnaissanceCallbacks[ object.actionSearch ] = function( arg )
        object.bActionSearchFinished = true
        _DEC_Connaissances_IdentifierToutesUnitesDansZone( myself, area.source )
    end
    reportFunction(eRC_DebutFouilleObjet )
    return true
end

--- Stop searching an object
-- @param object Object knowledge
-- @param noReport Boolean - if set to 'true', no report will be displayed.
-- @return true
integration.stopSearchObject = function( object, noReport )
    _DEC_Perception_DesactiverReconnaissanceLocalisation( myself, object.actionSearch )
    _DEC_Perception_DesactiverDetectionObjetLocalisation( myself, object.recceObj )
    if not noReport then
        reportFunction( eRC_FinFouilleObjet )
    end
    perceptionReconnaissanceCallbacks[ object.actionSearch ] = nil
    return true
end

-- --------------------------------------------------------------------------------
-- NBC Reco Object
-- --------------------------------------------------------------------------------
--- Return the nbc reconnaissance level of an object
-- @param object Object knowledge
-- @return 100 if the object has been reconnoitered for nbc, 0 otherwise
integration.getObjectNBC = function( object )
    return ( object.bActionNBCFinished and 100 ) or 0
end

--- Start reconnoitering an object for nbc
-- The reconnaisance will fill the attributes of nbc object knowledges inside the area of the parameter object when completed.
-- @param object Object knowledge
-- @return true
integration.startNBCObject = function( object )
    local lstObjects = {}
    object.actionNBC = _DEC_Perception_ActivateLocationProgressiveRecce( myself, DEC_ConnaissanceObjet_Localisation( object.source ), objectNBCSpeed )
    object.recceObjNBC = _DEC_Perception_ActiverDetectionObjetLocalisation( myself, DEC_ConnaissanceObjet_Localisation( object.source ), object:getPosition(), objectNBCSpeed )
    object.bActionNBCFinished = false
    perceptionReconnaissanceCallbacks[ object.actionNBC ] = function( arg )
        object.bActionNBCFinished = true
        lstObjects = _DEC_ObjectKnowledgesIntersectingInZone( myself, DEC_ConnaissanceObjet_Localisation( object.source ), { eTypeObjectNBCZone, eTypeObjectNBCCloud, eTypeObjectRota } )
        for _, object in pairs ( lstObjects ) do
            _DEC_ConnaissanceObjet_Reconnaitre( myself, object )
        end
    end
    reportFunction(eRC_DebutMesure )
    return true
end

--- Stop reconnoitering an object for nbc
-- @param object Object knowledge
-- @return true
integration.stopNBCObject = function( object )
    _DEC_Perception_DesactiverReconnaissancePoint( myself, object.actionNBC )
    _DEC_Perception_DesactiverDetectionObjetLocalisation( myself, object.recceObjNBC )
    reportFunction(eRC_FinMesure )
    perceptionReconnaissanceCallbacks[ object.actionNBC ] = nil
    return true
end

--- Returns true if the given target is within recognition range of this entity, false otherwise.
-- This method can only be called by an agent.
-- @param target Knowledge defining a "getPosition" method returning a simulation position
-- @return Boolean
integration.isInRecoRange = function( target )
    return integration.distance( meKnowledge, target ) < _DEC_Reconnoissance_MajorComponentMinDistance( myself )
end

--- Returns true if the ongoing reconnoitering action on the given target is finished, false otherwise.
-- @see integration.startReccePoint
-- @see integration.startRecceArea
-- @param target Area or point knowledge
-- @param waitForObjectRecce Boolean, whether or not this method should wait
-- for both the target reconnoitering and the object reconnoitering actions
-- to be finished before returning true.
-- @return Boolean
integration.startedRecce = function( target, waitForObjectRecce )
    return target.bActionRecceFinished and ( ( not waitForObjectRecce ) or target.bActionRecceObjFinished )
end

--- Returns true if the ongoing searching action on the given target is finished, false otherwise.
-- @see integration.startSearchPoint
-- @see integration.startSearchArea
-- @see integration.startSearchObject
-- @param target Area, object or point knowledge.
-- @return Boolean
integration.startedSearch = function( target )
    return target.bActionSearchFinished
end
