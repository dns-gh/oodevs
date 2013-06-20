
integration.isTask = function( self )
  local orderType = integration.getOrderType( self )
  return orderType == "france.military.platoon.combat.support.air.tasks.ModifierAltitude" or
         orderType == "france.military.platoon.combat.support.engineer.tasks.ActiverObstacles" or
         orderType == "france.military.platoon.tasks.Embarquer" or
         orderType == "france.military.platoon.tasks.Debarquer" or
         orderType == "france.military.platoon.tasks.Illuminer" or
         orderType == "france.military.platoon.combat.support.art.tasks.UtiliserALR" or
         orderType == "france.military.platoon.tasks.Observer" or
         orderType == "france.military.platoon.tasks.Orienter" or
         orderType == "france.military.platoon.tasks.RejoindreAToutPrix" or
         orderType == "france.military.platoon.tasks.DeposerUnite" or
         orderType == "security.behaviors.tasks.StopAndGoToGarage" or
         orderType == "Rep_OrderConduite_Interrompre" or
         orderType == "Rep_OrderConduite_AttendreSePoster" or
         orderType == "Rep_OrderConduite_RecupererTransporteurs" or
         orderType == "Rep_OrderConduite_SArreter" or
         orderType == "Rep_OrderConduite_Poursuivre" or
         orderType == "Rep_OrderConduite_PasserEnSilenceRadar" or
         orderType == "Rep_OrderConduite_ArreterSilenceRadar" or
         orderType == "Rep_OrderConduite_PasserEnSilenceRadio" or
         orderType == "Rep_OrderConduite_PasserEnSilenceRadioPartiel" or
         orderType == "Rep_OrderConduite_ArreterSilenceRadio" or
         orderType == "Rep_OrderConduite_MettreTenueNBC" or
         orderType == "Rep_OrderConduite_EnleverTenueNBC" or
         orderType == "Rep_OrderConduite_ChangerAmbiance" or
         orderType == "Rep_OrderConduite_Pion_InterdireMunition" or
         orderType == "Rep_OrderConduite_Pion_AutoriserMunition" or
         orderType == "Rep_OrderConduite_Pion_AutoriserToutesMunitions" or
         orderType == "Rep_OrderConduite_Population_ChangerAttitude" or
         orderType == "Rep_OrderConduite_Attendre" or
         orderType == "Rep_OrderConduite_ChangerReglesEngagementPopulation" or
         orderType == "Rep_OrderConduite_Pion_Engager" or
         orderType == "Rep_OrderConduite_Pion_Desengager" or
         orderType == "Rep_OrderConduite_ModifierRegimeTravailMaintenance" or
         orderType == "Rep_OrderConduite_ModifierPrioritesTactiquesBlesses" or
         orderType == "Rep_OrderConduite_ModifierPrioritesTactiquesReparations" or
         orderType == "Rep_OrderConduite_ModifierPrioritesReparations" or
         orderType == "Rep_OrderConduite_ModifierPrioritesBlesses" or
         orderType == "Rep_OrderConduite_Pion_RenforcerEnVSRAM" or
         orderType == "Rep_OrderConduite_Pion_TransfererVSRAM" or
         orderType == "Rep_OrderConduite_Pion_ReprendreAuxOrdresVSRAM" or
         orderType == "Rep_OrderConduite_Pion_RenforcerEnRemorqueurs" or
         orderType == "Rep_OrderConduite_Pion_TransfererRemorqueurs" or
         orderType == "Rep_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs" or
         orderType == "Rep_OrderConduite_MiseAFeuIED" or
         orderType == "Rep_OrderConduite_ROEM_ArreterSilenceRadar" or
         orderType == "Rep_OrderConduite_ROEM_PasserEnSilenceRadar"
end

