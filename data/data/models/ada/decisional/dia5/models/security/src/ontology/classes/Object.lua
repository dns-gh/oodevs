-- --------------------------------------------------------------------------------
--  Work
-- --------------------------------------------------------------------------------
method "createIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self )
        integration.startBuildItKnowledge( self )
        return self
    end,
    started = function( self )
        integration.updateBuildItSecu( self )
        return self
    end, 
    stop = function( self )
        integration.stopBuildItSecu( self )
        return self
    end
} ) )

method "isBuilt" (
    function( self )
        return integration.buildLevel( self ) == 100
    end )

method "canBeCreated" (
    function( self )
        local result = integration.canBuildNowObjectType( DEC_ConnaissanceObjet_Type( self.source ) ) 
        and integration.hasEnoughtDotationForObjectType( DEC_ConnaissanceObjet_Type( self.source ) )
        if not result then -- $$$ MIA not the right place to do this report
            meKnowledge:sendReport( eRC_ConstructionObjetImpossible )
        end
        return result
    end )

method "canBeDeconstructed" (
    function( self )
        return integration.canRemoveIt( self )
    end )

method "deconstructIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = integration.startRemoveIt, 
    started = integration.updateRemoveIt, 
    stop = integration.stopRemoveIt
} ) )

method "removeIt" (
    function( self )
        return integration.removeObjectInstantaneously( self )
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
        return true -- $$$ MIA TODO check physical capability
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