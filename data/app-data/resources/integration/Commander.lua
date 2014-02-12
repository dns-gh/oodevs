-------------------------------------------------------------------
---- BYPASSABLE INTERFACE IMPLEMENTATION
-------------------------------------------------------------------

--- Returns the company location
-- @param commander Directia commander
-- @return Simulation point
integration.getCommanderPosition = function( commander )
    return DEC_Automate_Position( commander.source )
end

--- Returns the company subordinates including the HQ
-- @param commander Directia commander
-- @return list of simulation agents
integration.getSubordinateAgentsFromCommander = function( commander )
    return DEC_Automate_PionsDeAutomateAvecPC( commander.source )
end

--- Deprecated : use integration.getCommanderPosition
-- Returns the company location
-- @param subordinate Directia commander
-- @return Simulation point 
integration.getSubordinatePosition = function( commander )
    return DEC_Automate_Position( commander.source ) 
end

--- Deprecated : use integration.getSubordinateAgentsFromCommander
-- Returns the company subordinates including the HQ
-- @param commander Simulation commander
-- @return list of simulation agents
integration.getAgentsFromCommander = function( commander )
    return commander:DEC_Automate_PionsAvecPC()
end

--- Returns true if the company is engaged, false otherwise
-- This method can only be called by an agent
-- @return Boolean, whether or not the company is engaged
integration.isCommanderEngaged = function( )
    return DEC_Agent_AutomateEstEmbraye()
end

--- Returns true if the company is engaged, false otherwise
-- @param agent Simulation agent
-- @return Boolean, whether or not the company is engaged
integration.isAgentCommanderEngaged = function( agent )
    return agent:DEC_Agent_AutomateEstEmbraye()
end

--- Returns the HQ of the company
-- @return Simulation agent
integration.commanderGetHQUnit = function( )
    return DEC_Automate_PionPC()
end

--- Returns the company of the agent
-- @param entity Simulation agent
-- @return Simulation commander
integration.getCommander = function( entity )
    return DEC_GetAutomate( entity )
end

--- Returns true if the agent is contaminated, false otherwise
-- This method can only be called by a company
-- @param entity Simulation agent
-- @return Boolean, whether or not the company is contaminated
integration.isCommanderContaminated = function( entity )
    return DEC_Automate_PionEstContamine( entity )
end

--- Returns true if the provided agent is poisoned
-- This method can only be called by a company
-- @param entity Simulation agent
-- @return Boolean, whether or not the company is poisoned
integration.isCommanderPoisoned = function( entity )
    return DEC_Automate_PionEstEmpoisonne( entity )
end

--- Returns the rules of engagement for this company against crowd
-- This method can only be called by a company
-- @return Integer, the rules of engagement type among one of the following : 
-- <ul> <li> eEtatROEPopulation_EmploiForceInterdit (forbidden fire) </li>
-- <li> eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux (non-lethal weapons) </li>
-- <li> eEtatROEPopulation_ArmesLetalesAutorisees (lethal weapons allowed) </li> </ul>
integration.getPopulationROE = DEC_Automate_ROEPopulation

--- Returns function integration.getPopulationROE
integration.getPopulationROEExist = function( )
    return integration.getPopulationROE
end

--- Returns the rules of engagement for company against military units 
-- This method can only be called by a company
-- @return rules of engagement type, Integer among one of the following
-- <ul> <li> eRoeStateFreeFire </li>
-- <li> eRoeStateRestrictedFire </li>
-- <li> eRoeStateFireByOrder </li> </ul>  
integration.getCommanderROE = DEC_Automate_ROE

--- Returns function integration.getCommanderROE
integration.getCommanderROEExist = function( )
    return integration.getCommanderROE
end

--- Returns true if the company is engaged, false otherwise
-- @return Boolean, whether or not the company is engaged 
integration.isEngagedCommander = function( )
    return DEC_Automate_EstEmbraye( )
end

--- Returns the agent efficiency for the selected capacity
-- This method can only be called by a company
-- @param agent DirectIA agent
-- @param capacity, Integer among one of the following :
-- <ul> <li> ePionEfficiencyRecon </li>
-- <li> ePionEfficiencyCombatSupport </li>
-- <li> ePionEfficiencyCombat </li>
-- <li> ePionEfficiencyMobilitySupport </li>
-- <li> ePionEfficiencyCounterMobilitySupport </li>
-- <li> ePionEfficiencyProtectionSupport </li>
-- <li> ePionEfficiencyEngineeringSupport </li>
-- <li> ePionEfficiencyUrbanArea </li>
-- <li> ePionEfficiencyCommon </li> </ul>  
-- @return Integer, value is between 0 and 100. The higher the percentage, the higher is the efficiency
integration.getCommanderEfficacity = function( agent, capacity )
    return DEC_Automate_GetEfficacite( agent, capacity )
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

