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
include "TaskKnowledge.lua"

integration.communication = {}

initializeAssignMissions = function()
    myself = myself or {}
    myself.ParameterRegistrationFunctor = {}
    myself.ParameterRegistrationFunctor["world.Area"] = DEC_AssignMissionLocationParameter
    myself.ParameterRegistrationFunctor["world.Company"] = DEC_AssignMissionAutomatParameter
    myself.ParameterRegistrationFunctor["world.Crowd"] = DEC_AssignMissionCrowdParameter
    myself.ParameterRegistrationFunctor["world.Object"] = DEC_AssignMissionObjectKnowledgeParameter
    myself.ParameterRegistrationFunctor["world.EngineerObject"] = DEC_AssignMissionGenObjectParameter
    myself.ParameterRegistrationFunctor["world.Point"] = DEC_AssignMissionPointParameter
    myself.ParameterRegistrationFunctor["world.Platoon"] = DEC_AssignMissionAgentKnowledgeParameter
    myself.ParameterRegistrationFunctor["world.PlatoonAlly"] = DEC_AssignMissionPionParameter
    myself.ParameterRegistrationFunctor["world.UrbanBlock"] = DEC_AssignMissionUrbanBlockParameter
    myself.ParameterRegistrationFunctor["world.ResourceNetwork"] = DEC_AssignMissionResourceNetworkParameter
    myself.ParameterRegistrationFunctor["world.Fuseau"] = function( missionPion, parameterName, value) DEC_AssignerFuseauAMissionPion_Mission( value, missionPion ) end
    myself.ParameterRegistrationFunctor["security.integration.Unit"] = DEC_AssignMissionPionParameter
    myself.ParameterRegistrationFunctor["number"] = DEC_AssignMissionNumericTypeParameter
    myself.ParameterRegistrationFunctor["userdata"] = DEC_AssignMissionDotationTypeParameter
    myself.ParameterRegistrationFunctor["boolean"] = DEC_AssignMissionBoolParameter

    myself.ParameterListRegistrationFunctor = {}
    myself.ParameterListRegistrationFunctor["world.Point"] = DEC_AssignMissionPointListParameter
    myself.ParameterListRegistrationFunctor["world.Platoon"] = DEC_AssignMissionAgentKnowledgeListParameter
    myself.ParameterListRegistrationFunctor["world.PlatoonAlly"] = DEC_AssignMissionPionListParameter
    myself.ParameterListRegistrationFunctor["world.Company"] = DEC_AssignMissionAutomatListParameter
    myself.ParameterListRegistrationFunctor["world.Object"] = DEC_AssignMissionObjectKnowledgeListParameter
    myself.ParameterListRegistrationFunctor["world.EngineerObject"] = DEC_AssignMissionGenObjectListParameter
    myself.ParameterListRegistrationFunctor["world.Area"] = DEC_AssignMissionLocationListParameter
    myself.ParameterListRegistrationFunctor["world.UrbanBlock"] = DEC_AssignMissionUrbanBlockListParameter
    myself.ParameterListRegistrationFunctor["security.integration.Unit"] = DEC_AssignMissionPionListParameter
    myself.ParameterListRegistrationFunctor["world.ResourceNetwork"] = DEC_AssignMissionResourceNetworkListParameter
end

local function InferType( param )
    if param.GetTypeName then
        return param:GetTypeName()
    elseif existsIndex(param , "__tag" ) then
        return tostring( masalife.brain.core.class.getType(param) )
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
            if InferType( currentValue ) ~= "world.Point" then
                error( "Expected point in path, received something else: "..tostring( InferType( currentValue ) ) )
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
                AssignMissionParameter( mission, parameterName, tostring( masalife.brain.core.class.getType(parameterValue) ), parameterValue.source )
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
    return CreateKnowledge(world.PlatoonAlly, agent)
end

integration.getAgentFromAutomatKnowledge = function( entity )
    local agent = DEC_Connaissance_EnAgent( entity.source )
    return CreateKnowledge(world.PlatoonAlly, agent)
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