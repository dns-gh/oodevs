defaultMethods
{
   
}
return
{
    moveToIt = function( point )
        return integration.moveToIt( point )
    end,
    
    reachEfficiency = function( point, keypoint )
        return integration.reachEfficiency( point, keypoint )
    end,
    
    proximityLevel = function( point )
        return integration.reachPriority( point )
    end,
    
    reachPriority = function( point )
        return integration.reachPriority( point )
    end,
    
    observeIt = function( point )
        --TODO 
    end,
    safeApproachIt = function( point )
        --TODO
    end,
    recce = function( point )
        --TODO
    end,
    getReconnaissanceState = function( area )
        --TODO
    end,
    isSafety = function( area )
        --TODO
    end,
    canReconnoitreIt = function( point )
      return integration.magnitude( point, knowledges.me ) < 200
    end
}
