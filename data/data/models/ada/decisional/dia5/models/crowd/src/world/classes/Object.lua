-- --------------------------------------------------------------------------------
-- Movement
-- --------------------------------------------------------------------------------
method "reachIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self, pathType )
        return integration.startMoveToItCrowd( self, pathType ) 
    end,
    started = function( self, pathType )
        if integration.canObjectFilterCrowds( self ) then
            meKnowledge:sendMessage( "The objective has the capacity to filter the crowd" )
        end
        return integration.updateMoveToItCrowd( self, pathType )
    end, 
    stop = function( self )
        return integration.deselectMoveToIt( self )
    end,
} ) )

-- -------------------------------------------------------------------------------- 
-- Installation
-- --------------------------------------------------------------------------------
method "canBeDamaged" (
    function( self )
        return self:isReached()
    end )

-- -------------------------------------------------------------------------------- 
-- DeconstmaintionPlot
-- --------------------------------------------------------------------------------
method "useToGetDecontaminated" (
    function( self )
        meKnowledge:sendReport( eRC_DemandeDecontamination )
        integration.requestForDecontamination( self )
        if not integration.crowdIsContamined( self ) then
            return true
        end
        return false
    end )

method "canBeUsedToGetDecontaminated" (
    function( self )
       return integration.canDecontaminateAgents( self )
    end )

method "damageIt" ( masalife.brain.integration.startStopAction( 
{
    start = function( self, quantityOfDamage )
        meKnowledge:sendReport( eRC_AttaqueInstallation )
        integration.damageObject( self, quantityOfDamage / 100 )
        return false
    end,
    started = function( self )
        return true
    end,
    stop = function( self )
        return true
    end
} ) )

method "isDamaged" (
    function( self )
       return false -- $$$ TODO constrcution level < 0
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