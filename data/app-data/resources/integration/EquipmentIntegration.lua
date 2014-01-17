--- Returns true if this entity has the given piece of equipment, false otherwise.
-- This method can only be called by an agent.
-- @param resource Resource type
-- @return Boolean
integration.hasDotation = function( resource )
    return DEC_HasDotation( resource )
end

--- Returns true if this entity has at least the given quantity of the provided piece
-- of equipment, false otherwise.
-- This method can only be called by an agent.
-- @param resource Resource type
-- @param quantity Integer, the quantity of needed resource
-- @return Boolean
integration.hasDotationForFiring = function( resource, quantity )
 return DEC_HasDotationForFiring( myself, resource, quantity )
end

--- Returns true if this entity can use the given piece of equipment, false otherwise.
-- This method can only be called by an agent.
-- @param resource Resource type
-- @return Boolean
integration.canUseDotation = function( resource )
    return DEC_CanUseDotation( resource )
end

--- Returns true if the given agent is contaminated, false otherwise.
-- @param agent Directia agent
-- @return Boolean
integration.isContaminated = function( agent )
    return agent.source:DEC_Agent_EstContamine()
end

--- Returns true if the decontamination of the given agent is finished, false otherwise.
-- @param agent Directia agent
-- @return Boolean
integration.isDecontaminationDone = function( agent )
    return agent.source:DEC_Agent_IsDecontaminationFinished()
end

--- Returns true if this entity is poisoned, false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.isPoisoned = function()
    return DEC_Agent_EstEmpoisonne()
end

--- Returns true if this entity is an NBC agent, false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.isAgentNBC = function()
    return DEC_Agent_EstAgentNBC()
end

--- Makes this entity equip its NBC outfit.
-- Displays reports.
-- Does nothing if this entity is already wearing
-- its NBC outfit, or if it does not have one.
-- This method can only be called by an agent.
-- @return Boolean, true if the outfit has been successfully equipped or if
-- it was already equipped, false if this entity has no NBC outfit to equip.
integration.equipNBCOutfitSecu = function()
    if meKnowledge.equipOutfit then
        return true
    end
    local RNBCLevel = integration.getRNBCProtectionLevel()
    if RNBCLevel and RNBCLevel ~= 0 then
        meKnowledge:sendReport( eRC_TenueProtectionNiveauNRBC, RNBCLevel )
        DEC_Agent_MettreTenueProtectionNBC()
        meKnowledge.equipOutfit = true
        return true
    else
        meKnowledge:sendReport( eRC_PasTenueNBC )
        return false
    end
end

--- Makes this entity unequip its NBC outfit.
-- Displays reports.
-- Does nothing if this entity is not wearing
-- its NBC outfit, or if it does not have one.
-- This method can only be called by an agent.
integration.unequipNBCOutfitSecu = function()
    if not meKnowledge.equipOutfit then
        return
    end
    local RNBCLevel = integration.getRNBCProtectionLevel()
    if RNBCLevel and RNBCLevel ~= 0 then
        meKnowledge:sendReport( eRC_TenueProtectionNBCEnlevee )
        DEC_Agent_EnleverTenueProtectionNBC()
        meKnowledge.equipOutfit = false
    else
        meKnowledge:sendReport( eRC_PasTenueNBC )
    end
end

--- Returns the RNBC protection level of this entity, as defined in the physical database.
-- This method can only be called by an agent.
-- @return Integer, the RNBC protection level from 0 to 5, 0 meaning there is no RNBC
-- protection available for this entity.
integration.getRNBCProtectionLevel = function()
    meKnowledge.RNBCProtectionLevel = meKnowledge.RNBCProtectionLevel or DEC_Agent_NiveauProtectionNBC()
    return meKnowledge.RNBCProtectionLevel
end

--- Makes the provided agent go on NBC alert level four.
-- Displays a report.
-- @param agent Directia agent
integration.goOnNBCAlert = function( agent )
    reportFunction( eRC_AlerteNBCNiveau4 )
    F_Pion_SeteEtatNbc( agent.source, eEtatNbc_Niv4 )
    myself.NBCAlert = true
