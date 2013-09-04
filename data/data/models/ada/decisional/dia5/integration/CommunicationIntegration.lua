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
    myself.ParameterRegistrationFunctor["sword.military.world.Area"] = DEC_AssignMissionLocationParameter
    myself.ParameterRegistrationFunctor["sword.military.world.Company"] = DEC_AssignMissionAutomatParameter
    myself.ParameterRegistrationFunctor["sword.military.world.Crowd"] = DEC_AssignMissionCrowdParameter
    myself.ParameterRegistrationFunctor["sword.military.world.Object"] = DEC_AssignMissionObjectKnowledgeParameter
    myself.ParameterRegistrationFunctor["sword.military.world.EngineerObject"] = DEC_AssignMissionGenObjectParameter
    myself.ParameterRegistrationFunctor["sword.military.world.Point"] = DEC_AssignMissionPointParameter
    myself.ParameterRegistrationFunctor["sword.military.world.Platoon"] = DEC_AssignMissionAgentKnowledgeParameter
    myself.ParameterRegistrationFunctor["sword.military.world.PlatoonAlly"] = DEC_AssignMissionPionParameter
    myself.ParameterRegistrationFunctor["sword.military.world.UrbanBlock"] = DEC_AssignMissionUrbanBlockParameter
    myself.ParameterRegistrationFunctor["sword.military.world.ResourceNetwork"] = DEC_AssignMissionResourceNetworkParameter
    myself.ParameterRegistrationFunctor["sword.military.world.Fuseau"] = function( missionPion, parameterName, value) DEC_AssignerFuseauAMissionPion_Mission( value, missionPion ) end
    myself.ParameterRegistrationFunctor["security.integration.Unit"] = DEC_AssignMissionPionParameter
    myself.ParameterRegistrationFunctor["number"] = DEC_AssignMissionNumericTypeParameter
    myself.ParameterRegistrationFunctor["userdata"] = DEC_AssignMissionDotationTypeParameter
    myself.ParameterRegistrationFunctor["boolean"] = DEC_AssignMissionBoolParameter

    myself.ParameterListRegistrationFunctor = {}
    myself.ParameterListRegistrationFunctor["sword.military.world.Point"] = DEC_AssignMissionPointListParameter
    myself.ParameterListRegistrationFunctor["sword.military.world.Platoon"] = DEC_AssignMissionAgentKnowledgeListParameter
    myself.ParameterListRegistrationFunctor["sword.military.world.PlatoonAlly"] = DEC_AssignMissionPionListParameter
    myself.ParameterListRegistrationFunctor["sword.military.world.Company"] = DEC_AssignMissionAutomatListParameter
    myself.ParameterListRegistrationFunctor["sword.military.world.Object"] = DEC_AssignMissionObjectKnowledgeListParameter
    myself.ParameterListRegistrationFunctor["sword.military.world.EngineerObject"] = DEC_AssignMissionGenObjectListParameter
    myself.ParameterListRegistrationFunctor["sword.military.world.Area"] = DEC_AssignMissionLocationListParameter
    myself.ParameterListRegistrationFunctor["sword.military.world.UrbanBlock"] = DEC_AssignMissionUrbanBlockListParameter
    myself.ParameterListRegistrationFunctor["security.integration.Unit"] = DEC_AssignMissionPionListParameter
    myself.ParameterListRegistrationFunctor["sword.military.world.ResourceNetwork"] = DEC_AssignMissionResourceNetworkListParameter
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
            if InferType( currentValue ) ~= "sword.military.world.Point" then
                currentValue = value[i]:getMyPosition() -- Transform the parameter into a point
                if InferType( currentValue ) ~= "sword.military.world.Point" then
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

-- Coordination Manager
integration.communication.SlowDown = function( content )
    myself.speedModulation.coordination = 0.1
end

-- Coordination Manager
integration.communication.StopMovingInFrontOfPE = function( content )
    myself.inFrontOfPE = true
end

integration.communication.Continue = function( content )
    myself.speedModulation.coordination = 1
    myself.inFrontOfPE = false
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

integration.RetrievePionTask = function( entity, targetTask )
    if DEC_IsMissionAvailable( entity.source, targetTask ) then
        local knowledgeName = integration.taskKnowledge[targetTask]
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

integration.initializeListenFrontElement = function()
    integration.listenFrontElementCallbacks[meKnowledge] = {}
end

integration.getAgentFromKnowledge = function( entity )
    local agent = DEC_ConnaissanceAgent_EnAgent(entity.source)
    return CreateKnowledge(sword.military.world.PlatoonAlly, agent)
end

integration.getAgentFromAutomatKnowledge = function( entity )
    local agent = DEC_Connaissance_EnAgent( entity.source )
    return CreateKnowledge(sword.military.world.PlatoonAlly, agent)
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