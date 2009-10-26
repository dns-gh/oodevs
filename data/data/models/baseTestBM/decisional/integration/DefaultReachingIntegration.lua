--Reaching Implementation
integration.isReachingFor = function( point, keypoint )
  --TODO
  return true
end

integration.reachEfficiency = function( point, keypoint )
  --TODO
  return LinearInterpolation( 0, 1, 200, 5000, false, DEC_Geometrie_Distance( point.source, keypoint.source) )
end