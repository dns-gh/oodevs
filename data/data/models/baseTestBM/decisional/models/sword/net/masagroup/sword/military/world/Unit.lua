defaultMethods
{
   
}
return
{
	moveToIt = function( unit )
        BreakForDebug( tostring( "moveToIt" ) )
        return integration.moveToIt( unit )
    end,
    
    observeIt = function( unit )
        --TODO 
    end,
    safeApproachIt = function( unit )
        --TODO
    end,
	
    computeDistance = function( element )
        return integration.magnitude( element, me )
    end,
    
    hostilityLevel = function()
        --TODO
        return 1
    end,
    proximityLevel = function()
        --TODO
        return 1
    end,
    accessibilityLevel = function()
        --TODO
        return 1
    end
}
