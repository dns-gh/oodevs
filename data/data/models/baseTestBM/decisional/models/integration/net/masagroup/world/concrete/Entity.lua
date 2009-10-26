defaultMethods
{
   
}


return
{
	--Observable
	observationPriority = function( point )
		return integration.observationPriority( point )
	end,
	observeIt = function( point )
		integration.observeIt( point )
	end,
	isHostile = function( point )
		return integration.isHostile( point )
	end,
	isNeutral = function( point )
		return integration.isNeutral( point )
	end,
	isFriend = function( point )
		return integration.isFriend( point )
	end,
	isInAOR = function( point )
		return integration.isInAOR( point )
	end,
	getHostilityLevel = function( point )
		return integration.getHostilityLevel( point )
	end,
	--Reachable
	reachPriority = function( point )
		return integration.reachPriority( point )
	end,
	moveToIt = function( point )
		BreakForDebug( tostring( "moveToIt" ) )
		return integration.moveToIt( point )
	end,
	isNearby = function( point )
		return integration.isNearby( point )
	end,
	isDistant = function( point )
		return integration.isDistant( point )
	end,
	isFar = function( point )
		return integration.isFar( point )
	end,
	getHostilityLevel = function( point )
		return integration.getHostilityLevel( point )
	end,
	getProximityLevel = function( point )
		return integration.getProximityLevel( point )
	end,
	getAccessibilityLevel = function( point )
		return integration.getAccessibilityLevel( point )
	end
}