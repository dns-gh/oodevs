--- Returns true if this entity has the given resource, false otherwise.
-- This method can only be called by an agent.
-- @param resource Resource type
-- @return Boolean
integration.hasDotation = function( resource )
    return _DEC_HasDotation( myself, resource )
end

--- Returns true if this entity has at least the given quantity of
-- the provided resource, false otherwise.
-- This method can only be called by an agent.
-- @param resource Resource type
-- @param quantity Integer, the quantity of needed resource
-- @return Boolean
integration.hasDotationForFiring = function( resource, quantity )
 return DEC_HasDotationForFiring( myself, resource, quantity )
end

--- Returns true if this entity can use the given resource, false otherwise.
-- This method can only be called by an agent.
-- @param resource Resource type
-- @return Boolean
integration.canUseDotation = function( resource )
    return _DEC_CanUseDotation( myself, resource )
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
    return _DEC_Agent_EstEmpoisonne( myself )
end

--- Returns true if this entity is an agent of type "NBC troop", false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.isAgentNBC = function()
    return _DEC_Agent_EstAgentNBC( myself )
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
        reportOnceFunction( eRC_TenueProtectionNiveauNRBC, RNBCLevel )
        _DEC_Agent_MettreTenueProtectionNBC( myself )
        meKnowledge.equipOutfit = true
        return true
    else
        reportOnceFunction( eRC_PasTenueNBC )
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
        reportOnceFunction( eRC_TenueProtectionNBCEnlevee )
        _DEC_Agent_EnleverTenueProtectionNBC( myself )
        meKnowledge.equipOutfit = false
    else
        reportOnceFunction( eRC_PasTenueNBC )
    end
end

--- Returns the NBC protection level of this entity, as defined in the physical database.
-- This method can only be called by an agent.
-- @return Integer, the NBC protection level from 0 to 5, 0 meaning there is no
-- protection available for this entity.
integration.getRNBCProtectionLevel = function()
    meKnowledge.RNBCProtectionLevel = meKnowledge.RNBCProtectionLevel or _DEC_Agent_NiveauProtectionNBC( myself )
    return meKnowledge.RNBCProtectionLevel
end

--- Alerts the provided agent to NBC level four.
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
    _DEC_Agent_MettreTenueProtectionNBC( myself )
end

--- Makes this entity unequip its NBC outfit and 
--- decreases its potential NBC alert level to 0.
-- Displays reports.
-- This method can only be called by an agent.
-- @see integration.goOnNBCAlert
integration.unequipNBCOutfit = function()
    reportFunction( eRC_TenueProtectionNBCEnlevee )
    if myself.NBCAlert == true then
        reportFunction( eRC_FinAlerteNBCNiveau4 )
        F_Pion_SeteEtatNbc( myself, eEtatNbc_Niv0 )
        myself.NBCAlert = false
    end
    _DEC_Agent_EnleverTenueProtectionNBC( myself )
end

--- Switches off the radio communications of the given agent.
-- Displays a report.
-- @param agent Directia agent
integration.switchOffRadio = function( agent )
    reportFunction( eRC_DebutSilenceRadio )
    F_Pion_SeteEtatRadio( agent.source, eEtatRadio_Silence )
    _DEC_Agent_PasserEnSilenceRadio( myself )
end

--- Switches off the outgoing radio emissions of the given agent.
-- Displays a report.
-- @param agent Directia agent
integration.switchEmitOnlyOffRadio = function( agent )
    reportFunction( eRC_DebutSilencePartielRadio )
    F_Pion_SeteEtatRadio( agent.source, eEtatRadio_Silence_Partiel )
    _DEC_Agent_PasserEnSilenceRadioPartiel( myself, true )
end

--- Switches on the radio communications of the given agent.
-- Displays a report.
-- @param agent Directia agent
integration.switchOnRadio = function( agent )
    reportFunction( eRC_FinSilenceRadio )
    F_Pion_SeteEtatRadio( agent.source, eEtatRadio_Ouverte )
    _DEC_Agent_ArreterSilenceRadio( myself )
end

integration.switchEmitOnly = function( self )
    F_Pion_SeteEtatRadio( self.source, eEtatRadio_Silence_Partiel )
    _DEC_Agent_PasserEnSilenceRadioPartiel( myself, false )
end

integration.disableEmitOnly = function( self )
    F_Pion_SeteEtatRadio( self.source, eEtatRadio_Ouverte )
    _DEC_Agent_ArreterSilenceRadio( myself )
end

integration.enabledSharedPerception = function( supportedUnit )
    _DEC_EnableSharedPerception( myself, supportedUnit.source )
end

integration.disabledSharedPerception = function( supportedUnit )
    _DEC_DisabledSharedPerception( myself, supportedUnit.source )
end

integration.enableSharingKnowledges = function( unit )
    _DEC_EnableSharingKnowledges( myself, unit.source )
end

integration.disabledSharingKnowledges = function( unit )
    _DEC_DisabledSharingKnowledges( myself, unit.source )
end

integration.enabledSharedPerceptionWithKnowledge = function( supportedUnit )
    DEC_EnableSharedPerceptionWithKnowledge( myself, supportedUnit.source )
end

integration.disableSharedPerceptionWithKnowledge = function( supportedUnit )
    DEC_DisabledSharedPerceptionWithKnowledge( myself, supportedUnit.source )
end

integration.enableSharingKnowledgesWithKnowledge = function( unit )
    DEC_EnableSharingKnowledgesWithKnowledge( myself, unit.source )
end

