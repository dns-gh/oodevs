--- Returns true if the given agent can be relieved, false otherwise.
-- This method can only be called by an agent.
-- @param agent DirectIA agent
-- @return Boolean
integration.canRelieveIt = function( agent )
    return DEC_PeutReleverPion( agent.source )
end

--- Relieves the given agent (i.e. starts a new mission
-- identical to the given agent's current mission, and cancels
-- the given agent's mission).
-- If this entity cannot relieve the given agent, then nothing happens.
-- May display a report.
-- This method can only be called by an agent.
-- @param agent DirectIA agent
-- @return Boolean, whether or not the given agent's was successfully relieved.
integration.relieveIt = function( agent )
    if DEC_ReleverPion( agent.source ) then
        reportFunction( eRC_Releve, agent.source )
        return true
    end
    return false
end

--- Returns true if the given reliever agent can relieve the given agent to relieve.
-- This method can only be called by a company.
-- The two provided agents must be from this company.
-- @param agentRelieving Simulation agent
-- @param agentToRelieve Simulation agent
-- @return Boolean
integration.isAgentInAutomatCanRelieveAgent = function( agentRelieving, agentToRelieve )
    return DEC_Automate_PionPeutReleverPion( agentRelieving, agentToRelieve )
end

--- Makes the given reliever agent relieve the given agent to relieve
-- (i.e. makes it start a new mission identical to the given agent's
-- current mission, and cancels the given agent's mission).
-- This method can only be called by a company.
-- The two provided agents must be from this company.
-- @param agentRelieving Simulation agent
-- @param agentToRelieve Simulation agent
-- @return Boolean, whether or not the given agent to relieve has 
integration.agentInAutomatRelieveAgent = function( agentRelieving, agentToRelieve )
    return DEC_Automate_PionRelevePion( agentRelieving, agentToRelieve )
end