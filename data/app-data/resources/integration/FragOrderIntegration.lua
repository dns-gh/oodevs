-------------------------------------------------------------------
---- FRAGMENTARY ORDERS METHODS IMPLEMENTATION
-------------------------------------------------------------------

--- Removes the given fragmentary order from the list of received fragmentary orders.
-- Removes the fragmentary order from myself.fragOrders table if it exists.
-- The myself.fragOrders table is used by agents of the simulation to received fragmentary orders from commanding agents.
-- @see integration.getOrdersCategory 
-- @param fragOrder a DirectIA fragmentary order knowledge
integration.cleanFragOrder = function( fragOrder )
    if myself.fragOrders then
        myself.fragOrders[ fragOrder ] = nil
    end
    DEC_RemoveFromOrdersCategory( fragOrder.source )
    DEC_DeleteRepresentation( fragOrder.source )
end

--- Sets the rule of engagement corresponding to the one issued as the given 'fragOrder' parameter.
-- Also displays a report.
-- Consumes the fragmentary order after use.
-- @see integration.cleanFragOrder
-- This method can only be called by an agent.
-- @see integration.getOrderConduiteChangerReglesEngagementParameter
-- @param fragOrder a DirectIA fragmentary order knowledge
integration.updateROE = function( fragOrder )
    DEC_Agent_ChangeEtatROE( integration.getOrderConduiteChangerReglesEngagementParameter( fragOrder ) )
    integration.CR_ROE ( integration.getROE() )
    integration.cleanFragOrder( fragOrder )
end

--- Sets the rule of engagement (ROE).
-- The ROE is defined by an enumeration.. 
-- Possible values are:
-- <ul> <li> eRoeStateFreeFire </li>
-- <li> eRoeStateRestrictedFire </li>
-- <li> eRoeStateFireByOrder </li> </ul>
-- It also displays a report.
-- This method can only be called by an agent.
-- @param etatROE the rule of engagement to be set.
integration.setROE = function( etatROE )
    DEC_Agent_ChangeEtatROE( etatROE )
    integration.CR_ROE ( etatROE )
end

--- Returns the current rule of engagement.
-- This method can only be called by an agent.
-- Possible values are:
-- <ul> <li> eRoeStateFreeFire </li>
-- <li> eRoeStateRestrictedFire </li>
-- <li> eRoeStateFireByOrder </li> </ul>
-- It also displays a report.
-- This method can only be called by an agent.
-- @return Integer the current rule of engagement.
integration.getROE = function()
    return DEC_Agent_GetEtatROE()
end

--- Sets the rule of engagement (ROE) for an automat.
-- The ROE is defined by an enumeration. 
-- Possible values are:
-- <ul> <li> eRoeStateFreeFire </li>
-- <li> eRoeStateRestrictedFire </li>
-- <li> eRoeStateFireByOrder </li> </ul>
-- It also displays a report.
-- This method can only be called by an automat.
-- @param etatROE the rule of engagement to be set.
integration.setCompanyROE = function( etatROE )
   DEC_Automate_ChangeEtatROE( etatROE )
   integration.CR_ROE ( etatROE )
end

--- Displays a report depending on the issued ROE.
-- The ROE are defined by an enumeration. 
-- Possible values are:
-- <ul> <li> eRoeStateFreeFire </li>
-- <li> eRoeStateRestrictedFire </li>
-- <li> eRoeStateFireByOrder </li> </ul>
-- @param typeROE the rule of engagement.
integration.CR_ROE  = function( typeROE )
    if typeROE == eRoeStateRestrictedFire then
        reportFunction(eRC_TirRestreint )
    elseif typeROE ==  eRoeStateFireByOrder then
        reportFunction(eRC_TirInterdit )
    elseif typeROE == eRoeStateFreeFire then
        reportFunction(eRC_TirLibre )
    end
end

--- Displays a report depending on the issued the rule of engagement (ROE) toward crowds.
-- The crowds ROE are defined by an enumeration. 
-- Possible values are:
-- <ul> <li> eEtatROEPopulation_EmploiForceInterdit</li>
-- <li> eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux </li>
-- <li> eEtatROEPopulation_ArmesLetalesAutorisees </li> </ul>
-- @param typeROE the rule of engagement.
integration.CR_ROE_Foules  = function( typeROE )
    if typeROE == eEtatROEPopulation_EmploiForceInterdit then
        reportFunction(eRC_EmploiForceInterdit )
    elseif typeROE ==  eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux then
        reportFunction(eRC_MaintienADistanceParMoyensNonLetaux )
    elseif typeROE == eEtatROEPopulation_ArmesLetalesAutorisees then
        reportFunction(eRC_ArmesLetalesAutorisees )
    end
