return 
{
    -- -------------------------------------------------------------------------------- 
    -- Movement toward the element
    -- --------------------------------------------------------------------------------
    moveToIt = masalife.brain.integration.startStopAction( { 
        start = integration.startMoveToItCrowd,
        started = integration.updateMoveToItCrowd, 
        stop = integration.deselectMoveToIt 
    } ),
    destroyMoveToIt = function( self )
        return integration.stopMoveToItCrowd( self )
    end,
    isReached = function( self )
        return DEC_Population_HasReachedDestination( self:getPosition() )
    end,
}