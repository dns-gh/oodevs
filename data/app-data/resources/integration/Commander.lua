integration.getCommanderPosition = function( commander )
    return DEC_Automate_Position( commander.source )
end

integration.getSubordinateAgentsFromCommander = function( commander )
    return DEC_Automate_PionsDeAutomateAvecPC( commander.source )
end

integration.getSubordinatePosition = function( subordinate )
    return DEC_Automate_Position( subordinate.source ) -- $$$ GGE: TODO a virer ? Doublon de getCommanderPosition. Etrange qu'un subordonné appelle une fonction DEC niveau automate
end

integration.getAgentsFromCommander = function( commander )
    return commander:DEC_Automate_PionsAvecPC()
end

integration.isCommanderEngaged = function( self )
    return DEC_Agent_AutomateEstEmbraye()
end

integration.isAgentCommanderEngaged = function( agent )
    return agent:DEC_Agent_AutomateEstEmbraye()
end

integration.commanderGetHQUnit = function( self )
    return DEC_Automate_PionPC()
end

integration.getCommander = function( entity )
    return DEC_GetAutomate( entity )
end

integration.isCommanderContaminated = function( entity )
    return DEC_Automate_PionEstContamine( entity )
end

integration.isCommanderPoisoned = function( entity )
    return DEC_Automate_PionEstEmpoisonne( entity )
end

integration.getPopulationROE = function( )
    return DEC_Automate_ROEPopulation( )
end

integration.getPopulationROEExist = function( )
    return DEC_Automate_ROEPopulation
end

integration.getCommanderROE = function( )
    return DEC_Automate_ROE( )
end

integration.getCommanderROEExist = function( )
    return DEC_Automate_ROE
end

integration.isEngagedCommander = function( )
    return DEC_Automate_EstEmbraye( )
end

integration.getCommanderEfficacity = function( commander, capacity )
    return DEC_Automate_GetEfficacite( commander, capacity )
end

--- Returns the HQ (simulation agent) of a company, or nil if there is none.
-- @return Simulation agent (or nil if there is no HQ).
integration.getHQ = function( )
    return DEC_Automate_PionPCDeAutomate( DEC_GetAutomate( myself ) )
end

--- Returns the HQ (DirectIA agent) of a company, or nil if there is none.
-- @return DirectIA agent (or nil if there is no HQ).
integration.getKnowledgeHQ = function( )
    local simHQ = integration.getHQ()
    return simHQ and CreateKnowledge( integration.ontology.types.agent, simHQ )
end
