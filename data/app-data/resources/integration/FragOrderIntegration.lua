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

integration.isROE = function( self )
  local returnValue = integration.getOrderType( self )=="Rep_OrderConduite_ChangerReglesEngagement"
  return returnValue
end

integration.isTask = function( self )
  local orderType = integration.getOrderType( self )
  return orderType == "platoon.combat.support.air.tasks.ModifierAltitude" or
         orderType == "platoon.combat.support.engineer.tasks.ActiverObstacles" or
         orderType == "platoon.tasks.Embarquer" or
         orderType == "Mount" or
         orderType == "GoTo" or
         orderType == "platoon.tasks.Debarquer" or
         orderType == "Dismount" or
         orderType == "platoon.tasks.Illuminer" or
         orderType == "platoon.combat.support.art.tasks.UtiliserALR" or
         orderType == "platoon.tasks.Observer" or
         orderType == "platoon.tasks.Orienter" or
         orderType == "platoon.tasks.RejoindreAToutPrix" or
         orderType == "platoon.tasks.DeposerUnite" or
         orderType == "Rep_OrderConduite_Interrompre" or
         orderType == "CancelMission" or
         orderType == "Rep_OrderConduite_AttendreSePoster" or
         orderType == "Rep_OrderConduite_RecupererTransporteurs" or
         orderType == "Rep_OrderConduite_SArreter" or
         orderType == "Rep_OrderConduite_Poursuivre" or
         orderType == "Rep_OrderConduite_PasserEnSilenceRadar" or
         orderType == "Rep_OrderConduite_ArreterSilenceRadar" or
         orderType == "Rep_OrderConduite_PasserEnSilenceRadio" or
         orderType == "DeactivateRadioCommunication" or --WW
         orderType == "Rep_OrderConduite_PasserEnSilenceRadioPartiel" or
         orderType == "Rep_OrderConduite_ArreterSilenceRadio" or
         orderType == "ActivateRadioCommunication" or
         orderType == "Rep_OrderConduite_MettreTenueNBC" or
         orderType == "ActivateNBCProtection" or -- WW
         orderType == "Rep_OrderConduite_EnleverTenueNBC" or
         orderType == "DeactivateNBCProtection" or -- WW
         orderType == "Rep_OrderConduite_ChangerAmbiance" or
         orderType == "ChangeAttitude" or -- WW base
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
  return orderType == "platoon.combat.support.air.tasks.ModifierAltitude" or
         orderType == "platoon.combat.support.engineer.tasks.ActiverObstacles" or
         orderType == "platoon.tasks.Embarquer" or 
         orderType == "platoon.tasks.Debarquer" or
         orderType == "platoon.tasks.DeposerUnite" or
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
         orderType == "Mount" or                        -- WW
         orderType == "Dismount" or                     -- WW
         orderType == "DeactivateRadioCommunication" or -- WW
         orderType == "ActivateRadioCommunication" or   -- WW
         orderType == "ActivateNBCProtection" or        -- WW
         orderType == "DeactivateNBCProtection" or      -- WW
         orderType == "ChangeAttitude" or               -- WW
         orderType == "GoTo" or                         -- WW
         orderType == "CancelMission"                   -- WW
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
local eAmbianceMission_Surete = 0
local eAmbianceMission_Vitesse = 1
integration.startFragOrderTask = function( self )
    local mission = {}
    
    local stopTask = function( task )
    if masalife.brain.core.isTaskRunning( task )  then
        masalife.brain.core.stopTask( task )
    end
    end

    local orderType = integration.getOrderType( self )
    -- ----------------------------------------------------------------------------
    -- ARTY illumination
    -- ----------------------------------------------------------------------------
    if orderType == "platoon.tasks.Illuminer" then
        mission.entity = CreateKnowledge( integration.ontology.types.agentKnowledge, integration.getAgentKnowledgeParameter( self ) )
        if self.source == meKnowledge.source then
            mission.ally = meKnowledge
        else
            mission.ally = CreateKnowledge( integration.ontology.types.agent, integration.getAgentParameter( self ) )
        end

    -- ----------------------------------------------------------------------------
    -- Use specific weapon for crowd 
    -- ----------------------------------------------------------------------------
    elseif orderType =="platoon.combat.support.art.tasks.UtiliserALR" then
        mission.entity = CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( self ) )
        mission.munition = integration.getMunitionsParameter( self )
        mission.interventionType = integration.getNbItParameter( self )

    -- ----------------------------------------------------------------------------
    -- Move at all cost
    -- ----------------------------------------------------------------------------
    elseif orderType == "platoon.tasks.RejoindreAToutPrix" then
        mission.objectif = CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( self ) )

    -- WW
    elseif orderType == "agent.frago.GoTo" then
        mission.objective = CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( self ) )
  
    -- ----------------------------------------------------------------------------
    -- Unload units
    -- ----------------------------------------------------------------------------
    elseif orderType == "platoon.tasks.DeposerUnite" then
        local targetPoint = integration.getpointCibleParameter( self )
        mission.unite = CreateKnowledge( integration.ontology.types.agentKnowledge, integration.getAgentKnowledgeParameter( self ) )
        if targetPoint ~= nil then
            mission.position = CreateKnowledge( integration.ontology.types.point, targetPoint )
        end

    -- ----------------------------------------------------------------------------
    -- Observe a specific position
    -- ----------------------------------------------------------------------------
    elseif orderType == "platoon.tasks.Observer" then
        mission.objective = CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( self ) )
    elseif orderType == "platoon.tasks.Orienter" then
        mission.objective = CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( self ) )
    -- WW    
    elseif orderType == "agent.frago.Observe" then
        mission.target = CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( self ) )
        
    -- ----------------------------------------------------------------------------
    -- Counter battery selfprotection
    -- ----------------------------------------------------------------------------
    elseif orderType == "Rep_OrderConduite_SauvegardeContreBatterie" then
        orderType = "platoon.combat.support.art.tasks.AssurerMiseEnOeuvre"
        mission.firePositions = { CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( self ) ) }
    
    -- ----------------------------------------------------------------------------
    -- Arty deployment
    -- ----------------------------------------------------------------------------
    elseif orderType == "Rep_OrderConduite_MiseEnBatterieInopinee" then
        orderType = "platoon.frago.RapidDeploiement"
        mission.entity = CreateKnowledge( integraTion.ontology.types.point, integration.getpointCibleParameter( self ) )
        mission.interventionType = integration.getNbItParameter( self )
        mission.munition = integration.getMunitionsParameter( self )
        mission.firePositions = { CreateKnowledge( integration.ontology.types.point, 
                                  integration.copyPoint( meKnowledge:getPosition() ) ) }
    
    -- ----------------------------------------------------------------------------
    -- Pause maneuver
    -- ----------------------------------------------------------------------------
    elseif orderType == "Rep_OrderConduite_AttendreSePoster" then
        orderType = "platoon.frago.Settle"
    elseif orderType == "Rep_OrderConduite_SArreter" then
        local point = CreateKnowledge( integration.ontology.types.point, integration.copyPoint( meKnowledge:getPosition() ) )
        mission.objectives = { point }
        orderType = "platoon.frago.WaitOn"
    elseif orderType == "Rep_OrderConduite_Attendre" then
        local point = CreateKnowledge( integration.ontology.types.point, integration.copyPoint( meKnowledge:getPosition() ) )
        mission.objective = point
        orderType = "sword.military.crowd.tasks.frago.WaitOn"

    -- Crowd WW 
    elseif orderType == "CrowdWait" then
        orderType = "behaviors.tasks.frago.Wait"

    -- ----------------------------------------------------------------------------
    -- Resume maneuver
    -- ----------------------------------------------------------------------------
    elseif orderType == "Rep_OrderConduite_Poursuivre" then
        stopTask( "platoon.frago.Settle" )
        stopTask( "platoon.frago.WaitOn" )
        stopTask( "sword.military.crowd.tasks.frago.WaitOn" )
        myself.blocked = nil
        myself.canBeBlocked = nil
        myself.blockForcesContinue = true
        integration.cleanFragOrder( self )
        return

    -- WW Crowd
    elseif orderType == "CrowdResume" then
        stopTask( "behaviors.tasks.frago.Wait" ) -- WW Crowd
        integration.cleanFragOrder( self )
        return
    
    -- ----------------------------------------------------------------------------
    -- Cancel mission
    -- ----------------------------------------------------------------------------
    elseif orderType =="Rep_OrderConduite_Interrompre" then
        if myself.currentMission then
            local missionCourante = integration.getMission( meKnowledge )
            if missionCourante.meetingPoint and missionCourante.meetingPoint ~= NIL then
                mission.objectives = { CreateKnowledge( integration.ontology.types.point, integration.copyPoint( missionCourante.meetingPoint.source ) ) }
                integration.communication.StartMissionPionVersPion( { mission_type = "platoon.tasks.FaireMouvement", 
                                                                      mission_objectives = { objectives = mission.objectives }, 
                                                                      echelon = eEtatEchelon_None } )
            else
                masalife.brain.core.stopTasks()
            end
        end
        integration.cleanFragOrder( self )
        return

    -- WW
    elseif orderType =="CancelMission" then
        if myself.currentMission then
                masalife.brain.core.stopTasks()
        end
        integration.cleanFragOrder( self )
        return

    -- ----------------------------------------------------------------------------
    -- Radar activation / Deactivation
    -- ----------------------------------------------------------------------------
    elseif orderType == "Rep_OrderConduite_PasserEnSilenceRadar" then
        stopTask( "platoon.frago.ActivateRadar" )
        orderType = "platoon.frago.ObserveRadarSilence"
    elseif orderType == "Rep_OrderConduite_ArreterSilenceRadar" then
        stopTask( "platoon.frago.ObserveRadarSilence" )
        orderType = "platoon.frago.ActivateRadar"

    -- ----------------------------------------------------------------------------
    -- Radio activation / Deactivation
    -- ----------------------------------------------------------------------------
    -- FR
    elseif orderType == "Rep_OrderConduite_PasserEnSilenceRadio" then
        stopTask( "platoon.frago.ObservePartialRadioSilence" )
        stopTask( "platoon.frago.ActivateRadio" )
        orderType = "platoon.frago.ObserveRadioSilence"
    elseif orderType == "Rep_OrderConduite_PasserEnSilenceRadioPartiel" then
        stopTask( "platoon.frago.ActivateRadio" )
        stopTask( "platoon.frago.ObserveRadioSilence" )
        orderType = "platoon.frago.ObservePartialRadioSilence"
    elseif orderType == "Rep_OrderConduite_ArreterSilenceRadio" then
        stopTask( "platoon.frago.ObservePartialRadioSilence" )
        stopTask( "platoon.frago.ObserveRadioSilence" )
        orderType = "platoon.frago.ActivateRadio"
    -- WW
    elseif orderType == "DeactivateRadioCommunication" then
        stopTask( "agent.frago.ActivateRadio" )
        orderType = "agent.frago.DeactivateRadio"
    elseif orderType == "ActivateRadioCommunication" then
        stopTask( "agent.frago.DeactivateRadio" )
        orderType = "agent.frago.ActivateRadio"        
        
    -- ----------------------------------------------------------------------------
    -- NBC alert
    -- ----------------------------------------------------------------------------
    --FR
    elseif orderType == "Rep_OrderConduite_MettreTenueNBC" then
        stopTask( "agent.tasks.UnequipNBCOutfit" )
        orderType = "agent.tasks.EquipNBCOutfit"
    elseif orderType == "Rep_OrderConduite_EnleverTenueNBC" then
        stopTask( "agent.tasks.EquipNBCOutfit" )
        orderType = "agent.tasks.UnequipNBCOutfit"
    --WW
    elseif orderType == "ActivateNBCProtection" then
        stopTask( "agent.frago.DeactivateNBCProtection" )
        orderType = "agent.frago.ActivateNBCProtection"
    elseif orderType == "DeactivateNBCProtection" then
        stopTask( "agent.frago.ActivateNBCProtection" )
        orderType = "agent.frago.DeactivateNBCProtection"
        

    -- ----------------------------------------------------------------------------
    -- Attitude
    -- ----------------------------------------------------------------------------
    -- FR base
    elseif orderType =="Rep_OrderConduite_ChangerAmbiance" then
        if integration.getConduiteChangerAmbianceParameter( self ) == eAmbianceMission_Surete then
        orderType = "platoon.tasks.ChangerAmbianceSurete"
    else
        orderType = "platoon.tasks.ChangerAmbianceVitesse"
        stopTask( "platoon.tasks.ChangerAmbianceSurete" )
    end
    -- WW base
    elseif orderType =="ChangeAttitude" then
        if integration.getConduiteChangerAmbianceParameter( self ) == eAmbianceMission_Surete then
            stopTask( "agent.frago.DeactivateSafetyAttitude" )
            orderType = "agent.frago.ActivateSafetyAttitude"
        else
            stopTask( "agent.frago.ActivateSafetyAttitude" )
            orderType = "agent.frago.DeactivateSafetyAttitude"
        end

    -- ----------------------------------------------------------------------------
    -- Flying altitude modifier 
    -- ----------------------------------------------------------------------------
    elseif orderType == "platoon.combat.support.air.tasks.ModifierAltitude" then
        mission.altitude = integration.getNbItParameter( self )
        elseif orderType == "Rep_OrderConduite_Population_ChangerAttitude" then
        integration.changeAttitude(integration.getorderConduitePopulationChangerAttitudeParameter( self ))
        integration.cleanFragOrder( self )
        return

    -- ----------------------------------------------------------------------------
    -- Ammunition 
    -- ----------------------------------------------------------------------------
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

    -- ----------------------------------------------------------------------------
    -- Mount/dismount orders 
    -- ----------------------------------------------------------------------------
    -- FR
    elseif orderType == "platoon.tasks.Embarquer" then
        stopTask( "platoon.tasks.Debarquer" )
        integration.cleanFragOrder( self )
    elseif orderType == "platoon.tasks.Debarquer" then
        stopTask( "platoon.tasks.Embarquer" )
        integration.cleanFragOrder( self )
    
    -- WW
    elseif orderType == "Mount" then
        stopTask( "agent.frago.Dismount" )
        orderType = "agent.frago.Mount"
        integration.cleanFragOrder( self )
    elseif orderType == "Dismount" then
        stopTask( "agent.frago.Mount" )
        orderType = "agent.frago.Dismount"
        integration.cleanFragOrder( self )

    -- ----------------------------------------------------------------------------
    -- Rules of engagement, enemy engagement
    -- ----------------------------------------------------------------------------
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
    
    -- ----------------------------------------------------------------------------
    -- LOG
    -- ----------------------------------------------------------------------------
    elseif orderType == "Rep_OrderConduite_ModifierRegimeTravailMaintenance" then
        if integration.isLogisticTypeUnit( ) then
            integration.changeMaintenanceWorkMode( integration.getOrderConduiteModifierRegimeTravailMaintenanceParameter( self ) )
        end
        integration.cleanFragOrder( self )
        return
        elseif orderType == "Rep_OrderConduite_ModifierPrioritesTactiquesBlesses" then
        if integration.isLogisticTypeUnit( ) then
            integration.changeTacticHealtPriority( integration.getOrderConduiteModifierPrioritesTactiquesBlessesParameter( self ) )
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
            integration.changeMaintenancePriority( integration.getOrderConduiteModifierPrioritesReparationsParameter( self )() )
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
            orderType = "platoon.combat.support.log.tasks.RenforcerEnVSRAM" 
            mission.pionARenforcer = CreateKnowledge( integration.ontology.types.agent, integration.getPionARenforcerParameter( self ) )
            mission.nbrAmbulances = integration.getNbrAmbulancesParameter( self )
        else
            integration.cleanFragOrder( self )
            return
        end
    elseif orderType == "Rep_OrderConduite_Pion_TransfererVSRAM" then
        if integration.isLogisticTypeUnit( ) then
            orderType = "platoon.combat.support.log.tasks.TransfererVSRAM"
            mission.pionRenforce = CreateKnowledge( integration.ontology.types.agent, integration.getPionRenforceParameter( self )() )
            mission.pionARenforcer = CreateKnowledge( integration.ontology.types.agent, integration.getPionARenforcerParameter( self ) )
            mission.nbrAmbulances = integration.getNbrAmbulancesParameter( self )
        else
            integration.cleanFragOrder( self )
            return
        end
        elseif orderType == "Rep_OrderConduite_Pion_ReprendreAuxOrdresVSRAM" then
        if integration.isLogisticTypeUnit( ) then
            orderType = "platoon.combat.support.log.tasks.ReprendreAuxOrdresVSRAM"
            mission.pionRenforce = CreateKnowledge( integration.ontology.types.agent, integration.getPionRenforceParameter( self )() )
            mission.nbrAmbulances = integration.getNbrAmbulancesParameter( self )
        else
            integration.cleanFragOrder( self )
            return
        end
    elseif orderType == "Rep_OrderConduite_Pion_RenforcerEnEquipements" then
        local equipments = integration.getOrderConduiteModifierPrioritesReparationsParameter( self )()
        if not equipments or #equipments ~= 1 then 
        	integration.cleanFragOrder( self )
        	error( "Need exactly one type of equipment" ) 
        end
        integration.startGiveEquipment( integration.getPionRenforceParameter( self )(), integration.getPionRenforceParameter( self )(), equipments[1], integration.getNbrAmbulancesParameter( self ) )
        integration.cleanFragOrder( self )
        return
        elseif orderType == "Rep_OrderConduite_Pion_TransfererEquipements" then
        local equipments = integration.getOrderConduiteModifierPrioritesReparationsParameter( self )()
        if not equipments or #equipments ~= 1 then 
        	integration.cleanFragOrder( self )
        	error( "Need exactly one type of equipment" ) 
        end
        integration.startTakeEquipment( integration.getPionRenforceParameter( self )(), equipments[1], integration.getNbrAmbulancesParameter( self ) )
        integration.startGiveEquipment( integration.getPionRenforceParameter( self )(), integration.getPionARenforcerParameter( self ), equipments[1], integration.getNbrAmbulancesParameter( self ) )
        integration.cleanFragOrder( self )
        return
    elseif orderType == "Rep_OrderConduite_Pion_ReprendreAuxOrdresEquipements" then
        local equipments = integration.getOrderConduiteModifierPrioritesReparationsParameter( self )()
        if not equipments or #equipments ~= 1 then 
        	integration.cleanFragOrder( self )
        	error( "Need exactly one type of equipment" ) 
        end
        integration.startTakeEquipment( integration.getPionRenforceParameter( self )(), equipments[1], integration.getNbrAmbulancesParameter( self ) )
        integration.cleanFragOrder( self )
        return
    elseif orderType == "Rep_OrderConduite_Pion_RenforcerEnRemorqueurs" then
        if integration.isLogisticTypeUnit( ) then
            orderType = "platoon.combat.support.log.tasks.RenforcerEnRemorqueurs"
            mission.pionARenforcer = CreateKnowledge( integration.ontology.types.agent, integration.getPionARenforcerParameter( self ) )
            mission.nbrRemorqueurs = integration.getNbrRemorqueursParameter( self )
        else
            integration.cleanFragOrder( self )
            return
        end
    elseif orderType == "Rep_OrderConduite_Pion_TransfererRemorqueurs" then
        if integration.isLogisticTypeUnit( ) then   
            orderType = "platoon.combat.support.log.tasks.TransfererRemorqueurs"
            mission.pionRenforce = CreateKnowledge( integration.ontology.types.agent, integration.getPionRenforceParameter( self )() )
            mission.pionARenforcer = CreateKnowledge( integration.ontology.types.agent, integration.getPionARenforcerParameter( self ) )
            mission.nbrRemorqueurs = integration.getNbrRemorqueursParameter( self )
        else
            integration.cleanFragOrder( self )
            return
        end
    elseif orderType == "Rep_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs" then
        if integration.isLogisticTypeUnit( ) then     
            orderType = "platoon.combat.support.log.tasks.ReprendreAuxOrdresRemorqueurs"
            mission.pionRenforce = CreateKnowledge( integration.ontology.types.agent, integration.getPionRenforceParameter( self )() )
            mission.nbrRemorqueurs = integration.getNbrRemorqueursParameter( self )
        else
            integration.cleanFragOrder( self )
            return
        end

    -- ----------------------------------------------------------------------------
    -- Engineer
    -- ----------------------------------------------------------------------------
    elseif orderType == "platoon.combat.support.engineer.tasks.ActiverObstacles" then
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

    -- ----------------------------------------------------------------------------
    -- Rules of engagement, enemy engagement
    -- ----------------------------------------------------------------------------
    elseif orderType == "Rep_OrderConduite_RecupererTransporteurs" then
        integration.retrieveTransporter()
        integration.cleanFragOrder( self )
        return

    -- ----------------------------------------------------------------------------
    -- Obsolete
    -- ----------------------------------------------------------------------------
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
    elseif orderType == "platoon.combat.support.art.tasks.AppliquerFeux" then
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

    -- ----------------------------------------------------------------------------
    -- ISSUING THE TASK
    -- ----------------------------------------------------------------------------
    masalife.brain.core.startTask( orderType, mission )
    integration.cleanFragOrder( self )
