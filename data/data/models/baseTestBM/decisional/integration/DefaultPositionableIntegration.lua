--Positionnable Implementation
integration.takeUpPosition = function( reachable )
  BreakForDebug( "posture"..tostring(DEC_Agent_NiveauInstallation()) )
  return DEC_Agent_NiveauInstallation() >= 4 -- 4 corresponding to E_PostureType::ePosturePoste
end