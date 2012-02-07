-------------------------------------------------------------------------------
-- FragOrder Implementation :
-- Regroup function to manage platoon equipement
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
  return self.source:GetType()=="Rep_OrderConduite_ChangerReglesEngagement"
end

integration.isTask = function( self )
  local orderType = self.source:GetType()
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
         orderType == "Rep_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs"
end

integration.mustBePropagate = function( self )
  local orderType = self.source:GetType()
  return orderType == "france.military.platoon.combat.support.air.tasks.ModifierAltitude" or
         orderType == "france.military.platoon.combat.support.engineer.tasks.ActiverObstacles" or
         orderType == "france.military.platoon.tasks.Embarquer" or 
         orderType == "france.military.platoon.tasks.Debarquer" or
         orderType == "france.military.platoon.tasks.DeposerUnite" or
         orderType == "Rep_OrderConduite_Interrompre" or
         orderType == "Rep_OrderConduite_AttendreSePoster" or
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
         orderType == "Rep_OrderConduite_ChangerReglesEngagement" or
         orderType == "Rep_OrderConduite_ChangerReglesEngagementPopulation" or
         orderType == "Rep_OrderConduite_Deboucher" or 
         orderType == "Rep_OrderConduite_Decrocher" or 
         orderType == "Rep_OrderConduite_Population_ChangerAttitude" or
         orderType == "Rep_OrderConduite_ModifierRegimeTravailMaintenance" or
         orderType == "Rep_OrderConduite_ModifierPrioritesTactiquesBlesses" or
         orderType == "Rep_OrderConduite_ModifierPrioritesTactiquesReparations" or
         orderType == "Rep_OrderConduite_ModifierPrioritesReparations" or
         orderType == "Rep_OrderConduite_ModifierPrioritesBlesses"
end

integration.updateROE = function( self )
  DEC_Agent_ChangeEtatROE(self.source:GetorderConduiteChangerReglesEngagement_())
  integration.CR_ROE ( DEC_Agent_GetEtatROE() )
  integration.cleanFragOrder( self )
end

integration.setROE = function( etatROE )
   DEC_Agent_ChangeEtatROE( etatROE )
   integration.CR_ROE ( etatROE )
end

integration.setCompanyROE = function( etatROE )
   DEC_Automate_ChangeEtatROE( etatROE )
   integration.CR_ROE ( etatROE )
end

local eAmbianceMission_Surete = 0
local eAmbianceMission_Vitesse = 1

