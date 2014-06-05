-------------------------------------------------------------------
---- COMMUNICATIONINTEGRATION INTERFACE IMPLEMENTATION
-------------------------------------------------------------------

integration.taskKnowledge = {}

integration.communication = {}

initializeAssignMissions = function()
    local myself = myself
    myself.ParameterRegistrationFunctor = {}
    local integration = integration
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

--- Returns the given parameter's type
-- @param param a parameter
-- @return String, The type of the given parameter
local function InferType( param )
    if param.GetTypeName then
        return param:GetTypeName()
    elseif existsIndex(param , "__tag" ) then
        return masalife.brain.core.class.getType(param)
    else
        return nil
    end
end

-- Assign parameters to the given mission
-- @param missionPion the mission
-- @param parameterName the name of the parameter (Adaptation tool, tab 'missions', part 'parameters', column 'name')
-- @param parameterType the type of the parameter (Adaptation tool, tab 'missions', part 'parameters', column 'type')
-- @param value the value of the parameter, filled by the user when he gives a mission
-- @return Error code
local AssignMissionParameter = function ( missionPion, parameterName, parameterType, value )
    if not myself.ParameterRegistrationFunctor then initializeAssignMissions() end
    if not myself.ParameterRegistrationFunctor[ parameterType ] then error( "No parameter assignment for "..tostring( parameterType ) ) end
    return myself.ParameterRegistrationFunctor[ parameterType ](missionPion, parameterName, value )
end

-- Assign list parameters to the given mission
-- @param missionPion the mission
-- @param parameterName the name of the parameter (Adaptation tool, tab 'missions', part 'parameters', column 'name')
-- @param value the value of the parameter, filled by the user when he gives a mission
local AssignMissionParameterList = function ( missionPion, parameterName, value )
    local myself = myself
    if not myself.ParameterListRegistrationFunctor then initializeAssignMissions() end
    local listParams = {}
    local nValues = #value
    local integration = integration
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

-- Fill all parameters of the given mission
-- @param mission the mission
-- @param params a table of mission parameters values, indexed by parameter name (string) (Adaptation tool, tab 'missions', part 'parameters'). For each element we have the parameter's name and the parameter's value
local function fillParameters( mission, params )
    local masalife = masalife
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

--- Assign the mission to the calling DirectIA agent
-- This method can only be called by an agent when his automat is engaged
-- @param content a table with the following entries:
-- <ul><li> mission_type string the type of the mission </li>
-- <li> echelon the echelon (eEtatEchelon_None; eEtatEchelon_Second; eEtatEchelon_First; eEtatEchelon_Scout; eEtatEchelon_Reserve )</li>
-- <li> mission_objectives a table of mission parameter values indexed by parameter names</li>
-- </ul>
integration.communication.StartMissionPion = function( content  )
  local mission = DEC_CreerMissionPion( content.mission_type )
  fillParameters( mission, content.mission_objectives )
  DEC_DonnerMissionPion( mission )
  F_Pion_SeteEtatEchelon( meKnowledge.source, content.echelon ) 
end

--- Assign the mission to the calling DirectIA agent
-- This method can only be called by an agent
-- @param content a table with the following entries:
-- <ul><li> mission_type string the type of the mission </li>
-- <li> echelon the echelon (eEtatEchelon_None; eEtatEchelon_Second; eEtatEchelon_First; eEtatEchelon_Scout; eEtatEchelon_Reserve )</li>
-- <li> mission_objectives a table of mission parameter values indexed by parameter names</li>
-- </ul>
integration.communication.StartMissionPionVersPion = function( content )
  local mission = DEC_CreerMissionPionVersPion( content.mission_type )
  fillParameters( mission, content.mission_objectives ) 
  DEC_DonnerMissionPionVersPion( mission )
  F_Pion_SeteEtatEchelon( meKnowledge.source, content.echelon ) 
end

