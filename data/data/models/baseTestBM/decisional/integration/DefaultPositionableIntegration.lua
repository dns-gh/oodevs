--Positionnable Implementation
integration.takeUpPosition = function( reachable )
  return DEC_Agent_NiveauInstallation() >= 4 -- 4 corresponding to E_PostureType::ePosturePoste
end