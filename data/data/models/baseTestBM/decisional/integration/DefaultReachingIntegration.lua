--Reaching Implementation
integration.isReachingFor = function( point, keypoint )
  return true
end

integration.reachEfficiency = function( point, keypoint )
  return LinearInterpolation( 0, 1, 200, 5000, false, integration.magnitude( point, keypoint ) )
end