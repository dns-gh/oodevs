--Communication Implementation
integration.communication = {}

local ParameterRegistrationFunctor = {}
local initializeAssignMissions = function()
  ParameterRegistrationFunctor["net.masagroup.sword.military.world.Area"] = DEC_AssignMissionLocationParameter
  ParameterRegistrationFunctor["net.masagroup.sword.military.world.Compagnie"] = DEC_AssignMissionAutomatParameter
  ParameterRegistrationFunctor["net.masagroup.sword.military.world.Object"] = DEC_AssignMissionObjectKnowledgeParameter
  ParameterRegistrationFunctor["net.masagroup.sword.military.world.Point"] = DEC_AssignMissionPointParameter
  ParameterRegistrationFunctor["net.masagroup.sword.military.world.Section"] = DEC_AssignMissionPionParameter
  ParameterRegistrationFunctor["net.masagroup.sword.military.world.UrbanBlock"] = DEC_AssignMissionUrbanBlockParameter
end


local AssignMissionParameter = function ( missionPion, parameterName, parameterType, value )
    if not ParameterRegistrationFunctor[ parameterType ] then initializeAssignMissions() end
    if not ParameterRegistrationFunctor[ parameterType ] then error( "No parameter assignment for "..tostring( parameterType ) ) end
    return ParameterRegistrationFunctor[ parameterType ](missionPion, parameterName, value )
end

integration.communication.StartMissionPion = function( content  )
  local mission = DEC_CreerMissionPion( content.params.mission_type )
  for parameterName, list in pairs( content.params.mission_objectives ) do
    for _, value in pairs( list ) do
      AssignMissionParameter( mission, parameterName, tostring( value:getClass() ), value.source )
    end
  end
  DEC_DonnerMissionPion( mission )
end

--TODO MGD generate from mission.xml
local missionConverter = {
  ["net.masagroup.sword.military.tasks.MoveTowardCrossroad"] = { ["net.masagroup.sword.military.world.Point"] = "T_Mission_Pion_MoveToPoint",
                                                                 ["net.masagroup.sword.military.world.UrbanBlock"] = "T_Mission_Pion_MoveToUrbanBlock" }
}
integration.communication.ConvertMission = function( missionType, params )
  BreakForDebug( "type"..missionType )
  BreakForDebug( "param"..tostring(params.objectives[1]:getClass()) )
  
  if not missionConverter[ missionType ][ tostring(params.objectives[1]:getClass()) ] then error( "No mission converter for :"..missionType..","..tostring(params.objectives[1]:getClass()) ) end
  return missionConverter[ missionType ][ tostring(params.objectives[1]:getClass()) ]
end 