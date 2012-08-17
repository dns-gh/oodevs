-------------------------------------------------------------------------------
-- Equipment Implementation : 
-- Regroup function to manage platoon equipement
-- @author MGD
-- @created 2010-04-27
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Math�matiques Appliqu�es SA (MASA)
-------------------------------------------------------------------------------

--- Indicate if the unit has the munition 
-- @param munition The id of a munition
-- @return bool
-- @author MGD
-- @created 2010-01-27
integration.hasDotation = function( munition )
    return DEC_HasDotation(munition)
end

integration.canUseDotation = function( munition )
    return DEC_CanUseDotation(munition)
end

integration.isContaminated = function( self )
    return self.source:DEC_Agent_EstContamine()
end

integration.isPoisoned = function()
    return DEC_Agent_EstEmpoisonne()
end

integration.isAgentNBC = function()
    return DEC_Agent_EstAgentNBC()
end

--- EquipNBC outfit
-- @author MGD
-- @release 2010-04-27
integration.equipNBCOutfitSecu = function()
    meKnowledge:sendReport( eRC_TenueProtectionNBCMise )
    DEC_Agent_MettreTenueProtectionNBC()
end

--- UnequipNBC outfit
-- @author MGD
-- @release 2010-04-27
integration.unequipNBCOutfitSecu = function()
    meKnowledge:sendReport( eRC_TenueProtectionNBCEnlevee )
    DEC_Agent_EnleverTenueProtectionNBC()
end

--- Define nbc alert enumeration
-- @author MGD
-- @release 2010-04-27
eEtatNbc_Niv0 = 0
eEtatNbc_Niv4 = 1

--- Switch on radar
-- @author MGD
-- @release 2010-04-27
integration.goOnNBCAlert = function( self )
    meKnowledge:RC( eRC_AlerteNBCNiveau4 )
    F_Pion_SeteEtatNbc( self.source, eEtatNbc_Niv4 )
    myself.NBCAlert = true
end

--- EquipNBC outfit
-- @author MGD
-- @release 2010-04-27
integration.equipNBCOutfit = function()
    meKnowledge:RC( eRC_TenueProtectionNBCMise )
    DEC_Agent_MettreTenueProtectionNBC()
end

--- UnequipNBC outfit
-- @author MGD
-- @release 2010-04-27
integration.unequipNBCOutfit = function( self )
    meKnowledge:RC( eRC_TenueProtectionNBCEnlevee )
    if myself.NBCAlert == true then
        meKnowledge:RC( eRC_FinAlerteNBCNiveau4 )
        F_Pion_SeteEtatNbc( self.source, eEtatNbc_Niv0 )
        myself.NBCAlert = false
    end
    DEC_Agent_EnleverTenueProtectionNBC()
end

--- Define etat radio enumeration
-- @author MGD
-- @release 2010-04-27
eEtatRadio_Ouverte = 0
eEtatRadio_Silence = 1
eEtatRadio_Silence_Partiel = 2

--- Switch off radio
-- @author MGD
-- @release 2010-04-27
integration.switchOffRadio = function( self )
    meKnowledge:RC( eRC_DebutSilenceRadio )
    F_Pion_SeteEtatRadio( self.source, eEtatRadio_Silence )
    DEC_Agent_PasserEnSilenceRadio()
end

--- Switch off radio
-- @author HBD
-- @release 2010-06-25
integration.switchEmitOnlyOffRadio = function( self )
    meKnowledge:RC( eRC_DebutSilencePartielRadio )
    F_Pion_SeteEtatRadio( self.source, eEtatRadio_Silence_Partiel )
    DEC_Agent_PasserEnSilenceRadioPartiel()
end

--- Switch on radio
-- @author MGD
-- @release 2010-04-27
integration.switchOnRadio = function( self )
    meKnowledge:RC( eRC_FinSilenceRadio )
    F_Pion_SeteEtatRadio( self.source, eEtatRadio_Ouverte )
    DEC_Agent_ArreterSilenceRadio()
end


--- Switch off radar
-- @author MGD
-- @release 2010-04-27
integration.switchOffRadar = function( self )
    meKnowledge:RC( eRC_DebutSilenceRadar )
    F_Pion_SeteEtatRadar( self.source, eEtatRadar_Silence )
    DEC_Perception_DesactiverRadar( eRadarType_Radar )
    DEC_Perception_DesactiverRadar( eRadarType_Ecoute )
    DEC_Perception_DesactiverRadar( eRadarType_EcouteRadar )
