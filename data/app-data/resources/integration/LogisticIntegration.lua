------------------------------------------------------------------
--- Logistic Integration
------------------------------------------------------------------
integration.getActionCouranteConvoi = function()
    return _DEC_Ravitaillement_Convoi_ActionCourante( myself )
end

-- Returns the supplies provider. 
-- /!\ Returns nil if the agent is not performing a logistic convoy mission
integration.getRavitailleur = function() 
    return _DEC_Ravitaillement_Convoi_Ravitailleur( myself )
end
-- Returns the supplies receiver.
-- /!\ Returns nil if the agent is not performing a logistic convoy mission
integration.getDestinataireCourant = function() 
    return _DEC_Ravitaillement_Convoi_DestinataireCourant( myself )
end
-- Returns the vehicles provider.
-- /!\ Returns nil if the agent is not performing a logistic convoy mission
integration.getTransporter = function()
    return _DEC_Ravitaillement_Convoi_Transporteur( myself )
end

integration.getItineraireVersProchaineDestination = function()
    return _DEC_Ravitaillement_Convoi_ItineraireVersProchaineDestination( myself )
end
integration.deplacementVersRavitailleurEffectue = function()
    _DEC_Ravitaillement_Convoi_DeplacementVersRavitailleurEffectue( myself )
end

integration.deplacementVersDestinataireEffectue = function()
    _DEC_Ravitaillement_Convoi_DeplacementVersDestinataireEffectue( myself )
end

integration.deplacementVersTransporteurEffectue = function()
    _DEC_Ravitaillement_Convoi_DeplacementVersTransporteurEffectue( myself )
end

integration.finMissionConvoi = function()
    _DEC_Ravitaillement_Convoi_FinMission( myself )
end

integration.isMedical = function( agent )
    return DEC_Agent_CanPerformHealthEvacuation( agent.source ) -- works with myself
end

integration.startGiveEquipment = function( agent, agentToReinforce, equipment, nbrAmbulance )
    _DEC_StartPreterComposantes( myself, agent, agentToReinforce, equipment, nbrAmbulance )
end

integration.startTakeEquipment = function( agent, equipment, nbrAmbulance )
	_DEC_RecupererComposantes( myself, agent, equipment, nbrAmbulance )
end

integration.changeMaintenanceWorkMode = function( workOrganization )
    _DEC_Maintenance_ChangerRegimeTravail( myself, workOrganization )
end

integration.changeTacticHealtPriority = function( tacticPriority )
    _DEC_Sante_ChangerPrioritesTactiques( myself, tacticPriority )
end

integration.changeTacticMaintenancePriority = function( tacticPriority )
    _DEC_Maintenance_ChangerPrioritesTactiques( myself, tacticPriority )
end

integration.changeMaintenancePriority = function( priority )
    _DEC_Maintenance_ChangerPriorites( myself, priority )
end

integration.changeHealthPriority = function( priority )
    _DEC_Sante_ChangerPriorites( myself, priority )
end

integration.changeTacticPriority = function( priority )
    _DEC_Sante_ChangerPrioritesTactiques( myself, priority )
end

integration.isLogSupplyPushFlow = function()
    return _DEC_Ravitaillement_Convoi_EstFluxPousse( myself )
end

integration.isLogisticAutomat = function( )
    return _DEC_Automate_isLogistic( myself )
end

integration.isLogisticAutomatExist = function( )
    return _DEC_Automate_isLogistic
end

integration.getWorkRegime = function( )
    return _DEC_Maintenance_RegimeTravail( myself )
end

integration.getWorkRegimeExist = function( )
    return _DEC_Maintenance_RegimeTravail
end

integration.getTacticalPrioritiesHealth = function( )
    return _DEC_Sante_PrioritesTactiques( myself )
end

integration.getTacticalPrioritiesHealthExist = function( )
    return _DEC_Sante_PrioritesTactiques
end

integration.getTacticalPrioritiesMaintenance = function( )
    return _DEC_Maintenance_PrioritesTactiques( myself )
end

integration.getTacticalPrioritiesMaintenanceExist = function( )
    return _DEC_Maintenance_PrioritesTactiques
end

integration.getPrioritiesMaintenance = function( )
    return _DEC_Maintenance_Priorites( myself )
end

integration.getPrioritiesMaintenanceExist = function( )
    return _DEC_Maintenance_Priorites
end

integration.getPrioritiesHealth = function( )
    return _DEC_Sante_Priorites( myself )
end

integration.getPrioritiesHealthExist = function( )
    return _DEC_Sante_Priorites
end

integration.supplyRequest = function( )
    return _DEC_DemandeDeRavitaillement( myself )
end

-- Returns the HQ of the current TC2 (returns nil if there is no current TC2)
integration.getHQofTC2 = function( )
    return _DEC_Pion_PcDeTC2( myself )
end

-- Returns the knowledge of the HQ of the current TC2 (returns nil if there is no current TC2)
integration.getHQknowledgeOfTC2 = function( )
    local tc2 = _DEC_Pion_PcDeTC2( myself )
    if tc2 then
        return CreateKnowledge( integration.ontology.types.agent, tc2 )
    end
    return nil
end

-- Changes the amount of resource for the agent, taking from or giving back to the logistic superior stocks,
-- with respect to a capacity factor.
-- @param ressourceType : integer, the type of ressource (e.g. eDotation_Fuel)
-- @param capacityFactor : float, the proportion (over the total capacity) of resource that will be set
integration.changeResourceValueUsingCombatTrains = function( resourceType, capacityFactor )
    _DEC_ChangeValeurDotations( myself, resourceType, capacityFactor, -1 )