end





integration.getFireParameters = function( self )
    local params = {}
    local uggly = function() params.entities = { CreateKnowledge( integration.ontology.types.agentKnowledge, integration.getAgentKnowledgeParameter( self ) ) } end
    if not pcall( uggly ) then
        params.entities = { CreateKnowledge( integration.ontology.types.point, integration.getpointCibleParameter( self ) ) }
    end
    params.munition = integration.getMunitionsParameter( self )
    params.interventionType = integration.getNbItParameter( self )
    return params
end

-- CR ROE 
integration.CR_ROE  = function( typeROE )
    if typeROE == eRoeStateRestrictedFire then
        DEC_RC( eRC_TirRestreint )
    elseif typeROE ==  eRoeStateFireByOrder then
        DEC_RC( eRC_TirInterdit )
    elseif typeROE == eRoeStateFreeFire then
        DEC_RC( eRC_TirLibre )
    end
end

-- CR ROE Foule
integration.CR_ROE_Foules  = function( typeROE )
    if typeROE == eEtatROEPopulation_EmploiForceInterdit then
        DEC_RC( eRC_EmploiForceInterdit )
    elseif typeROE ==  eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux then
        DEC_RC( eRC_MaintienADistanceParMoyensNonLetaux )
    elseif typeROE == eEtatROEPopulation_ArmesLetalesAutorisees then
        DEC_RC( eRC_ArmesLetalesAutorisees )
    end
end

-- CR Attitude Foule
integration.CR_Attitude_Foules  = function ( attitude )
    if attitude == eAttitudeCalme then
        DEC_RC( eRC_EmploiForceInterdit )
    elseif attitude ==  eAttitudeAgitee then
        DEC_RC( eRC_MaintienADistanceParMoyensNonLetaux )
    elseif attitude == eAttitudeExcitee then
        DEC_RC( eRC_ArmesLetalesAutorisees )
    elseif attitude == eAttitudeAgressive then
        DEC_RC( eRC_ArmesLetalesAutorisees )
    end
end

integration.changeCrowdROEForAutomat = function( roe )
    DEC_Automate_ChangeEtatROEPopulation( roe )
end

integration.changeCrowdROEForAgent = function( roe )
    DEC_Agent_ChangeEtatROEPopulation( roe )
end

integration.getCrowdROEForAgent = function()
    return DEC_Agent_RoePopulation()
end

integration.retrieveTransporter = function()
    DEC_RecupererTransporteursSansDelai()
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
    return fragorder.source:GetType()
end