end

--- Makes this entity equip its NBC outfit.
-- Displays reports.
-- This method can only be called by an agent.
integration.equipNBCOutfit = function()
    reportFunction( eRC_TenueProtectionNBCMise )
    DEC_Agent_MettreTenueProtectionNBC()
end

--- Makes this entity unequip its NBC outfit and 
--- cease its potential NBC alert state.
-- Displays reports.
-- This method can only be called by an agent.
integration.unequipNBCOutfit = function()
    reportFunction( eRC_TenueProtectionNBCEnlevee )
    if myself.NBCAlert == true then
        reportFunction( eRC_FinAlerteNBCNiveau4 )
        F_Pion_SeteEtatNbc( myself, eEtatNbc_Niv0 )
        myself.NBCAlert = false
    end
    DEC_Agent_EnleverTenueProtectionNBC()
end

--- Switches off the radio communications of the given agent.
-- Displays a report.
-- @param agent Directia agent
integration.switchOffRadio = function( agent )
    reportFunction( eRC_DebutSilenceRadio )
    F_Pion_SeteEtatRadio( agent.source, eEtatRadio_Silence )
    DEC_Agent_PasserEnSilenceRadio()
end

--- Switches off the outgoing radio emissions of the given agent.
-- Displays a report.
-- @param agent Directia agent
integration.switchEmitOnlyOffRadio = function( agent )
    reportFunction( eRC_DebutSilencePartielRadio )
    F_Pion_SeteEtatRadio( agent.source, eEtatRadio_Silence_Partiel )
    DEC_Agent_PasserEnSilenceRadioPartiel()
end

--- Switches on the radio communications of the given agent.
-- Displays a report.
-- @param agent Directia agent
integration.switchOnRadio = function( agent )
    reportFunction( eRC_FinSilenceRadio )
    F_Pion_SeteEtatRadio( agent.source, eEtatRadio_Ouverte )
    DEC_Agent_ArreterSilenceRadio()
end

--- Enables radar silence for the given agent.
-- Displays a report.
-- @param agent Directia agent
integration.switchOffRadar = function( agent )
    reportFunction( eRC_DebutSilenceRadar )
    F_Pion_SeteEtatRadar( agent.source, eEtatRadar_Silence )
    DEC_Perception_DesactiverRadar( eRadarType_Radar )
    DEC_Perception_DesactiverRadar( eRadarType_Ecoute )
    DEC_Perception_DesactiverRadar( eRadarType_EcouteRadar )
end

--- Enables radar silence for the given agent.
-- Displays a report.
-- @param agent Directia agent
integration.switchOnRadar = function( agent )
    reportFunction( eRC_FinSilenceRadar )
    F_Pion_SeteEtatRadar( agent.source, eEtatRadar_Ouvert )
    DEC_Perception_ActiverRadar( eRadarType_Radar )
    DEC_Perception_ActiverRadar( eRadarType_Ecoute )
    DEC_Perception_ActiverRadar( eRadarType_EcouteRadar )
end

--- Switches on this entity's safety attitude, in order for it to take cover,
--- move slower, fly lower (if it is a flying agent), activate sensors, etc.
-- May display reports.
-- This method can only be called by an agent.
-- @see integration.switchOffSafetyMode
-- @see integration.setMovementPace
integration.switchOnSafetyMode = function()
    DEC_Perception_ActiverCoupsDeSonde()
    DEC_Agent_ChangerAmbianceEnSurete( true )
    if integration.isFlying() then
        DEC_Agent_HauteurDeVol( DEC_Agent_GetTacticalFlyingHeight( myself ) )
    end
    if not myself.reportSafetyMode or myself.reportSafetyMode == nil then
         myself.reportSafetyMode = true
         reportFunction( eRC_AmbianceSurete )
    end
    myself.speedModulation = myself.speedModulation or {}
    myself.speedModulation.switchOnSafetyMode = 0.3 -- modulationMax / 10 Scipio
    myself.safetyMode = true -- WW base
end