integration.disabledSharingKnowledgesKnowledge = function( unit )
    DEC_DisabledSharingKnowledgesWithKnowledge( myself, unit.source )
end

integration.agentKnowledgeSharedPerception = function( self )
    local agent = DEC_ConnaissanceAgent_EnAgent( self.source)
    DEC_Knowledge_EnableSharedPerceptionWithKnowledge( agent, myself )
end

integration.agentKnowledgeStopSharedPerceptionWithKnowledge = function( self )
    local agent = DEC_ConnaissanceAgent_EnAgent( self.source)
    DEC_Knowledge_DisabledSharedPerceptionWithKnowledge( agent, myself )
end

integration.communicateWithKnowledge = function( supportedUnit )
   return DEC_Knowledge_CommunicateWithKnowledgeGroup( myself, supportedUnit.source )
end

integration.isPartialRadioState = function( self )
    return eEtatRadio_Silence_Partiel == integration.getRadioState( self )
end

integration.getRadioState = function( self )
    return F_Pion_GeteEtatRadio( myself )
end

--- Disables radar silence for the given agent.
-- Displays a report.
-- @param agent Directia agent
integration.switchOffRadar = function( agent )
    reportFunction( eRC_DebutSilenceRadar )
    F_Pion_SeteEtatRadar( agent.source, eEtatRadar_Silence )
    _DEC_Perception_DesactiverRadar( myself, eRadarType_Radar )
    _DEC_Perception_DesactiverRadar( myself, eRadarType_Ecoute )
    _DEC_Perception_DesactiverRadar( myself, eRadarType_EcouteRadar )
end

--- Enables radar silence for the given agent.
-- Displays a report.
-- @param agent Directia agent
integration.switchOnRadar = function( agent )
    reportFunction( eRC_FinSilenceRadar )
    F_Pion_SeteEtatRadar( agent.source, eEtatRadar_Ouvert )
    _DEC_Perception_ActiverRadar( myself, eRadarType_Radar )
    _DEC_Perception_ActiverRadar( myself, eRadarType_Ecoute )
    _DEC_Perception_ActiverRadar( myself, eRadarType_EcouteRadar )
end

--- Switches this entity's safety attitude on, in order for it to take cover,
--- move slower, fly lower (if it is a flying agent), activate sensors, etc.
-- May display reports.
-- This method can only be called by an agent.
-- @see integration.switchOffSafetyMode
-- @see integration.setMovementPace
integration.switchOnSafetyMode = function()
    _DEC_Perception_ActiverCoupsDeSonde( myself )
    _DEC_Agent_ChangerAmbianceEnSurete( myself, true )
    if integration.isFlying() then
        local tacticalFlyingHeight = DEC_Agent_GetTacticalFlyingHeight( myself ) 
        if tacticalFlyingHeight > 0 then
            _DEC_Agent_HauteurDeVol( myself, tacticalFlyingHeight )
        else
            if _DEC_Agent_EstEnVol( myself ) and myself.altitude then
                _DEC_Agent_HauteurDeVol( myself, myself.altitude * 0.2 )
            end
        end
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
    _DEC_Perception_DesactiverCoupsDeSonde( myself )
    _DEC_Agent_ChangerAmbianceEnSurete( myself, false )
    if integration.isFlying() then
        _DEC_Agent_HauteurDeVol( myself, DEC_Agent_GetStandardFlyingHeight( myself ) )
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
    _DEC_Perception_DesactiverCoupsDeSonde( myself )
    _DEC_Agent_ChangerAmbianceEnSurete( myself, false )
    reportFunction(eRC_CouvertureDesactive )
	myself.speedModulation = myself.speedModulation or {}
	myself.speedModulation.switchOnCoverMode = 1 --scipio
    myself.coverMode = false -- ww base
end

--- Returns true if this entity can be fully loaded, false otherwise.
-- For this method to return true, all the units' equipments must be tagged as loadable in the physical database.
-- This method can be called by an agent or company.
-- @param agent DirectIA agent
-- @return Boolean
integration.canBeFullyLoaded = function( agent )
    return DEC_Agent_EstCompletementEmbarquable( agent.source )
end

--- Returns true if this entity can dismount, false otherwise.
-- For this method to return true, the unit must have equipments that can load
-- other equipments (typically vehicles carrying infantry).
-- This method can only be called by an agent.
-- @see integration.canMount
-- @return Boolean
integration.canDismount = function()
    return _DEC_Agent_EstEmbarquable( myself )
end

--- Returns true if this entity can mount, false otherwise.
-- For this method to return true, the following conditions must be met :
-- <ul> <li> The unit must have equipments that can load other equipments (typically vehicles carrying infantry) </li>
-- <li> The unit is not located on a non-trafficable urban position </li>
-- <li> The unit is not moving underground </li> </ul>
-- This method can only be called by an agent.
-- @see integration.canDismount
-- @return Boolean
integration.canMount = function()
    return _DEC_Agent_EstEmbarquable( myself ) and DEC_CanMount( myself )
end

--- Returns true if the given agent is underground, false otherwise.
-- @param agent Simulation agent
-- @return Boolean
integration.isUnderground = function( agent )
    return DEC_Agent_IsUnderground( agent )
end

--- Returns true if this entity should dismount, i.e. if it can dismount
--- and if it is located on a non-trafficable urban position, false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.shouldDismount = function()
    return _DEC_Agent_EstEmbarquable( myself ) and not _DEC_Agent_EstDebarque( myself )
           and not DEC_IsPointInUrbanBlockTrafficableForPlatoon( myself, _DEC_Agent_Position( myself ) )
