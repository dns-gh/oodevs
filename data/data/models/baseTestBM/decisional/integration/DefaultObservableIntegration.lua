--Default Observable Implementation
  
integration.observationPriority = function( point )
  --TODO
  return LinearInterpolation( 0, 1, 200, 5000, false, DEC_Geometrie_Distance( point.source, self.source) )
end

integration.observeIt = function( point )
  --TODO
end

integration.isHostile = function( point )
  --TODO
  return false
end

integration.isNeutral = function( point )
  --TODO
  return false
end

integration.isFriend = function( point )
  --TODO
  return false
end

integration.isInAOR = function( point )
  --TODO
  return true
end

getHostilityLevel = function( point )
  --TODO
  return 0
end