--- Switches on this entity's cover attitude, in order for it to move slower.
-- This method can only be called by an agent.
-- @see integration.switchOffCoverMode
-- @see integration.setMovementPace
integration.switchOnCoverMode = function()
    --scipio
    myself.speedModulation = myself.speedModulation or {}
	if meKnowledge:hasBadForceRatio() then
        myself.speedModulation.switchOnCoverMode = 0.3
    else
        myself.speedModulation.switchOnCoverMode = 0.4
    end

	-- WW base
	myself.coverMode = true
end

--- Switches off this entity's safety attitude.
-- May display reports.
-- This method can only be called by an agent.
-- @see integration.switchOnSafetyMode
-- @see integration.setMovementPace
integration.switchOffSafetyMode = function()
    DEC_Perception_DesactiverCoupsDeSonde()
    DEC_Agent_ChangerAmbianceEnSurete( false )
    if integration.isFlying() then
        DEC_Agent_HauteurDeVol( DEC_Agent_GetStandardFlyingHeight( myself ) )
    end
    if myself.reportSafetyMode or myself.reportSafetyMode == nil then
        myself.reportSafetyMode = false
        reportFunction(eRC_AmbianceVitesse )
    end

    myself.speedModulation = myself.speedModulation or {}
    myself.speedModulation.switchOnSafetyMode = 1 -- scipio
    myself.safetyMode = false  -- ww base
end

--- Switches off this entity's cover attitude.
-- This method can only be called by an agent.
-- @see integration.switchOnCoverMode
-- @see integration.setMovementPace
integration.switchOffCoverMode = function()
    DEC_Perception_DesactiverCoupsDeSonde()
    DEC_Agent_ChangerAmbianceEnSurete( false )
    reportFunction(eRC_CouvertureDesactive )
	myself.speedModulation = myself.speedModulation or {}
	myself.speedModulation.switchOnCoverMode = 1 --scipio
    myself.coverMode = false -- ww base
end

--- Returns true if this entity can dismount, false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.canDismount = function()
    return DEC_Agent_EstEmbarquable()
end

--- Returns true if this entity can mount, false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.canMount = function()
    return DEC_Agent_EstEmbarquable() and DEC_CanMount( myself )
end

--- Returns true if this entity should dismount, i.e. if it can dismount
--- and if it is located on a non-trafficable urban position, false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.shouldDismount = function()
    return DEC_Agent_EstEmbarquable() and not DEC_Agent_EstDebarque()
           and not DEC_IsPointInUrbanBlockTrafficable( DEC_Agent_Position() )
end

--- Makes this entity start mounting if it can (no other mounting action is taking place
--- and the entity is dismounted) and if it is supposed to (this entity is located on
--- a trafficable position).
--- Otherwise, this method stops the potential current mounting action (started by a
--- previous call of this method).
-- This method can only be called by an agent.
-- @see integration.updateMoveToIt
-- @see integration.stopMount
-- @return Boolean, false if the action is ongoing, true otherwise.
integration.mountStarted = function()
    myself.stayDismounted = false
    if( myself.mount == nil and myself.dismount == nil ) -- means that movement use mount or dismount action
      and not DEC_Agent_EstEmbarque() 
      and not myself.enteringNonTrafficableElement
      and DEC_IsPointInUrbanBlockTrafficable( DEC_Agent_Position() ) then
        myself.orderMount = myself.orderMount or DEC_StartEmbarquement()
        return false
    else
        if myself.orderMount then
            DEC__StopAction( myself.orderMount )
            myself.orderMount = nil
        end
    end
    return true
end

--- Makes this entity start dismounting if it can (no other dismounting action is taking place,
--- and the entity is mounted).
--- Otherwise, this method stops the potential current dismounting action (started by a
--- previous call of this method).
-- This method can only be called by an agent.
-- @see integration.updateMoveToIt
-- @see integration.stopDismount
-- @return Boolean, false if the action is ongoing, true otherwise.
integration.dismountStarted = function()
    myself.stayDismounted = true
    if( myself.mount == nil and myself.dismount == nil ) -- means that movement use mount or dismount action
        and not DEC_Agent_EstDebarque() then
        myself.orderDismount = myself.orderDismount or DEC_StartDebarquement()
        return false
    else -- movement, manage
        if myself.orderDismount then
            DEC__StopAction( myself.orderDismount )
            myself.orderDismount = nil
        end
    end
    return true