end

--- Displays a report depending on the given crowd 'attitude'. 
-- The crowds 'attitude' is defined by an enumeration which values can be:
-- <ul> <li> eAttitudeCalme </li>
-- <li> eAttitudeAgitee </li>
-- <li> eAttitudeAgressive </li>
-- <li> eAttitudeExcitee </li> </ul>
-- @param attitude the crowd's attitude.
integration.CR_Attitude_Foules  = function ( attitude )
    if attitude == eAttitudeCalme then
        reportFunction( eRC_EmploiForceInterdit )
    elseif attitude == eAttitudeAgitee then
        reportFunction( eRC_MaintienADistanceParMoyensNonLetaux )
    elseif attitude == eAttitudeExcitee then
        reportFunction( eRC_ArmesLetalesAutorisees )
    elseif attitude == eAttitudeAgressive then
        reportFunction( eRC_ArmesLetalesAutorisees )
    end
end

--- For an automat, set the rule of engagement (ROE) for crowds, with one issued as parameter.
-- The crowds ROE are defined by an enumeration. 
-- Possible values are:
-- <ul> <li> eEtatROEPopulation_EmploiForceInterdit</li>
-- <li> eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux </li>
-- <li> eEtatROEPopulation_ArmesLetalesAutorisees </li> </ul>
-- This method can only be called by an automat.
-- @param roe the rule of engagement to be set.
integration.changeCrowdROEForAutomat = function( roe )
    DEC_Automate_ChangeEtatROEPopulation( roe )
end

--- Set the rule of engagement (ROE) for crowds, with one issued as parameter.
-- The ROE is defined by an enumeration. 
-- Possible values are:
-- <ul> <li> eEtatROEPopulation_EmploiForceInterdit</li>
-- <li> eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux </li>
-- <li> eEtatROEPopulation_ArmesLetalesAutorisees </li> </ul>
-- This method can only be called by an agent.
-- @param roe the rule of engagement to be set.
integration.changeCrowdROEForAgent = function( roe )
    DEC_Agent_ChangeEtatROEPopulation( roe )
end

--- Returns the current rule of engagement towards crowds.
-- This method can only be called by an agent.
-- @return Integer, the current rule of crowd engagement among one of the following constants:
-- <ul> <li> eEtatROEPopulation_EmploiForceInterdit (lethal fire forbidden) </li>
-- <li> eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux (maintaining distances with non-lethal fire) </li>
-- <li> eEtatROEPopulation_ArmesLetalesAutorisees (lethal fire authorized) </li> </ul>
integration.getCrowdROEForAgent = function()
    return DEC_Agent_RoePopulation()
end

--- Set the "attitude" (safety attitude or speedy attitude), using a given fragmentary order.
-- The attitude values can be:
-- <ul> <li> eEtatAmbiance_Surete</li>
-- <li> eEtatAmbiance_Vitesse </li> </ul>
-- This method can only be called by an agent.
-- @param fragOrder a fragmentary order
-- @param value integer, the enumeration value
integration.setOrderConduiteChangerAmbiance = function( fragOrder, value )
    fragOrder:SetorderConduiteChangerAmbiance_( value )
end

--- Returns the 'work mode' of the maintenance logistic chain, set as parameter in the given fragmentary order.
-- The 'work mode' is an enumeration defined in authoring tool in the 'Missions' and 'Fragmentary' tab.
-- See the 'Change work mode' fragmentary order in authoring tool.
-- @param fragorder a fragmentary order
-- @return integer the value of the 'work mode' enumeration.
integration.getOrderConduiteModifierRegimeTravailMaintenanceParameter = function( fragorder )
    return fragorder.source:GetorderConduiteModifierRegimeTravailMaintenance_()
end

--- Returns the list of automatons to be processed in priority by the logistic health chain. 
-- The priority is defined by the order of the automatons in the given list (first element has the priority). 
-- The list is set as a parameter of the given fragmentary order.
-- See the 'Change tactical wounded treatment priorities' fragmentary order in authoring tool.
-- @param fragorder a fragmentary order
-- @return a list of simulation automatons.
integration.getOrderConduiteModifierPrioritesTactiquesBlessesParameter = function( fragorder )
    return fragorder.source:GetorderConduiteModifierPrioritesTactiquesBlesses_()
end

