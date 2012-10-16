-- -------------------------------------------------------------------------------- 
-- Specific methods
-- --------------------------------------------------------------------------------
method "canBeAttacked" (
    function( self )
        return integration.unitIsInCrowd( self )
    end )

method "attackIt" ( masalife.brain.integration.startStopAction(
{ 
    start = function( self )
        integration.startAttackingIt( self )
    end,
    started = function( self )
        integration.updateAttackingIt( self )
        return integration.UnitIsNeutralized( self )
    end,
    stop = function( self )
        integration.stopAttackingIt( self )
        return true
    end
} ) )

method "getPosition" (
    function( self )
        return integration.getTeammatePosition( self ) -- $$$ TODO Not sure, a tester
    end )

method "getPositions" ( 
    function( self ) 
        return { integration.getTeammatePosition( self ) }
    end )

return {}