integration.mustBePropagate = function( self )
  local orderType = integration.getOrderType( self )
  return orderType == "france.military.platoon.combat.support.air.tasks.ModifierAltitude" or
         orderType == "france.military.platoon.combat.support.engineer.tasks.ActiverObstacles" or
         orderType == "france.military.platoon.tasks.Embarquer" or 
         orderType == "france.military.platoon.tasks.Debarquer" or
         orderType == "france.military.platoon.tasks.DeposerUnite" or
         orderType == "Rep_OrderConduite_Interrompre" or
         orderType == "Rep_OrderConduite_AttendreSePoster" or
         orderType == "Rep_OrderConduite_RecupererTransporteurs" or
         orderType == "Rep_OrderConduite_SArreter" or
         orderType == "Rep_OrderConduite_Poursuivre" or
         orderType == "Rep_OrderConduite_PoursuivreConvoi" or
         orderType == "Rep_OrderConduite_PasserEnSilenceRadar" or
         orderType == "Rep_OrderConduite_ArreterSilenceRadar" or
         orderType == "Rep_OrderConduite_PasserEnSilenceRadio" or
         orderType == "Rep_OrderConduite_PasserEnSilenceRadioPartiel" or
         orderType == "Rep_OrderConduite_ArreterSilenceRadio" or
         orderType == "Rep_OrderConduite_MettreTenueNBC" or
         orderType == "Rep_OrderConduite_EnleverTenueNBC" or
         orderType == "Rep_OrderConduite_ChangerAmbiance" or
         orderType == "Rep_OrderConduite_Pion_InterdireMunition" or
         orderType == "Rep_OrderConduite_Pion_AutoriserMunition" or
         orderType == "Rep_OrderConduite_Pion_AutoriserToutesMunitions" or
         orderType == "Rep_OrderConduite_ChangerReglesEngagement" or
         orderType == "Rep_OrderConduite_ChangerReglesEngagementPopulation" or
         orderType == "Rep_OrderConduite_Deboucher" or 
         orderType == "Rep_OrderConduite_Decrocher" or 
         orderType == "Rep_OrderConduite_Population_ChangerAttitude" or
         orderType == "Rep_OrderConduite_ModifierRegimeTravailMaintenance" or
         orderType == "Rep_OrderConduite_ModifierPrioritesTactiquesBlesses" or
         orderType == "Rep_OrderConduite_ModifierPrioritesTactiquesReparations" or
         orderType == "Rep_OrderConduite_ModifierPrioritesReparations" or
         orderType == "Rep_OrderConduite_ModifierPrioritesBlesses" or
         orderType == "Rep_OrderConduite_RejoindrePointLancement" or
         orderType == "Rep_OrderConduite_ROEM_ArreterSilenceRadar" or
         orderType == "Rep_OrderConduite_ROEM_PasserEnSilenceRadar"		 
end

integration.setAutomatFragOrder = function( self )
  local orderType = integration.getOrderType( self ) -- ne pas faire de cleanFragOrder ici, le frago est réutilisé pour les pions
  
  if orderType == "Rep_OrderConduite_ModifierRegimeTravailMaintenance" then
      integration.changeMaintenanceWorkMode( integration.getOrderConduiteModifierRegimeTravailMaintenanceParameter( self ) )
  elseif orderType == "Rep_OrderConduite_ModifierPrioritesTactiquesBlesses" then
      integration.changeTacticHealtPriority( integration.getOrderConduiteModifierPrioritesTactiquesBlessesParameter( self ) )
  elseif orderType == "Rep_OrderConduite_ModifierPrioritesTactiquesReparations" then
      integration.changeTacticMaintenancePriority( integration.getOrderConduiteModifierPrioritesTactiquesReparationsParameter( self ) )
  elseif orderType == "Rep_OrderConduite_ModifierPrioritesReparations" then
      integration.changeMaintenancePriority( integration.getOrderConduiteModifierPrioritesReparationsParameter( self ) )
  elseif orderType == "Rep_OrderConduite_ModifierPrioritesBlesses" then
      integration.changeHealthPriority( integration.getOrderConduiteModifierPrioritesBlessesParameter( self ) )
  elseif orderType == "Rep_OrderConduite_ChangerReglesEngagementPopulation" then
      integration.changeCrowdROEForAutomat(integration.getOrderConduiteChangerReglesEngagementPopulationParameter( self ))
      integration.CR_ROE_Foules ( integration.getOrderConduiteChangerReglesEngagementPopulationParameter( self ) )
  end
end