end

--- Makes this entity start mounting if it is supposed to (this entity is located
--- on a trafficable position) and if it can (the entity is dismounted and 
--- no mounting action has been started by the potential movement of this entity
--- (i.e. by a call to the integration.updateMoveToIt method)).
--- Otherwise, if this entity cannot or should not mount (or has finished mounting),
--- this method stops the potential current mounting action (started by a previous
--- call of this method).
--- Calling this method prevents any mounting action from being started by
--- the integration.updateMoveToIt method.
-- This method can only be called by an agent.
-- @see integration.updateMoveToIt
-- @see integration.stopMount
-- @return Boolean, false if the action is ongoing, true otherwise.
integration.mountStarted = function()
    myself.stayDismounted = false
    if( myself.mount == nil and myself.dismount == nil ) -- means that movement use mount or dismount action
      and not _DEC_Agent_EstEmbarque( myself ) 
      and not myself.enteringNonTrafficableElement
      and DEC_IsPointInUrbanBlockTrafficableForPlatoon( myself, _DEC_Agent_Position( myself ) ) then
        myself.orderMount = myself.orderMount or _DEC_StartEmbarquement( myself )
        return false
    else
        if myself.orderMount then
            _DEC__StopAction( myself, myself.orderMount )
            myself.orderMount = nil
        end
    end
    return true
end

--- Makes this entity start dismounting if it can (the entity is mounted and 
--- no dismounting action has been started by the potential movement of this entity
--- (i.e. by a call to the integration.updateMoveToIt method)).
--- Otherwise, if this entity cannot dismount (or has finished dismounting),
--- this method stops the potential current dismounting action (started by a previous
--- call to this method).
-- This method can only be called by an agent.
-- @see integration.updateMoveToIt
-- @see integration.stopDismount
-- @return Boolean, false if the action is ongoing, true otherwise.
integration.dismountStarted = function()
    myself.stayDismounted = true
    if( myself.mount == nil and myself.dismount == nil ) -- means that movement use mount or dismount action
        and not _DEC_Agent_EstDebarque( myself ) then
        myself.orderDismount = myself.orderDismount or _DEC_StartDebarquement( myself )
        return false
    else -- movement, manage
        if myself.orderDismount then
            _DEC__StopAction( myself, myself.orderDismount )
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
        _DEC__StopAction( myself, myself.orderDismount )
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
        _DEC__StopAction( myself, myself.orderMount )
        myself.orderMount = nil
    end
end

--- Returns true if this entity is mounted, false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.isMounted = function()
    return _DEC_Agent_EstEmbarque( myself )
end

--- Returns true if this entity is dismounted, false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.isDismounted = function()
    return _DEC_Agent_EstDebarque( myself )
end

--- Deploys this entity for it to complete its positioning.
-- The deployment duration is defined in the physical database, 
-- in the "Deployment" section of the "Unit" tab.
-- @see integration.undeploy
-- This method can only be called by an agent.
integration.startDeploy = function()
    _DEC_Agent_Deploy( myself )
end

--- Undeploys this entity.
-- The un-deployment duration is defined in the physical database, 
-- in the "Deployment" section of the "Unit" tab.
-- @see integration.startDeploy
-- This method can only be called by an agent.
integration.undeploy = function()
    _DEC_Agent_Undeploye( myself )
end

--- Returns true if this entity is deployed, false otherwise.
-- This method can only be called by an agent.
integration.isDeployed = function()
    return _DEC_Agent_IsDeployed( myself )
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
		    _DEC_Agent_Deploy( myself )
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
-- and if it is close enough to this entity.
-- This method can only be called by an agent (e.g. a UAV station).
-- @param minFuelQuantity Integer, the minimum quantity of fuel that the drone
-- must have in order to be considered available (3 units by default).
-- @param maxDistance Float, the maximal distance between this entity and the drone for
-- the latter to be considered available (in meters, 80 by default).
-- @see integration.startActivateDrone
integration.setAvailableDrones = function( minFuelQuantity, maxDistance )
    minFuelQuantity = minFuelQuantity or 3
    maxDistance = maxDistance or 80
    myself.droneAvailable  = nil
    local integration = integration
    local listePions = integration.getAgentsWithoutHQ()
    for _,pion in pairs( listePions or emptyTable ) do
        local operationalLevel = pion:DEC_Agent_EtatOpsMajeur() * 100
        local fuelDotationNumber = DEC_Agent_GetFuelDotationNumber( pion )	
        if operationalLevel ~= 0 and fuelDotationNumber > minFuelQuantity then -- The drone should be operational
            if DEC_Geometrie_DistanceBetweenPoints( _DEC_Agent_Position( myself ), DEC_Agent_PositionPtr(pion) ) < maxDistance and not pion:GetbMiseEnOeuvre_() then
                integration.setUAVDeployed( pion, true ) -- mandatory to permit the flight
                integration.removeFromLoadedUnits( pion )
                integration.removeFromCapturedUnits( pion )
                _DEC_Transport_DebarquerPionSansDelais( myself, pion )
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
    for i = 1, #( listePions or emptyTable ) do
        local pion = listePions[i]
        if pion:DEC_Agent_EtatOpsMajeur() ~= 0 and not integration.isUAVDeployed( pion ) then
            return true
        end
    end
    return false
end