end

--- Makes this entity stop its potential current dismounting action, started by a
--- call to the integration.dismountStarted method.
-- This method can only be called by an agent.
-- @see integration.dismountStarted
integration.stopDismount = function()  
    myself.stayDismounted = false
    if myself.orderDismount then
        DEC__StopAction( myself.orderDismount )
        myself.orderDismount = nil
    end
end

--- Makes this entity stop its potential current mounting action, started by a
--- call to the integration.mountStarted method.
-- This method can only be called by an agent.
-- @see integration.mountStarted
integration.stopMount = function()
    myself.stayDismounted = false
    if myself.orderMount then
        DEC__StopAction( myself.orderMount )
        myself.orderMount = nil
    end
end

--- Returns true if this entity is mounted, false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.isMounted = function()
    return DEC_Agent_EstEmbarque()
end

--- Returns true if this entity is dismounted, false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.isDismounted = function()
    return DEC_Agent_EstDebarque()
end

--- Deploys this entity.
-- This method can only be called by an agent.
integration.startDeploy = function()
    DEC_Agent_Deploy()
end

--- Undeploys this entity.
-- This method can only be called by an agent.
integration.undeploy = function()
    DEC_Agent_Undeploye()
end

--- Returns true if this entity is deployed, false otherwise.
-- This method can only be called by an agent.
integration.isDeployed = function()
    return DEC_Agent_IsDeployed()
end

--- Finds an available drone and starts activating it (deploys it).
-- Display reports.
-- This method can only be called by an agent.
-- @see integration.setAvailableDrones
-- @see integration.startedActivateDrone
-- @param self Deprecated, unused
-- @param alreadyDeployed Boolean, this method does not deploy the drone if set to true
integration.startActivateDrone = function ( self, alreadyDeployed )
    integration.setAvailableDrones()
	myself.Deployed = false
    if myself.droneAvailable == nil then
        reportFunction( eRC_PasDeDroneDisponible )
        if not integration.isCommanderEngaged() then
            meKnowledge:sendNoDisponibleDrone( meKnowledge:getAutomat() )
        end
	else
	    if not alreadyDeployed then
		    DEC_Agent_Deploy()
		end
		reportFunction( eRC_DebutMiseEnOeuvreDrone )
	end
end

--- Continues the activation of the currently deployed drone (if there is one).
-- Notifies this entity's company of the deployment's completion.
-- May display reports.
-- This method can only be called by an agent.
-- @return Boolean, whether or not the activation of the drone has been successfully completed.
integration.startedActivateDrone = function()
    if not myself.Deployed and meKnowledge:isDeployed() and myself.droneAvailable then
        myself.Deployed = true
        integration.setUAVDeployed( myself.droneAvailable, true ) -- mandatory to permit the flight
        local droneKn = CreateKnowledge( integration.ontology.types.agent, myself.droneAvailable )
        reportFunction( eRC_FinMiseEnOeuvreDrone )
        meKnowledge:sendRC( droneKn, eRC_DroneDisponible )
        meKnowledge:sendDisponibleDrone( meKnowledge:getAutomat(), droneKn )
        return true
    elseif myself.droneAvailable == nil then
        return false
    end
end

