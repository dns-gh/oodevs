--- Adds the given unit to the table of units currently loaded by this entity.
-- Creates the table if it does not already exist
-- @param unit Directia agent or agent knowledge
integration.addToLoadedUnits = function( unit )
    myself.loadedUnits = myself.loadedUnits or {}
    myself.loadedUnits[ unit ] = true
end

--- Adds the given unit to the table of units currently captured by this entity.
-- Creates the table if it does not already exist
-- @param unit Directia agent or agent knowledge
integration.addToCapturedUnits = function( unit )
    myself.capturedUnits = myself.capturedUnits or {}
    myself.capturedUnits[ #myself.capturedUnits + 1 ] = unit
end

--- Removes the given unit to the table of units currently loaded by this entity.
-- @param unit Directia agent or agent knowledge
integration.removeFromLoadedUnits = function( unit )
    if myself.loadedUnits then
        myself.loadedUnits[ unit ] = nil
    end
end

--- Removes the given unit to the table of units currently loaded by this entity.
-- @param unit Directia agent or agent knowledge
integration.removeFromCapturedUnits = function( unit )
    if myself.capturedUnits then
        myself.capturedUnits = removeFromList( unit, myself.capturedUnits )
    end
end

--- Instantaneously unloads the given agent into the provided camp.
-- If the given unit is a refugee, then it will be taken into account by
-- the logistic units in the camp upon unloading.
-- @param unit Directia agent
-- @param camp Object knowledge (camp)
-- @return Boolean, whether or not the unloading succeeded
integration.dischargeAgent = function( unit, camp )
    integration.removeFromLoadedUnits( unit )
    integration.removeFromCapturedUnits( unit )
    if DEC_Agent_EstRefugie( unit.source ) then
        if DEC_Agent_RefugieEstEmbarque( myself, unit.source ) then
            DEC_Agent_DebarquerRefugiesDansCamp( myself, unit.source, camp.source )
            return true
        end
    else
        DEC_Transport_DebarquerPionSansDelais( unit.source )
        return true
    end
    return false
end

--- Instantaneously unloads the given agent knowledge into the provided camp.
-- The given unit must be a refugee or a prisoner for the unloading to take place.
-- The given unit will be taken into account by the logistic units in the camp upon unloading.
-- @param enemy Directia agent knowledge
-- @param camp Object knowledge (camp)
-- @return Boolean, whether or not the unloading succeeded
integration.dischargeAgentKnowledge = function( enemy, camp )
    integration.removeFromLoadedUnits( enemy )
    integration.removeFromCapturedUnits( enemy )
    if DEC_ConnaissanceAgent_EstRefugie( enemy.source ) then
        if DEC_Refugies_EstEmbarque( enemy.source ) then
            DEC_Refugies_DebarquerDansCamp( enemy.source, camp.source )
            return true
        end
    elseif DEC_ConnaissanceAgent_EstPrisonnier( enemy.source ) then
        if DEC_Prisonniers_EstEmbarque( enemy.source ) then
            DEC_Prisonniers_DebarquerDansCamp( enemy.source, camp.source )
            return true
        end
    end
    return false
end

--- Instantaneously loads the given agent knowledge.
-- If the agent knowledge is surrendered to this entity's side,
-- then it is loaded and captured.
-- If the agent knowledge is a refugee, then it is loaded and
-- orientated.
-- @see integration.unloadFriendOrFoe
-- @param unit Directia agent knowledge
-- @return Boolean, whether or not the loading succeeded
integration.loadFriendOrFoe = function( unit )
    integration.addToLoadedUnits( unit )
    if DEC_ConnaissanceAgent_EstRenduAMonCamp( myself, unit.source ) then
        if not DEC_Prisonniers_EstEmbarque( unit.source ) then
            DEC_Prisonniers_CapturerEtEmbarquer( unit.source )
            integration.addToCapturedUnits( unit )
            return true
        end
    elseif DEC_ConnaissanceAgent_EstRefugie( unit.source ) then
        if not DEC_Refugies_EstEmbarque( unit.source ) then
            DEC_Refugies_OrienterEtEmbarquer( unit.source )
            return true
        end
    else
        DEC_Connaissance_Transporter( myself, unit.source )
        return true
    end
    return false
end

--- Instantaneously unloads the given agent knowledge
-- @see integration.loadFriendOrFoe
-- @see integration.capture
-- @param unit Directia agent knowledge
-- @return Boolean, whether or not the unloading succeeded
integration.unloadFriendOrFoe = function( unit )
    integration.removeFromLoadedUnits( unit )
    integration.removeFromCapturedUnits( unit )
    
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

--- Returns true if the given agent knowledge is currently transported, false otherwise.
-- @param unit Directia agent knowledge
-- @return Boolean, whether or not the agent knowledge is transported
integration.isFriendOrFoeTransported = function( unit )
    if DEC_Prisonniers_EstEmbarque( unit.source ) then
        return true
    end
    if DEC_Refugies_EstEmbarque( unit.source ) then
        return true
    end
    return false
end

--- Instantaneously loads the given agent
-- The agent will also be properly orientated if it is a refugee.
-- @see integration.unloadFriend
-- @param unit Directia agent
-- @return Boolean, whether or not the loading succeeded
integration.loadFriend = function( unit )
    integration.addToLoadedUnits( unit )
    if DEC_Agent_EstRefugie( unit.source ) then
        DEC_Agent_OrienterEtEmbarquer( meKnowledge.source, unit.source )
    else
        DEC_Transport_EmbarquerPionSansDelais( unit.source, false )
    end
    return true
end

--- Instantaneously unloads the given agent
-- @see integration.loadFriend
-- @param unit Directia agent
-- @return Boolean, whether or not the unloading succeeded
integration.unloadFriend = function( unit )
    integration.removeFromLoadedUnits( unit )
    integration.removeFromCapturedUnits( unit )
    DEC_Transport_DebarquerPionSansDelais( unit.source )
    return true
end

--- Returns true if this entity is transported, false otherwise.
-- @return Boolean, vwhether or not this entity is transported
integration.isBodyTransported = function()
    return DEC_Agent_EstTransporte()
end

--- Returns true if the given agent is currently transported, false otherwise.
-- @param unit Directia agent
-- @return Boolean, whether or not the agent is transported
integration.isFriendTransported = function( unit )
    return unit.source:DEC_Agent_EstTransporte()
end

--- Starts loading the units in its current loading queue.
-- @see integration.addPlatoonInQueue
-- @see integration.updateLoadQueue
-- @see integration.stopLoadQueue
-- @return Boolean, false
integration.startLoadQueue = function()
    myself.actionEmbarquer = DEC_Start_TransportEmbarquer(  )
    actionCallbacks[ myself.actionEmbarquer ] = function( arg ) myself.eEtatTransport = arg end
    return false
end

--- Continues loading the units in its current loading queue.
-- @see integration.startLoadQueue
-- @see integration.stopLoadQueue
-- Displays a report if the loading is impossible.
-- @return Boolean, whether or not the loading is finished
integration.updateLoadQueue = function()
    if myself.eEtatTransport == eActionTransport_Finished then
        return true
    elseif myself.eEtatTransport == eActionTransport_Impossible then
        reportFunction(eRC_TransportImpossiblePasDeMoyens )
    end
    return false
end

--- Stops loading the units in its current loading queue.
-- The simulation action of loading is stopped. 
-- @see integration.startLoadQueue
-- @see integration.updateLoadQueue
integration.stopLoadQueue = function()
    myself.actionEmbarquer = DEC__StopAction( myself.actionEmbarquer )
    myself.eEtatTransport = nil
end

--- Starts unloading all the previously loaded units.
-- @see integration.updateUnloadQueue
-- @see integration.stopUnloadQueue
-- @return Boolean, false
integration.startUnloadQueue = function()
    myself.actionDebarquer = DEC_Start_TransportDebarquer( nil )
    actionCallbacks[ myself.actionDebarquer ] = function( arg ) myself.eEtatTransport = arg end
    return false
end

--- Continues unloading the units in its current unloading queue.
-- Displays a report if the unloading is impossible.
-- @see integration.startUnloadQueue
-- @see integration.stopUnloadQueue
-- @return Boolean, whether or not the unloading is finished
integration.updateUnloadQueue = function()
    if myself.eEtatTransport == eActionTransport_Finished then
        return true
    elseif myself.eEtatTransport == eActionTransport_Impossible then
        reportFunction(eRC_TransportImpossiblePasDeMoyens )
    end
    return false
end

--- Stops unloading the units in its current unloading queue.
-- The simulation action of unloading is stopped. 
-- @see integration.startUnloadQueue
-- @see integration.updateUnloadQueue
integration.stopUnloadQueue = function()
    myself.actionDebarquer = DEC__StopAction( myself.actionDebarquer )
    myself.eEtatTransport = nil
end

--- Adds the given agent to the current loading queue.
-- @param unit Directia agent
-- @param onlyLoadable Boolean, whether or not the transport of this unit will only
-- take into account components that are defined as 'loadable' in the physical database.
integration.addPlatoonInQueue = function( unit, onlyLoadable )
    DEC_Transport_AjouterPion( unit.source, onlyLoadable )
end

--- Adds the given agent knowledge to the current loading queue.
-- @param unit Directia agent knowledge
-- @param onlyLoadable Boolean, whether or not the transport of this unit will only
-- take into account components that are defined as 'loadable' in the physical database.
integration.addKnowledgeInQueue = function( knowledge, onlyLoadable )
    DEC_TransportConnaissance_AjouterPion( myself, knowledge.source, onlyLoadable )
end

--- Returns true if the given agent currently has the mission matching the provided task name, false otherwise
-- This method can be used to determine whether or not an agent is ready to be transported
-- (e.g. if it has a 'Get Transported' mission).
-- @param unit Directia agent
-- @param taskName String
-- @return Boolean
integration.friendHasTransportationMission = function( unit, taskName )
    local mission = DEC_GetRawMission( unit.source )
    if mission ~= nil and ( integration.getAnyType( mission ) == taskName ) then
       return true 
    end
    return false
end

--- Returns true if the given agent currently has the mission matching the provided task name, false otherwise
-- This method can be used to determine whether or not an agent is ready to be transported
-- (e.g. if it has a 'Get Transported' mission).
-- @param unit Directia agent
-- @param taskName String
-- @return Boolean
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

-- distanceMax is equal to -1 if the unit is ready to load when it is into an area.
local readyForLoad = function( knowledge, distanceMax, area, mission )
    distanceMax = distanceMax or 100
    if isAMissionForTransport( mission ) then
        if ( distanceMax == -1 and DEC_Geometrie_EstPointDansLocalisation( knowledge:getPosition(), area ) ) or 
           ( DEC_Geometrie_Distance( meKnowledge:getPosition(), knowledge:getPosition() ) < distanceMax ) then
            return true 
        end
    end
    return false
end

--- Returns true if the given agent is ready to be loaded, false otherwise.
-- For it to be deemed ready to be loaded, the agent must have a mission with one of the following DIA types : 
-- <ul> <li> T_Task_Pion_SeFaireTransporter </li>
-- <li> T_Mission_Pion_SeFaireTransporter </li>
-- <li> platoon.tasks.SeFaireTransporter </li>
-- <li> worldwide.agent.tasks.GetTransported </li>
-- <li> france.military.platoon.tasks.SeFaireTransporter </li> </ul>
-- The agent must also meet one of the following two conditions :
-- <ul> <li> The distanceMax parameter is positive, and the distance between this entity and the agent is
-- lower than distanceMax, or; </li>
-- <li> The distanceMax parameter is equal to -1, and the agent is inside the given area </li> </ul>
-- @param unit Directia agent, defining a "getPosition" method returning a simulation point
-- @param distanceMax Float, the maximal distance between this entity and the given agent for the latter to be
-- ready to be loaded. If this parameters equals to -1, then the distance is ignored, and the area parameter
-- is taken into account instead.
-- @param area Simulation area, taken into account only if the distanceMax parameter is equal to -1.
-- @return Boolean, whether or not the given agent is ready to be loaded
integration.readyForLoad = function( unit, distanceMax, area )
    local mission = DEC_GetRawMission( unit.source )
    return readyForLoad( unit, distanceMax, area, mission )
end

--- Returns true if the given agent knowledge is ready to be loaded, false otherwise.
-- For it to be deemed ready to be loaded, the agent must have a mission with one of the following DIA types : 
-- <ul> <li> T_Task_Pion_SeFaireTransporter </li>
-- <li> T_Mission_Pion_SeFaireTransporter </li>
-- <li> platoon.tasks.SeFaireTransporter </li>
-- <li> worldwide.agent.tasks.GetTransported </li>
-- <li> france.military.platoon.tasks.SeFaireTransporter </li> </ul>
-- The agent must also meet one of the following two conditions :
-- <ul> <li> The distanceMax parameter is positive, and the distance between this entity and the agent is
-- lower than distanceMax, or; </li>
-- <li> The distanceMax parameter is equal to -1, and the agent is inside the given area </li> </ul>
-- @param unit Directia agent knowledge, defining a "getPosition" method returning a simulation point
-- @param distanceMax Float, the maximal distance between this entity and the given agent for the latter to be
-- ready to be loaded. If this parameters equals to -1, then the distance is ignored, and the area parameter
-- is taken into account instead.
-- @param area Simulation area, taken into account only if the distanceMax parameter is equal to -1.
-- @return Boolean, whether or not the given agent is ready to be loaded
integration.knowledgeReadyForLoad = function( knowledge, distanceMax, area )
    local mission = DEC_Connaissance_GetRawMission( knowledge.source )
    return readyForLoad( knowledge, distanceMax, area, mission )
end

--- Returns true if the transport action is finished for this entity, false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.isTransportFinished = function()
    return DEC_Transport_EstTermine()
end

--- Returns true if this entity can transport the given agent, false otherwise.
-- This method can only be called by an agent.
-- @param unit Directia agent
-- @param onlyLoadable Boolean, whether or not this method will only take into
-- account components that are defined as 'loadable' in the physical database.
-- @return Boolean
integration.canTransportUnit = function( unit, onlyLoadable )
    return DEC_Agent_PeutTransporterPion( unit.source, onlyLoadable )
end

--- Returns true if this entity can transport the given agent knowledge, false otherwise.
-- This method can only be called by an agent.
-- @param unit Directia agent knowledge
-- @param onlyLoadable Boolean, whether or not this method will only take into
-- account components that are defined as 'loadable' in the physical database.
-- @return Boolean
integration.canTransportKnowledge = function( unit, onlyLoadable )
    return DEC_Connaissance_PeutTransporterPion( myself, unit.source, onlyLoadable )
end

--- Returns true if the transporting agent can transport the knowledge to transport, false otherwise.
-- @param transportingAgent Directia agent, the transporting agent
-- @param knowledgeToTransport Directia agent knowledge, the agent knowledge to transport
-- @param onlyLoadable Boolean, whether or not this method will only take into
-- account components that are defined as 'loadable' in the physical database.
-- @return Boolean
integration.canAgentTransportKnowledge = function( transportingAgent, knowledgeToTransport, onlyLoadable )
    return DEC_Connaissance_PeutTransporterPion( transportingAgent.source, knowledgeToTransport.source, onlyLoadable )
end

--- Returns the necessary number of round trips for this entity to transport the given agent.
-- This method can only be called by an agent.
-- @param unit Directia agent knowledge
-- @param onlyLoadable Boolean, whether or not this method will only take into
-- account components that are defined as 'loadable' in the physical database.
-- @return Integer
integration.transportUnitRoundTrip = function( unit, onlyLoadable )
    return DEC_Agent_TransportNombreAllerRetour( unit.source, onlyLoadable )
end

--- Returns the necessary number of round trips for this entity to transport the given agent.
-- This method can only be called by an agent.
-- @param unit Directia agent
-- @param onlyLoadable Boolean, whether or not this method will only take into
-- account components that are defined as 'loadable' in the physical database.
-- @return Integer
integration.transportKnowledgeRoundTrip = function( unit, onlyLoadable )
    return DEC_Connaissance_TransportNombreAllerRetour( myself, unit.source, onlyLoadable )
end

--- Returns true if this entity can transport crowds, false otherwise.
-- @return Boolean
integration.canTransportCrowd = function()
    return DEC_Agent_PeutTransporterFoule( myself )
end

--- Returns the necessary number of round trips for this entity to transport the given crowd.
-- @param crowd Crowd knowledge, the crowd to transport
-- @return Integer, the necessary number of round trips. 0 if this entity has no capacity to transport crowds.
integration.transportCrowdRoundTrip = function( crowd )
    local capacityTransport = DEC_Agent_GetCapacityToTransportCrowd( myself )
    local crowdsNumber = DEC_GetNombrePersonnesDansFoule( myself, crowd.source )  
    if capacityTransport > 0 then
        return math.ceil( crowdsNumber / capacityTransport )
    else
        return 0
    end
end

--- Starts loading the given concentration of the provided crowd.
-- @see integration.startedLoadCrowd
-- @see integration.stopLoadCrowd
-- @param crowd Crowd knowledge
-- @param concentration Simulation concentration
-- @return Boolean, false
integration.startLoadCrowd = function( crowd, concentration )
    reportFunction(eRC_TransportEmbarquement )
    myself.actionLoadCrowd = DEC_StartEmbarquerFouleDUneConcentration( crowd.source, concentration )
    actionCallbacks[ myself.actionLoadCrowd ] = function( arg ) myself.eEtatTransportCrowd = arg end
    return false
end

--- Continues the previously started crowd loading action.
-- Displays a report if the loading is impossible.
-- @see integration.startLoadCrowd
-- @see integration.stopLoadCrowd
-- @return Boolean, true if the loading is finished or impossible, false otherwise.
integration.startedLoadCrowd = function()
    if myself.eEtatTransportCrowd == eActionTransport_Finished then
        return true
    elseif myself.eEtatTransportCrowd == eActionTransport_Impossible then
        DEC_Trace( "transportation impossible" )
        reportFunction(eRC_TransportImpossiblePasDeMoyens )
        return true  -- loading is impossible, so let us stop here
    end
    return false
end

--- Stops the previously started crowd loading action.
-- The simulation action of loading is stopped.
-- @see integration.startLoadCrowd
-- @see integration.startedLoadCrowd
integration.stopLoadCrowd = function()
    myself.actionLoadCrowd = DEC__StopAction( myself.actionLoadCrowd )
    myself.eEtatTransportCrowd = nil
end

--- Starts unloading the last loaded crowd.
-- @see integration.startedUnloadCrowd
-- @see integration.stopUnloadCrowd
-- @param crowd Deprecated, unused
-- @return Boolean, false
integration.startUnloadCrowd = function( crowd )
    local crowdId
    if crowd == nil or crowd == NIL then
        crowdId = 0 -- if the crowd is not issued, unload current transported crowd concentration.
    else
        crowdId = crowd.source
    end
    reportFunction( eRC_TransportDebarquement )
    myself.actionUnloadCrowd = DEC_StartDebarquerFouleSurPosition( crowdId, meKnowledge:getPosition() )
    actionCallbacks[ myself.actionUnloadCrowd ] = function( arg ) myself.eEtatTransportUnloadCrowd = arg end
    return false
end

--- Continues the previously started crowd unloading action.
-- Displays a report if the unloading is impossible.
-- @see integration.startUnloadCrowd
-- @see integration.stopUnloadCrowd
-- @return Boolean, true if the unloading is finished or impossible, false otherwise.
integration.startedUnloadCrowd = function()
    if myself.eEtatTransportUnloadCrowd == eActionTransport_Finished then
        return true
    elseif myself.eEtatTransportUnloadCrowd == eActionTransport_Impossible then
        reportFunction(eRC_TransportImpossiblePasDeMoyens )
        return true  -- unloading is impossible, so let us stop here
    end
    return false
end

--- Stops the previously started crowd unloading action.
-- The simulation action of unloading is stopped.
-- @see integration.startUnloadCrowd
-- @see integration.startedUnloadCrowd
integration.stopUnloadCrowd = function()
    if myself.actionUnloadCrowd ~= nil then
        myself.actionUnloadCrowd = DEC__StopAction( myself.actionUnloadCrowd )
    end
    myself.eEtatTransportUnloadCrowd = nil
end

--- Returns true if this entity is transporting a crowd, false otherwise.
-- @return Boolean
integration.isTransportingCrowd = function()
    return DEC_Agent_TransporteFoule( myself )
end

--- Returns true if the given agent knowledge is transported, false otherwise.
-- This method can only be called by an agent.
-- @param enemy Directia agent knowledge
-- @return Boolean
integration.isTransported = function( enemy )
    return DEC_ConnaissanceAgent_EstTransporte( enemy.source )
end

--- Instantaneously leaves this entity's carriers on its current location.
-- This can be used to make this entity move inside an underground network, for instance.
-- This method can only be called by an agent.
-- @see integration.retrieveCarriers
integration.allowCarriers = function()
    DEC_LaisserTransporteursSansDelai()
end

--- Instantaneously retrieves this entity's carriers (if they have previously been left behind).
-- This method can only be called by an agent.
-- @see integration.allowCarriers
integration.retrieveCarriers = function()
    DEC_RecupererTransporteursSansDelai()
end

--- Returns true if the given agent is surrendered, false otherwise.
-- @param friend Directia agent
-- @return Boolean
integration.isAgentSurrendered = function( friend )
    return DEC_Agent_EstRendu( friend.source )
end

--- Returns true if the given agent knowledge is surrendered to this entity's side, false otherwise.
-- @param platoon Directia agent knowledge
-- @return Boolean
integration.isKnowledgeAgentSurrendered = function( platoon )
    return DEC_ConnaissanceAgent_EstRenduAMonCamp( myself, platoon.source )
end

--- Returns true if this entity is currently in the middle of transporting troops or crowds, false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.isDuringTransport = function( )
    return DEC_Transport_EnCoursDeTransport()
end

--- Returns true if the given agent has loaded its supplies, false otherwise
-- This method is used in logistic convoy supplying missions.
-- @see integration.setSuppliesLoaded
-- @param unit Directia agent
-- @return Boolean
integration.areSuppliesLoaded = function( unit )
    return unit.source:GetsuppliesLoaded_()
end

--- Sets the current supplies loading state for the given agent to the
--- provided boolean state.
-- @see integration.areSuppliesLoaded
-- @param unit Directia agent
-- @param bool Boolean, new supplies loading state
integration.setSuppliesLoaded = function( unit, bool )
    unit.source:SetsuppliesLoaded_( bool )
end

------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

--- Deprecated
integration.isFriendTranported = integration.isFriendTransported

--- Deprecated
integration.canTransportCrowdConcentration = function( crowd, concentration )
    return DEC_CrowdKnowledge_CanLoadCrowdConcentration( myself, crowd.source, concentration )
end

--- Deprecated
integration.canLoadFriend = function( unit, onlyLoadable )
    return true -- DEC_Agent_PeutTransporterPion( unit.source , onlyLoadable )
end