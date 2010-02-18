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
    
    proximityLevel = function( urbanBlock )
        return integration.reachPriority( urbanBlock )
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
    end,
    canReconnoitreIt = function( point )
      return integration.magnitude( point, knowledges.me ) < 200
    end,
    isRecognized = function( urbanBlock )
      if not tick then --TODO MGD remove hack
        tick = 0
      end
      tick = tick + 1
      return tick > 40 --DEC_Urban_IsRecognized( urbanBlock.source )
    end
}