integration.startFragOrderTask = function( self )
  local mission = {}

  local stopTask = function( task )
    if masalife.brain.core.isTaskRunning( task )  then
        masalife.brain.core.stopTask( task )
    end
  end

  local orderType = self.source:GetType()

  if orderType == "france.military.platoon.tasks.Illuminer" then
      mission.entity = CreateKnowledge( integration.ontology.types.agentKnowledge, self.source:GetAgentKnowledge_() )
      if self.source == meKnowledge.source then
        mission.ally = meKnowledge
      else
        mission.ally = CreateKnowledge( integration.ontology.types.agent, self.source:GetAgent_() )
      end
  elseif orderType =="france.military.platoon.combat.support.art.tasks.UtiliserALR" then
    mission.entity = CreateKnowledge( integration.ontology.types.point, self.source:GetpointCible_() )
    mission.munition = self.source:Getmunitions_()
    mission.interventionType = self.source:GetnbIT_()
  elseif orderType == "france.military.platoon.tasks.RejoindreAToutPrix" then
    mission.objectif = CreateKnowledge( integration.ontology.types.point, self.source:GetpointCible_() )
  elseif orderType == "france.military.platoon.tasks.DeposerUnite" then
    local targetPoint = self.source:GetpointCible_()
    mission.unite = CreateKnowledge( integration.ontology.types.agentKnowledge, self.source:GetAgentKnowledge_() )
    if targetPoint ~= nil then
        mission.position = CreateKnowledge( integration.ontology.types.point, targetPoint )
    end
  elseif orderType == "france.military.platoon.tasks.Observer" then
    mission.objective = CreateKnowledge( integration.ontology.types.point, self.source:GetpointCible_() )
  elseif orderType == "france.military.platoon.tasks.Orienter" then
    mission.objective = CreateKnowledge( integration.ontology.types.point, self.source:GetpointCible_() )
  elseif orderType == "Rep_OrderConduite_AttendreSePoster" then
    local point = CreateKnowledge( integration.ontology.types.point, DEC_Geometrie_CopiePoint( meKnowledge:getPosition() ) )
    mission.objectives = { point }
    orderType = "sword.military.platoon.tasks.frago.WaitOn"
  elseif orderType == "Rep_OrderConduite_Attendre" then
    local point = CreateKnowledge( integration.ontology.types.point, DEC_Geometrie_CopiePoint( meKnowledge:getPosition() ) )
    mission.objective = point
    orderType = "sword.military.crowd.tasks.frago.WaitOn"
  elseif orderType == "Rep_OrderConduite_Poursuivre" then
    stopTask( "sword.military.platoon.tasks.frago.WaitOn" )
    stopTask( "sword.military.crowd.tasks.frago.WaitOn" )
    myself.blocked = nil
    myself.canBeBlocked = nil
    myself.blockForcesContinue = true
    integration.cleanFragOrder( self )
    return
  elseif orderType =="Rep_OrderConduite_Interrompre" then
    masalife.brain.core.stopTasks()
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
    if self.source:GetorderConduiteChangerAmbiance_() == eAmbianceMission_Surete then
        orderType = "france.military.platoon.tasks.ChangerAmbianceSurete"
    else
        orderType = "france.military.platoon.tasks.ChangerAmbianceVitesse"
        stopTask( "france.military.platoon.tasks.ChangerAmbianceSurete" )
    end
  elseif orderType =="Rep_OrderConduite_Pion_InterdireMunition" then
    DEC_Pion_InterdireMunition(self.source:Getmunitions_())
    integration.cleanFragOrder( self )
    return
  elseif orderType =="Rep_OrderConduite_Pion_AutoriserMunition" then
    DEC_Pion_AutoriserMunition(self.source:Getmunitions_())
    integration.cleanFragOrder( self )
    return
  elseif orderType =="Rep_OrderConduite_Pion_AutoriserToutesMunitions" then
    DEC_Pion_AutoriserToutesMunitions()
    integration.cleanFragOrder( self )
    return
  elseif orderType == "france.military.platoon.tasks.Embarquer" then
    stopTask( "france.military.platoon.tasks.Debarquer" )
    integration.cleanFragOrder( self )
  elseif orderType == "france.military.platoon.tasks.Debarquer" then
    stopTask( "france.military.platoon.tasks.Embarquer" )
    integration.cleanFragOrder( self )
  elseif orderType == "france.military.platoon.combat.support.air.tasks.ModifierAltitude" then
    mission.altitude = self.source:GetnbIT_()
  elseif orderType == "Rep_OrderConduite_Population_ChangerAttitude" then
    DEC_Population_ChangerAttitude(self.source:GetorderConduitePopulationChangerAttitude_())
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_ChangerReglesEngagementPopulation" then
    DEC_Agent_ChangeEtatROEPopulation(self.source:GetorderConduiteChangerReglesEngagementPopulation_())
    integration.CR_ROE_Foules ( DEC_Agent_RoePopulation() )
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_Pion_Engager" then
    orderType = "france.military.platoon.combat.support.art.tasks.Engager"
  elseif orderType == "Rep_OrderConduite_Pion_Desengager" then
    orderType = "france.military.platoon.combat.support.art.tasks.Desengager"
  elseif orderType == "Rep_OrderConduite_ModifierRegimeTravailMaintenance" then
    DEC_Maintenance_ChangerRegimeTravail( self.source:GetorderConduiteModifierRegimeTravailMaintenance_() )
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_ModifierPrioritesTactiquesBlesses" then
    DEC_Sante_ChangerPrioritesTactiques( self.source:GetorderConduiteModifierPrioritesTactiquesBlesses_() )
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_ModifierPrioritesTactiquesReparations" then
    DEC_Maintenance_ChangerPrioritesTactiques( self.source:GetorderConduiteModifierPrioritesTactiquesReparations_() )
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_ModifierPrioritesReparations" then
    DEC_Maintenance_ChangerPriorites( self.source:GetorderConduiteModifierPrioritesReparations_() )
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_ModifierPrioritesBlesses" then
    DEC_Sante_ChangerPriorites( self.source:GetorderConduiteModifierPrioritesBlesses_() )
    integration.cleanFragOrder( self )
    return
  elseif orderType == "Rep_OrderConduite_Pion_RenforcerEnVSRAM" then
    orderType = "france.military.platoon.combat.support.log.tasks.RenforcerEnVSRAM" 
    mission.pionARenforcer = CreateKnowledge( integration.ontology.types.agent, self.source:GetpionARenforcer_() )
    mission.nbrAmbulances = self.source:GetnbrAmbulances_()
  elseif orderType == "Rep_OrderConduite_Pion_TransfererVSRAM" then
    orderType = "france.military.platoon.combat.support.log.tasks.TransfererVSRAM"
    mission.pionRenforce = CreateKnowledge( integration.ontology.types.agent, self.source:GetpionRenforce_() )
    mission.pionARenforcer = CreateKnowledge( integration.ontology.types.agent, self.source:GetpionARenforcer_() )
    mission.nbrAmbulances = self.source:GetnbrAmbulances_()
  elseif orderType == "Rep_OrderConduite_Pion_ReprendreAuxOrdresVSRAM" then
    orderType = "france.military.platoon.combat.support.log.tasks.ReprendreAuxOrdresVSRAM"
    mission.pionRenforce = CreateKnowledge( integration.ontology.types.agent, self.source:GetpionRenforce_() )
    mission.nbrAmbulances = self.source:GetnbrAmbulances_()
  elseif orderType == "Rep_OrderConduite_Pion_RenforcerEnRemorqueurs" then
    orderType = "france.military.platoon.combat.support.log.tasks.RenforcerEnRemorqueurs"
    mission.pionARenforcer = CreateKnowledge( integration.ontology.types.agent, self.source:GetpionARenforcer_() )
    mission.nbrRemorqueurs = self.source:GetnbrRemorqueurs_()
  elseif orderType == "Rep_OrderConduite_Pion_TransfererRemorqueurs" then
    orderType = "france.military.platoon.combat.support.log.tasks.TransfererRemorqueurs"
    mission.pionRenforce = CreateKnowledge( integration.ontology.types.agent, self.source:GetpionRenforce_() )
    mission.pionARenforcer = CreateKnowledge( integration.ontology.types.agent, self.source:GetpionARenforcer_() )
    mission.nbrRemorqueurs = self.source:GetnbrRemorqueurs_()
  elseif orderType == "Rep_OrderConduite_Pion_ReprendreAuxOrdresRemorqueurs" then
    orderType = "france.military.platoon.combat.support.log.tasks.ReprendreAuxOrdresRemorqueurs"
    mission.pionRenforce = CreateKnowledge( integration.ontology.types.agent, self.source:GetpionRenforce_() )
    mission.nbrRemorqueurs = self.source:GetnbrRemorqueurs_()
  elseif orderType == "france.military.platoon.combat.support.engineer.tasks.ActiverObstacles" then
    local obstacles = {}
    local obstaclesList = self.source:GetObjectKnowledge_()
    for _, obstacle in pairs( obstaclesList ) do
        obstacles[#obstacles + 1] = CreateKnowledge( integration.ontology.types.object, obstacle )
    end
    mission.obstacles = obstacles
    obstacles = nil
  end

  masalife.brain.core.startTask( orderType, mission )
  integration.cleanFragOrder( self )
end

integration.getParameters = function( self )
    local params = {}
    local uggly = function() params.entities = { CreateKnowledge( integration.ontology.types.agentKnowledge, self.source:GetAgentKnowledge_() ) } end
    if not pcall( uggly ) then
        params.entities = { CreateKnowledge( integration.ontology.types.point, self.source:GetpointCible_() ) }
    end
    params.munition = self.source:Getmunitions_()
    params.interventionType = self.source:GetnbIT_()
    return params
end

-- CR ROE 
integration.CR_ROE  = function ( typeROE )
    if typeROE == eRoeStateRestrictedFire then
        DEC_RC( eRC_TirRestreint )
    elseif typeROE ==  eRoeStateFireByOrder then
        DEC_RC( eRC_TirInterdit )
    elseif typeROE == eRoeStateFreeFire then
        DEC_RC( eRC_TirLibre )
    end
end

-- CR ROE Foule
integration.CR_ROE_Foules  = function ( typeROE )
    if typeROE == eEtatROEPopulation_EmploiForceInterdit then
        DEC_RC( eRC_EmploiForceInterdit )
    elseif typeROE ==  eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux then
        DEC_RC( eRC_MaintienADistanceParMoyensNonLetaux )
    elseif typeROE == eEtatROEPopulation_ArmesLetalesAutorisees then
        DEC_RC( eRC_ArmesLetalesAutorisees )
    end
end

integration.setOrderConduiteChangerAmbiance = function ( fragOrder, value )
    fragOrder:SetorderConduiteChangerAmbiance_( value )
end