--- Finds an available drone in the company of this entity, and sets it in the myself.droneAvailable variable.
-- A drone is deemed available if it is operational, if it has enough fuel,
-- and if it is at less than 80 meters away from this entity.
-- This method can only be called by an agent (e.g. a UAV station).
-- @see integration.startActivateDrone
integration.setAvailableDrones = function()
    myself.droneAvailable  = nil
    local integration = integration
    local listePions = integration.getAgentsWithoutHQ()
    for _,pion in pairs( listePions or emptyTable ) do
        local operationalLevel = pion:DEC_Agent_EtatOpsMajeur() * 100
        local fuelDotationNumber = DEC_Agent_GetFuelDotationNumber( pion )	
        -- if DEC_GetSzName( pion ) == "Masalife.RENS.Drone SDTI" and operationalLevel ~= 0 and fuelDotationNumber > 0 then
        if operationalLevel ~= 0 and fuelDotationNumber > 3 then -- Le drone doit être opérationnel et avoir un minimum de carburant
            if DEC_Geometrie_DistanceBetweenPoints( DEC_Agent_Position(), DEC_Agent_PositionPtr(pion) ) < 80 and not pion:GetbMiseEnOeuvre_() then
                integration.setUAVDeployed( pion, true ) -- mandatory to permit the flight
                integration.removeFromLoadedUnits( pion )
                integration.removeFromCapturedUnits( pion )
                DEC_Transport_DebarquerPionSansDelais( pion )
                myself.droneAvailable = pion
                break
            end
        end
    end
end

--- Returns true if this company has an available drone, false otherwise.
-- This method can only be called by a company.
-- A drone is deemed available if it is operational and if it is not already deployed.
-- @return Boolean
integration.companyHasAvailableDrones = function()
    local integration = integration
    local listePions = integration.getUnitsWithoutHQCommunication()
    for _, pion in pairs( listePions or emptyTable ) do
        local operationalLevel = pion:DEC_Agent_EtatOpsMajeur() * 100
        if operationalLevel ~= 0 and not integration.isUAVDeployed( pion ) then
            return true
        end
    end
    return false
end

--- Undeploys this entity.
-- This method can only be called by an agent.
-- @param self Deprecated, unused
-- @param alreadyUnDeployed Boolean, this method does not undeploy the drone if set to true
integration.stopActivateDrone = function( self, alreadyUnDeployed )
    if not alreadyUnDeployed then
        DEC_Agent_Undeploye()
    end
end

-- -------------------------------------------------------------------------------- 
-- The unit is moving
-- @author MIA
-- @release 2011-05-13
-- --------------------------------------------------------------------------------
integration.bodyIsMoving = function( self )
    return self.source:DEC_Agent_EstEnMouvement()
end
integration.activateInjuredSorting = function( self )
    DEC_Sante_ActiverFonctionTri()
end

integration.desactivateInjuredSorting = function( self )
    DEC_Sante_DesactiverFonctionTri()
end

integration.activateLogisticChains = function( self )
    if integration.isLogisticTypeUnit() then
        DEC_Sante_ActiverChaine()
        DEC_Maintenance_ActiverChaine()
        DEC_Ravitaillement_ActiverChaine()
        DEC_Sante_ActiverFonctionSoin()
        return true
    else
        return false
    end
end

integration.desactivateLogisticChains = function( self )
    if integration.isLogisticTypeUnit() then
        DEC_Sante_DesactiverChaine()
        DEC_Maintenance_DesactiverChaine()
        DEC_Ravitaillement_DesactiverChaine()
        DEC_Sante_DesactiverFonctionSoin()
        return true
    else
        return false
    end
end

-- -------------------------------------------------------------------------------- 
-- Activate injured people treatment
-- @author GGE
-- @release 2011-08-03
-- --------------------------------------------------------------------------------
integration.activateInjuredTreatment = function ( )
    DEC_Sante_ActiverFonctionSoin()
end

-- -------------------------------------------------------------------------------- 
-- Desactivate injured people treatment
-- @author GGE
-- @release 2011-08-03
-- --------------------------------------------------------------------------------
integration.desactivateInjuredTreatment = function ( )
    DEC_Sante_DesactiverFonctionSoin()
end

-- -------------------------------------------------------------------------------- 
-- Modify human wound priority
-- @author GGE
-- @release 2011-08-03
-- --------------------------------------------------------------------------------
integration.modifyHumanWoundPriority = function ( blessuresTraitees )
    if ( blessuresTraitees ~= nil ) then
        local List = {}
        for parameterName, parameterValue in pairs( blessuresTraitees ) do
            List[parameterName] = parameterValue.source
        end
        integration.changeHealthPriority( List )
    end
end