--- Returns the number of non-deployed and operational units in this company
-- (except the command post) who can do the given task.
-- This method can only be called by a company.
-- @param droneTask String, the name of the task
-- @return Integer
integration.numberOfAvailableDrones = function( droneTask )
    local integration = integration
    local listePions = integration.getUnitsWithoutHQCommunication()
    local availableDroneCount = 0
    for i = 1, #( listePions or emptyTable ) do
        local pion = listePions[i]
        if pion:DEC_Agent_EtatOpsMajeur() ~= 0 and not integration.isUAVDeployed( pion ) and DEC_IsMissionAvailable( pion, droneTask ) then
            availableDroneCount = availableDroneCount + 1
        end
    end
    return availableDroneCount
end

--- Returns time in minutes to cover a distance in a straight line
-- The theorical maximum speed is used to compute time
-- @param agent Simulation agent
-- @param distance Float the distance in meter to cover 
-- @param loaded Boolean, whether or not the agent is embarked
-- @return Float, time to cover the distance
integration.getTheoricTimeToCoverDistance = function( agent, distance, loaded )
    return DEC_PionTempsTheoriquePourParcourirDistanceEnLigneDroite( agent, distance, loaded )
end

--- Returns time in minutes to cover a distance in a straight line
-- The theorical maximum speed is used to compute time
-- @param agent Simulation agent
-- @param distance Float the distance in meter to cover 
-- @param loaded Boolean, whether or not the agent is embarked
-- @return Boolean, true if the unit can cover the fly track, false otherwise
integration.checkFlyTrack = function ( distance, agent )
    if agent then
        local unitPC = meKnowledge:getUnitPC()
        if unitPC.source.pluggedUnits and unitPC.source.pluggedUnits[ agent ] then
            return false
        end
        local tempsDeParcours = integration.getTheoricTimeToCoverDistance( agent, distance, true )
        local autonomy = DEC_AutonomieEnDeplacement( agent )
        if autonomy < tempsDeParcours then
            return false
        end
        return true
    else
        local tempsDeParcours = _DEC_Agent_TempsPourParcourirDistanceEnLigneDroite( myself, distance )
        local autonomy = _DEC_Agent_AutonomieEnDeplacement( myself )
        if autonomy < tempsDeParcours then
            return false
        end
        return true
    end
end

--- Returns time in minutes to cover a distance in a straight line
-- The theorical maximum speed is used to compute time
-- @param distance Float the distance in meter to cover 
-- @return Float, time to cover the distance
integration.getTimeToFlyOverDistance = function( distance )
    return _DEC_Agent_TempsPourParcourirDistanceEnLigneDroite( myself, distance )
end

--- Returns the autonomy in minutes for an agent
-- The time is computed on the assumption that the agent is moving
-- @return Float, the time in minutes
integration.movingAutonomy = function( )
    return _DEC_Agent_AutonomieEnDeplacement( myself )
end

--- Returns true if this agent is moving, false otherwise.
-- @param agent Directia agent
-- @return Boolean
integration.AgentIsMoving = function( agent )
    return agent.source:DEC_Agent_EstEnMouvement()
end

--- Makes the given supporting unit lend tugs to the given supported unit.
-- @see integration.StartGetTugs
-- @param supportingUnit Directia agent, the agent lending tugs
-- @param supportedUnit Directia agent, the agent to lend tugs to.
-- @param nbrTugs Integer, the number of tugs to lend.
-- This method can only be called by an agent.
integration.StartLendTugs = function ( supportingUnit, supportedUnit, nbrTugs )
    _DEC_StartPreterRemorqueurs( myself, supportingUnit.source, supportedUnit.source, nbrTugs )
end

--- Makes the given supported unit give back the tugs previously lent to it by this entity.
-- @see integration.StartLendTugs
-- @param supportedUnit Directia agent, the agent giving back the tugs.
-- @param nbrTugs Integer, the number of tugs to retrieve.
-- This method can only be called by an agent.
integration.StartGetTugs = function ( supportedUnit, nbrTugs )
    _DEC_RecupererRemorqueurs( myself, supportedUnit.source, nbrTugs )
end

--- Makes the given supporting unit lend ambulances to the given supported unit.
-- @see integration.GetBackAmbulances
-- @param supportingUnit Simulation agent, the agent lending ambulances
-- @param supportedUnit Simulation agent, the agent to lend ambulances to.
-- @param nbrAmbulances Integer, the number of ambulances to lend.
-- This method can only be called by an agent.
integration.LendAmbulances = function ( supportingUnit, supportedUnit, nbrAmbulances )
    _DEC_StartPreterVSRAM( myself, supportingUnit, supportedUnit, nbrAmbulances )
end

--- Makes the given supported unit give back the ambulances previously lent to it by this entity.
-- @see integration.LendAmbulances
-- @param supportedUnit Simulation agent, the agent giving back the ambulances.
-- @param nbrAmbulances Integer, the number of ambulances to retrieve.
-- This method can only be called by an agent.
integration.GetBackAmbulances = function ( supportedUnit, nbrAmbulances )
    _DEC_RecupererVSRAM( myself, supportedUnit, nbrAmbulances )
end

--- Enables all types of radar detection on the given area.
-- May display a report.
-- This method does nothing if radar detection was already enabled by a previous
-- call to this method without having been disabled with a call to the integration.deactivateRadar method.
-- This method can only be called by an agent.
-- @see integration.deactivateRadar
-- @see integration.activeRadarOnLocalisation
-- @param area Area knowledge
-- @return Boolean, whether or not the call to this method has activated radar detection.
integration.activateRadar = function ( area )
    if not myself.radarActivated then
        myself.ecoute = integration.activeRadarOnLocalisation( eRadarType_Ecoute, area.source )
        myself.radar = integration.activeRadarOnLocalisation( eRadarType_Radar, area.source )
        myself.ecouteRadar = integration.activeRadarOnLocalisation( eRadarType_EcouteRadar, area.source )
        myself.zoneAEcouter = area.source
        myself.radarActivated = true

        reportFunction( eRC_DebutSurveillance )
        return true
    else
        return false
    end
