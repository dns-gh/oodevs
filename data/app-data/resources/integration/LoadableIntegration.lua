-- -------------------------------------------------------------------------------- 
-- Discharge agent
-- --------------------------------------------------------------------------------

integration.dischargeAgent = function( unit, camp )
    if DEC_Agent_EstRefugie( unit.source ) then
        if DEC_Agent_RefugieEstEmbarque( meKnowledge.source, unit.source ) then
            DEC_Agent_DebarquerRefugiesDansCamp( meKnowledge.source, unit.source, camp.source )
            return true
        end
    else
        DEC_Transport_DebarquerPionSansDelais( unit.source )
        return true
    end
    return false
end

-- -------------------------------------------------------------------------------- 
-- Discharge agent knowledge
-- --------------------------------------------------------------------------------

integration.dischargeAgentKnowledge = function( ennemy, camp )
     if DEC_ConnaissanceAgent_EstRefugie( ennemy.source ) then
        if DEC_Refugies_EstEmbarque( ennemy.source ) then
            DEC_Refugies_DebarquerDansCamp( ennemy.source, camp.source )
            return true
        end
    elseif DEC_ConnaissanceAgent_EstPrisonnier( ennemy.source ) then
        if DEC_Prisonniers_EstEmbarque( ennemy.source ) then
            DEC_Prisonniers_DebarquerDansCamp( ennemy.source, camp.source )
            return true
        end
    end
    return false
end

-- -------------------------------------------------------------------------------- 
-- Default loadable/unloadable integration for friends or foe (agent knowledge)
-- --------------------------------------------------------------------------------
integration.loadFriendOrFoe = function( unit )
    if DEC_ConnaissanceAgent_EstRenduAMonCamp( myself, unit.source ) then
        if not DEC_Prisonniers_EstEmbarque( unit.source ) then
            DEC_Prisonniers_CapturerEtEmbarquer( unit.source )
            return true
        end
    elseif DEC_ConnaissanceAgent_EstRefugie( unit.source ) then
        if not DEC_Refugies_EstEmbarque( unit.source ) then
            DEC_Refugies_OrienterEtEmbarquer( unit.source )
            return true
        end
    else
        DEC_Connaissance_Transporter( myself, unit.source ) -- transpoter un otage
        return true
    end
    return false
end

integration.unloadFriendOrFoe = function( unit )
    if DEC_Prisonniers_EstEmbarque( unit.source ) then
        DEC_Prisonniers_Debarquer( unit.source )
        return true
    end
    if DEC_Refugies_EstEmbarque( unit.source ) then
        DEC_Refugies_Debarquer( unit.source )
        return true
    end
    return false
end

integration.isFriendOrFoeTransported = function( unit )
    if DEC_Prisonniers_EstEmbarque( unit.source ) then
        return true
    end
    if DEC_Refugies_EstEmbarque( unit.source ) then
        return true
    end
    return false
end

-- -------------------------------------------------------------------------------- 
-- Default loadable/unloadable integration for friends
-- --------------------------------------------------------------------------------
integration.canLoadFriend = function( unit, onlyLoadable )
    return true -- DEC_Agent_PeutTransporterPion( unit.source , onlyLoadable )
end
integration.loadFriend = function( unit )
    if DEC_Agent_EstRefugie( unit.source ) then
        DEC_Agent_OrienterEtEmbarquer( meKnowledge.source, unit.source )
    else
        DEC_Transport_EmbarquerPionSansDelais( unit.source, false )
    end
    return true
end
integration.unloadFriend = function( unit )
    DEC_Transport_DebarquerPionSansDelais( unit.source )
    return true
end

integration.isFriendTranported = function( unit )
    return unit.source:DEC_Agent_EstTransporte()
end

-- -----------------------------------------------------------------------------------
-- Non magic Load Integration
-- -----------------------------------------------------------------------------------
integration.startLoadQueue = function()
    myself.actionEmbarquer = DEC_Start_TransportEmbarquer(  )
    actionCallbacks[ myself.actionEmbarquer ] = function( arg ) myself.eEtatTransport = arg end
    return false