end

--- Switch on radar
-- @author MGD
-- @release 2010-04-27
integration.switchOnRadar = function( self )
    meKnowledge:RC( eRC_FinSilenceRadar )
    F_Pion_SeteEtatRadar( self.source, eEtatRadar_Ouvert )
    DEC_Perception_ActiverRadar( eRadarType_Radar )
    DEC_Perception_ActiverRadar( eRadarType_Ecoute )
    DEC_Perception_ActiverRadar( eRadarType_EcouteRadar )
end
-- -------------------------------------------------------------------------------- 
-- Switch on safety attitude
-- @author MIA
-- @release 2010-09-09
-- @ On mod�lise par les effets, le r�sultat d'une posture de progression tactique
-- de niveau section qui se couvre etc. pour mieux voir, �tre moins vue, mieux r�agir
-- en cas de prise � partie.
-- --------------------------------------------------------------------------------
integration.switchOnSafetyMode = function( self )
    myself.speedModulation.switchOnSafetyMode = 0.3 -- modulationMax / 10
    DEC_Perception_ActiverCoupsDeSonde()
    if DEC_Agent_EstEnVol() and myself.altitude then
        DEC_Agent_HauteurDeVol( myself.altitude * 0.2 )
    end
end

-- -------------------------------------------------------------------------------- 
-- Switch on cover attitude
-- @author MIA
-- @release 2010-09-09
-- Modulation de la vitesse max [0;1]. 
-- --------------------------------------------------------------------------------
integration.switchOnCoverMode = function( self )
    if meKnowledge:hasBadForceRatio() then
        myself.speedModulation.switchOnCoverMode = 0.3
    else
        myself.speedModulation.switchOnCoverMode = 0.4
    end
end

-- -------------------------------------------------------------------------------- 
-- Switch off safety attitude
-- @author MIA
-- @release 2010-09-09
-- --------------------------------------------------------------------------------
integration.switchOffSafetyMode = function( self )
    myself.speedModulation.switchOnSafetyMode = 1
    DEC_Perception_DesactiverCoupsDeSonde()
    if DEC_Agent_EstEnVol() and myself.altitude then
        DEC_Agent_HauteurDeVol( myself.altitude )
    end
    meKnowledge:RC( eRC_AmbianceVitesse )
end
-- -------------------------------------------------------------------------------- 
-- Switch off cover attitude
-- @author MIA
-- @release 2010-09-09
-- --------------------------------------------------------------------------------
integration.switchOffCoverMode = function( self )
    DEC_Perception_DesactiverCoupsDeSonde()
    myself.speedModulation.switchOnCoverMode = 1
    meKnowledge:RC( eRC_CouvertureDesactive )
end
-- -------------------------------------------------------------------------------- 
-- @return The unit can dismount
-- @author LMT
-- @release 2010-10-20
-- --------------------------------------------------------------------------------
integration.canDismount = function()
    return DEC_Agent_EstEmbarquable()
end

-- -------------------------------------------------------------------------------- 
-- @return The unit can mount
-- @author LMT
-- @release 2010-10-20
-- --------------------------------------------------------------------------------
integration.canMount = function()
    return DEC_Agent_EstEmbarquable() and DEC_CanMount( myself )
end

integration.shouldDismount = function()
    return DEC_Agent_EstEmbarquable() and not DEC_Agent_EstDebarque()
           and not DEC_IsPointInUrbanBlockTrafficable( DEC_Agent_Position() )
end

-- -------------------------------------------------------------------------------- 
-- The unit need to mount
-- @author LMT / review MIA 2011-09-20
-- @release 2010-10-20
-- --------------------------------------------------------------------------------
integration.mountStarted = function( self )
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

-- -------------------------------------------------------------------------------- 
-- The unit need to dismount
-- @author LMT / review MIA 2011-09-20
-- @release 2010-10-20
-- --------------------------------------------------------------------------------
integration.dismountStarted = function( self )
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

-- -------------------------------------------------------------------------------- 
-- The unit stop dismounting
-- @author LMT / review MIA 2011-09-20
-- @release 2010-10-20
-- --------------------------------------------------------------------------------
integration.stopDismount = function( self )  
    myself.stayDismounted = false
    if myself.orderDismount then
        DEC__StopAction( myself.orderDismount )
        myself.orderDismount = nil
    end