end

--- Disables all types of radar detection on the given area.
-- May display a report.
-- This method does nothing if radar detection was not previously enabled by
-- a call to the integration.activateRadar method.
-- This method can only be called by an agent.
-- @see integration.activateRadar
-- @see integration.disableRadarOnLocalisation
-- @param area Area knowledge
-- @return Boolean, whether or not the call to this method has disabled radar detection.
integration.deactivateRadar = function ( area )
    if myself.radarActivated then
        integration.disableRadarOnLocalisation( eRadarType_Ecoute, myself.ecoute )
        integration.disableRadarOnLocalisation( eRadarType_Radar, myself.radar )
        integration.disableRadarOnLocalisation( eRadarType_EcouteRadar, myself.ecouteRadar )
        myself.radarActivated = false

        reportFunction( eRC_FinSurveillance )	
        return true
    else
        return false
    end
end

--- Starts the detection of indirect fire in the given area.
-- Displays a report.
-- This method can only be called by an agent.
-- @see integration.stopActivateRadarTirIndirect
-- @param area Area knowledge
integration.startActivateRadarTirIndirect = function ( area )
    area[myself] = area[myself] or {}
    area[myself].actionRadar = _DEC_Perception_ActiverPerceptionTirsIndirect( myself, area.source )
    actionCallbacks[ area[myself].actionRadar ] = function( arg ) area[myself].actionRadar = arg end

    reportFunction( eRC_DebutSurveillance )
end

--- Stops the detection of indirect fire in the given area.
-- May display a report.
-- This method can only be called by an agent.
-- @see integration.startActivateRadarTirIndirect
-- @param area Area knowledge 
integration.stopActivateRadarTirIndirect = function ( area )
    area[myself] = area[myself] or {} 
    if area[myself].actionRadar == eActionObjetTerminee then
        reportFunction( eRC_FinSurveillance )
    end  
    if area[myself].actionRadar then
        area[myself].actionRadar = _DEC_Perception_DesactiverPerceptionTirsIndirect( myself, area[myself].actionRadar )
    end
end

--- Returns true if the given agent has a radar of the given radar type, false otherwise.
-- @param agent Directia agent
-- @param typeRadar Integer, the type of radar among one of the following : 
-- <ul> <li> eRadarType_Radar (radar) </li>
-- <li> eRadarType_Ecoute (listening) </li>
-- <li> eRadarType_EcouteRadar (radar listening) </li> </ul>
-- @return Boolean
integration.hasRadar = function ( agent, typeRadar )
    return DEC_Agent_ARadar( agent.source, typeRadar )
end

--- Returns true if it is night, false otherwise.
-- @return Boolean
integration.isNight = function()
    return DEC_Nuit()
end

--- Activates the recording mode for this entity. Entities perceived by this entity will not
--- be transmitted to its knowledge group until integration.deactivateRecording has been called.
-- @see integration.deactivateRecording
-- This method can only be called by an agent.
integration.activateRecording = function()
    _DEC_Perception_ActiverModeEnregistrement( myself )
end

--- Deactivates the recording mode for this entity.
-- Sends all perceptions acquired during record mode to the entity's knowledge group.
-- @see integration.activateRecording
-- This method can only be called by an agent.
integration.deactivateRecording = function()
    _DEC_Perception_DesactiverModeEnregistrement( myself )
end

--- Activates this entity's sensors (but neither its special sensors nor sensors set as 'activated on request').
-- @see integration.deactivateSensors
-- This method can only be called by an agent.
integration.activateSensors = function()
    _DEC_Perception_ActiverSenseurs( myself, true )
end

--- Deactivates this entity's sensors (but neither its special sensors nor sensors set as 'activated on request').
-- @see integration.activateSensors
-- This method can only be called by an agent.
integration.deactivateSensors = function()
    _DEC_Perception_ActiverSenseurs( myself, false )
end

--- Increases the production of the given resource node with the provided quantity.
-- @param resourceNode Resource node knowledge
-- @param quantity Integer, the quantity
-- @return Boolean, true
integration.increaseResourceNodeProduction = function( resourceNode, quantity )
    DEC_ReseauRessourceAugmenteProduction( resourceNode.source, quantity )
    return true
end

--- Decreases the production of the given resource node with the provided quantity.
-- @param resourceNode Resource node knowledge
-- @param quantity Integer, the quantity
-- @return Boolean, true
integration.decreaseResourceNodeProduction = function( resourceNode, quantity )
    DEC_ReseauRessourceBaisseProduction( resourceNode.source, quantity )
    return true
end

--- Enables the given resource node.
-- Displays a report.
-- @see integration.enableResourceNode
-- @param resourceNode Resource node knowledge
-- @return Boolean, true
integration.enable = function( resourceNode )
    reportFunction( eRC_ResourceNodeEnabled )
    integration.enableResourceNode( resourceNode.source ) 
    return true
end

--- Disables the given resource node.
-- Displays a report.
-- @see integration.disableResourceNode
-- @param resourceNode Resource node knowledge
-- @return Boolean, true
integration.disable = function( resourceNode )
    reportFunction( eRC_ResourceNodeDisabled )
    integration.disableResourceNode( resourceNode.source ) 
    return true
end