--- Assign the given mission to the calling DirectIA agent with the sender as parameter (usually a follow mission )
-- This method can only be called by an agent
-- @param sender the DirectIA agent, the mission's parameter
-- @param missionName the name of the task associated to the mission 
integration.communication.FollowMe = function( missionName, sender )
    local mission = DEC_CreerMissionPionVersPion( missionName )
    local followParam = DEC_AssignMissionPionListParameter( sender )   -- entities to folow: the sender of the message
    DEC_AssignMissionListParameter( mission, "entities", { followParam } )
    DEC_AssignMissionListParameter( mission, "positions", NIL )        -- positions
    DEC_AssignMissionNumericTypeParameter( mission, "minDistance", 0 ) -- distance min 0 meters
    DEC_DonnerMissionPionVersPion( mission )                           -- Issue the mission
end

--- Assign the given mission to the calling DirectIA agent with the waypoints as parameter (usually a moveTo mission )
-- This method can only be called by an agent
-- @param missionName the name of the task associated to the mission 
-- @param waypoints the list of DirectIA positions given into parameter
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

--- Assign the given mission to the calling DirectIA agent with the works as parameter (usually a build or remove mission )
-- This method can only be called by an agent
-- @param missionName the name of the task associated to the mission 
-- @param works the list of Object knowledge given into parameter
-- @param instantaneously Boolean, if true, the mission is done instantaneously (no delays, no resource used). If false, the mission takes time.
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

--- Assign the mission to the calling DirectIA automat
-- This method can only be called by an automat
-- @param content a table with the following entries:
-- <ul><li> entity the receiver automat </li>
-- <li> taskName the name of the task associated to the mission</li>
-- <li> mission_params a table of mission parameter values indexed by parameter names</li>
-- </ul>
integration.communication.StartMissionAutomate = function( content )
  local mission = DEC_CreerMissionAutomate( content.entity.source, content.taskName )
  fillParameters( mission, content.params ) 
  DEC_DonnerMissionADAAutomate( mission )
end

--- Prevent the agent from moving in front of first echelon units if it's in the second echelon (depends of the dangerous direction)
-- This method can only be called by an agent
integration.communication.StopMovingInFrontOfPE = function()
    myself.inFrontOfPE = true
end

--- Force the calling agent to slow down (usually because it is too far away from the other agents).
-- This method can only be called by an agent
-- @see integration.communication.Continue
integration.communication.SlowDown = function()
    myself.speedModulation = myself.speedModulation or {}
    myself.speedModulation.coordination = 0.01 -- scipio 
    myself.slowDown = true -- ww base
end

--- Lift the speed restrictions set by integration.communication.SlowDown.
-- This method can only be called by an agent
-- @see integration.communication.SlowDown
integration.communication.Continue = function()
    myself.speedModulation = myself.speedModulation or {}
    myself.speedModulation.coordination = 1 -- scipio 
    myself.slowDown = false -- ww base
    myself.inFrontOfPE = false
end

--- Returns true if the given automat can do the given mission (defined in adaptation tool, tab decisional model )
-- This method can only be called by an automat
-- @param entity the DirectIA automat
-- @param targetTask the task's name
-- @return Boolean, true if the given automat can do the given mission (defined in adaptation tool, tab decisional model ), false otherwise
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

--- Returns true if the given agent can do the given mission (defined in adaptation tool, tab decisional model )
-- @param entity the DirectIA agent
-- @param targetTask the task's name
-- @return Boolean, true if the given agent can do the given mission (defined in adaptation tool, tab decisional model ), false or nil otherwise
integration.RetrieveAutomatInBatallionTask = function( entity, targetTask )
    if DEC_IsAutomateMissionAvailable( entity.source, targetTask ) then
        local knowledgeName = integration.taskKnowledge[targetTask]
        return taskKnowledge[ knowledgeName ]
    end
    return nil
end

--- Returns knowledge class if the given agent can do the given mission (defined in adaptation tool, tab decisional model )
-- @param entity the DirectIA agent
-- @param targetTask the task's name
-- @return the knowledge class of the task if the given agent can do the given mission (defined in adaptation tool, tab decisional model ), nil otherwise
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

