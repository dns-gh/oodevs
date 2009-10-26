--Positionnable Implementation

integration.canTakePosition = function( point )
  --TODO
  return DEC_Geometrie_Distance( point.source, DEC_Agent_Position() ) < 50
end

integration.takeUpPosition = function( point )
  DEC_Agent_SInstaller()
  return DEC_Agent_EstInstalle()
end

integration.getOccupationLevel = function( point )
  --TODO
  return 1
end

integration.isNearObstacle = function( point )
  --TODO
  return false
end