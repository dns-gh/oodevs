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
    end,
    --Abstract function from net.masagroup.military.generic.world.unit define in conceptual unit
    destroy = function( target )
      return integration.destroyIt( target )
    end,
    neutralize = function( target )
      return integration.destroyIt( target )
    end,
    computeAggressiveness = function ( target )
    	return 1
    end,
    identifyIt = function( target )
    	--TODO
    end,
    getAttrition = function( target )
    	--TODO
    end,
    getEstimateAttrition = function( target )
    	--TODO
    end,
    respondToIt = function( target )
    	--TODO
    end,
    getAttrition = function( target )
    	--TODO
    end,
    computeSupportCapability = function( target )
    	--TODO
    end,
    computeEstimateDestroyAttritionOnEnemy = function( target )
    	--TODO
    end,
    computeEstimateNeutralizeAttritionOnEnemy = function( target )
    	--TODO
    end,
     computeEstimateAttritionOnMe = function( target )
    	--TODO
    end,
    setFront = function( target )
    	--TODO
    end,
    setSupport = function( target )
    	--TODO
    end,
    computeReconnaissanceCapability = function( target )
    	--TODO
    end
}