integration.startFragOrderTask = function( self )
  local mission = {}

  local stopTask = function( task )
    if masalife.brain.core.isTaskRunning( task )  then
        masalife.brain.core.stopTask( task )
    end
  end

  local orderType = integration.getOrderType( self )

  if orderType == "france.military.platoon.tasks.Illuminer" then
      mission.entity = CreateKnowledge( integration.ontology.types.agentKnowledge, integration.getAgentKnowledgeParameter( self ) )
      if self.source == meKnowledge.source then
        mission.ally = meKnowledge
      else
        mission.ally = CreateKnowledge( integration.ontology.types.agent, integration.getAgentParameter( self ) )
      end
  elseif orderType =="france.military.platoon.combat.support.art.tasks.UtiliserALR" then
    mission.entity = CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( self ) )
    mission.munition = integration.getMunitionParameter( self )
    mission.interventionType = integration.getNbItParameter( self )
  elseif orderType == "france.military.platoon.tasks.RejoindreAToutPrix" then
    mission.objectif = CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( self ) )
  elseif orderType == "france.military.platoon.tasks.DeposerUnite" then
        local targetPoint = integration.getpointCibleParameter( self )
        mission.unite = CreateKnowledge( integration.ontology.types.agentKnowledge, integration.getAgentKnowledgeParameter( self ) )
        if targetPoint ~= nil then
            mission.position = CreateKnowledge( integration.ontology.types.point, targetPoint )
        end
  elseif orderType == "france.military.platoon.tasks.Observer" then
    mission.objective = CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( self ) )
  elseif orderType == "france.military.platoon.tasks.Orienter" then
    mission.objective = CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( self ) )
  elseif orderType == "Rep_OrderConduite_SauvegardeContreBatterie" then
    orderType = "france.military.platoon.combat.support.art.tasks.AssurerMiseEnOeuvre"
    mission.firePositions = {CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( self ) )}
  elseif orderType == "Rep_OrderConduite_MiseEnBatterieInopinee" then
    orderType = "sword.military.platoon.tasks.frago.RapidDeploiement"
    mission.entity = CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( self ) )
    mission.interventionType = integration.getNbItParameter( self )
    mission.munition = integration.getMunitionParameter( self )
    mission.firePositions = {CreateKnowledge( integration.ontology.types.point, integration.copyPoint( meKnowledge:getPosition() ) )}
  elseif orderType == "Rep_OrderConduite_AttendreSePoster" then
    orderType = "sword.military.platoon.tasks.frago.Settle"
  elseif orderType == "Rep_OrderConduite_SArreter" then
    local point = CreateKnowledge( integration.ontology.types.point, integration.copyPoint( meKnowledge:getPosition() ) )
    mission.objectives = { point }
    orderType = "sword.military.platoon.tasks.frago.WaitOn"
  elseif orderType == "Rep_OrderConduite_Attendre" then
    local point = CreateKnowledge( integration.ontology.types.point, integration.copyPoint( meKnowledge:getPosition() ) )
    mission.objective = point
    orderType = "sword.military.crowd.tasks.frago.WaitOn"
  elseif orderType == "Rep_OrderConduite_Poursuivre" then
    stopTask( "sword.military.platoon.tasks.frago.Settle" )
    stopTask( "sword.military.platoon.tasks.frago.WaitOn" )
    stopTask( "sword.military.crowd.tasks.frago.WaitOn" )
    myself.blocked = nil
    myself.canBeBlocked = nil
    myself.blockForcesContinue = true
    integration.cleanFragOrder( self )
    return
  elseif orderType =="Rep_OrderConduite_Interrompre" then
    if myself.currentMission then
        local missionCourante = integration.getMission( meKnowledge )
        if missionCourante.meetingPoint and missionCourante.meetingPoint ~= NIL then
            mission.objectives = { CreateKnowledge( integration.ontology.types.point, integration.copyPoint( missionCourante.meetingPoint.source ) ) }
            integration.communication.StartMissionPionVersPion( {mission_type = "france.military.platoon.tasks.FaireMouvement", 
                                                              mission_objectives = { objectives = mission.objectives}, 
                                                              echelon = eEtatEchelon_First } )
        else
            integration.stopMission( )
            local automat = integration.GetSuperiorKnowledge( meKnowledge )
            if automat then
            integration.SendMessage( "TaskDone", automat, { element = myself }, { type = "dynamic" } ) -- need to advice skill lead
            integration.SendMessage( "TaskStopped", automat, { element = myself }, { type = "dynamic" } ) -- need to advice skill lead
            end
        end
    end
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_PasserEnSilenceRadar" then
    stopTask( "sword.military.platoon.tasks.frago.ActivateRadar" )
    orderType = "sword.military.platoon.tasks.frago.ObserveRadarSilence"
  elseif orderType == "Rep_OrderConduite_ArreterSilenceRadar" then
    stopTask( "sword.military.platoon.tasks.frago.ObserveRadarSilence" )
    orderType = "sword.military.platoon.tasks.frago.ActivateRadar"
  elseif orderType == "Rep_OrderConduite_PasserEnSilenceRadio" then
    stopTask( "sword.military.platoon.tasks.frago.ObservePartialRadioSilence" )
    stopTask( "sword.military.platoon.tasks.frago.ActivateRadio" )
    orderType = "sword.military.platoon.tasks.frago.ObserveRadioSilence"
  elseif orderType == "Rep_OrderConduite_PasserEnSilenceRadioPartiel" then
    stopTask( "sword.military.platoon.tasks.frago.ActivateRadio" )
    stopTask( "sword.military.platoon.tasks.frago.ObserveRadioSilence" )
    orderType = "sword.military.platoon.tasks.frago.ObservePartialRadioSilence"
  elseif orderType == "Rep_OrderConduite_ArreterSilenceRadio" then
    stopTask( "sword.military.platoon.tasks.frago.ObservePartialRadioSilence" )
    stopTask( "sword.military.platoon.tasks.frago.ObserveRadioSilence" )
    orderType = "sword.military.platoon.tasks.frago.ActivateRadio"
  elseif orderType == "Rep_OrderConduite_MettreTenueNBC" then
    stopTask( "sword.military.platoon.tasks.UnequipNBCOutfit" )
    orderType = "sword.military.platoon.tasks.EquipNBCOutfit"
  elseif orderType == "Rep_OrderConduite_EnleverTenueNBC" then
    stopTask( "sword.military.platoon.tasks.EquipNBCOutfit" )
    orderType = "sword.military.platoon.tasks.UnequipNBCOutfit"
  elseif orderType =="Rep_OrderConduite_ChangerAmbiance" then
    if integration.getConduiteChangerAmbianceParameter( self ) == eAmbianceMission_Safety then
        orderType = "france.military.platoon.tasks.ChangerAmbianceSurete"
    else
        orderType = "france.military.platoon.tasks.ChangerAmbianceVitesse"
        stopTask( "france.military.platoon.tasks.ChangerAmbianceSurete" )
    end
  elseif orderType =="Rep_OrderConduite_Pion_InterdireMunition" then
    integration.forbidAmmunition(integration.getMunitionsParameter( self ))
    integration.cleanFragOrder( self )
    return
  elseif orderType =="Rep_OrderConduite_Pion_AutoriserMunition" then
    integration.autoriseAmmunition(integration.getMunitionsParameter( self ))
    integration.cleanFragOrder( self )
    return
  elseif orderType =="Rep_OrderConduite_Pion_AutoriserToutesMunitions" then
    integration.autoriseAllAmmunitions()
    integration.cleanFragOrder( self )
    return
  elseif orderType == "france.military.platoon.tasks.Embarquer" then
    stopTask( "france.military.platoon.tasks.Debarquer" )
    integration.cleanFragOrder( self )
  elseif orderType == "france.military.platoon.tasks.Debarquer" then
    stopTask( "france.military.platoon.tasks.Embarquer" )
    integration.cleanFragOrder( self )
  elseif orderType == "france.military.platoon.combat.support.air.tasks.ModifierAltitude" then
    mission.altitude = integration.getNbItParameter( self )
  elseif orderType == "Rep_OrderConduite_Population_ChangerAttitude" then
    integration.changeAttitude(integration.getorderConduitePopulationChangerAttitudeParameter( self ))
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_ChangerReglesEngagementPopulation" then
    integration.changeCrowdROEForAgent(integration.getOrderConduiteChangerReglesEngagementPopulationParameter( self ))
    integration.CR_ROE_Foules ( integration.getCrowdROEForAgent() )
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_ChangerReglesEngagement" then
    integration.updateROE( self )
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_Pion_Engager" then
    if myself.isEngaged then
        meKnowledge:RC( eRC_AlreadyEngaged )
    else
        meKnowledge:RC( eRC_Engaged )
        myself.isEngaged = true
    end
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_Pion_Desengager" then
    if myself.isEngaged then
        meKnowledge:RC( eRC_Disengaged )
        myself.isEngaged = false
    else
        meKnowledge:RC( eRC_AlreadyDisengaged )
    end
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_ModifierRegimeTravailMaintenance" then
    if integration.isLogisticTypeUnit( ) then
        integration.changeMaintenanceWorkMode( integration.getOrderConduiteModifierRegimeTravailMaintenanceParameter( self ) )
    end
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_ModifierPrioritesTactiquesBlesses" then
    if integration.isLogisticTypeUnit( ) then
        integration.changeTacticPriority( integration.getOrderConduiteModifierPrioritesTactiquesBlessesParameter( self ) )
    end
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_ModifierPrioritesTactiquesReparations" then
    if integration.isLogisticTypeUnit( ) then
        integration.changeTacticMaintenancePriority( integration.getOrderConduiteModifierPrioritesTactiquesReparationsParameter( self ) )
    end
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_ModifierPrioritesReparations" then
    if integration.isLogisticTypeUnit( ) then
        integration.changeMaintenancePriority( integration.getOrderConduiteModifierPrioritesReparationsParameter( self ) )
    end
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_ModifierPrioritesBlesses" then
    if integration.isLogisticTypeUnit( ) then
        integration.changeHealthPriority( integration.getOrderConduiteModifierPrioritesBlessesParameter( self ) )
    end
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_Pion_RenforcerEnVSRAM" then
    if integration.isLogisticTypeUnit( ) then
        orderType = "france.military.platoon.combat.support.log.tasks.RenforcerEnVSRAM" 
        mission.pionARenforcer = CreateKnowledge( integration.ontology.types.agent, integration.getPionARenforcerParameter( self ) )
        mission.nbrAmbulances = integration.getNbrAmbulancesParameter( self )
    else
        integration.cleanFragOrder( self )
        return
    end
  elseif orderType == "Rep_OrderConduite_Pion_TransfererVSRAM" then
    if integration.isLogisticTypeUnit( ) then
        orderType = "france.military.platoon.combat.support.log.tasks.TransfererVSRAM"
        mission.pionRenforce = CreateKnowledge( integration.ontology.types.agent, integration.getPionRenforceParameter( self ) )
        mission.pionARenforcer = CreateKnowledge( integration.ontology.types.agent, integration.getPionARenforcerParameter( self ) )
        mission.nbrAmbulances = integration.getNbrAmbulancesParameter( self )
    else
        integration.cleanFragOrder( self )
        return
    end
  elseif orderType == "Rep_OrderConduite_Pion_ReprendreAuxOrdresVSRAM" then
    if integration.isLogisticTypeUnit( ) then
        orderType = "france.military.platoon.combat.support.log.tasks.ReprendreAuxOrdresVSRAM"
        mission.pionRenforce = CreateKnowledge( integration.ontology.types.agent, integration.getPionRenforceParameter( self ) )
        mission.nbrAmbulances = integration.getNbrAmbulancesParameter( self )
    else
        integration.cleanFragOrder( self )
        return
    end
  elseif orderType == "Rep_OrderConduite_Pion_RenforcerEnEquipements" then
    local equipments = integration.getOrderConduiteModifierPrioritesReparationsParameter( self )
    if not equipments or #equipments ~= 1 then 
        integration.cleanFragOrder( self )
        error( "Need exactly one type of equipment" ) 
    end
    integration.startGiveEquipment( integration.getPionRenforceParameter( self ), integration.getPionRenforceParameter( self ), equipments[1], integration.getNbrAmbulancesParameter( self ) )
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_Pion_TransfererEquipements" then
    local equipments = integration.getOrderConduiteModifierPrioritesReparationsParameter( self )
    if not equipments or #equipments ~= 1 then 
        integration.cleanFragOrder( self )
        error( "Need exactly one type of equipment" ) 
    end
    integration.startTakeEquipment( integration.getPionRenforceParameter( self ), equipments[1], integration.getNbrAmbulancesParameter( self ) )
    integration.startGiveEquipment( integration.getPionRenforceParameter( self ), integration.getPionARenforcerParameter( self ), equipments[1], integration.getNbrAmbulancesParameter( self ) )
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_Pion_ReprendreAuxOrdresEquipements" then
    local equipments = integration.getOrderConduiteModifierPrioritesReparationsParameter( self )
    if not equipments or #equipments ~= 1 then 
        integration.cleanFragOrder( self )
        error( "Need exactly one type of equipment" ) 
    end
    integration.startTakeEquipment( integration.getPionRenforceParameter( self ), equipments[1], integration.getNbrAmbulancesParameter( self ) )
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_Pion_RenforcerEnRemorqueurs" then
    if integration.isLogisticTypeUnit( ) then
        mission.pionARenforcer = CreateKnowledge( integration.ontology.types.agent, integration.getPionARenforcerParameter( self ) )
        mission.nbrRemorqueurs = integration.getNbrRemorqueursParameter( self )
        integration.StartLendTugs ( meKnowledge, mission.pionARenforcer, mission.nbrRemorqueurs  )
    end
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_Pion_TransfererRemorqueurs" then
    if integration.isLogisticTypeUnit( ) then   
        mission.pionRenforce = CreateKnowledge( integration.ontology.types.agent, integration.getPionRenforceParameter( self ) )
        mission.pionARenforcer = CreateKnowledge( integration.ontology.types.agent, integration.getPionARenforcerParameter( self ) )
        mission.nbrRemorqueurs = integration.getNbrRemorqueursParameter( self )
        integration.StartGetTugs ( mission.pionRenforce, mission.nbrRemorqueurs )
        integration.StartLendTugs ( mission.pionRenforce, mission.pionARenforcer, mission.nbrRemorqueurs  )
    end
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs" then
    if integration.isLogisticTypeUnit( ) then
        mission.pionRenforce = CreateKnowledge( integration.ontology.types.agent, integration.getPionRenforceParameter( self ) )
        mission.nbrRemorqueurs = integration.getNbrRemorqueursParameter( self )
        integration.StartGetTugs ( mission.pionRenforce, mission.nbrRemorqueurs )
    end
    integration.cleanFragOrder( self )
    return
  elseif orderType == "france.military.platoon.combat.support.engineer.tasks.ActiverObstacles" then
    if myself.obstacleToActivate then  -- les unités qui ont construits des obstacles à activer sont les seules à recevoir l'ODC
        local obstacles = {}
        local obstaclesList = integration.getObjectKnowledgeParameter( self )
        for _, obstacle in pairs( obstaclesList ) do
            obstacles[#obstacles + 1] = CreateKnowledge( integration.ontology.types.object, obstacle )
        end
        mission.obstacles = obstacles
        obstacles = nil
    else
        integration.cleanFragOrder( self )
        return
    end
  elseif orderType == "Rep_OrderConduite_RecupererTransporteurs" then
    integration.retrieveCarriers()
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_ROEM_ArreterSilenceRadar" then
    if not myself.radarActivated and myself.zoneAEcouter then
        myself.ecoute = integration.activeRadarOnLocalisation( eRadarType_Ecoute, myself.zoneAEcouter )
        myself.radar = integration.activeRadarOnLocalisation( eRadarType_Radar, myself.zoneAEcouter )
        myself.ecouteRadar = integration.activeRadarOnLocalisation( eRadarType_EcouteRadar, myself.zoneAEcouter )
        myself.radarActivated = true
        meKnowledge:RC( eRC_FinSilenceRadar )	
    end
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_ROEM_PasserEnSilenceRadar" then
    if myself.radarActivated then
        integration.disableRadarOnLocalisation( eRadarType_Ecoute, myself.ecoute )
        integration.disableRadarOnLocalisation( eRadarType_Radar, myself.radar )
        integration.disableRadarOnLocalisation( eRadarType_EcouteRadar, myself.ecouteRadar )
        myself.radarActivated = false
        meKnowledge:RC( eRC_DebutSilenceRadar )	
    end
    integration.cleanFragOrder( self )
    return
  elseif orderType == "france.military.platoon.combat.support.art.tasks.AppliquerFeux" then
    return
  elseif orderType == "Rep_OrderConduite_Decrocher" then
    return
  elseif orderType == "Rep_OrderConduite_PoursuivreConvoi" then
    return
  elseif orderType == "Rep_OrderConduite_Pion_AbandonnerTravaux" then
    return
  elseif orderType == "Rep_OrderConduite_RejoindrePointLancement" then
    return
  elseif orderType == "Rep_OrderConduite_Deboucher" then
    return
  elseif orderType == "Rep_OrderConduite_MiseAFeuIED" then
    return
  end

  masalife.brain.core.startTask( orderType, mission )
  integration.cleanFragOrder( self )
end

integration.isROE = function( self )
  local returnValue = integration.getOrderType( self )=="Rep_OrderConduite_ChangerReglesEngagement"
  return returnValue
end