--- Sends a message to the given automat or agent
-- Any additional parameter will be used as parameters of the message.
-- @return Boolean, the error code
integration.SendMessage = function( ... )
    local status, err = pcall( masalife.brain.communication.protocol.send, ... )
    if not status then
        DEC_Trace( "Sending message failed: "..tostring( err ).." from "..debug.traceback() )
    end
    return status
end

--- Returns true if the given agent can do the given mission (defined in adaptation tool, tab decisional model )
-- @param entity the DirectIA agent
-- @param targetTaskName the task's name
-- @return Boolean, true if the given agent can do the given mission (defined in adaptation tool, tab decisional model ), false otherwise
integration.isMissionAvailable = function( entity, targetTaskName )
    if DEC_IsMissionAvailable( entity.source, targetTaskName ) then
        return true
    end
    return false
end

--- Returns true if the given agent can do the given mission (defined in adaptation tool, tab decisional model )
-- @param entity the DirectIA agent
-- @param targetTaskName the task's name
-- @return Boolean, true if the given agent can do the given mission (defined in adaptation tool, tab decisional model ), false otherwise
integration.isMissionAgentAvailable = function( entity, targetTaskName )
    return DEC_IsMissionPionAvailable( entity, targetTaskName )
end

--- Returns true if the given agent can do the given fragmentary order (defined in adaptation tool, tab decisional model )
-- @param entity the DirectIA agent
-- @param fragOrderName the name of the fragmentary order
-- @return Boolean, true if the given agent can do the given fragmentary order (defined in adaptation tool, tab decisional model ), false otherwise
integration.isFragOrderAvailable = function( entity, fragOrderName )
    return DEC_IsFragOrderAvailable( entity, fragOrderName )
end