--- Enables the given resource node.
-- @param resourceNode Simulation resource node
integration.enableResourceNode = function( resourceNode )
    DEC_ReseauRessource_ActiverElement( resourceNode )
end

--- Disables the given resource node.
-- @param resourceNode Simulation resource node
integration.disableResourceNode = function( resourceNode )
    DEC_ReseauRessource_DesactiverElement( resourceNode )
end

--- Returns a list of all the resource nodes known by the given agent
-- in the provided area.
-- @param agent Simulation agent
-- @param area Simulation area
-- @return List of simulation resource nodes
integration.getResourceNetworkNodesInZone = function( agent, area )
    return DEC_ResourceNetwork_NodesInZone( agent, area )
end

--- Returns true if the caller of this method is dead, false otherwise.
-- This method can only be called by an agent or a crowd.
-- @return Boolean
integration.isDead = function()
    return _DEC_Agent_EstMort( myself )
end

--- Instantaneously decontaminates this entity.
-- This method can only be called by an NBC agent.
-- @return Boolean, true if the unit can self-decontaminate, false otherwise
integration.selfDecontaminate = function()
    _DEC_Agent_SeDecontaminer( myself )
    return true
end

--- Sets the flying height of this entity to the given height.
-- This method can only be called by an agent.
-- @param height Float, the new flying height (in meters)
integration.changeHeight = function( height )
    myself.altitude = height
    _DEC_Agent_HauteurDeVol( myself, height )
end

--- Returns the flying height of the caller entity.
-- @param agent Simulation agent
-- @return height Float, the flying height (in meters)
integration.getHeight = function( agent )
    return DEC_Agent_Height( agent )
end

--- Returns true if this entity is flying, false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.isFlying = function()
    return _DEC_Agent_EstEnVol( myself )
end

--- Returns true if the given agent knowledge is flying, false otherwise.
-- @param agent Simulation agent knowledge
-- @return Boolean
integration.isAgentFlying = function( agent )
    return DEC_ConnaissanceAgent_EstEnVol( agent )
end

--- Immunizes or stops immunizing the given agent.
-- @param agent Directia agent
-- @param immunize Whether the given agent should be immunized or should stop immunizing
integration.forcerImmunisationNbc = function( agent, immunize )
    DEC_Agent_ForcerImmunisationNbc( agent.source, immunize )
end

--- Returns true if the given agent is of a logistic type, false otherwise.
-- If no agent is given, this method can only be called by an agent, and
-- will instead return true if this entity is of a logistic type, false otherwise.
-- @param platoon Simulation agent (optional)
-- @return Boolean
integration.isLogisticTypeUnit = function( platoon )
    platoon = platoon or myself
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

--- Returns true if the given agent is a logistic convoy, false otherwise.
-- @param platoon Simulation agent
-- @return Boolean
integration.isLogisticConvoy = function( platoon )
    local typePion = DEC_Pion_GetMilPionType( platoon )
    if typePion == "Pion LOG Convoi" then
        return true
    else
        return false
    end
end

--- Returns true if this entity is transported, false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.meKnowledgeIsTransported = function()
    return DEC_Agent_EstTransporte( myself )
end

--- Enables a radar of the given radar type to detect activity in the given area.
-- This method can only be called by an agent.
-- @see integration.disableRadarOnLocalisation
-- @param radarType Integer, the type of radar among one of the following : 
-- <ul> <li> eRadarType_Radar (radar) </li>
-- <li> eRadarType_Ecoute (listening) </li>
-- <li> eRadarType_EcouteRadar (radar listening) </li> </ul>
-- @param area Simulation area
-- @return Integer, the id of the activated radar
integration.activeRadarOnLocalisation = function( radarType, area )
    return _DEC_Perception_ActiverRadarSurLocalisation( myself, radarType, area )
end

--- Disables the given radar of the given radar type, previously enabled
--- to detect activity in an area.
-- @see integration.activeRadarOnLocalisation
-- @param radarType Integer, the type of radar among one of the following : 
-- <ul> <li> eRadarType_Radar (radar) </li>
-- <li> eRadarType_Ecoute (listening) </li>
-- <li> eRadarType_EcouteRadar (radar listening) </li> </ul>
-- @param radar Integer, the id of the radar to disable.
integration.disableRadarOnLocalisation = function( radarType, radar )
    _DEC_Perception_DesactiverRadarSurLocalisation( myself, radarType, radar )
end

--- Returns the current speed of the provided agent in m/s
-- @param agent Simulation agent
integration.getCurrentSpeed = function( agent )
    return DEC_Agent_GetCurrentSpeed( agent )
end

--- Returns the maximal speed of the provided agent in m/s
-- @param agent Directia agent
integration.getMaxSpeed = function( agent )
    return DEC_Agent_MaxSpeed( agent.source )
end

--- Returns the direction this entity is facing.
-- This method can only be called by an agent.
-- @return Simulation direction
integration.getAgentDirection = function()
    return _DEC_Agent_Direction( myself )
end

--- Returns the list of all agents in the same company
--- as this entity except the HQ.
-- This method can only be called by an agent.
-- @return List of simulation agents
integration.getAgentsWithoutHQ = function()
    return _DEC_Pion_PionsSansPC( myself )
end

--- Returns the list of all agents in this company (if this method
--- is called by a company), or of all agents in the same company
--- as this entity (if this method is called by an agent).
-- This method can be called by an agent or by a company.
-- @return List of simulation agents
integration.getAgentsWithHQ = function()
    return myself:DEC_Pion_PionsAvecPC()
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