-- -------------------------------------------------------------------------------- 
-- Start Lend Tugs
-- @author GGE
-- @release 2011-10-05
-- --------------------------------------------------------------------------------
integration.StartLendTugs = function ( pionRenforce, pionARenforcer, nbrRemorqueurs  )
    DEC_StartPreterRemorqueurs( pionRenforce.source, pionARenforcer.source, nbrRemorqueurs )
end

-- -------------------------------------------------------------------------------- 
-- Start Get Tugs
-- @author GGE
-- @release 2011-10-05
-- --------------------------------------------------------------------------------
integration.StartGetTugs = function ( pionRenforce, nbrRemorqueurs )
    DEC_RecupererRemorqueurs( pionRenforce.source, nbrRemorqueurs )
end

-- -------------------------------------------------------------------------------- 
-- Start Lend VSRAM
-- @author GGE
-- @release 2011-10-05
-- --------------------------------------------------------------------------------
integration.StartLendVSRAM = function ( pionRenforce, pionARenforcer, nbrAmbulances )
    DEC_StartPreterVSRAM( pionRenforce.source, pionARenforcer.source, nbrAmbulances )
end
integration.LendAmbulances = function ( pionRenforce, pionARenforcer, nbrAmbulances )
    DEC_StartPreterVSRAM( pionRenforce, pionARenforcer, nbrAmbulances )
end

-- -------------------------------------------------------------------------------- 
-- Start Get VSRAM
-- @author GGE
-- @release 2011-10-05
-- --------------------------------------------------------------------------------
integration.StartGetVSRAM = function ( pionRenforce, nbrAmbulances )
    DEC_RecupererVSRAM( pionRenforce.source, nbrAmbulances )
end
integration.GetBackAmbulances = function ( pionRenforce, nbrAmbulances )
    DEC_RecupererVSRAM( pionRenforce, nbrAmbulances )
end

-- -------------------------------------------------------------------------------- 
-- Activate radar on area
-- @param area
-- @author PSN
-- @release 2012-07-12
-- -------------------------------------------------------------------------------- 
integration.activateRadar = function ( area )
    if not myself.radarActivated then
        myself.ecoute = integration.activeRadarOnLocalisation( eRadarType_Ecoute, area.source )
        myself.radar = integration.activeRadarOnLocalisation( eRadarType_Radar, area.source )
        myself.ecouteRadar = integration.activeRadarOnLocalisation( eRadarType_EcouteRadar, area.source )
        myself.zoneAEcouter = area.source
        myself.radarActivated = true

        reportFunction(eRC_DebutSurveillance )
        return true
    else
        return false
    end
end

-- -------------------------------------------------------------------------------- 
-- Deactivate radar on area
-- @param area
-- @author PSN
-- @release 2012-07-12
-- -------------------------------------------------------------------------------- 
integration.deactivateRadar = function ( area )
    if myself.radarActivated then
        integration.disableRadarOnLocalisation( eRadarType_Ecoute, myself.ecoute )
        integration.disableRadarOnLocalisation( eRadarType_Radar, myself.radar )
        integration.disableRadarOnLocalisation( eRadarType_EcouteRadar, myself.ecouteRadar )
        myself.radarActivated = false

        reportFunction(eRC_FinSurveillance )	
        return true
    else
        return false
    end
end

-- -------------------------------------------------------------------------------- 
-- Activate radar for indirect fire on area
-- @param area
-- @author LMT
-- @release 2012-06-13
-- -------------------------------------------------------------------------------- 
integration.startActivateRadarTirIndirect = function ( area )
    area[myself] = area[myself] or {}
    area[myself].actionRadar = DEC_Perception_ActiverPerceptionTirsIndirect( area.source )
    actionCallbacks[ area[myself].actionRadar ] = function( arg ) area[myself].actionRadar = arg end

    reportFunction(eRC_DebutSurveillance )
end
-- -------------------------------------------------------------------------------- 
-- Desactivate radar for indirect fire on area
-- @param area
-- @author LMT
-- @release 2012-06-13
-- -------------------------------------------------------------------------------- 
integration.stopActivateRadarTirIndirect = function ( area )
    area[myself] = area[myself] or {} 
    if area[myself].actionRadar == eActionObjetTerminee then
        reportFunction(eRC_FinSurveillance )
    end  
    if area[myself].actionRadar then
        area[myself].actionRadar = DEC_Perception_DesactiverPerceptionTirsIndirect( area[myself].actionRadar )
    end