end

integration.updateLoadQueue = function()
    if myself.eEtatTransport == eActionTransport_Finished then
        return true
    elseif myself.eEtatTransport == eActionTransport_Impossible then
        meKnowledge:RC( eRC_TransportImpossiblePasDeMoyens )
    end
    return false
end

integration.stopLoadQueue = function()
    myself.actionEmbarquer = DEC__StopAction( myself.actionEmbarquer )
    myself.eEtatTransport = nil
end

integration.startUnloadQueue = function()
    myself.actionDebarquer = DEC_Start_TransportDebarquer( nil )
    actionCallbacks[ myself.actionDebarquer ] = function( arg ) myself.eEtatTransport = arg end
    return false
end

integration.updateUnloadQueue = function()
    if myself.eEtatTransport == eActionTransport_Finished then
        return true
    elseif myself.eEtatTransport == eActionTransport_Impossible then
        meKnowledge:RC( eRC_TransportImpossiblePasDeMoyens )
    end
    return false
end

integration.stopUnloadQueue = function()
    myself.actionDebarquer = DEC__StopAction( myself.actionDebarquer )
    myself.eEtatTransport = nil
end

integration.addPlatoonInQueue = function( unit, onlyLoadable )
    DEC_Transport_AjouterPion( unit.source, onlyLoadable )
end

integration.addKnowledgeInQueue = function( knowledge, onlyLoadable )
    DEC_TransportConnaissance_AjouterPion( myself, knowledge.source, onlyLoadable )
end

-- distanceMin is equal to -1 if the unit is ready to load when it is into an area.
integration.readyForLoad = function( unit, distanceMin, area )
    if not distanceMin then
        distanceMin = 300
    end
    local unitSrc = unit.source
    local mission = DEC_GetRawMission( unitSrc )
    if mission ~= nil and ( 
           integration.getAnyType( mission ) == "T_Task_Pion_SeFaireTransporter" 
        or integration.getAnyType( mission ) == "T_Mission_Pion_SeFaireTransporter" 
        or integration.getAnyType( mission ) == "platoon.tasks.SeFaireTransporter"
        or integration.getAnyType( mission ) == "agent.tasks.GetTransported"
        or integration.getAnyType( mission ) == "france.military.platoon.tasks.SeFaireTransporter" ) then
        if ( distanceMin == -1 and integration.isPointInLocalisation( unit , area ) ) or 
           ( DEC_Geometrie_Distance( meKnowledge:getPosition() , unit:getPosition() ) < distanceMin ) then
            return true 
        end
    end
    return false
end

-- distanceMin is equal to -1 if the knowledge is ready to load when it is into an area.
integration.knowledgeReadyForLoad = function( knowledge, distanceMin, area )
    if not distanceMin then
        distanceMin = 300
    end
    local knowledgeSrc = knowledge.source
    local mission = DEC_Connaissance_GetRawMission( knowledgeSrc )
    if mission ~= nil and ( integration.getAnyType( mission ) == "T_Task_Pion_SeFaireTransporter" 
                         or integration.getAnyType( mission ) == "T_Mission_Pion_SeFaireTransporter" 
                         or integration.getAnyType( mission ) == "platoon.tasks.SeFaireTransporter"
                         or integration.getAnyType( mission ) == "agent.tasks.GetTransported"
                         or integration.getAnyType( mission ) == "france.military.platoon.tasks.SeFaireTransporter" ) then
         if ( distanceMin == -1 and integration.isPointInLocalisation( knowledge , area ) ) or 
           ( DEC_Geometrie_Distance( meKnowledge:getPosition() , knowledge:getPosition() ) < distanceMin ) then
            return true 
        end
    end
    return false
end

integration.isTransportFinished = function()
    return DEC_Transport_EstTermine()
end

integration.canTransportUnit = function( unit, onlyLoadable )
    return DEC_Agent_PeutTransporterPion( unit.source, onlyLoadable )
end

integration.canTransportKnowledge = function( knowledge, onlyLoadable )
    return DEC_Connaissance_PeutTransporterPion( myself, knowledge.source, onlyLoadable )
