-- --------------------------------------------------------------------------------
-- Delays for magic actions in MINUTES
-- --------------------------------------------------------------------------------
local loadingActionDelays   = 1
local unloadingActionDelays = 1

-- --------------------------------------------------------------------------------
-- -- Victims unit implementation
-- --------------------------------------------------------------------------------
method "canVictimsBeEvacuated" (
    function( self )
        if not self:isReached() then
            meKnowledge:sendReport( eRC_VictimsCannotBeEvacuated )
            return false -- $$$ MIA Add rule: "hasVictims"
        else
            return true
        end
    end )

method "canVictimsBeExtracted" (
    function( self )
        -- $$$ teammate unit cannot be extracted for now.
        return false
    end )

method "extractVictimsFromIt" (
    function( self, victimsUnit )
        -- $$$ MIA: TODO
        return false
    end )

-- --------------------------------------------------------------------------------
-- MedicalUnit implementation
-- --------------------------------------------------------------------------------
method "evacuateVictims" (
    function( self, victimsUnit )  -- This element (self) is a medical unit
        return integration.ensureVictimsEvacuation( victimsUnit, self )
    end )

method "canEvacuateVictims" ( 
    function( self, victimsUnit )
        -- $$$ MIA todo vérifier que self est un TC2
        return true
    end )

-- --------------------------------------------------------------------------------
-- Transportable implementation
-- --------------------------------------------------------------------------------
method "canBeLoaded" ( 
    function( self )
        return integration.canLoadFriend( self, false ) 
               and self:isReached()and not self:isLoaded()
    end )

method "canBeUnloaded" (
    function( self )
        return self:isLoaded()
    end )

method "isLoaded" (
    function( self )
        return integration.isFriendTranported( self )
    end )

method "loadIt" ( masalife.brain.integration.startStopAction(
{ 
    started = function( self )
        if waitInMin( self, loadingActionDelays ) then -- $$$ temp
            meKnowledge:sendReport( eRC_ElementLoaded )
            return integration.loadFriend( self, false )
        end
        return false
    end
} ) )

method "unloadIt" ( masalife.brain.integration.startStopAction(
{ 
    started = function( self )
        if waitInMin( self, unloadingActionDelays ) then -- $$$ temp
            meKnowledge:sendReport( eRC_ElementUnloaded )
            return integration.unloadFriend( self )
        end
        return false
    end
} ) )

-- --------------------------------------------------------------------------------
-- Information on source object
-- --------------------------------------------------------------------------------
method "getPosition" (
    function( self )
        return integration.getTeammatePosition( self )
    end )

method "getPositions" ( function( self )
    return integration.getUnitPositions( self:getPosition() )
end )

return{}