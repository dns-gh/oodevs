defaultMethods
{
   
}
return
{
    moveToIt = function( urbanBlock )
        return integration.moveToIt( urbanBlock )
    end,
    
    reachEfficiency = function( urbanBlock, keypoint )
        return integration.reachEfficiency( urbanBlock, keypoint )
    end,
	
	reachPriority = function( point )
        return integration.reachPriority( point )
    end,
    
    observeIt = function( urbanBlock )
        --TODO 
    end,
    safeApproachIt = function( urbanBlock )
        --TODO
    end,
    recce = function( urbanBlock )
        --TODO
    end,
    getReconnaissanceState = function( area )
        --TODO
    end,
    isSafety = function( area )
        --TODO
    end
}