end

-- -------------------------------------------------------------------------------- 
-- The unit stop mounting
-- @author LMT / review MIA 2011-09-20
-- @release 2010-10-20
-- --------------------------------------------------------------------------------
integration.stopMount = function( self )
    myself.stayDismounted = false
    if myself.orderMount then
        DEC__StopAction( myself.orderMount )
        myself.orderMount = nil
    end
end

-- -------------------------------------------------------------------------------- 
-- The unit is mounted
-- @author LMT
-- @release 2010-10-20
-- --------------------------------------------------------------------------------
integration.isMounted = function( self )
    return DEC_Agent_EstEmbarque()
end

-- -------------------------------------------------------------------------------- 
-- The unit is dismounted
-- @author LMT
-- @release 2010-10-20
-- --------------------------------------------------------------------------------
integration.isDismounted = function( self )
    return DEC_Agent_EstDebarque()
end

-- -------------------------------------------------------------------------------- 
-- Deploy the unit
-- @author PSN
-- @release 2011-12-20
-- --------------------------------------------------------------------------------
integration.startDeploy = function( self )
    DEC_Agent_Deploy()
end

-- -------------------------------------------------------------------------------- 
-- Undeploy the unit
-- @author PSN
-- @release 2011-12-20
-- --------------------------------------------------------------------------------
integration.undeploy = function( self )
    DEC_Agent_Undeploye()
end

-- -------------------------------------------------------------------------------- 
-- The unit is deployed
-- @author CMA
-- @release 2011-04-12
-- --------------------------------------------------------------------------------
integration.isDeployed = function( self )
    return DEC_Agent_IsDeployed()
end

-- -------------------------------------------------------------------------------- 
-- The drones are implemented
-- @author GGE
-- @release 2011-12-16
-- --------------------------------------------------------------------------------
integration.startActivateDrone = function ( self )
    integration.setAvailableDrones()
	myself.Deployed = false
    if myself.droneAvailable == nil then
        meKnowledge:RC( eRC_PasDeDroneDisponible )
        if not meKnowledge:isSelfCommanding() then
            meKnowledge:sendNoDisponibleDrone( meKnowledge:getAutomat() )
        end
	else
		DEC_Agent_Deploy()
		meKnowledge:RC( eRC_DebutMiseEnOeuvreDrone )
	end
end

integration.startedActivateDrone = function ( self )
    if not myself.Deployed and meKnowledge:isDeployed() and myself.droneAvailable then
        myself.Deployed = true
        myself.droneAvailable:SetbMiseEnOeuvre_( true ) -- mandatory to permit the flight
        local droneKn = CreateKnowledge( world.PlatoonAlly, myself.droneAvailable )
        meKnowledge:RC( eRC_FinMiseEnOeuvreDrone )
        meKnowledge:sendRC( droneKn, eRC_DroneDisponible )
    end
end

integration.setAvailableDrones = function ( self )
    myself.droneAvailable  = nil
    local listePions = DEC_Pion_PionsSansPC()
    for _,pion in pairs( listePions or emptyTable ) do
        local operationalLevel = pion:DEC_Agent_EtatOpsMajeur() * 100
        local fuelDotationNumber = DEC_Agent_GetFuelDotationNumber( pion )	
        -- if DEC_GetSzName( pion ) == "Masalife.RENS.Drone SDTI" and operationalLevel ~= 0 and fuelDotationNumber > 0 then
        if operationalLevel ~= 0 and fuelDotationNumber > 3 then -- Le drone doit �tre op�rationnel et avoir un minimum de carburant
            if DEC_Geometrie_DistanceBetweenPoints( DEC_Agent_Position(), DEC_Agent_PositionPtr(pion) ) < 80 and  not pion:GetbMiseEnOeuvre_() and not pion:GetbEnExploitation_() then
                pion:SetbMiseEnOeuvre_( true ) -- mandatory to permit the flight
                DEC_Transport_DebarquerPionSansDelais( pion )
                myself.droneAvailable = pion 
                break
            end
        end
    end
end

integration.stopActivateDrone = function( self )
    DEC_Agent_Undeploye()
    return true
end

