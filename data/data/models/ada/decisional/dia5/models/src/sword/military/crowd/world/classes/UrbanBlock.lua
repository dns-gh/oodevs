predicate "isDestroyed"
{
    dependencies = {},
    method = function( self )
        return self:structuralState() == 0
    end 
}
return
{
    -- -------------------------------------------------------------------------------- 
    -- Integration methods
    -- --------------------------------------------------------------------------------
    damageIt = masalife.brain.integration.startStopAction( 
    {
        start = integration.startDamageUrbanBlock,
        stop = integration.stopDamageUrbanBlock
    } ),
    
    getPosition = function( self )
        return integration.getUrbanBlockPosition( self )
    end,
    structuralState = function( self )
        return integration.getUrbanBlockState( self )
    end,
    
    isReached = function( self )
        return integration.hasReachUrbanBlock( self )
    end,
}