--- Returns the list of automatons to be processed in priority by the logistic reparation chain.
-- The priority is defined by the order of the automatons in the given list (first element has the priority).
-- The list is set as a parameter of the given fragmentary order.
-- See the 'Change tactical reparations priorities' fragmentary order in authoring tool.
-- @param fragorder a fragmentary order
-- @return a list of simulation automatons.
integration.getOrderConduiteModifierPrioritesTactiquesReparationsParameter = function( fragorder )
    return fragorder.source:GetorderConduiteModifierPrioritesTactiquesReparations_()
end

--- Returns the list of equipement types to be processed in priority by the logistic reparation chain.
-- The priority is defined by the order of the equipement types in the given list (first element has the priority).
-- The list is set as a parameter of the given fragmentary order.
-- See the 'Change reparations priorities' fragmentary order in authoring tool.
-- @param fragorder a fragmentary order
-- @return a list of equipement types. 
integration.getOrderConduiteModifierPrioritesReparationsParameter = function( fragorder )
    return fragorder.source:GetorderConduiteModifierPrioritesReparations_()
end

--- Returns the fragmentary order's "Equipment type list" parameter
-- @param fragorder DirectIA fragmentary order
-- @param parameterName String, the name of the parameter
-- @return List of equipment types
integration.getEquipmentTypeListParameter = function( fragorder, parameterName )
    return fragorder.source:GetEquipmentTypeListParameter( parameterName )
end

--- Returns the fragmentary order's "Integer" parameter with the given name.
-- @param fragorder DirectIA fragmentary order
-- @param parameterName String, the name of the parameter
-- @return Integer
integration.getIntegerParameter = function( fragorder, parameterName )
    if not fragorder.source:HasIntegerParameter( parameterName ) then
        return nil
    end
    return fragorder.source:GetIntegerParameter( parameterName )
end

--- Returns the fragmentary order's "Automat list" parameter with the given name.
-- @param fragorder DirectIA fragmentary order
-- @param parameterName String, the name of the parameter
-- @return List of simulation companies
integration.getAutomatListParameter = function( fragorder, parameterName )
    return fragorder.source:GetAutomatListParameter( parameterName )
end

--- Returns the fragmentary order's "Agent" parameter with the given name.
-- @param fragorder DirectIA fragmentary order
-- @param parameterName String, the name of the parameter
-- @return Simulation agent
integration.getAgentTypeParameter = function( fragorder, parameterName )
    return fragorder.source:GetAgentParameter( parameterName )
end

--- Returns the fragmentary order's "Location" parameter with the given name.
-- @param fragorder DirectIA fragmentary order
-- @param parameterName String, the name of the parameter
-- @return Simulation area
integration.getLocationParameter = function( fragorder, parameterName )
    return fragorder.source:GetLocationParameter( parameterName )
end

--- Returns the fragmentary order's "Point" parameter with the given name.
-- @param fragorder DirectIA fragmentary order
-- @param parameterName String, the name of the parameter
-- @return Simulation point
integration.getPointParameter = function( fragorder, parameterName )
    return fragorder.source:GetPointParameter( parameterName )
end

--- Returns the fragmentary order's "Human wounds list" parameter with the given name.
-- @param fragorder DirectIA fragmentary order
-- @param parameterName String, the name of the parameter
-- @return List of simulation human wounds
integration.getHumanWoundsListParameter = function( fragorder, parameterName )
    return fragorder.source:GetMedicalPriorityParameter( parameterName )
end

--- Returns the fragmentary order's "Agent knowledge" parameter with the given name.
-- @param fragorder DirectIA fragmentary order
-- @param parameterName String, the name of the parameter
-- @return Simulation agent knowledge
integration.getAgentKnowledgeTypeParameter = function( fragorder, parameterName )
    return fragorder.source:GetAgentKnowledgeParameter( parameterName )
end

--- Returns the fragmentary order's "Resource type" parameter with the given name.
-- @param fragorder DirectIA fragmentary order
-- @param parameterName String, the name of the parameter
-- @return Simulation resource type
integration.getResourceTypeParameter = function( fragorder, parameterName )
    return fragorder.source:GetResourceTypeParameter( parameterName )
end

--- Returns the fragmentary order's "Resource type list" parameter with the given name.
-- @param fragorder DirectIA fragmentary order
-- @param parameterName String, the name of the parameter
-- @return List of simulation resource types
integration.getResourceTypeListParameter = function( fragorder, parameterName )
    return fragorder.source:GetResourceTypeListParameter( parameterName )
end

