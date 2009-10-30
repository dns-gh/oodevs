defaultMethods
{
   
}
return
{
	moveToIt = function( point )
        BreakForDebug( tostring( "moveToIt" ) )
        return integration.moveToIt( point )
    end,
    
    reachEfficiency = function( point, keypoint )
        return integration.reachEfficiency( point, keypoint )
    end,
}
