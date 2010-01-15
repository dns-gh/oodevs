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
  BreakForDebug( tostring(content.params.mission_type) )
  local mission = DEC_CreerMissionPion( content.params.mission_type )
  for parameterName, list in pairs( content.params.mission_objectives ) do
    for _, value in pairs( list ) do
      AssignMissionParameter( mission, parameterName, tostring(value:getClass()), value.source )
    end
  end
  DEC_DonnerMissionPion( mission )
end