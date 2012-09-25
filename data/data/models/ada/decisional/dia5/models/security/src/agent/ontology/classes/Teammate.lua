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
        return self:isReached()
    end )

method "canVictimsBeExtracted" (
    function( self )
        return false  -- teammate unit cannot be extracted for now.
    end )

method "extractVictimsFromIt" (
    function( self, victimsUnit )
        return false -- teammate unit cannot be extracted for now.
    end )

-- --------------------------------------------------------------------------------
-- MedicalUnit methods implementations
-- --------------------------------------------------------------------------------
method "evacuateVictims" (
    function( self, victimsUnit )  -- This element (self) is a medical unit
        return integration.ensureVictimsEvacuation( victimsUnit, self )
    end )

method "canEvacuateVictims" ( 
    function( self, victimsUnit )
        return integration.isMedical( self )
    end )

-- --------------------------------------------------------------------------------
-- Transportable methods implementations
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
-- Reinforceable
-- --------------------------------------------------------------------------------
method "reinforceIt" ( masalife.brain.integration.startStopAction(
{ 
    start = function( self )
        return integration.beginReinforcementPlatoon( self )
    end,
    started = function( self )
        return true
    end,
    stop = function( self )
        return integration.cancelReinforcement( self )
    end
} ) )

method "canBeReinforced" (
    function( self )
    if self:isReached() then
        return true
    else
        -- $$$ MIA: maybe not the good place to do those reports.
        meKnowledge:sendReport( eRC_ImpossibleAtteindreObjectif )
        return false
    end
    end )

-- --------------------------------------------------------------------------------
-- Follower
-- --------------------------------------------------------------------------------
method "canFollowMe" (
    function( self )
        return integration.isMissionAvailable( self, "behaviors.tasks.Follow" )
    end )

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