--- Returns the fragmentary order's "Object" parameter with the given name.
-- @param fragorder DirectIA fragmentary order
-- @param parameterName String, the name of the parameter
-- @return Simulation object
integration.getObjectParameter = function( fragorder, parameterName )
    return fragorder.source:GetObjectParameter( parameterName )
end

--- Returns the fragmentary order's "Object list" parameter with the given name.
-- @param fragorder DirectIA fragmentary order
-- @param parameterName String, the name of the parameter
-- @return List of simulation objects
integration.getObjectListParameter = function( fragorder, parameterName )
    return fragorder.source:GetObjectListParameter( parameterName )
end

--- Returns the list of urgence types to be processed in priority by the logistic health chain.
-- The medical priority is defined by the order of the urgence types in the returned list (first element has the priority).
-- The list is set as a parameter of the given fragmentary order.
-- See the 'Change wounded treatment priorities' fragmentary order in authoring tool.
-- @param fragorder a fragmentary order
-- @return a list of urgence types.
integration.getOrderConduiteModifierPrioritesBlessesParameter = function( fragorder )
    return fragorder.source:GetorderConduiteModifierPrioritesBlesses_()
end

--- Returns the rule of engagement (ROE) toward crowds set as parameter in the given fragmentary order.
-- The rules of engagement are defined by an enumeration which values are set in authoring 
-- tool in the 'Missions' and 'Fragmentary' tab.
-- See the 'Change crowd ROE' fragmentary order in authoring tool.
-- @param fragorder a fragmentary order
-- @return integer the value corresponding to the rule of engagement.
integration.getOrderConduiteChangerReglesEngagementPopulationParameter = function( fragorder )
    return fragorder.source:GetorderConduiteChangerReglesEngagementPopulation_()
end

--- Returns the rule of engagement set as parameter in the given fragmentary order.
-- The rules of engagement are defined by an enumeration which values are set in authoring 
-- tool in the 'Missions' and 'Fragmentary' tab.
-- See the 'Change ROE' fragmentary order in authoring tool.
-- @param fragorder a fragmentary order
-- @return integer the value corresponding to the rule of engagement.
integration.getOrderConduiteChangerReglesEngagementParameter = function( fragorder )
    return fragorder.source:GetorderConduiteChangerReglesEngagement_()
end

--- Returns the agent knowledge parameter of the given fragmentary order.
-- @param fragorder a fragmentary order
-- @return a knowledge agent.
integration.getAgentKnowledgeParameter = function( fragorder )
    return fragorder.source:GetAgentKnowledge_()
end

--- Returns the agent parameter of the given fragmentary order.
-- The 'fragorder' parameter must be of type 'agent_' as defined 
-- in the fragmentary order definition (see authoring tool,'Missions' then 'Fragmentary' tab).
-- @param fragorder a fragmentary order
-- @return an agent.
integration.getAgentParameter = function( fragorder )
    return fragorder.source:GetAgent_()
end

--- Returns the 'attitude' parameter of the given fragmentary order.
-- The 'attitude' parameter is defined by an enumeration which values are set in the authoring tool.
-- in the fragmentary order definition.
-- See the 'Switch posture' fragmentary order in authoring tool.
-- @param fragorder a fragmentary order
-- @return integer the value corresponding to the attitude.
integration.getConduiteChangerAmbianceParameter = function( fragorder )
    return fragorder.source:GetorderConduiteChangerAmbiance_()
end

--- Returns the 'resource type' parameter of the given fragmentary order.
-- See the 'Fire' and 'Fire in area' fragmentary orders in authoring tool.
-- @param fragorder a fragmentary order
-- @return the ressource type.
integration.getMunitionParameter = function( fragorder )
    return fragorder.source:Getmunition_()
end

--- Returns a list of 'resource type', set as parameter in the given fragmentary order.
-- See the 'Fire' and 'Fire in area' fragmentary orders definitions in authoring tool.
-- @param fragorder a fragmentary order
-- @return the list of ressource types.
integration.getMunitionsParameter = function( fragorder )
    return fragorder.source:Getmunitions_()
end

--- Returns the 'number of salvoes' parameter of the given fragmentary order.
-- See the 'Fire' and 'Fire in area' fragmentary orders definitions in authoring tool.
-- @param fragorder a fragmentary order
-- @return integer a number of salvoes.
integration.getNbItParameter = function( fragorder )
    return fragorder.source:GetnbIT_()
end

--- Returns the 'target position' parameter of the given fragmentary order.
-- See the 'Fire' and 'Fire in area' fragmentary orders definitions in authoring tool.
-- @param fragorder a fragmentary order
-- @return a simulation position.
integration.getpointCibleParameter = function( fragorder )
    return fragorder.source:GetpointCible_()