end

integration.transportUnitRoundTrip = function( unit, onlyLoadable )
    return DEC_Agent_TransportNombreAllerRetour( unit.source, onlyLoadable )
end

integration.transportKnowledgeRoundTrip = function( knowledge, onlyLoadable )
    return DEC_Connaissance_TransportNombreAllerRetour( myself, knowledge.source, onlyLoadable )
end

-- -----------------------------------------------------------------------------------
-- Transport of crowd
-- -----------------------------------------------------------------------------------
integration.canTransportCrowd = function()
    return DEC_Agent_PeutTransporterFoule( myself )
end

integration.canTransportCrowdConcentration = function( crowd, concentration )
    return DEC_CrowdKnowledge_CanLoadCrowdConcentration( myself, crowd.source, concentration )
end

integration.transportCrowdRoundTrip = function( crowd )
    local capacityTransport = DEC_Agent_GetCapacityToTransportCrowd( myself )
    local crowdsNumber = DEC_GetNombrePersonnesDansFoule( myself, crowd.source )  
    if capacityTransport > 0 then
        return math.ceil( crowdsNumber / capacityTransport )
    else
        return 0
    end
end

-- Load crowd
integration.startLoadCrowd = function( crowd, concentration )
    meKnowledge:RC( eRC_TransportEmbarquement )
    myself.actionLoadCrowd = DEC_StartEmbarquerFouleDUneConcentration( crowd.source, concentration )
    actionCallbacks[ myself.actionLoadCrowd ] = function( arg ) myself.eEtatTransportCrowd = arg end
    return false
end

integration.startedLoadCrowd = function( crowd, concentration )
    if myself.eEtatTransportCrowd == eActionTransport_Finished then
        return true
    elseif myself.eEtatTransportCrowd == eActionTransport_Impossible then
        DEC_Trace( "transportation impossible" )
        meKnowledge:RC( eRC_TransportImpossiblePasDeMoyens )
    end
    return false
end

integration.stopLoadCrowd = function( crowd, concentration )
    myself.actionLoadCrowd = DEC__StopAction( myself.actionLoadCrowd )
    myself.eEtatTransportCrowd = nil
end

-- Unload crowd
integration.startUnloadCrowd = function( crowd, position )
    meKnowledge:RC( eRC_TransportDebarquement )
    myself.actionUnloadCrowd = DEC_StartDebarquerFouleSurPosition( crowd.source, position:getPosition() )
    actionCallbacks[ myself.actionUnloadCrowd ] = function( arg ) myself.eEtatTransportUnloadCrowd = arg end
    return false
end

integration.startedUnloadCrowd = function( crowd )
    if myself.eEtatTransportUnloadCrowd == eActionTransport_Finished then
        return true
    elseif myself.eEtatTransportUnloadCrowd == eActionTransport_Impossible then
        meKnowledge:RC( eRC_TransportImpossiblePasDeMoyens )
    end
    return false
end

integration.stopUnloadCrowd = function( crowd )
    if myself.actionUnloadCrowd ~= nil then
        myself.actionUnloadCrowd = DEC__StopAction( myself.actionUnloadCrowd )
    end
    myself.eEtatTransportUnloadCrowd = nil
end

integration.isTransportingCrowd = function()
    return DEC_Agent_TransporteFoule( myself )
end

integration.isTransported = function( enemy )
    return DEC_ConnaissanceAgent_EstTransporte( enemy.source )
end

integration.allowCarriers = function()
    DEC_LaisserTransporteursSansDelai()
end

integration.retrieveCarriers = function()
    DEC_RecupererTransporteursSansDelai()
end

integration.isAgentSurrendered = function( friend )
    return DEC_Agent_EstRendu( friend.source )
end

integration.isKnowledgeAgentSurrendered = function( platoon )
    return DEC_ConnaissanceAgent_EstRenduAMonCamp( meKnowledge.source, platoon.source )
end

integration.isDuringTransport = function( )
    return DEC_Transport_EnCoursDeTransport()
end