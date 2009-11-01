defaultMethods
{
   
}
return
{
    moveToIt = function( area )
        return integration.moveToIt( area )
    end,
    
    reachEfficiency = function( area, keypoint )
        return integration.reachEfficiency( DEC_BMArea_Barycenter( area ), keypoint )
    end,
    
    observeIt = function( area )
        --TODO 
    end,
    safeApproachIt = function( area )
        --TODO
    end,
    recce = function( area )
        --TODO
    end
}
