-- --------------------------------------------------------------------------------
-- Delays for magic actions in MINUTES
-- --------------------------------------------------------------------------------
local loadingActionDelays   = 2 --min
local unloadingActionDelays = 2 --min

-- --------------------------------------------------------------------------------
-- Transport
-- --------------------------------------------------------------------------------
method "canBeLoaded" ( 
    function( self )
        if self:isReached()and not self:isLoaded() then
            return true
        else
            return false
        end
    end )

method "canBeUnloaded" (
    function( self )
        if self:isLoaded() then
            return true
        else
            return false
        end
    end )

method "loadIt" ( masalife.brain.integration.startStopAction(
{ 
    started = function( self )
        if waitInMin( self, loadingActionDelays ) then -- $$$ temp
            meKnowledge:sendReport( eRC_ElementLoaded )
            return integration.loadFriendOrFoe( self )
        end
        return false
    end
} ) )

method "unloadIt" ( masalife.brain.integration.startStopAction(
{ 
    started = function( self )
        if waitInMin( self, unloadingActionDelays ) then -- $$$ temp
            meKnowledge:sendReport( eRC_ElementUnloaded )
            return integration.unloadFriendOrFoe( self )
        end
        return false
    end
} ) )

method "isLoaded" ( 
    function( self )
        return integration.isFriendOrFoeTranported( self )
    end )

-- --------------------------------------------------------------------------------
-- Specific classe methods
-- --------------------------------------------------------------------------------
method "getPosition" ( 
    function( self )
        return integration.getPlatoonPosition( self )
    end )

method "getPositions" ( 
    function( self )
        return integration.getUnitPositions( self:getPosition() )
    end )

return {}