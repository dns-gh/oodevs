return
{
    -- -------------------------------------------------------------------------------- 
    -- Integration methods
    -- --------------------------------------------------------------------------------
    getPosition = function( self )
        return integration.getPointPosition( self )
    end,
    attackIt = masalife.brain.integration.startStopAction( 
    {
        --start = integration.startAgressCrowd,
        --started = integration.updateAgressCrowd,
        --stop = integration.stopAgressCrowd
    } ),
}