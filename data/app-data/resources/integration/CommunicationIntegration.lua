-------------------------------------------------------------------------------
-- Communication Implementation
-- @author MGD
-- @created 2010-01-15
-- @modified MGD 2010-04-16
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 MASA Group
-------------------------------------------------------------------------------

integration.taskKnowledge = {}

integration.communication = {}

initializeAssignMissions = function()
    myself = myself or {}
    myself.ParameterRegistrationFunctor = {}
    myself.ParameterRegistrationFunctor[integration.ontology.types.area] = DEC_AssignMissionLocationParameter
    myself.ParameterRegistrationFunctor[integration.ontology.types.automat] = DEC_AssignMissionAutomatParameter
    myself.ParameterRegistrationFunctor[integration.ontology.types.population] = DEC_AssignMissionCrowdParameter
    myself.ParameterRegistrationFunctor[integration.ontology.types.object] = DEC_AssignMissionObjectKnowledgeParameter
    myself.ParameterRegistrationFunctor[integration.ontology.types.genObject] = DEC_AssignMissionGenObjectParameter
    myself.ParameterRegistrationFunctor[integration.ontology.types.point] = DEC_AssignMissionPointParameter
    myself.ParameterRegistrationFunctor[integration.ontology.types.agentKnowledge] = DEC_AssignMissionAgentKnowledgeParameter
    myself.ParameterRegistrationFunctor[integration.ontology.types.agent] = DEC_AssignMissionPionParameter
    myself.ParameterRegistrationFunctor[integration.ontology.types.urbanBlock] = DEC_AssignMissionUrbanBlockParameter
    myself.ParameterRegistrationFunctor[integration.ontology.types.resourceNetwork] = DEC_AssignMissionResourceNetworkParameter
    myself.ParameterRegistrationFunctor[integration.ontology.types.fuseau] = function( missionPion, parameterName, value) DEC_AssignerFuseauAMissionPion_Mission( value, missionPion ) end
    myself.ParameterRegistrationFunctor["number"] = DEC_AssignMissionNumericTypeParameter
    myself.ParameterRegistrationFunctor["userdata"] = DEC_AssignMissionDotationTypeParameter
    myself.ParameterRegistrationFunctor["boolean"] = DEC_AssignMissionBoolParameter

    myself.ParameterListRegistrationFunctor = {}
    myself.ParameterListRegistrationFunctor[integration.ontology.types.point] = DEC_AssignMissionPointListParameter
    myself.ParameterListRegistrationFunctor[integration.ontology.types.agentKnowledge] = DEC_AssignMissionAgentKnowledgeListParameter
    myself.ParameterListRegistrationFunctor[integration.ontology.types.agent] = DEC_AssignMissionPionListParameter
    myself.ParameterListRegistrationFunctor[integration.ontology.types.automat] = DEC_AssignMissionAutomatListParameter
    myself.ParameterListRegistrationFunctor[integration.ontology.types.object] = DEC_AssignMissionObjectKnowledgeListParameter
    myself.ParameterListRegistrationFunctor[integration.ontology.types.genObject] = DEC_AssignMissionGenObjectListParameter
    myself.ParameterListRegistrationFunctor[integration.ontology.types.area] = DEC_AssignMissionLocationListParameter
    myself.ParameterListRegistrationFunctor[integration.ontology.types.urbanBlock] = DEC_AssignMissionUrbanBlockListParameter
    myself.ParameterListRegistrationFunctor[integration.ontology.types.resourceNetwork] = DEC_AssignMissionResourceNetworkListParameter
    myself.ParameterListRegistrationFunctor[integration.ontology.types.population] = DEC_AssignMissionCrowdListParameter
end

local function InferType( param )
    if param.GetTypeName then
        return param:GetTypeName()
    elseif existsIndex(param , "__tag" ) then
        return masalife.brain.core.class.getType(param)
    else
        return nil
    end
end

local AssignMissionParameter = function ( missionPion, parameterName, parameterType, value )
    if not myself.ParameterRegistrationFunctor then initializeAssignMissions() end
    if not myself.ParameterRegistrationFunctor[ parameterType ] then error( "No parameter assignment for "..tostring( parameterType ) ) end
    return myself.ParameterRegistrationFunctor[ parameterType ](missionPion, parameterName, value )
end

local AssignMissionParameterList = function ( missionPion, parameterName, value )
    if not myself.ParameterListRegistrationFunctor then initializeAssignMissions() end
    local listParams = {}
    local nValues = #value
    if DEC_Mission_IsPath( missionPion, parameterName ) then
        for i = 1, nValues do
            local currentValue = value[i]
            if InferType( currentValue ) ~= integration.ontology.types.point then
                currentValue = value[i]:getMyPosition() -- Transform the parameter into a point
                if InferType( currentValue ) ~= integration.ontology.types.point then
                    error( "Expected point in path, received something else: "..tostring( InferType( currentValue ) ) )
                end
            end
            listParams[i] = currentValue.source
        end
        DEC_AssignMissionPathConstParameter( missionPion, parameterName, listParams )
    else
        for i = 1, nValues do
           local parameterValue = value[i]
           local valueType = InferType( parameterValue )
           if not myself.ParameterListRegistrationFunctor[valueType] then error( "No parameter assignment for list of type"..tostring( valueType ) ) end
           listParams[i] = myself.ParameterListRegistrationFunctor[valueType]( parameterValue.source )
        end
        DEC_AssignMissionListParameter( missionPion, parameterName, listParams )
    end
