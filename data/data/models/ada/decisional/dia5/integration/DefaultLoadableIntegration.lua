-- -------------------------------------------------------------------------------- 
-- Camp integration
-- --------------------------------------------------------------------------------
integration.discharge = function( unit, camp )
    if DEC_ConnaissanceAgent_EstRefugie( unit.source ) then
        if DEC_Refugies_EstEmbarque( unit.source ) then
            DEC_Refugies_DebarquerDansCamp( unit.source, camp.source )
            return true
        end
    elseif DEC_ConnaissanceAgent_EstPrisonnier( unit.source ) then
        if DEC_Prisonniers_EstEmbarque( unit.source ) then
            DEC_Prisonniers_DebarquerDansCamp( unit.source, camp.source )
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

integration.isFriendOrFoeTranported = function( unit )
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
integration.loadFriend = function( unit, onlyLoadable )
    DEC_Transport_EmbarquerPionSansDelais( unit.source, onlyLoadable )
    return true
end
integration.unloadFriend = function( unit )
    DEC_Transport_DebarquerPionSansDelais( unit.source )
    return true
end

integration.isFriendTranported = function( unit )
    return unit.source:DEC_Agent_EstTransporte() -- $$$ MIA TODO TEMP immonde
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

integration.readyForLoad = function( unit )
    local unitSrc = unit.source
    local mission = DEC_GetRawMission( unitSrc )
    if mission ~= nil and ( mission:GetType() == "T_Task_Pion_SeFaireTransporter" or mission:GetType() == "T_Mission_Pion_SeFaireTransporter" or mission:GetType() == "france.military.platoon.tasks.SeFaireTransporter" ) then
        if DEC_Geometrie_Distance( meKnowledge:getPosition() , unit:getPosition() ) < 300 then -- $$$$ HARD CODDED VALUE
            return true 
        end
    end
    return false
end
    
integration.knowledgeReadyForLoad = function( knowledge )
    local knowledgeSrc = knowledge.source
    local mission = DEC_Connaissance_GetRawMission( knowledgeSrc )
    if mission ~= nil and ( mission:GetType() == "T_Task_Pion_SeFaireTransporter" or mission:GetType() == "T_Mission_Pion_SeFaireTransporter" or mission:GetType() == "france.military.platoon.tasks.SeFaireTransporter" ) then
        if DEC_Geometrie_Distance( meKnowledge:getPosition() , knowledge:getPosition() ) < 300 then -- $$$$ HARD CODDED VALUE
            return true 
        end
    end
    return false
end

integration.isTransportFinished = function()
    return DEC_Transport_EstTermine()
end

integration.canTransportUnit = function( unit, onlyLoadable )
    return DEC_Agent_PeutTransporterPion( unit.source , onlyLoadable )
end

integration.canTransportKnowledge = function( knowledge, onlyLoadable )
    return DEC_Connaissance_PeutTransporterPion( myself, knowledge.source , onlyLoadable )
end


-- -----------------------------------------------------------------------------------
-- Transport of crowd
-- -----------------------------------------------------------------------------------
integration.canTransportCrowd = function()
    return DEC_Agent_PeutTransporterFoule( myself )
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
    myself.actionUnloadCrowd = DEC__StopAction( myself.actionUnloadCrowd )
    myself.eEtatTransportUnloadCrowd = nil
end

integration.isTransportingCrowd = function()
    return DEC_Agent_TransporteFoule( myself )
end

integration.isTransported = function( enemy )
    return DEC_ConnaissanceAgent_EstTransporte( enemy.source )
end

integration.isSurrendered = function( )
    return DEC_Agent_EstRendu( myself )
end