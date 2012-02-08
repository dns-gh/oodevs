-- --------------------------------------------------------------------------------
--  Creation
-- --------------------------------------------------------------------------------
method "createIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self )
        integration.startBuildItKnowledge( self )
        return self
    end,
    started = function( self )
        integration.updateBuildIt( self )
        return self
    end, 
    stop = function( self )
        integration.stopBuildIt( self )
        return self
    end
} ) )

method "isCreated" ( 
    function( self )
        return integration.buildLevel( self ) == 100
    end )

-- --------------------------------------------------------------------------------
--  Fire
-- --------------------------------------------------------------------------------
method "isExtinguished" ( 
    function( self )
        return integration.getBurningState( self ) == 0
    end )

method "extinguishIt" ( masalife.brain.integration.startStopAction( 
{ 
        start = function( self )
            return integration.startExtinguish( self )
        end, 
        started = function( self )
            return integration.updateExtinguish( self )
        end, 
        stop = function( self )
            return integration.stopExtinguish( self )
        end,
} ) )

method "canBeExtinguished" ( 
    function( self )
        return true -- $$$ MIA TODO
    end )

-- --------------------------------------------------------------------------------
-- Specific classe methods
-- --------------------------------------------------------------------------------
method "getPosition" ( 
    function( self ) 
        return integration.getObjectPosition( self )
    end )

method "getPositions" ( 
    function( self ) 
        return integration.getObjectPositions( self )
    end )

return {}