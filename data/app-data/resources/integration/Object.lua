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

integration.getObjectNearestBorderPosition = function( object )
    object.getObjectNearestBorderPosition = object.getObjectNearestBorderPosition or nil
    if object.getObjectNearestBorderPosition == nil then
        local localisation = DEC_Geometrie_AgrandirLocalisation( DEC_ConnaissanceObjet_Localisation( object.source ) , 10 )
        object.getObjectNearestBorderPosition = DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), localisation )
    end
    return object.getObjectNearestBorderPosition
end

integration.getPlannedObjectNearestBorderPosition = function( object )
    object.getPlannedObjectNearestBorderPosition = object.getPlannedObjectNearestBorderPosition or nil
    if object.getPlannedObjectNearestBorderPosition == nil then
        local localisation = DEC_Geometrie_AgrandirLocalisation( DEC_GenObject_Localisation( object.source ) , 10 )
        object.getPlannedObjectNearestBorderPosition = DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), localisation )
    end
    return object.getPlannedObjectNearestBorderPosition
end


integration.getObjectsKnowledgeInZoneWithCapacity = function( capacityName, zone )
    return DEC_ObjectKnowledge_GetObjectsInZone( meKnowledge.source, capacityName, zone.source )
end

integration.buildObjectInstantaneously = function( object )
    integration.pionRC( eRC_FinTravauxObjet, object.source )
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

integration.getPositionsAroundObject = function( object )
    object.getObjectNearestBorderPosition = object.getObjectNearestBorderPosition or nil
    if object.getObjectNearestBorderPosition == nil then
        local localisation = DEC_Geometrie_AgrandirLocalisation( DEC_ConnaissanceObjet_Localisation( object.source ) , 20 )
        object.getObjectNearestBorderPosition = DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), localisation )
    end
    return { CreateKnowledge( world.Point, object.getObjectNearestBorderPosition ) }
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

integration.getKnowledgeObjectType = function( object )
    return DEC_ConnaissanceObjet_Type( object )
end

integration.getNextObjectOnPath = function( model, distance, paramsList )
    return DEC_GetNextObjectOnPath( model, distance, paramsList )
end

integration.isManeuverGenObject = function( genObject )
    return DEC_GenObject_TypeObstacleManoeuvre( genObject )
end

integration.isValidKnowledgeObject = function( object )
    return DEC_IsValidKnowledgeObject( object )
end