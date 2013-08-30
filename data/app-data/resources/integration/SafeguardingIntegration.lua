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
    local positionNextTo = meKnowledge:getPosition()
    local DEC_Geometrie_AgrandirLocalisation = DEC_Geometrie_AgrandirLocalisation
    local DEC_Geometrie_ComputeNearestBorder = DEC_Geometrie_ComputeNearestBorder
    local CreateKnowledge = CreateKnowledge
    local integration = integration
    local nObjects = #objects

    for i = 1, nObjects do
        local object = objects[ i ]
        scaledObject = DEC_Geometrie_AgrandirLocalisation(  object:getLocalisation() , distance )
        objectPosition = DEC_Geometrie_ComputeNearestBorder( positionNextTo, scaledObject )
        points[#points + 1] = CreateKnowledge( integration.ontology.types.point, objectPosition )
    end
    return points
end

-- Compute a position away from issued agents.
-- /!\ /!\ /!\ CAN RETURN A NIL VALUE
integration.getSimPositionAwayFromknowledgeAgents = function( knowledgeAgents, distanceToGo )

    -- security: verifying the 'knowledgeAgents' is not empty. If yes return a nil value.
    if not next( knowledgeAgents ) then return nil end

    local DEC_Geometrie_Distance = DEC_Geometrie_Distance

    -- Computing to average distance between the agent and the given agents
    local totalDistance = 0
    for _, kAgent in pairs ( knowledgeAgents ) do
        if kAgent:isValid() then 
            totalDistance = totalDistance + DEC_Geometrie_Distance( kAgent:getPosition(), meKnowledge:getPosition() )
        end
    end
    local averageDistance = totalDistance / #knowledgeAgents

    -- Get the agents located into the circle with a radius defined by the average distance
    local simAgentsInCircle = {}
    for _, kAgent in pairs ( knowledgeAgents ) do
        if kAgent:isValid() then
            if DEC_Geometrie_Distance( kAgent:getPosition(), meKnowledge:getPosition() ) < averageDistance + 100 then -- + 100 meters to avoid precision errors
                simAgentsInCircle[ #simAgentsInCircle + 1 ] = kAgent.source
            end
        end
    end

    if not next( simAgentsInCircle ) then return nil end
 
    -- Computing a position by getting the barycenter of units located into the circle.
    -- The position is returned if it is located away from the agent. Else it is not returned
    local simBaryPos = DEC_Geometrie_CalculerBarycentreListeConnaissancesAgents( simAgentsInCircle )
    local dangerDirection = DEC_Geometrie_CreerDirection( simBaryPos, meKnowledge:getPosition() )
    local safetyPos = DEC_Geometrie_PositionTranslateDir( simBaryPos, dangerDirection, distanceToGo )
    if DEC_Geometrie_Distance( simBaryPos, meKnowledge:getPosition() ) < DEC_Geometrie_Distance( simBaryPos, safetyPos ) then
        return safetyPos
    else
        return nil
    end
end