end
-- -------------------------------------------------------------------------------- 
-- Return true if agent has tapping radar
-- @author DDA
-- @release 2011-12-10
-- -------------------------------------------------------------------------------- 
integration.hasRadar = function ( self, typeRadar )
    return DEC_Agent_ARadar( self.source, typeRadar )
end


integration.isNight = function( self )
    return DEC_Nuit()
end

integration.activateRecording = function( self )
    DEC_Perception_ActiverModeEnregistrement()
end
integration.deactivateRecording = function( self )
    DEC_Perception_DesactiverModeEnregistrement()
end
integration.activateSensors= function( self )
    DEC_Perception_ActiverSenseurs()
end
integration.deactivateSensors = function( self )
    DEC_Perception_DesactiverSenseurs()
end
integration.increaseNodeProduction = function( resourceNode, quantity )
    integration.increaseResourceNodeProduction( resourceNode, quantity )
    return true
end
integration.increaseResourceNodeProduction = function( resourceNode, quantity )
    DEC_ReseauRessourceAugmenteProduction( resourceNode.source, quantity )
end
integration.decreaseNodeProduction = function( resourceNode, quantity )
    integration.decreaseResourceNodeProduction( resourceNode, quantity )
    return true
end
integration.decreaseResourceNodeProduction = function( resourceNode, quantity )
    DEC_ReseauRessourceBaisseProduction( resourceNode.source, quantity )
end
integration.enable = function( resourceNode )
    reportFunction(eRC_ResourceNodeEnabled )
    integration.enableResourceNode( resourceNode.source ) 
    return true
end
integration.disable = function( resourceNode )
    reportFunction(eRC_ResourceNodeDisabled )
    integration.disableResourceNode( resourceNode.source ) 
    return true
end

integration.enableResourceNode = function( resourceNode )
    DEC_ReseauRessource_ActiverElement( resourceNode )
end

integration.disableResourceNode = function( resourceNode )
    DEC_ReseauRessource_DesactiverElement( resourceNode )
end

integration.isDead = function( self )
    return DEC_Agent_EstMort()
end

integration.selfDecontaminate = function( self )
    DEC_Agent_SeDecontaminer()
    return true
end

integration.changeHeight = function( height )
    myself.altitude = height
    DEC_Agent_HauteurDeVol( height )
end

integration.isFlying = function( self )
    return DEC_Agent_EstEnVol()
end

integration.startImmuniserNbc = function( self )
    DEC_Agent_ImmuniserNbc( )
end

integration.stopImmuniserNbc = function( self )
    DEC_Agent_StopImmuniserNbc( )
end

integration.forcerImmunisationNbc = function( self, immunize )
    DEC_Agent_ForcerImmunisationNbc( self.source, immunize )
end

integration.getUnitPC = function( self ) 
    return CreateKnowledge( integration.ontology.types.agent, DEC_Pion_PionPC() )
end

integration.isLogisticTypeUnit = function( platoon )
    if platoon == nil then
        platoon = myself
    end
    local typePion = DEC_Pion_GetMilPionType( platoon )
    if typePion == "Pion LOG TC2"
      or typePion == "Pion LOG BLD Sante"
      or typePion == "Pion LOG BLD Maintenance"
      or typePion == "Pion LOG BLD Ravitaillement"
      or typePion == "Pion LOG BLT Sante"
      or typePion == "Pion LOG BLT Maintenance"
      or typePion == "Pion LOG BLT Ravitaillement"
      or typePion == "Pion Organization" then
        return true
    else
        return false
    end
end

integration.isLogisticConvoy = function( platoon )
    local typePion = DEC_Pion_GetMilPionType( platoon )
    if typePion == "Pion LOG Convoi" then
        return true
    else
        return false
    end
end

integration.unitAltitude = function( entity )
    local nomPion = DEC_Pion_GetMilPionName( entity )
    local altitude = 1000
    if string.find (nomPion, "SDTI") then
        altitude = 1000
    elseif  string.find (nomPion, "DRAC") then
        altitude = 200
    end
    return altitude
