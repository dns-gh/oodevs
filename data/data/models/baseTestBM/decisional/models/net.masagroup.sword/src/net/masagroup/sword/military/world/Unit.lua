defaultMethods
{
    proximityLevel = function() return default_engine.methods.load( "proximityLevel") end,
    accessibilityLevel = function() return default_engine.methods.load( "accessibilityLevel") end,
    perceptionLevel = function() return default_engine.methods.load( "element_perceptionLevel") end,
    isReachingFor = function() return default_engine.predicates.load( "isReachingFor" ) end,
    observationPriority = function() return default_engine.methods.load( "generic_observationPriority" ) end,
}
return
{
    moveToIt = function( unit )
        return integration.moveToIt( unit )
    end,
    
    observeIt = function( unit )
        --TODO 
    end,   
    computeDistance = function( element )

        return integration.magnitude( element, kBase.me.body )
    end,
    
    hostilityLevel = function()
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
    selfProtect = function( target )
      --TODO
    end,
    computeReconnaissanceCapability = function( target )
    	--TODO
    end,
    
}
