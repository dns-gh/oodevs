-------------------------------------------------------------------------------
-- Safeguarding Implementation :
-- @author DDA
-- @created 2010-04-01
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------

integration.getPointCoverAndConcealmentLevelFor = function( self, entity, objective )
    return 100 - objective:computeEstimatePerceptionCapability( entity, self )
end

integration.getUrbanBlockCoverAndConcealmentLevelFor = function( self, entity, objective )
    local res = 0
    local protectionLevel = DEC_NiveauDeProtectionMaterielComposantes( objective.source, self.source )
    if protectionLevel ~= -1 then
        res = res + ( protectionLevel * DEC_ConnaissanceBlocUrbain_RapForLocal( self.source ) * 100 ) -- $$$ MIA sortir DEC_ConnaissanceBlocUrbain_RapForLocal dans la notion de dangerosité d'un BU.
        if res < 0 then
            res = 0
        elseif res > 100 then
            res = 100
        end
    end
    if  integration.getPerception( self, objective ) > 0 then 
        res = res + 0 -- point is not concealed, means that it has no protection capability.
    else 
        res = res + 100
    end
    return res
end

integration.getSafetyPositionFromCrowd = function( crowd, distance )
    local position = DEC_Geometrie_CalculerPositionSureteAvecPopulation( crowd.source, distance )
    if position then
        return CreateKnowledge( integration.ontology.types.point, position)
    else
        return nil
    end
end

integration.getSafetyPositionsFromFireObjects = function( objects, distance )
    local points = {}
    local nObjects = #objects
    local positionNextTo = meKnowledge:getPosition()
    for i = 1, nObjects do
        local object = objects[ i ]
        scaledObject = DEC_Geometrie_AgrandirLocalisation(  object:getLocalisation() , distance )
        objectPosition = DEC_Geometrie_ComputeNearestBorder( positionNextTo, scaledObject )
        points[#points + 1] = CreateKnowledge( integration.ontology.types.point, objectPosition )
    end
    return points
end