--- Enables a radar of the given radar type to detect activity in the given area.
-- Displays a report.
-- This method can only be called by an agent.
-- @see integration.deactivateSpecialSensors
-- @param radarType Integer, the type of radar among one of the following : 
-- <ul> <li> eRadarType_Radar (radar) </li>
-- <li> eRadarType_Ecoute (listening) </li>
-- <li> eRadarType_EcouteRadar (radar listening) </li> </ul>
-- @param area Area knowledge
-- @return Boolean, true
integration.activateSpecialSensors = function( area, eType )
    area[ myself ] = area[ myself ] or {}
    area[ myself ].actionRadar = _DEC_Perception_ActiverRadarSurLocalisation( myself, eType, area.source )
    reportFunction( eRC_DebutSurveillance )
    return true
end

--- Disables a radar of the given radar type to detect activity in the given area.
-- May display a report.
-- This method does nothing if radar detection was not previously enabled by
-- a call to the integration.activateSpecialSensors method.
-- This method can only be called by an agent.
-- @see integration.activateSpecialSensors
-- @param radarType Integer, the type of radar among one of the following : 
-- <ul> <li> eRadarType_Radar (radar) </li>
-- <li> eRadarType_Ecoute (listening) </li>
-- <li> eRadarType_EcouteRadar (radar listening) </li> </ul>
-- @param area Area knowledge
integration.deactivateSpecialSensors = function( area, eType )
    if area[ myself ] and area[ myself ].actionRadar then
        area[ myself ].actionRadar = _DEC_Perception_DesactiverRadarSurLocalisation( myself, eType, area[ myself ].actionRadar )
        reportFunction( eRC_FinSurveillance )
    end
end

--- Enables this entity's sensors set as 'activated on request' (see authoring tool).
-- This method can only be called by an agent.
-- @see integration.deactivateSensorsUponRequest
integration.activateSensorsUponRequest = function ()
    _DEC_Perception_ActiverSenseursSurDecision( myself )
end

--- Disables this entity's sensors set as 'activated on request' (see authoring tool).
-- This method can only be called by an agent.
-- @see integration.activateSensorsUponRequest
integration.deactivateSensorsUponRequest = function ()
    _DEC_Perception_DesactiverSenseursSurDecision( myself )
end

--- Returns true if a toxic plume is detected by this entity, false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.isToxicPlumeDetected = function()
    return #_DEC_Connaissances_CollisionsDesastres( myself ) > 0
end

--- Returns a list of all the toxic plumes detected by this entity.
-- This method can only be called by an agent.
-- @return List of object knowledges
integration.getToxicPlumeDetected = function()
    return _DEC_Connaissances_CollisionsDesastres( myself )
end

--- Makes the caller agent start to consume the given percentage of the given resource.
-- The consumption is done during the time defined by the 'duration' parameter.
-- This method can only be called by an agent.
-- @param resourceType Integer, id of the resource defined in the physical database
-- @param percentage Integer, the percentage of resource to consume.
-- Can be positive (consume) or negative (replenish)
-- @param duration Integer, the duration (in minutes) the agent will take to consume the percentage of resource
integration.startConsumeResource = function( resourceType, percentage, duration )
    myself.consumeAction = myself.consumeAction or {}
    myself.consumeAction[ resourceType ] = {}
    myself.consumeAction[ resourceType ].actionId = _DEC_StartConsumingResources( myself, resourceType, -percentage, duration * 60 )
    actionCallbacks[ myself.consumeAction[ resourceType ].actionId ] = function( arg )
        myself.consumeAction[ resourceType ].etatAction = arg
    end
end
--- Makes the caller agent check the consumption action feedback. 
-- @param resourceType Integer, id of the resource defined in the physical database
-- @param percentage Integer The percentage of resource to consume
-- Can be positive (consume) or negative (replenish)
-- @param duration Integer, the duration (in minutes) the agent will take to consume the percentage of resource
integration.updateConsumeResource = function( resourceType, percentage, duration )
    local etat = myself.consumeAction[ resourceType ].etatAction
    if etat == eActionFinished then
        _DEC__StopAction( myself, myself.consumeAction[ resourceType ].actionId )
        myself.consumeAction[ resourceType ] = nil
        return true
    elseif etat == eActionNotAllowed then
        _DEC_Trace( myself, "the agent cannot consume the given resource "..tostring( resourceType ) )
        return false
    else
        return false -- action is running
    end
end

------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

--- Deprecated, use integration.forcerImmunisationNbc instead (with the immunize parameter set to true)
integration.startImmuniserNbc = function( self )
    _DEC_Agent_ImmuniserNbc( myself )
end

--- Deprecated, use integration.forcerImmunisationNbc instead (with the immunize parameter set to false)
integration.stopImmuniserNbc = function( self )
    _DEC_Agent_StopImmuniserNbc( myself )
end

--- Deprecated, use integration.getKnowledgeHQ instead
integration.getUnitPC = function( self ) 
    return CreateKnowledge( integration.ontology.types.agent, DEC_Pion_PionPC() )
end

--- Deprecated
integration.getModulationMaxSpeed = function( agent )
    return DEC_GetModulationVitesseMax( agent )
end

--- Deprecated, use integration.undeploy instead
integration.stopActivateDrone = function( self, alreadyUnDeployed )
    if not alreadyUnDeployed then
        _DEC_Agent_Undeploye( myself )
    end
end

