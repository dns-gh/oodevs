-------------------------------------------------------------------
---- BYPASSABLE INTERFACE IMPLEMENTATION
-------------------------------------------------------------------

--- Returns the company location
-- Can be called by an agent or a company
-- @param commander Ditectia commander
-- @return location Simulation location
integration.getCommanderPosition = function( commander )
    return DEC_Automate_Position( commander.source )
end

--- Returns the company subordinates with the HQ
-- Can be called by an agent or a company
-- @param commander Directia commander
-- @return list of simulation agents
integration.getSubordinateAgentsFromCommander = function( commander )
    return DEC_Automate_PionsDeAutomateAvecPC( commander.source )
end

--- Deprecated  : use integration.getCommanderPosition
-- Returns the company location
-- Can be called by an agent or a company
-- @param subordinate Directia commander
-- @return location Simulation location 
integration.getSubordinatePosition = function( commander )
    return DEC_Automate_Position( commander.source ) 
end

--- Returns the company subordinates with the HQ
-- Can be called by an agent or a company
-- @param commander Simulation commander
-- @return list of simulation agents
integration.getAgentsFromCommander = function( commander )
    return commander:DEC_Automate_PionsAvecPC()
end

--- Returns true if the company is engaged, false otherwise
-- This method can only be called by an agent
-- @param self Deprecated, unused
-- @return Boolean, whether or not the company is engaged
integration.isCommanderEngaged = function( self )
    return DEC_Agent_AutomateEstEmbraye()
end

--- Returns true if the company of the provided simulation agent is engaged, false otherwise
-- This method can only be called by an agent
-- @param agent Simulation agent
-- @return Boolean, whether or not the company is engaged
integration.isAgentCommanderEngaged = function( agent )
    return agent:DEC_Agent_AutomateEstEmbraye()
end

--- Returns the HQ of the company
-- Can be called by an agent or a company
-- @param self Deprecated, unused
-- @return Simulation agent
integration.commanderGetHQUnit = function( self )
    return DEC_Automate_PionPC()
end

--- Returns the company of the unit
-- Can be called by an agent or a company
-- @param entity Simulation agent
-- @return Simulation commander
integration.getCommander = function( entity )
    return DEC_GetAutomate( entity )
end

--- Returns true if the unit is contaminated, false otherwise
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

--- Returns the rules of engagement for company against crowd
-- This method can only be called by a company
-- @return Integer, the rules of engagement type among one of the following : 
-- <ul> <li> eEtatROEPopulation_EmploiForceInterdit (forbidden fire) </li>
-- <li> eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux (non-lethal weapons) </li>
-- <li> eEtatROEPopulation_ArmesLetalesAutorisees (lethal weapons allowed) </li> </ul>
integration.getPopulationROE = function( )
    return DEC_Automate_ROEPopulation( )
end

--- Returns true if method DEC_Automate_ROEPopulation exists in simulation
-- This method can only be called by a company
-- @see integration.getCommanderROE
-- @return Boolean, whether or not the method exists in simulation
integration.getPopulationROEExist = function( )
    return DEC_Automate_ROEPopulation
end

--- Returns the rules of engagement for company against military units 
-- This method can only be called by a company
-- @return rules of engagement type, Integer among one of the following
-- <ul> <li> eRoeStateFreeFire </li>
-- <li> eRoeStateRestrictedFire </li>
-- <li> eRoeStateFireByOrder </li> </ul>  
integration.getCommanderROE = function( )
    return DEC_Automate_ROE( )
end

--- Returns true if method DEC_Automate_ROEPopulation exists in simulation
-- This method can only be called by a company
-- @see integration.getCommanderROE 
-- @return Boolean, whether or not the method exists in simulation
integration.getCommanderROEExist = function( )
    return DEC_Automate_ROE
end

--- Returns true if the company is engaged, false otherwise
-- Can be called by an agent or a company
-- @return Boolean, whether or not the company is engagd 
integration.isEngagedCommander = function( )
    return DEC_Automate_EstEmbraye( )
end

--- Returns the agent efficiency for the selected capacity
-- This method can only be called by a company
-- @param agent DirectIA agent
-- @param capacity, Integer among one of the following :
-- <ul> <li> PionEfficiencyRecon </li>
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

---- Returns the HQ (simulation agent) of a company
-- Can be called by an agent or a company
-- @return Simulation agent
integration.getHQ = function( )
    return DEC_Automate_PionPCDeAutomate( DEC_GetAutomate( myself ) )
end

---- Returns the HQ (directIA agent knowledge) of a company
-- Can be called by an agent or a company
-- @return knowledge HQ
integration.getKnowledgeHQ = function( )
    return CreateKnowledge( integration.ontology.types.agent, integration.getHQ() )
end