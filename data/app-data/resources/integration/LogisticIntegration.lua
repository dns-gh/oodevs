------------------------------------------------------------------
--- Logistic Integration
------------------------------------------------------------------
integration.getActionCouranteConvoi = function()
    return DEC_Ravitaillement_Convoi_ActionCourante()
end

-- Returns the supplies provider. 
-- /!\ Returns nil if the agent is not performing a logistic convoy mission
integration.getRavitailleur = function() 
    return DEC_Ravitaillement_Convoi_Ravitailleur()
end
-- Returns the supplies receiver.
-- /!\ Returns nil if the agent is not performing a logistic convoy mission
integration.getDestinataireCourant = function() 
    return DEC_Ravitaillement_Convoi_DestinataireCourant()
end
-- Returns the vehicles provider.
-- /!\ Returns nil if the agent is not performing a logistic convoy mission
integration.getTransporter = function()
    return DEC_Ravitaillement_Convoi_Transporteur()
end

integration.getItineraireVersProchaineDestination = function()
    return DEC_Ravitaillement_Convoi_ItineraireVersProchaineDestination()
end
integration.deplacementVersRavitailleurEffectue = function()
    DEC_Ravitaillement_Convoi_DeplacementVersRavitailleurEffectue()
end

integration.deplacementVersDestinataireEffectue = function()
    DEC_Ravitaillement_Convoi_DeplacementVersDestinataireEffectue()
end

integration.deplacementVersTransporteurEffectue = function()
    DEC_Ravitaillement_Convoi_DeplacementVersTransporteurEffectue()
end

integration.finMissionConvoi = function()
    DEC_Ravitaillement_Convoi_FinMission()
end

integration.isMedical = function( agent )
    return DEC_Agent_CanPerformHealthEvacuation( agent.source ) -- works with myself
end

integration.startGiveEquipment = function( agent, agentToReinforce, equipment, nbrAmbulance )
    DEC_StartPreterComposantes( agent, agentToReinforce, equipment, nbrAmbulance )
end

integration.startTakeEquipment = function( agent, equipment, nbrAmbulance )
	DEC_RecupererComposantes( agent, equipment, nbrAmbulance )
end

integration.changeMaintenanceWorkMode = function( workOrganization )
    DEC_Maintenance_ChangerRegimeTravail( workOrganization )
end

integration.changeTacticHealtPriority = function( tacticPriority )
    DEC_Sante_ChangerPrioritesTactiques( tacticPriority )
end

integration.changeTacticMaintenancePriority = function( tacticPriority )
    DEC_Maintenance_ChangerPrioritesTactiques( tacticPriority )
end

integration.changeMaintenancePriority = function( priority )
    DEC_Maintenance_ChangerPriorites( priority )
end

integration.changeHealthPriority = function( priority )
    DEC_Sante_ChangerPriorites( priority )
end

integration.changeTacticPriority = function( priority )
    DEC_Sante_ChangerPrioritesTactiques( priority )
end

integration.isLogSupplyPushFlow = function()
    return DEC_Ravitaillement_Convoi_EstFluxPousse()
end

integration.isLogisticAutomat = function( )
    return DEC_Automate_isLogistic( )
end

integration.isLogisticAutomatExist = function( )
    return DEC_Automate_isLogistic
end

integration.getWorkRegime = function( )
    return DEC_Maintenance_RegimeTravail( )
end

integration.getWorkRegimeExist = function( )
    return DEC_Maintenance_RegimeTravail
end

integration.getTacticalPrioritiesHealth = function( )
    return DEC_Sante_PrioritesTactiques( )
end

integration.getTacticalPrioritiesHealthExist = function( )
    return DEC_Sante_PrioritesTactiques
end

integration.getTacticalPrioritiesMaintenance = function( )
    return DEC_Maintenance_PrioritesTactiques( )
end

integration.getTacticalPrioritiesMaintenanceExist = function( )
    return DEC_Maintenance_PrioritesTactiques
end

integration.getPrioritiesMaintenance = function( )
    return DEC_Maintenance_Priorites( )
end

integration.getPrioritiesMaintenanceExist = function( )
    return DEC_Maintenance_Priorites
end

integration.getPrioritiesHealth = function( )
    return DEC_Sante_Priorites( )
end

integration.getPrioritiesHealthExist = function( )
    return DEC_Sante_Priorites
end

integration.supplyRequest = function( )
    return DEC_DemandeDeRavitaillement()
end

-- Returns the HQ of the current TC2 (returns nil if there is no current TC2)
integration.getHQofTC2 = function( )
    return DEC_Pion_PcDeTC2()
end

-- Returns the knowledge of the HQ of the current TC2 (returns nil if there is no current TC2)
integration.getHQknowledgeOfTC2 = function( )
    local tc2 = DEC_Pion_PcDeTC2()
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
    DEC_ChangeValeurDotations2( resourceType, capacityFactor )
end

-- Changes the amount of resource for the agent, taking from or giving back to the logistic superior stocks,
-- with respect to predefined resource quotas (ranges)
-- @param range : integer, the range corresponding to the desired resource quota (e.g. eAviationRange_MediumRange)
integration.enforceAviationResourcesFromRange = function( range )
    DEC_EnforceAviationResources( myself, range )
end

--- Triggers the decrease over time of resources for the agent.
--- @param dotation the dotation category
--- @param value the total fraction of the maximum to decrease
--- @param duration the duration of the decrease in seconds
integration.startConsumingResources = function( dotation, value, duration )
    DEC_StartConsumingResources( myself, category, value, duration )
end
