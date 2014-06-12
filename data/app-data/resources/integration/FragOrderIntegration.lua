-------------------------------------------------------------------------------
-- FragOrder Implementation :
-- Regroup function to manage fragorder
-- @author DDA
-- @created 2010-02-05
-- @modified MGD 2010-04-27
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------
integration.cleanFragOrder = function( fragOrder )
      if myself.fragOrders then
          myself.fragOrders[ fragOrder ] = nil
      end
      DEC_RemoveFromOrdersCategory( fragOrder.source)
      DEC_DeleteRepresentation( fragOrder.source )
end

integration.updateROE = function( self )
  DEC_Agent_ChangeEtatROE(integration.getOrderConduiteChangerReglesEngagementParameter( self ))
  integration.CR_ROE ( integration.getROE() )
  integration.cleanFragOrder( self )
end

integration.setROE = function( etatROE )
   DEC_Agent_ChangeEtatROE( etatROE )
   integration.CR_ROE ( etatROE )
end

integration.getROE = function()
    return DEC_Agent_GetEtatROE()
end

integration.setCompanyROE = function( etatROE )
   DEC_Automate_ChangeEtatROE( etatROE )
   integration.CR_ROE ( etatROE )
end


-- ============================================================================
-- FRAGO MANAGEMENT
-- Activate tasks depending on frag orders value.
-- ============================================================================

-- CR ROE 
integration.CR_ROE  = function( typeROE )
    if typeROE == eRoeStateRestrictedFire then
        reportFunction(eRC_TirRestreint )
    elseif typeROE ==  eRoeStateFireByOrder then
        reportFunction(eRC_TirInterdit )
    elseif typeROE == eRoeStateFreeFire then
        reportFunction(eRC_TirLibre )
    end
end

-- CR ROE Foule
integration.CR_ROE_Foules  = function( typeROE )
    if typeROE == eEtatROEPopulation_EmploiForceInterdit then
        reportFunction(eRC_EmploiForceInterdit )
    elseif typeROE ==  eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux then
        reportFunction(eRC_MaintienADistanceParMoyensNonLetaux )
    elseif typeROE == eEtatROEPopulation_ArmesLetalesAutorisees then
        reportFunction(eRC_ArmesLetalesAutorisees )
    end
end

-- CR Attitude Foule
integration.CR_Attitude_Foules  = function ( attitude )
    if attitude == eAttitudeCalme then
        reportFunction(eRC_EmploiForceInterdit )
    elseif attitude ==  eAttitudeAgitee then
        reportFunction(eRC_MaintienADistanceParMoyensNonLetaux )
    elseif attitude == eAttitudeExcitee then
        reportFunction(eRC_ArmesLetalesAutorisees )
    elseif attitude == eAttitudeAgressive then
        reportFunction(eRC_ArmesLetalesAutorisees )
    end
end

integration.changeCrowdROEForAutomat = function( roe )
    DEC_Automate_ChangeEtatROEPopulation( roe )
end

integration.changeCrowdROEForAgent = function( roe )
    DEC_Agent_ChangeEtatROEPopulation( roe )
end

--- Returns the current rule of engagement towards crowds for this entity.
-- This method can only be called by an agent.
-- @return Integer, the current rule of crowd engagement among one of the following constants :
-- <ul> <li> eEtatROEPopulation_EmploiForceInterdit (lethal fire forbidden) </li>
-- <li> eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux (maintaining distances with non-lethal fire) </li>
-- <li> eEtatROEPopulation_ArmesLetalesAutorisees (lethal fire authorized) </li> </ul>
integration.getCrowdROEForAgent = function()
    return DEC_Agent_RoePopulation()
end

integration.setOrderConduiteChangerAmbiance = function( fragOrder, value )
    fragOrder:SetorderConduiteChangerAmbiance_( value )
end

integration.getOrderConduiteModifierRegimeTravailMaintenanceParameter = function( fragorder )
    return fragorder.source:GetorderConduiteModifierRegimeTravailMaintenance_()
end

integration.getOrderConduiteModifierPrioritesTactiquesBlessesParameter = function( fragorder )
    return fragorder.source:GetorderConduiteModifierPrioritesTactiquesBlesses_()
end

integration.getOrderConduiteModifierPrioritesTactiquesReparationsParameter = function( fragorder )
    return fragorder.source:GetorderConduiteModifierPrioritesTactiquesReparations_()
end

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

--- Returns the fragmentary order's "Object list" parameter with the given name.
-- @param fragorder DirectIA fragmentary order
-- @param parameterName String, the name of the parameter
-- @return List of simulation objects
integration.getObjectListParameter = function( fragorder, parameterName )
    return fragorder.source:GetObjectListParameter( parameterName )
end

integration.getOrderConduiteModifierPrioritesBlessesParameter = function( fragorder )
    return fragorder.source:GetorderConduiteModifierPrioritesBlesses_()
end

integration.getOrderConduiteChangerReglesEngagementPopulationParameter = function( fragorder )
    return fragorder.source:GetorderConduiteChangerReglesEngagementPopulation_()
end

integration.getOrderConduiteChangerReglesEngagementParameter = function( fragorder )
    return fragorder.source:GetorderConduiteChangerReglesEngagement_()
end

integration.getAgentKnowledgeParameter = function( fragorder )
    return fragorder.source:GetAgentKnowledge_()
end

integration.getAgentParameter = function( fragorder )
    return fragorder.source:GetAgent_()
end

integration.getConduiteChangerAmbianceParameter = function( fragorder )
    return fragorder.source:GetorderConduiteChangerAmbiance_()
end

integration.getMunitionParameter = function( fragorder )
    return fragorder.source:Getmunition_()
end

integration.getMunitionsParameter = function( fragorder )
    return fragorder.source:Getmunitions_()
end

integration.getNbItParameter = function( fragorder )
    return fragorder.source:GetnbIT_()
end

integration.getpointCibleParameter = function( fragorder )
    return fragorder.source:GetpointCible_()
end

integration.getorderConduitePopulationChangerAttitudeParameter = function( fragorder )
    return fragorder.source:GetorderConduitePopulationChangerAttitude_()
end

integration.getPionARenforcerParameter = function( fragorder )
    return fragorder.source:GetpionARenforcer_()
end

integration.getPionRenforceParameter = function( fragorder )
    return fragorder.source:GetpionRenforce_()
end

integration.getNbrAmbulancesParameter = function( fragorder )
    return fragorder.source:GetnbrAmbulances_()
end

integration.getNbrRemorqueursParameter = function( fragorder )
    return fragorder.source:GetnbrRemorqueurs_()
end

integration.getObjectKnowledgeParameter = function( fragorder )
    return fragorder.source:GetObjectKnowledge_()
end

integration.getOrderType = function( fragorder )
    return integration.getAnyType( fragorder.source )
end

integration.getOrdersCategory = function( )
    return DEC_GetOrdersCategory()
end

integration.getFireParameters = function( self )
    local params = {}
    local uggly = function() params.entities = { CreateKnowledge( integration.ontology.types.agentKnowledge, integration.getAgentKnowledgeParameter( self ) ) } end
    if not pcall( uggly ) then
        params.entities = { CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( self ) ) }
    end
    params.munition = integration.getMunitionParameter( self )
    params.interventionType = integration.getNbItParameter( self )
    return params
end

integration.createFragOrder = function( fragOrderType )
    return CreateFragOrder( fragOrderType )
end