end

local function fillParameters( mission, params )
    for parameterName, parameterValue in pairs( params ) do
        if type(parameterValue) == "table" then
            if existsIndex(parameterValue , "__tag" ) then
                AssignMissionParameter( mission, parameterName, masalife.brain.core.class.getType(parameterValue), parameterValue.source )
            else
                local tempList = {}
                for _, value in pairs( parameterValue ) do
                    tempList[#tempList+1] = value
                end
                if next( parameterValue ) then
                    AssignMissionParameterList( mission, parameterName, tempList )
                    end
                end
        elseif type(parameterValue) == "userdata" and parameterValue.GetTypeName then
            AssignMissionParameter( mission, parameterName, parameterValue:GetTypeName(), parameterValue )
        else
            AssignMissionParameter( mission, parameterName, type(parameterValue), parameterValue )
        end
    end
end


integration.communication.StartMissionPion = function( content  )
  local mission = DEC_CreerMissionPion( content.mission_type )
  fillParameters( mission, content.mission_objectives )
  DEC_DonnerMissionPion( mission )
  F_Pion_SeteEtatEchelon( meKnowledge.source, content.echelon ) 
end

integration.communication.StartMissionPionVersPion = function( content )
  local mission = DEC_CreerMissionPionVersPion( content.mission_type )
  fillParameters( mission, content.mission_objectives ) 
  DEC_DonnerMissionPionVersPion( mission )
  F_Pion_SeteEtatEchelon( meKnowledge.source, content.echelon ) 
end

integration.communication.FollowMe = function( missionName, sender )
    local mission = DEC_CreerMissionPionVersPion( missionName )
    local followParam = DEC_AssignMissionPionListParameter( sender )   -- entities to folow: the sender of the message
    DEC_AssignMissionListParameter( mission, "entities", { followParam } )
    DEC_AssignMissionListParameter( mission, "positions", NIL )        -- positions
    DEC_AssignMissionNumericTypeParameter( mission, "minDistance", 0 ) -- distance min 0 meters
    DEC_DonnerMissionPionVersPion( mission )                           -- Issue the mission
end

integration.communication.OrderMoveToFromCommander = function( missionName, waypoints )
    local mission = DEC_CreerMissionPion( missionName )
    local tempSimPoints = {}
    for i = 1, #waypoints do
        tempSimPoints[ i ] = DEC_AssignMissionPointListParameter( waypoints[ i ].source )
    end
    DEC_AssignMissionListParameter( mission, "waypoints", tempSimPoints )
    DEC_AssignMissionBoolParameter( mission, "urgency", false )
    DEC_DonnerMissionPion( mission ) -- Issue the mission
end

integration.communication.OrderPerformWorkFromCommander = function( missionName, works, instantaneously )
    local mission = DEC_CreerMissionPion( missionName )
    local tempSimObjects = {}
    for i = 1, #works do
        tempSimObjects[ i ] = DEC_AssignMissionGenObjectListParameter( works[ i ].source )
    end
    DEC_AssignMissionListParameter( mission, "works", tempSimObjects )
    DEC_AssignMissionBoolParameter( mission, "instantaneously", instantaneously )
    DEC_DonnerMissionPion( mission ) -- Issue the mission
end


integration.communication.StartMissionAutomate = function( content )
  local mission = DEC_CreerMissionAutomate( content.entity.source, content.taskName )
  fillParameters( mission, content.params ) 
  DEC_DonnerMissionADAAutomate( mission )
end

-- Coordination Manager
integration.communication.SlowDown = function( content )
    myself.speedModulation.coordination = 0.1
end

integration.communication.Continue = function( content )
    myself.speedModulation.coordination = 1
end

integration.RetrieveAutomateTask = function( entity, targetTask )
    local fun = function( task )
        if task.name == targetTask then
            return task
        end
    end
    local task = masalife.brain.knowledge.me.tasks:accept( fun ) 
    if not task then
        error( "Task :"..tostring(targetTask).." not available in DIA5 role with body " .. tostringr( entity.type ) .. tostringr( entity ) .. "but available in mission.xml" )
    end
    do return task end
end

integration.RetrieveAutomatInBatallionTask = function( entity, targetTask )
    if DEC_IsAutomateMissionAvailable( entity.source, targetTask ) then
        local knowledgeName = integration.taskKnowledge[targetTask]
        return taskKnowledge[ knowledgeName ]
    end
    return nil
end

integration.RetrievePionTask = function( entity, targetTask )
    if DEC_IsMissionAvailable( entity.source, targetTask ) then
        local knowledgeName = integration.taskKnowledge[ targetTask ]
        if knowledgeName == nil then
            DEC_Trace( "The knowledge of the task "..tostring( targetTask ).." is missing" )
            return nil
        end
        return taskKnowledge[ knowledgeName ]
    end
    return nil
end

integration.SendMessage = function( ... )
    local status, err = pcall( masalife.brain.communication.protocol.send, ... )
    if not status then
        DEC_Trace( "Sending message failed: "..tostring( err ).." from "..debug.traceback() )
    end
    return status
end

integration.isMissionAvailable = function( entity, targetTaskName )
    if DEC_IsMissionAvailable( entity.source, targetTaskName ) then
        return true
    end
    return false
end

integration.isMissionAgentAvailable = function( entity, targetTaskName )
    return DEC_IsMissionPionAvailable( entity, targetTaskName )
end

masalife.brain.communication.setMessageTreatment( "TaskDone",
    function( message )
        local myFrontElements = integration.listenFrontElementCallbacks[meKnowledge]
        local frontElement = message.element
        if myFrontElements and frontElement then
            if myFrontElements[ frontElement ] then
                myFrontElements[ frontElement ] = nil
                if not next( myFrontElements ) then
                    myself.feedback = true
                    integration.listenFrontElementCallbacks[meKnowledge] = {}
                end
            end
        end
    end
)

integration.listenFrontElementCallbacks = {}
integration.ListenFrontElement = function( entity )
    -- comparer la connaissance meKnowledge (pion) avec la connaissance que l'automate a de ce pion ne fonctionne pas
    -- maintenant on compare l'objet c++ qui est derriere
    integration.listenFrontElementCallbacks[meKnowledge] = integration.listenFrontElementCallbacks[meKnowledge] or {}
    local listenFrontElementCallbacks = integration.listenFrontElementCallbacks[meKnowledge]
    listenFrontElementCallbacks[entity.source] = true
end

integration.getAgentFromKnowledge = function( entity )
    local agent = DEC_ConnaissanceAgent_EnAgent(entity.source)
    return CreateKnowledge(integration.ontology.types.agent, agent)
end

integration.getAgentFromAutomatKnowledge = function( entity )
    local agent = DEC_Connaissance_EnAgent( entity.source )
    return CreateKnowledge(integration.ontology.types.agent, agent)
end

function DEC_GetMission( entity )
    local mission = DEC_GetRawMission( entity )
    if entity.mission == mission then
        return entity.mission
    end
    DEC_FillMissionParameters( mission, mission )
    entity.mission = mission
    return mission
end

integration.getMission = function( entity )
    return DEC_GetMission( entity.source )
end

integration.stopMission = function()
    DEC_FinMission()
end

integration.getRawMission = function( entity )
    return DEC_GetRawMission( entity )
end

integration.getDirectionEnemy = function( mission )
    return DEC_GetDirectionEnnemi( mission )
end

integration.getUnitsWithoutHQCommunication = function( )
    return DEC_Automate_PionsSansPCCommunication( )
end

integration.hasMission = function( entity )
    return DEC_HasMission( entity )
end

integration.giveCommanderTask = function( task )
    return DEC_DonnerMissionAutomate( task )
end

integration.getDangerousDirection = function( task )
    return DEC_GetDirectionDanger( task )
end

integration.getAORFromCommander = function()
    return DEC_Fuseau()
end

integration.getAORWidth = function( AOR )
    return DEC_Fuseau_Width( AOR )
end

integration.setEchelonState = function( agent, echelonState )
    F_Pion_SeteEtatEchelon( agent, echelonState )
end

integration.getEchelonState = function( agent )
    return F_Pion_GeteEtatEchelon( agent )
end

integration.getAgentMovement = function( agent )
    return F_Pion_GetitMvt( agent)
end

integration.getAgentEscort = function( agent )
    return F_Pion_GetpionEnEscorte( agent )
end

integration.setAgentEscort = function( agentToEscort, escortAgent )
    F_Pion_SetpionEnEscorte( agentToEscort, escortAgent )
end

integration.setAgentNewEscorted = function( agent, boolean )
    F_Pion_SetNewEscorted( agent, boolean )
end

integration.getAgentNewEscorted = function( agent )
    return F_Pion_GetNewEscorted( agent )
end

integration.setRadioState = function( agent, radioState )
    F_Pion_SeteEtatRadio( agent, radioState )
end

integration.getAgentDecisionalState = function( agent )
    return F_Pion_GeteEtatDec( agent )
end

integration.setAgentDecisionalState = function( agent, decisionalState )
    F_Pion_SeteEtatDec( agent, decisionalState )
end

integration.setRadarState = function( agent, radarState )
    F_Pion_SeteEtatRadar( agent, radarState )
end

integration.getAnyType = function( any )
    return any:GetType()
end