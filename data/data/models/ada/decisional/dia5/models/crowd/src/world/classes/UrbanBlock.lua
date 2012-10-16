-- -------------------------------------------------------------------------------- 
-- Installation
-- --------------------------------------------------------------------------------
method "canBeDamaged" (
    function( self )
        return integration.hasReachUrbanBlock( self )
    end )

-- -------------------------------------------------------------------------------- 
-- Specific classe methods
-- --------------------------------------------------------------------------------
method "isDamaged" (
    function( self )
        return integration.getUrbanBlockState( self ) < 100
    end )

method "damageIt" ( masalife.brain.integration.startStopAction( 
{
    start = function( self, quantityOfDamage )
        meKnowledge:sendReport( eRC_AttaqueInstallation )
        self.initState = integration.getUrbanBlockState( self ) * 100
        integration.startDamageUrbanBlock( self )
        return false
    end,
    started = function( self, quantityOfDamage )
        local quatityDamaged = self.initState - integration.getUrbanBlockState( self ) * 100
        if ( integration.getUrbanBlockState( self ) == 0 ) or 
           ( quatityDamaged >= quantityOfDamage ) then
            return true
        end
        return false
    end,
    stop = function( self, quantityOfDamage )
        return integration.stopDamageUrbanBlock( self )
    end
} ) )

method "getPosition" ( 
    function( self )
        return integration.getUrbanBlockPosition( self )
    end )

method "getPositions" ( 
    function( self )
        return integration.getUrbanBlockPositions( self )
    end )

return {}