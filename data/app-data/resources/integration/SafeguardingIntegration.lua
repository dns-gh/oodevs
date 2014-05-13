--- Returns the given urban block's cover and concealement level to this entity
-- to protect it from the given enemy.
-- This method can only be called by an agent.
-- @param urbanBlock Urban block knowledge
-- @param entity Deprecated
-- @param enemy DirectIA agent knowledge
-- @return Float between 0 and 100, 0 meaning no protection and 100 meaning full protection.
integration.getUrbanBlockCoverAndConcealmentLevelFor = function( urbanBlock, entity, enemy )
    local res = 0
    local protectionLevel = DEC_NiveauDeProtectionMaterielComposantes( enemy.source, urbanBlock.source )
    -- If the urban block does not have much attrition, then it may give cover to the entity
    if protectionLevel ~= -1 then
        res = res + ( protectionLevel * DEC_ConnaissanceBlocUrbain_RapForLocal( urbanBlock.source ) * 50 ) -- $$$ MIA sortir DEC_ConnaissanceBlocUrbain_RapForLocal dans la notion de dangerosité d'un BU.
        -- If the entity has a favorable local force ratio (inside the urban block), then the urban block gives
        -- cover to the entity.
        if res < 0 then
            res = 0
        elseif res > 50 then
            res = 50
        end
    end
    -- The urban block has even more protection capability if it is concealed to the enemy.
    if integration.getPerception( urbanBlock, enemy ) == 0 then
        res = res + 50
    end
    return res
end

--- Returns a safe position near the given crowd (i.e. a position at the given distance of
-- the periphery of the given crowd at the opposite of the current danger direction).
-- This method can only be called by an agent.
-- @param crowd Crowd knowledge
-- @param distance Float
-- @return Point knowledge, or nil if the given crowd is not known by this entity.
integration.getSafetyPositionFromCrowd = function( crowd, distance )
    local position = DEC_Geometrie_CalculerPositionSureteAvecPopulation( crowd.source, distance )
    if position then
        return CreateKnowledge( integration.ontology.types.point, position )
    end
    return nil
end

--- Returns a list of positions safe from the given objects.
-- For each given object, a safe position is computed (a position at
-- the given distance from the object's border).
-- This method can only be called by an agent, defining a "getPosition" method returning its position.
-- @param objects List of object knowledges, each defining a "getLocalisation" method returning an area knowledge
-- @param distance Float
-- @return List of point knowledges
integration.getSafetyPositionsFromFireObjects = function( objects, distance )
    local points = {}
    local positionNextTo = meKnowledge:getPosition()
    local DEC_Geometrie_AgrandirLocalisation = DEC_Geometrie_AgrandirLocalisation
    local DEC_Geometrie_ComputeNearestBorder = DEC_Geometrie_ComputeNearestBorder
    local CreateKnowledge = CreateKnowledge
    local integration = integration

    for i = 1, #objects do
        scaledObject = DEC_Geometrie_AgrandirLocalisation( objects[i]:getLocalisation(), distance )
        objectPosition = DEC_Geometrie_ComputeNearestBorder( positionNextTo, scaledObject )
        points[i] = CreateKnowledge( integration.ontology.types.point, objectPosition )
    end
    return points
end

--- Computes a position away from the given agent knowledges at the provided distance
-- from the barycenter of the closest given agent knowledges, and returns it if
-- it is deemed sufficiently distant from these agent knowledges.
-- Example :
--
--   e         , - ~ ~ ~ - ,
--         , '               ' ,
--       ,                  e    ,
--      ,                         ,
--     ,                       e   ,
--     ,   C2        M  C1 X       ,
--     ,                           ,
--      ,                  e      ,
--  e    ,         e             ,
--         ,                  , '
--           ' - , _ _ _ ,  '
--
-- e : enemy
-- M : this agent's position (myPosition)
-- X : barycenter (simBaryPos)
-- The safety position is computed with a translation from X towards M with the given distance (distanceToGo)
-- C1 : computed safety position for a distanceToGo value lesser than the distance between X and M,
-- which is deemed not sufficiently distant (and therefore is not returned).
-- C2 : computed safety position for a distanceToGo value greater than the distance between X and M,
-- which is deemed sufficiently distant (and therefore is returned).
-- If the knowledgeAgents parameter is empty (or has no valid agent), this method returns nil.
-- This method can only be called by an agent, defining a "getPosition" method returning its position.
-- @param knowledgeAgents Table of agent knowledges, each defining a "getPosition" method 
-- returning the agent's simulation position; and an "isValid" method, returning whether
-- or not the agent is valid.
-- @param distanceToGo Float
-- @return Simulation point (or nil)
integration.getSimPositionAwayFromknowledgeAgents = function( knowledgeAgents, distanceToGo )
    -- Compute valid agent positions once and for all
    local kAgentPositions = {}
    for _, kAgent in pairs( knowledgeAgents ) do
        if kAgent:isValid() then
            kAgentPositions[ #kAgentPositions + 1 ] = kAgentPositions:getPosition()
        end
    end
    
    -- If there are no agent (or no valid agent), return nil
    if #kAgentPositions == 0 then
        return nil
    end

    local DEC_Geometrie_Distance = DEC_Geometrie_Distance
    local myPosition = meKnowledge:getPosition()

    -- Computing to average distance between the agent and the given agents
    local totalDistance = 0
    for i = 1, #kAgentPositions do
        totalDistance = totalDistance + DEC_Geometrie_Distance( kAgentPositions[i], myPosition )
    end
    local averageDistance = totalDistance / #kAgentPositions

    -- Get the agents located into the circle with a radius defined by the average distance
    local simAgentsInCircle = {}
    for i = 1, #kAgentPositions do
        if DEC_Geometrie_Distance( kAgentPositions[i], myPosition ) < ( averageDistance + 100 ) then -- + 100 meters to avoid precision errors
            simAgentsInCircle[ #simAgentsInCircle + 1 ] = kAgent.source
        end
    end
 
    -- Computing a position by getting the barycenter of units located into the circle.
    -- The position is returned if it is located away from the agent. Else it is not returned
    local simBaryPos = DEC_Geometrie_CalculerBarycentreListeConnaissancesAgents( simAgentsInCircle )
    local dangerDirection = DEC_Geometrie_CreerDirection( simBaryPos, myPosition )
    local safetyPos = DEC_Geometrie_PositionTranslateDir( simBaryPos, dangerDirection, distanceToGo )
    if DEC_Geometrie_Distance( simBaryPos, myPosition ) < DEC_Geometrie_Distance( simBaryPos, safetyPos ) then
        return safetyPos
    else
        return nil
    end
end

------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

--- Deprecated
integration.getPointCoverAndConcealmentLevelFor = function( point, entity, objective )
    return 100 - objective:computeEstimatePerceptionCapability( entity, point )
end