-- -------------------------------------------------------------------------------- 
-- Random position in circle
-- @author CMA
-- @release 2011-05-04
-- --------------------------------------------------------------------------------
integration.randomPositionInCircle = function( position, radius )
    return CreateKnowledge( world.Point, DEC_Geometrie_PositionAleatoireDansCercle( position:getPosition(), radius ) )
end

-- -------------------------------------------------------------------------------- 
-- Split area into subareas
-- @author CMA
-- @release 2011-05-04
-- --------------------------------------------------------------------------------
integration.splitArea = function( area, numberOfParts )
    local subAreas = DEC_Geometry_SplitLocalisation( area.source, numberOfParts, nil )
    subAreas = subAreas.first
    myself.leadData.subAreas = {}
    for _, localArea in pairs( subAreas ) do
        myself.leadData.subAreas[#myself.leadData.subAreas + 1] = CreateKnowledge( world.Area, localArea )
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

integration.activateFunctions = function( self )
    if integration.isLogisticTypeUnit( ) then
        DEC_Sante_ActiverChaine()
        DEC_Maintenance_ActiverChaine()
        DEC_Ravitaillement_ActiverChaine()
    end
end

integration.desactivateFunctions = function( self )
    if integration.isLogisticTypeUnit( ) then
        DEC_Sante_DesactiverChaine()
        DEC_Maintenance_DesactiverChaine()
        DEC_Ravitaillement_DesactiverChaine()
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
        DEC_Sante_ChangerPriorites( List )
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

-- -------------------------------------------------------------------------------- 
-- Start Get VSRAM
-- @author GGE
-- @release 2011-10-05
-- --------------------------------------------------------------------------------
integration.StartGetVSRAM = function ( pionRenforce, nbrAmbulances )
    DEC_RecupererVSRAM( pionRenforce.source, nbrAmbulances )
end

-- -------------------------------------------------------------------------------- 
-- Activate radar on area
-- @param area
-- @author PSN
-- @release 2012-07-12
-- -------------------------------------------------------------------------------- 
integration.activateRadar = function ( area )
    if not myself.radarActivated then
        myself.ecoute = DEC_Perception_ActiverRadarSurLocalisation( eRadarType_Ecoute, area.source )
        myself.radar = DEC_Perception_ActiverRadarSurLocalisation( eRadarType_Radar, area.source )
        myself.ecouteRadar = DEC_Perception_ActiverRadarSurLocalisation( eRadarType_EcouteRadar, area.source )
        myself.zoneAEcouter = area.source
        myself.radarActivated = true

        meKnowledge:RC( eRC_DebutSurveillance )
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
        DEC_Perception_DesactiverRadarSurLocalisation( eRadarType_Ecoute, myself.ecoute )
        DEC_Perception_DesactiverRadarSurLocalisation( eRadarType_Radar, myself.radar )
        DEC_Perception_DesactiverRadarSurLocalisation( eRadarType_EcouteRadar, myself.ecouteRadar )
        myself.radarActivated = false

        meKnowledge:RC( eRC_FinSurveillance )	
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

    meKnowledge:RC( eRC_DebutSurveillance )
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
        meKnowledge:RC( eRC_FinSurveillance )
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
    DEC_ReseauRessourceAugmenteProduction( resourceNode.source, quantity )
    return true
end
integration.decreaseNodeProduction = function( resourceNode, quantity )
    DEC_ReseauRessourceBaisseProduction( resourceNode.source, quantity )
    return true
end
integration.enable = function( resourceNode )
    meKnowledge:RC( eRC_ResourceNodeEnabled )
    DEC_ReseauRessource_ActiverElement( resourceNode.source ) 
    return true
end
integration.disable = function( resourceNode )
    meKnowledge:RC( eRC_ResourceNodeDisabled )
    DEC_ReseauRessource_DesactiverElement( resourceNode.source ) 
    return true
end

integration.isDead = function( self )
    return DEC_Agent_EstMort()
end

integration.selfDecontaminate = function( self )
    DEC_Agent_SeDecontaminer()
    return true
end

integration.changeHeight = function( height )
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
    return CreateKnowledge( world.PlatoonAlly, DEC_Pion_PionPC() )
end

integration.isLogisticTypeUnit = function( )
    local typePion = DEC_Pion_GetMilPionType( myself )
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
