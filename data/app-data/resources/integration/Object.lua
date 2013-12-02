integration.createResourceLinkWithObject = function( object, resourceNode )
    return DEC_ResourceNetwork_CreateLinkFromObject( object.source, resourceNode.source )
end
integration.requestForDecontamination = function( decontaminationPlot )
    DEC_ConnaissanceObjet_DemandeDeDecontamination( decontaminationPlot.source )
end

integration.isConstructed = function( object )
    if DEC_ConnaissanceObjet_EstConstuit( object.source ) == eTristate_True then
        return true
    end
    return false
end

integration.isObjectRemoved = function( object )
    return not DEC_IsValidKnowledgeObject( object.source )
end

--- Returns the nearest position to the border of the provided object, at a specified distance
-- @param object The knowledge of the object
-- @param distance Optional, the minimal distance in meters between the returned position and the border of the object (20 by default)
integration.getObjectNearestBorderPosition = function( object, distance )
    distance = distance or 20 -- meters
    local localisation = DEC_Geometrie_AgrandirLocalisation( DEC_ConnaissanceObjet_Localisation( object.source ) , distance )
    return DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), localisation )
end

integration.getObjectNearestPositionOnBorder = function( object )
    local localisation = DEC_Geometrie_AgrandirLocalisation( DEC_ConnaissanceObjet_Localisation( object.source ) , 0 )
    return DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), localisation )
end


integration.getObjectPositionsForWork = function( object, distance ) -- object is a buildable element (object or plannedWork)
    if distance == nil then
        distance = 20 -- meters
    end

    object.getObjectPositionsForWork = object.getObjectPositionsForWork or {}
    if not next( object.getObjectPositionsForWork ) then
        local localisation = DEC_Geometrie_AgrandirLocalisation( object:getLocalisation() , distance )
        local roadsIntersections = integration.findRoadIntersectionWithZone( localisation )
        if not next( roadsIntersections ) then -- no road around the object
            object.getObjectPositionsForWork = { DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), localisation ) }
        else
            object.getObjectPositionsForWork = roadsIntersections
        end
    end
    return object.getObjectPositionsForWork
end

--- Returns a list of simulation objects with a given capacity in a zone
-- @param capacityName Name of the capacity. The list of supported capacities is defined in ../schemas/(version)/physical/objects.xsd : All possible elements of T_ObjectCapacities - except geometries which defines the geometry of an object -. Examples include "avoidable", "burn", "bypassable"...
-- @param zone Knowledge of a geometry
integration.getObjectsKnowledgeInZoneWithCapacity = function( capacityName, zone )
    return DEC_ObjectKnowledge_GetObjectsInZone( meKnowledge.source, capacityName, zone.source )
end

--- Returns the instantaneously object built
-- @param object Object to build instantaneously
-- @param withReport Boolean if set to true display a report to indicate the beginning of the work
integration.buildObjectInstantaneously = function( object, withReport )
    if withReport then
        integration.pionRC( eRC_FinTravauxObjet, object.source )
    end
    return DEC_ObjectKnowledge_BuildInstantaneously( myself, object.source )
end

integration.canFilter = function( object )
    return DEC_ObjectKnowledge_HasCapacity( object.source, "population-filter" )
end

integration.canDecontaminateAgents = function( object )
    return DEC_ObjectKnowledge_HasCapacity( object.source, "decontamination" )
end

integration.isCamp = function( object )
    return DEC_ObjectKnowledge_HasCapacity( object.source, "logistic" )
end

integration.isTrafficable = function( object )
    return DEC_ObjectKnowledge_HasCapacity( object.source, "trafficability" )
end

--- Returns position around the provided object
-- @param object The knowledge of an object
integration.getPositionsAroundObject = function( object )
    local CreateKnowledge = CreateKnowledge
    local typePoint = integration.ontology.types.point
    
    local result = {}
    local simPoints = object:getPositions() 
    for i = 1, #simPoints do
        result[i] = CreateKnowledge( typePoint, simPoints[i] )
    end
    local localisation = DEC_Geometrie_AgrandirLocalisation( DEC_ConnaissanceObjet_Localisation( object.source ) , 20 )
    result[ #result + 1 ] = CreateKnowledge( typePoint, DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), localisation ) )
    return result
end

integration.getKnowledgesObjectsInCircle = function( position, distance, objectTypeList )
    return DEC_Knowledges_ObjectsInCircle( position, distance, objectTypeList )
end

integration.getKnowledgeObjectLocation = function( object )
    return DEC_ConnaissanceObjet_Localisation( object )
end

integration.getKnowledgeGenObjectLocation = function( genObject )
    return DEC_GenObjectKnowledge_Localisation( genObject )
end

integration.getGenObjectLocation = function( genObject )
    return DEC_GenObject_Localisation( genObject )
end

integration.getGenObjectType = function( genObject )
    return DEC_GenObject_Type( genObject )
end

integration.hasCapacityGenObject = function( capacity )
    return DEC_GenObject_HasCapacity( capacity )
end

integration.hasCapacityKnowledgeObject = function( object, capacity )
    return DEC_ObjectKnowledge_HasCapacity( object, capacity )
end

integration.getKnowledgeObjectType = function( object )
    return DEC_ConnaissanceObjet_Type( object )
end

integration.getNextObjectOnPath = function( model, distance, paramsList )
    return DEC_GetNextObjectOnPath( model, distance, paramsList )
end

integration.getNextObjectOnPathWithBypassed = function( model, distance, paramsList )
    return DEC_GetNextObjectOnPathWithBypassed( model, distance, paramsList )
end

integration.isManeuverGenObject = function( genObject )
    return DEC_GenObject_TypeObstacleManoeuvre( genObject )
end

integration.isValidKnowledgeObject = function( object )
    return DEC_IsValidKnowledgeObject( object )
end

integration.addKnowledgeToDecontaminationList = function( unit, object )
    DEC_Agent_DecontamineConnaissance(unit.source, object.source)
end

integration.addToDecontaminationList = function( unit, object )
    DEC_ConnaissanceObjet_DemandeDeDecontaminationSurPion(unit.source, object.source)
end

integration.genObjectNeedsImprovement = function( plannedObject )
    return DEC_GenObject_Mining( plannedObject.source )
end

integration.objectNeedsImprovement = function( kObject )
    return DEC_ObjectKnowledge_MustBeMined( kObject.source )
end

-- Returns a position (a ML knowledge) outsides provided objects locations.
-- The method computes the convex hull of the 'kObjects' locations, and then
-- computes the nearest border position, taking into account the given
-- 'distanceFromBorder' (METERS)
integration.getPositionOutsideObjects = function( kObjects, distanceFromBorder )
    distanceFromBorder = distanceFromBorder or 20 -- meters
    local localisations = {}
    for i = 1, #kObjects do
        localisations[ i ] = DEC_ConnaissanceObjet_Localisation( kObjects[ i ].source )
    end 
    if #localisations > 0 then
        local localisation = DEC_Geometrie_AgrandirLocalisation( DEC_Geometrie_ConvexHull( localisations ) , distanceFromBorder )
        local simPosition = DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), localisation )
        return CreateKnowledge( integration.ontology.types.point, simPosition )
    else
        return nil
    end
end
