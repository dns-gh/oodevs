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
    myself.loadedUnits = myself.loadedUnits or {}
    myself.loadedUnits[ unit ] = true
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
    if myself.loadedUnits then
        myself.loadedUnits[ unit ] = nil
    end
    if myself.capturedUnits and #myself.capturedUnits > 0 then
        myself.capturedUnits = removeFromList( unit, myself.capturedUnits ) -- remove from captured list
    end
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
    myself.loadedUnits = myself.loadedUnits or {}
    myself.loadedUnits[ unit ] = true
    if DEC_Agent_EstRefugie( unit.source ) then
        DEC_Agent_OrienterEtEmbarquer( meKnowledge.source, unit.source )
    else
        DEC_Transport_EmbarquerPionSansDelais( unit.source, false )
    end
    return true
end
integration.unloadFriend = function( unit )
    if myself.loadedUnits then
        myself.loadedUnits[ unit ] = nil
    end
    if myself.capturedUnits and #myself.capturedUnits > 0 then
        myself.capturedUnits = removeFromList( unit, myself.capturedUnits ) -- remove from captured list
    end
    DEC_Transport_DebarquerPionSansDelais( unit.source )
    return true
end

-- Returns true if the agent body is transported, false otherwise
integration.isBodyTransported = function()
    return DEC_Agent_EstTransporte()
end

-- Returns true if the provided friendly unit is transported, false otherwise
-- @param unit : knowledge of a friendly agent
integration.isFriendTransported = function( unit )
    return unit.source:DEC_Agent_EstTransporte()
end

integration.isFriendTranported = integration.isFriendTransported -- To ensure backward compatibility

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

integration.friendHasTransportationMission = function( unit, missionName )
    local mission = DEC_GetRawMission( unit.source )
    if mission ~= nil and ( integration.getAnyType( mission ) == missionName ) then
       return true 
    end
    return false
end

integration.unitHasTransportationMission = function( unit, missionName )
    local mission = DEC_Connaissance_GetRawMission( unit.source )
    if mission ~= nil and ( integration.getAnyType( mission ) == missionName ) then
        return true
    end
    return false
end

local isAMissionForTransport = function( mission )
    if not mission then return false end
    local missionType =  integration.getAnyType( mission )
    local taskKnowledge = integration.taskKnowledge[ missionType ]
    if taskKnowledge and taskKnowledge.isLoadMission then
        return taskKnowledge:isLoadMission()
    end
    return mission ~= nil and ( 
           missionType == "T_Task_Pion_SeFaireTransporter" 
        or missionType == "T_Mission_Pion_SeFaireTransporter" 
        or missionType == "platoon.tasks.SeFaireTransporter"
        or missionType == "worldwide.agent.tasks.GetTransported"
        or missionType == "france.military.platoon.tasks.SeFaireTransporter" )
end

-- distanceMin is equal to -1 if the unit is ready to load when it is into an area.
local readyForLoad = function( knowledge, distanceMin, area, mission )
    if not distanceMin then
        distanceMin = 100
    end
    if isAMissionForTransport( mission ) then
        if ( distanceMin == -1 and integration.isPointInLocalisation( unit, area ) ) or 
           ( DEC_Geometrie_Distance( meKnowledge:getPosition(), knowledge:getPosition() ) < distanceMin ) then
            return true 
        end
    end
    return false
end

integration.readyForLoad = function( unit, distanceMin, area )
    local mission = DEC_GetRawMission( unit.source )
    return readyForLoad( unit, distanceMin, area, mission )
end

-- distanceMin is equal to -1 if the knowledge is ready to load when it is into an area.
integration.knowledgeReadyForLoad = function( knowledge, distanceMin, area )
    local mission = DEC_Connaissance_GetRawMission( knowledge.source )
    return readyForLoad( knowledge, distanceMin, area, mission )
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

-- Returns true if the transporting agent can transport the knowledge to transport, false otherwise
-- @param transportingAgent : the transporting agent
-- @param knowledgeToTransport : knowledge of the agent to transport
-- @param onlyLoadable : whether or not the non-loadable equipments should be ignored
integration.canAgentTransportKnowledge = function( transportingAgent, knowledgeToTransport, onlyLoadable )
    return DEC_Connaissance_PeutTransporterPion( transportingAgent.source, knowledgeToTransport.source, onlyLoadable )
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

integration.startedLoadCrowd = function()
    if myself.eEtatTransportCrowd == eActionTransport_Finished then
        return true
    elseif myself.eEtatTransportCrowd == eActionTransport_Impossible then
        DEC_Trace( "transportation impossible" )
        meKnowledge:RC( eRC_TransportImpossiblePasDeMoyens )
        return true  -- loading is impossible, so let us stop here
    end
    return false
end

integration.stopLoadCrowd = function()
    myself.actionLoadCrowd = DEC__StopAction( myself.actionLoadCrowd )
    myself.eEtatTransportCrowd = nil
end

-- Unload crowd
integration.startUnloadCrowd = function( crowd )
    local crowdId
    if crowd == nil or crowd == NIL then
         crowdId = 0 -- if the crowd is not issued, unload current transported crowd concentration.
    else
        crowdId = crowd.source
    end
    meKnowledge:RC( eRC_TransportDebarquement )
    myself.actionUnloadCrowd = DEC_StartDebarquerFouleSurPosition( crowdId, meKnowledge:getPosition() )
    actionCallbacks[ myself.actionUnloadCrowd ] = function( arg ) myself.eEtatTransportUnloadCrowd = arg end
    return false
end

integration.startedUnloadCrowd = function()
    if myself.eEtatTransportUnloadCrowd == eActionTransport_Finished then
        return true
    elseif myself.eEtatTransportUnloadCrowd == eActionTransport_Impossible then
        meKnowledge:RC( eRC_TransportImpossiblePasDeMoyens )
        return true  -- unloading is impossible, so let us stop here
    end
    return false
end

integration.stopUnloadCrowd = function()
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

integration.areSuppliesLoaded = function( unit )
    return unit.source:GetsuppliesLoaded_()
end

integration.setSuppliesLoaded = function( unit, bool )
    return unit.source:SetsuppliesLoaded_( bool )
end

--- Tests whether an agent is currently able to load at least one of the
-- equipments of another agent.
-- @param unit the transporter agent
-- @param target the agent to transport
-- @param onlyLoadable a boolean specifying if only the components defined as 'loadable'
-- in the physical database must be considered
-- @return a boolean indicating if the agent is able to transport part of the target
integration.canLoad( unit, target, onlyLoadable )
    return DEC_CanLoad( unit.source, target.source, onlyLoadable )
end
