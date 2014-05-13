local immutablePosition = function( knowledge )
   return (not knowledge.proxy) and (knowledge == meKnowledge or not masalife.brain.core.class.isOfType( knowledge, integration.ontology.types.agent ))
end

--- Return the physical level of perception of a knowledge. This value is provided by the simulation when
-- the knowledge is perceived by the agent.
-- This method can only be called on an agent.
-- @param knowledgeA Directia knowledge defining a getPosition method. Position from which to perceive
-- @param knowledgeB Directia knowledge defining a getPosition method. Position to perceive
-- @return the value of the physical level of perception, between 0 and the maximum perception distance of the perceiving agent, in meters.
integration.getPerception = function( knowledgeA, knowledgeB )
    if immutablePosition( knowledgeA ) and immutablePosition( knowledgeB ) then
        knowledgeA.source.cachegetPerception = knowledgeA.source.cachegetPerception or {}
        knowledgeA.source.cachegetPerception[ knowledgeB.source ] = knowledgeA.source.cachegetPerception[ knowledgeB.source ] or DEC_GetPerception( knowledgeA:getPosition(), knowledgeB:getPosition() )
        return knowledgeA.source.cachegetPerception[ knowledgeB.source ]
    end
    return DEC_GetPerception( knowledgeA:getPosition(), knowledgeB:getPosition() )
end

--- Returns true if the given agent knowledge is inside the cone of perception of this agent.
-- The cone of perception is defined by the given direction and angle,
-- and its distance is determined by this agent's sensors max range (defined in the authoring tool).
-- This method returns false if the given agent knowledge is not valid.
-- This method can only be called by an agent.
-- @param eni DirectIA agent knowledge
-- @param direction Direction knowledge
-- @param angle Float, angle in degrees
-- @return Boolean
integration.connaissanceAgentEstAPorteDeCapteurDansCone = function( eni, direction, angle )
    return DEC_ConnaissanceAgent_EstAPorteDeCapteurDansCone( eni.source, direction.source, angle )
end

--- Returns a list of all perceived living enemies by this agent.
-- This method can only be called by an agent.
-- @return List of agent knowledges
integration.knowledgePerceivedLivingEnemiesAgents = function( )
    return DEC_Connaissances_UnitesEnnemiesVivantesPercues()
end