end

--- Returns the requester of the given fragmentary order
-- @param fragorder a fragmentary order
-- @return integer the value corresponding to the attitude
integration.getRequesterParameter = function( fragorder, parameterName )
    local requester = nil
    local agent = nil
    local agentKnowledge = integration.getAgentKnowledgeTypeParameter( fragorder, parameterName )
    if agentKnowledge then
        agent = DEC_ConnaissanceAgent_EnAgent( agentKnowledge )
    else
        agent = integration.getAgentTypeParameter( fragorder, parameterName )
    end
    if agent then
        requester = CreateKnowledge( integration.ontology.types.agent, agent )
    end
    return requester
end

--- Returns the crowd 'attitude' parameter of the given fragmentary order.
-- The crowd attitude is defined by an enumeration which values are set in the authoring tool. 
-- See the 'Population - Change attitude' fragmentary order definition in authoring tool.
-- @param fragorder a fragmentary order
-- @return integer the value corresponding to the attitude
integration.getorderConduitePopulationChangerAttitudeParameter = function( fragorder )
    return fragorder.source:GetorderConduitePopulationChangerAttitude_()
end

--- Returns the 'agent' to be reinforced (with new equipments), set as parameter in the given fragmentary order.
-- See the 'Get back lended ambulances' fragmentary order definition in authoring tool.
-- @param fragorder a fragmentary order
-- @return the agent
integration.getPionARenforcerParameter = function( fragorder )
    return fragorder.source:GetpionARenforcer_()
end

--- Returns the reinforced 'agent', set as parameter in the given fragmentary order.
-- See the 'Send lended ambulances' and 'Send lended carriers' fragmentary orders definitions in authoring tool.
-- @param fragorder a fragmentary order
-- @return the agent
integration.getPionRenforceParameter = function( fragorder )
    return fragorder.source:GetpionRenforce_()
end

--- Returns the number of ambulances to be used for medical evacuation, set as parameter in the given fragmentary order.
-- See the 'Send lended ambulances' fragmentary order definition in authoring tool.
-- @param fragorder a fragmentary order
-- @return integer number of ambulances
integration.getNbrAmbulancesParameter = function( fragorder )
    return fragorder.source:GetnbrAmbulances_()
end

--- Returns the number of tow equipements to be used for maintenance evacuation, set as parameter in the given fragmentary order.
-- See the 'Send lended carriers' fragmentary order definition in authoring tool.
-- @param fragorder a fragmentary order
-- @return integer number of equipements
integration.getNbrRemorqueursParameter = function( fragorder )
    return fragorder.source:GetnbrRemorqueurs_()
end

--- Returns 'object' set as parameter in the given fragmentary order.
-- @param fragorder a fragmentary order
-- @return a simulation object
integration.getObjectKnowledgeParameter = function( fragorder )
    return fragorder.source:GetObjectKnowledge_()
end

--- Returns the type of in the given fragmentary order.
-- @see integration.getAnyType method.
-- @param fragorder a fragmentary order
-- @return string the name of the fragmentary order
integration.getOrderType = function( fragorder )
    return integration.getAnyType( fragorder.source )
end

--- Returns the list of fragmentary orders representation to be processed by the agent.
-- @return a list of orders
integration.getOrdersCategory = function( )
    return DEC_GetOrdersCategory()
end

--- Returns a table containing all parameters to be used to apply the 'Fire' fragorder.
-- See the 'Fire' fragmentary order in authoring tool.
-- @see integration.getMunitionParameter
-- @see integration.getNbItParameter
-- @see integration.getpointCibleParameter
-- @see integration.getAgentKnowledgeParameter
-- @param fragorder the fire fragmentary order
-- @return a simulation object
integration.getFireParameters = function( fragOrder )
    local params = {}
    local uggly = function() 
        params.entities = { CreateKnowledge( integration.ontology.types.agentKnowledge, integration.getAgentKnowledgeParameter( fragOrder ) ) } 
    end
    if not pcall( uggly ) then
        params.entities = { CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( fragOrder ) ) }
    end
    params.munition = integration.getMunitionParameter( fragOrder )
    params.interventionType = integration.getNbItParameter( fragOrder )
    return params
end

--- Create a fragmentary order knowledge of the given type.
-- @param fragOrderType a string
-- @return the created fragmentary order
integration.createFragOrder = function( fragOrderType )
    return CreateFragOrder( fragOrderType )
end