masalife.brain.communication.setMessageTreatment( "TaskDone",
    function( message )
        local myFrontElements = integration.listenFrontElementCallbacks[meKnowledge]
        local frontElement = message.element
        if myFrontElements and frontElement then
            local myself = myself
            if myself.leadData then
                myself.leadData.finishedSubordinates = myself.leadData.finishedSubordinates or {}
                if not exists( myself.leadData.finishedSubordinates, frontElement ) then
                    myself.leadData.finishedSubordinates[ #myself.leadData.finishedSubordinates + 1 ] = frontElement
                end
            end
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
--- Add the given agent to the list of elements to listen for feedback done message
-- When all listened agents have finished their mission, the calling automat will send a feedback done
-- This method can only be called by an automat
-- @see integration.initializeListenFrontElement for initialization
-- @param entity the DirectIA agent to add to the list.
integration.ListenFrontElement = function( entity )
    integration.listenFrontElementCallbacks[meKnowledge] = integration.listenFrontElementCallbacks[meKnowledge] or {}
    local listenFrontElementCallbacks = integration.listenFrontElementCallbacks[meKnowledge]
    listenFrontElementCallbacks[entity.source] = true
end

--- Initialize the list of elements to listen for feedback done message
-- @see integration.ListenFrontElement
integration.initializeListenFrontElement = function()
    integration.listenFrontElementCallbacks[meKnowledge] = {}
end

--- If the entity is already a DirectIA agent, return it, otherwise, transform the DirectIA agent knowledge into the corresponding DirectIA agent
-- @param entity the DirectIA agent (can be a knowledge or an agent)
-- @return DirectIA agent, if the given agent is already of the good type, returns it, otherwise, returns the the associated DirectIA agent
integration.getAgentFromKnowledge = function( entity )
    if masalife.brain.core.class.isOfType( entity, integration.ontology.types.agent) then
        return entity
    else
        local agent = DEC_ConnaissanceAgent_EnAgent(entity.source)
        return CreateKnowledge(integration.ontology.types.agent, agent)
    end
end

--- Deprecated
integration.getAgentFromAutomatKnowledge = integration.getAgentFromKnowledge

--- Returns the current mision of the given agent
-- @param entity the Simulation agent
-- @return simulation mission, the current mission of the given entity
function DEC_GetMission( entity )
    local mission = DEC_GetRawMission( entity )
    if entity.mission == mission then
        return entity.mission
    end
    DEC_FillMissionParameters( mission, mission )
    entity.mission = mission
    return mission
end

--- Returns the current mision of the given agent
-- @see DEC_GetMission
-- @param entity the DirectIA agent
-- @return simulation mission, the current mission of the given entity filling with parameters
integration.getMission = function( entity )
    return DEC_GetMission( entity.source )
end

--- Stop the current mission of the calling element
integration.stopMission = function()
    DEC_FinMission()
end

--- Returns the current mision of the given agent
-- It is not possible to access parameters of the mission returned by this method. Use DEC_GetMission instead if you need to access the mission parameters
-- @see DEC_GetMission
-- @param entity the Simulation agent
-- @return simulation mission, the current mission of the given entity without any parameter
integration.getRawMission = function( entity )
    return DEC_GetRawMission( entity )
end

--- Returns the danger direction of the given mission
-- @param mission the Simulation mission
-- @return simulation direction, the danger direction of the given mission
integration.getDirectionEnemy = function( mission )
    return DEC_GetDirectionEnnemi( mission )
end

--- Returns the subordinate agents of the calling automat without the Command Post. Does not returned jammed subordinates, and returns no subordinates if the command post is jammed.
-- This method can only be called by an automat
-- @return list of simulation agent
integration.getUnitsWithoutHQCommunication = function( )
    return DEC_Automate_PionsSansPCCommunication( )
end

--- Returns if the given agent has got a mission
-- @param entity the DirectIA agent
-- @return Boolean, true if the given agent has got a mission, false otherwise
integration.hasMission = function( entity )
    return DEC_HasMission( entity )
end

--- Give the given mission to the calling automat
-- This method can only be called by an automat
-- @param task the task's name
integration.giveCommanderTask = function( task )
    DEC_DonnerMissionAutomate( task )
end

--- Returns the danger direction of the given agent
-- @param entity the DirectIA agent
-- @return simulation direction, the danger direction of the current agent's mission
integration.getDangerousDirection = function( task )
    return DEC_GetDirectionDanger( task )
end

--- Returns the area of responsibility of the calling element
-- @return simulation area of responsibility, the element AOR
integration.getAORFromCommander = function()
    return DEC_Fuseau()
end

--- Returns the area of responsibility of the calling element
-- @return simulation area of responsibility
integration.getAORFromPlatoon = function()
    return DEC_Fuseau()
end

--- Returns the area of responsibility of entity
-- @param entity Simulation agent or automat
-- @return simulation area of responsibility, the element AOR
integration.getAOR = function( entity )
    return DEC_GetFuseau( entity )
end

--- Returns true if the given AOR is not set
-- @param AOR Simulation area of responsibility
-- @return true if the given area of responsibility is null, false otherwise
integration.AORisNull = function( AOR )
	return DEC_Fuseau_IsNull( AOR )
end

--- Returns the given AOR width
-- @param AOR Simulation area of responsibility
-- @return Integer, the width of the given area of responsibility
integration.getAORWidth = function( AOR )
    return DEC_Fuseau_Width( AOR )
end

--- Sets the echelon of the given agent to the given echelon
-- @param agent Simulation agent
-- @param echelonState, the echelon state ( eEtatEchelon_None; eEtatEchelon_Second; eEtatEchelon_First; eEtatEchelon_Scout; eEtatEchelon_Reserve )
integration.setEchelonState = function( agent, echelonState )
    F_Pion_SeteEtatEchelon( agent, echelonState )
end

--- Returns the echelon of the given agent
-- @param agent, Simulation agent
-- @return Integer, the echelon state ( 0 = eEtatEchelon_None; 1 = eEtatEchelon_Second; 2 = eEtatEchelon_First; 3 = eEtatEchelon_Scout; 4 = eEtatEchelon_Reserve )
integration.getEchelonState = function( agent )
    return F_Pion_GeteEtatEchelon( agent )
end

--- Returns the path of the given agent
-- @param agent DirectIA agent
-- @return Simulation path of the given agent
integration.getAgentMovement = function( agent )
    return F_Pion_GetitMvt( agent)
end

--- Returns the escort agent of the given agent
-- @param agent DirectIA agent
-- @return Simulation agent, the escort of the given agent
integration.getAgentEscort = function( agent )
    return F_Pion_GetpionEnEscorte( agent )
end

--- Sets the escort of the given agent
-- @param agentToEscort Simulation agent to escort
-- @param escortAgent, Simulation agent escort
integration.setAgentEscort = function( agentToEscort, escortAgent )
    F_Pion_SetpionEnEscorte( agentToEscort, escortAgent )
end

--- Sets if the given agent has got a new escort
-- @param agent Simulation agent to escort
-- @param boolean, true if the given agent has got a new escort, false otherwise
integration.setAgentNewEscorted = function( agent, boolean )
    F_Pion_SetNewEscorted( agent, boolean )
end

--- Returns if the given agent has got a new escort
-- @param agent DirectIA agent to escort
-- @return Boolean, true if the given agent has got a new escort, false otherwise
integration.getAgentNewEscorted = function( agent )
    return F_Pion_GetNewEscorted( agent )
end

--- Sets the radio state of the given agent
-- @param agent Simulation agent
-- @param radioState the radio state of the given agent
-- <ul><li> 0 = eEtatRadio_Ouverte (on)</li>
-- <li> 1 = eEtatRadio_Silence (off)</li>
-- <li> 2 = eEtatRadio_Silence_Partiel (listen but do not emit)</li>
-- </ul>
integration.setRadioState = function( agent, radioState )
    F_Pion_SeteEtatRadio( agent, radioState )
end

--- Returns the decisional state of the given agent
-- @param agent DirectIA agent
-- @param Integer, the decisional state of the given agent
-- <ul><li> 0 = eEtatDec_RAS (there is no enemy)</li>
-- <li> 1 = eEtatDec_Continu (there is enemy but)</li>
-- <li> 2 = eEtatDec_Sauvegarde (the agent is in bad force ratio and will try to self-protect)</li>
-- </ul>
integration.getAgentDecisionalState = function( agent )
    return F_Pion_GeteEtatDec( agent )
end

--- Returns the decisional state of the given knowledge agent
-- @param kAgent DirectIA knowledge agent
-- @param Integer, the decisional state of the given agent
-- <ul><li> 0 = eEtatDec_RAS (there is no enemy)</li>
-- <li> 1 = eEtatDec_Continu (there is enemy but)</li>
-- <li> 2 = eEtatDec_Sauvegarde (the agent is in bad force ratio and will try to self-protect)</li>
-- </ul>
integration.getKnowledgeAgentDecisionalState = function( kAgent )
    local agent = DEC_ConnaissanceAgent_EnAgent( kAgent)
    return F_Pion_GeteEtatDec( agent )
end

--- Sets the decisional state of the given agent
-- @param agent Simulation agent
-- @param decisionalState, the decisional state of the given agent to set (eEtatDec_RAS = 0; eEtatDec_Continu = 1; eEtatDec_Sauvegarde = 2)
integration.setAgentDecisionalState = function( agent, decisionalState )
    F_Pion_SeteEtatDec( agent, decisionalState )
end

--- Sets the radar state of the given agent
-- @param agent Simulation agent
-- @param radarState the radar state of the given agent
-- <ul><li> 0 = eEtatRadar_Silence (off)</li>
-- <li> 1 = eEtatRadar_Ouvert (on)</li>
-- </ul>
integration.setRadarState = function( agent, radarState )
    F_Pion_SeteEtatRadar( agent, radarState )
end

--- Returns the type of anything (like Simulation fragmentary order, Simulation agent, Simulation crowd, DirectiA EngineerObject, DirectIA Object, DirectIA Urban Block )
-- @param any the element (can be a Simulation fragmentary order, Simulation agent, Simulation crowd, DirectiA EngineerObject, DirectIA Object, DirectIA Urban Block )
-- @return String, the type (for Simulation fragmentary order it returns his name, for Simulation agent or crowd it returns the type define in Adaptation tool )
integration.getAnyType = function( any )
    return any:GetType()
end