end

-- Changes the amount of resource for the agent, taking from or giving back to the logistic superior stocks,
-- with respect to predefined resource quotas (ranges)
-- @param range : integer, the range corresponding to the desired resource quota (e.g. eAviationRange_MediumRange)
integration.enforceAviationResourcesFromRange = function( range )
    DEC_EnforceAviationResources( myself, range )
end

--- Enables the sorting of wounded units.
-- This method can only be called by a logistic agent or company.
-- @see integration.desactivateInjuredSorting
integration.activateInjuredSorting = function()
    _DEC_Sante_ActiverFonctionTri( myself )
end

--- Disables the sorting of wounded units.
-- This method can only be called by a logistic agent or company.
-- @see integration.activateInjuredSorting
integration.desactivateInjuredSorting = function()
    _DEC_Sante_DesactiverFonctionTri( myself )
end

--- Enables the logistics system (medical, supply and maintenance chains).
-- Returns true if the caller of this method is of a logistic type, false otherwise.
-- This method can only be called by a logistic agent or company.
-- @see integration.desactivateLogisticChains
-- @return Boolean, whether or not the caller is of a logistic type.
integration.activateLogisticChains = function()
    if integration.isLogisticTypeUnit() then
        _DEC_Sante_ActiverChaine( myself )
        _DEC_Maintenance_ActiverChaine( myself )
        _DEC_Ravitaillement_ActiverChaine( myself )
        _DEC_Sante_ActiverFonctionSoin( myself )
        return true
    else
        return false
    end
end

--- Disables the logistics system (medical, supply and maintenance chains).
-- Returns true if the caller of this method is of a logistic type, false otherwise.
-- This method can only be called by a logistic agent or company.
-- @see integration.activateLogisticChains
-- @return Boolean, whether or not the caller is of a logistic type.
integration.desactivateLogisticChains = function()
    if integration.isLogisticTypeUnit() then
        _DEC_Sante_DesactiverChaine( myself )
        _DEC_Maintenance_DesactiverChaine( myself )
        _DEC_Ravitaillement_DesactiverChaine( myself )
        _DEC_Sante_DesactiverFonctionSoin( myself )
        return true
    else
        return false
    end
end

--- Enables the treatment of wounded units.
-- This method can only be called by a logistic agent or company.
-- @see integration.desactivateInjuredTreatment
integration.activateInjuredTreatment = function()
    _DEC_Sante_ActiverFonctionSoin( myself )
end

--- Disables the treatment of wounded units.
-- This method can only be called by a logistic agent or company.
-- @see integration.activateInjuredTreatment
integration.desactivateInjuredTreatment = function ( )
    _DEC_Sante_DesactiverFonctionSoin( myself )
end

--- Changes the wounded units treatment priority.
-- @see integration.changeHealthPriority
-- @param woundedPriorities List of medical priority knowledges
integration.modifyHumanWoundPriority = function ( woundedPriorities )
    if ( woundedPriorities ~= nil ) then
        local list = {}
        for i = 1, #woundedPriorities do
            list[ #list + 1 ] = woundedPriorities[i].source
        end
        integration.changeHealthPriority( list )
    end
end

--- Triggers the decrease over time of resources for the agent.
-- @param dotation the dotation category
-- @param value the total percentage of the maximum to decrease
-- @param duration the duration of the decrease in seconds
integration.startConsumingResources = function( dotation, value, duration )
    _DEC_StartConsumingResources( myself, dotation, value, duration )
end

--- Triggers a breakdown of the given id to the equipment of the
-- provided type of this entity.
-- This method can only be called by an agent.
-- @param equipmentType Equipment type
-- @param breakdownID Integer, the id of breakdown
-- @return Boolean, whether or not the breakdown has been successfully triggered.
integration.triggerBreakdown = function( equipmentType, breakdownID )
    return _DEC_CreateBreakdown( myself, equipmentType, breakdownID )
end

--- Evacuate victims through a medical logistic chain.
-- Displays a report
-- @param victimsUnit DirectIA agent
-- @param medicalUnit DirectIA agent
-- @return Boolean, true
integration.ensureVictimsEvacuation = function( victimsUnit, medicalUnit )
    reportFunction( eRC_VictimsEvacuated )
    DEC_EvacuerBlessesVersTC2( victimsUnit.source, medicalUnit.source )
    return true
end

--- Returns true if the object can distribute stocks ("stock" capacity ticked in the authoring tool).
-- @param object Object knowledge. The object to be queried
integration.stockCanBeDistributed = function( object )
    return DEC_Stock_IsDistributePossible( object, nil )
end

--- Returns true if the agent can take resources of the specified type from the object.
-- This requires the object to have stocked these resources and the querying agent to have some room for more of these resources in their stock.
-- Can only be called on an agent.
-- @param object Objectknowledge. The object to extract resources from
-- @param resourceTypes List of simulation resource type to be extracted.
integration.stockCanBeExtracted = function( object, resourceTypes )
    return DEC_Stock_IsExtractPossible( object, resourceTypes )
end

--- Returns true if the agent can put resources of the specified type into the object.
-- This requires the object to have the "stock" capacity defined in authoring and be able to stock the specified resource.
-- The querying agent must have some of these resources in their stock.
-- Can only be called on an agent.
-- @param object Objectknowledge. The object to put resources into
-- @param resourceTypes List of simulation  resource type to be added.
integration.stockCanBeSupplied = function( object, resourceTypes )
    return DEC_Stock_IsSupplyPossible( object, resourceTypes )
end