end

integration.meKnowledgeIsTranported = function( self )
    return DEC_Agent_EstTransporte()
end

integration.activeRadarOnLocalisation = function( radarType, area )
    return DEC_Perception_ActiverRadarSurLocalisation( radarType, area )
end

integration.disableRadarOnLocalisation = function( radarType, radar )
    return DEC_Perception_DesactiverRadarSurLocalisation( radarType, radar )
end

integration.isAgentFlying = function( agent )
    return DEC_ConnaissanceAgent_EstEnVol( agent )
end

-- Returns the current speed of the provided agent in m/s
-- @param agent : an agent
integration.getCurrentSpeed = function( agent )
    return DEC_Agent_GetCurrentSpeed( agent )
end

-- Returns the maximal speed of the provided agent in m/s
-- @param agent : an agent knowledge
integration.getMaxSpeed = function( agent )
    return DEC_Agent_MaxSpeed( agent.source )
end

integration.getModulationMaxSpeed = function( agent )
    return DEC_GetModulationVitesseMax( agent )
end

integration.getAgentDirection = function()
    return DEC_Agent_Direction()
end

integration.getAgentsWithoutHQ = function()
    return DEC_Pion_PionsSansPC()
end

integration.getAgentsWithHQ = function()
    return DEC_Pion_PionsAvecPC()
end

--- Returns true if the given drone is exploited, false otherwise.
-- @param drone Simulation agent
-- @return Boolean
integration.isUAVExploited = function( drone )
    return drone:GetbEnExploitation_()
end

--- Returns true if the given drone is deployed, false otherwise.
-- @param drone Simulation agent
-- @return Boolean
integration.isUAVDeployed = function( drone )
    return drone:GetbMiseEnOeuvre_()
end

--- Set the given drone's current deployment state to the provided boolean.
-- @param drone Simulation agent
-- @param boolean Boolean, the new value
integration.setUAVDeployed = function( drone, boolean )
    drone:SetbMiseEnOeuvre_( boolean )
end

-- -------------------------------------------------------------------------------- 
-- Activate special sensors on area
-- --------------------------------------------------------------------------------  
integration.activateSpecialSensors = function( area, eType )
    area[ myself ] = area[ myself ] or {}
    area[ myself ].actionRadar = DEC_Perception_ActiverRadarSurLocalisation( eType, area.source )
    reportFunction(eRC_DebutSurveillance )
    return true
end

-- -------------------------------------------------------------------------------- 
-- deactivate special sensors on area
-- -------------------------------------------------------------------------------- 
integration.deactivateSpecialSensors = function( area, eType )
    if area[ myself ].actionRadar then
        area[ myself ].actionRadar = DEC_Perception_DesactiverRadarSurLocalisation( eType, area[ myself ].actionRadar )
        reportFunction(eRC_FinSurveillance )
    end
end

-- -------------------------------------------------------------------------------- 
-- activate special sensors on toxic CBRN plume
-- --------------------------------------------------------------------------------  
integration.activateSpecialSensorForCBRN = function ()
    DEC_Perception_ActiverSenseursSurDecision()
end

-- -------------------------------------------------------------------------------- 
-- deactivate special sensors on toxic CBRN plume
-- -------------------------------------------------------------------------------- 
integration.deactivateSpecialSensorForCBRN = function ()
    DEC_Perception_DesactiverSenseursSurDecision()
end

-- -------------------------------------------------------------------------------- 
-- colliding with a toxic plume
-- -------------------------------------------------------------------------------- 
integration.isToxicPlumeDetected = function( agent )
    local objects = integration.getToxicPlumeDetected( agent )
    return #objects > 0
end

-- -------------------------------------------------------------------------------- 
-- returning the toxic plume
-- -------------------------------------------------------------------------------- 
integration.getToxicPlumeDetected = function( agent )
    local objects = {}
    objects = DEC_Connaissances_CollisionsDesastres()
    return objects
end

------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

--- Deprecated
integration.stopActivateDrone