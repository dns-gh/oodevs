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

-- --------------------------------------------------------------------------------
--  Fire capacity
-- renvoie true si l'objet r�el associ� � la connaissance existe, br�le et est �teignable, 
-- et si l'agent est capable d'�teindre l'objet r�el et dispose des ressources n�cessaires
-- --------------------------------------------------------------------------------
integration.canBeExtinguished = function( object )
    if DEC_ObjectKnowledge_HasCapacity( object.source, "burn" ) then
        return true
    else
        meKnowledge:sendReport( eRC_ImpossibleToExtinguishFire )
        return false
    end
end

integration.getObjectsKnowledgeInZoneWithCapacity = function( capacityName, zone )
    return DEC_ObjectKnowledge_GetObjectsInZone( meKnowledge.source, capacityName, zone.source )
end