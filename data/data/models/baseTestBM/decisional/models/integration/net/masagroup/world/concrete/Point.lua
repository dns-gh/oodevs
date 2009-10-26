defaultMethods
{
   
}


return
{
	--Positionable
	canTakePosition = function( point )
		return integration.canTakePosition( point )
	end,
	takeUpPosition = function( point )
		return integration.takeUpPosition( point )
	end,
	getOccupationLevel = function( point )
		return integration.getOccupationLevel( point )
	end,
	isNearObstacle = function( point )
		return integration.isNearObstacle( point )
	end,
	--Reaching
	isReachingFor = function( point, keypoint )
		return integration.isReachingFor( point, keypoint )
	end,
	reachEfficiency = function( point, keypoint )
		return integration.reachEfficiency( point, keypoint )
	end,
	--TODO MGD move to integration
	isReached = function( point )
		return canTakePosition( point )
	end
}