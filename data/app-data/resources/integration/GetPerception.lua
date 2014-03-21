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

integration.connaissanceAgentEstAPorteDeCapteurDansCone = function( eni, direction, angleOuverture )
    return DEC_ConnaissanceAgent_EstAPorteDeCapteurDansCone( eni.source, direction.source, angleOuverture )
end

integration.knowledgePerceivedLivingEnemiesAgents = function( )
    return DEC_Connaissances_UnitesEnnemiesVivantesPercues()
end