--- Deprecated, use integration.LendAmbulances instead
--- Makes the given supporting unit lend ambulances to the given supported unit.
-- @see integration.StartGetVSRAM
-- @param supportingUnit Directia agent, the agent lending ambulances
-- @param supportedUnit Directia agent, the agent to lend ambulances to.
-- @param nbrAmbulances Integer, the number of ambulances to lend.
-- This method can only be called by an agent.
integration.StartLendVSRAM = function ( supportingUnit, supportedUnit, nbrAmbulances )
    integration.LendAmbulances( supportingUnit.source, supportedUnit.source, nbrAmbulances )
end

--- Deprecated, use integration.GetBackAmbulances instead
--- Makes the given supported unit give back the ambulances previously lent to it by this entity.
-- @see integration.GetBackAmbulances
-- @param supportedUnit Directia agent, the agent giving back the ambulances.
-- @param nbrAmbulances Integer, the number of ambulances to retrieve.
-- This method can only be called by an agent.
integration.StartGetVSRAM = function ( supportedUnit, nbrAmbulances )
    integration.GetBackAmbulances( supportedUnit.source, nbrAmbulances )
end

--- Deprecated
integration.increaseNodeProduction = integration.increaseResourceNodeProduction

--- Deprecated
integration.decreaseNodeProduction = integration.decreaseResourceNodeProduction

--- Deprecated
-- @see integration.getStandardFlyingHeight
-- @see integration.getTacticalFlyingHeight
integration.unitAltitude = function( entity )
    local nomPion = DEC_Pion_GetMilPionName( entity )
    local altitude = 1000
    if string.find( nomPion, "DRAC" ) then
        altitude = 200
    end
    return altitude
end

--- Deprecated
integration.bodyIsMoving = integration.AgentIsMoving

--- Deprecated
integration.meKnowledgeIsTranported = integration.meKnowledgeIsTransported

--- Deprecated, use integration.startActivateDrone
integration.startDeployDrone = function ( self, distance, listPoints )
    integration.setAvailableDronesForFlyTrack( distance, listPoints )
    myself.Deployed = false
    if myself.droneAvailable == nil then
        reportFunction( eRC_PasDeDroneDisponible )
        if not meKnowledge:isSelfCommanding() then
            meKnowledge:sendNoDisponibleDrone( meKnowledge:getAutomat() )
        end
    else
        _DEC_Agent_Deploy( myself )
        reportFunction( eRC_DebutMiseEnOeuvreDrone, myself.droneAvailable )
    end
end

--- Deprecated, use integration.startedActivateDrone
integration.startedDeployDrone = function ( self, distance, listPoints )
    if not myself.Deployed and meKnowledge:isDeployed() and myself.droneAvailable then
        myself.Deployed = true
        myself.droneAvailable:SetbMiseEnOeuvre_( true ) -- mandatory to permit the flight
        local droneKn = CreateKnowledge( sword.military.world.PlatoonAlly, myself.droneAvailable )
        reportFunction( eRC_FinMiseEnOeuvreDrone, myself.droneAvailable )
        meKnowledge:sendRC( droneKn, eRC_DroneDisponible )
        meKnowledge:sendDisponibleDrone(meKnowledge:getAutomat(), droneKn)
    end
end

--- Deprecated, use integration.stopActivateDrone
integration.stopDeployDrone = function( self, distance, listPoints )
    _DEC_Agent_Undeploye( myself )
    return true
end

--- Deprecated, use integration.setAvailableDrones
integration.setAvailableDronesForFlyTrack = function ( distance, listPoints )
    myself.droneAvailable  = nil
    local listePions = _DEC_Pion_PionsSansPC( myself )
    local autonomie = false
    for _,pion in pairs( listePions or emptyTable ) do
        local operationalLevel = pion:DEC_Agent_EtatOpsMajeur() * 100
        local nomPion = DEC_Pion_GetMilPionName( pion )
        local possible = true
        if string.find (nomPion, "SDTI") then
            if DEC_Agent_GetFuelDotationNumber( pion ) <= 3 then
                possible = false
            end
        end
        if string.find (nomPion, "DRAC") then
            if string.find (nomPion, "PVP") then
                possible = false
            else
                possible = integration.checkFlyTrack( distance, pion )
            end
        end
        if possible then
            autonomie = true
        end	
        if operationalLevel ~= 0 and possible then
            if DEC_Geometrie_DistanceBetweenPoints( _DEC_Agent_Position( myself ), DEC_Agent_PositionPtr(pion) ) < 80 and  not pion:GetbMiseEnOeuvre_() then
                _DEC_Transport_DebarquerPionSansDelais( myself, pion )
                myself.droneAvailable = pion
                meKnowledge:sendFlyTrack( CreateKnowledge( sword.military.world.PlatoonAlly, pion ), listPoints )
                break
            end
        end
    end
    if not autonomie then
        reportFunction( eRC_AutonomyDrone )
    end
end

--- Returns if the provided agent is autonomous (i.e. unmanned vehicle)
-- @param agent Simulation agent
-- @return Boolean true if the given agent is autonomous, false otherwise.
integration.agentIsAutonomous = function( agent )
    return DEC_Agent_EstAutonome( agent )
end

--- Enables this entity's special sensors to detect CBRN toxic plume.
-- This method can only be called by an agent.
-- @see integration.deactivateSpecialSensorForCBRN
integration.activateSpecialSensorForCBRN = integration.activateSensorsUponRequest


--- Disables this entity's special sensors to detect CBRN toxic plume.
-- This method can only be called by an agent.
-- @see integration.activateSpecialSensorForCBRN
integration.deactivateSpecialSensorForCBRN = integration.deactivateSensorsUponRequest