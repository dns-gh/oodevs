------------------------------------------------------------------
--- Logistic Integration
------------------------------------------------------------------
integration.getActionCouranteConvoi = function()
    return DEC_Ravitaillement_Convoi_ActionCourante()
end

integration.getRavitailleur = function()
    return DEC_Ravitaillement_Convoi_Ravitailleur()
end
integration.getDestinataireCourant = function()
    return DEC_Ravitaillement_Convoi_DestinataireCourant()
end
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