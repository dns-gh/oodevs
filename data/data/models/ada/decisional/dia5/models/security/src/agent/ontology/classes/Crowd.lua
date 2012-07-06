-- --------------------------------------------------------------------------------
-- Delays for magic actions in MINUTES
-- --------------------------------------------------------------------------------
local healActionDelays        = 2
local extractionActionDelays  = 2

-- --------------------------------------------------------------------------------
-- Distance to find crowd's concentration around in METERS
-- --------------------------------------------------------------------------------
local crowdConcentrationRadius = 100

-- --------------------------------------------------------------------------------
-- VictimsUnit implementation
-- --------------------------------------------------------------------------------
method "canVictimsBeEvacuated" (
    function( self )
        -- $$$ MIA TODO: evacuate victims toward medical unit
        return false -- not for now.
    end )

method "canVictimsBeExtracted" (
    function( self )
        local hasWoundedHumans = self:isWounded()
        if self:isReached() and hasWoundedHumans then
            return true
        else
            meKnowledge:sendReport( eRC_VictimsCannotBeExtracted )
            if not hasWoundedHumans then
                meKnowledge:sendReport( eRC_NoWoundedVictims )
            end
            return false
        end
    end )

method "extractVictimsFromIt" ( masalife.brain.integration.startStopAction(
{
    start = function( self )
        meKnowledge:sendReport( eRC_StartingToExtractVictimsFromCrowd )
        return false
    end,
    started = function( self )
        if waitInMin( self, extractionActionDelays ) then
            meKnowledge:sendReport( eRC_VictimsExtracted )
            return integration.extractVictimsFromCrowd( self ) -- magic action
        end
        return false
    end
} ) )

-- --------------------------------------------------------------------------------
-- Wounded
-- --------------------------------------------------------------------------------
method "isWounded" ( 
    function( self )
        return integration.crowdHasWoundHumans( self )
    end )

method "healIt" ( masalife.brain.integration.startStopAction(
{
    start = function( self )
        meKnowledge:sendReport( eRC_StartingToHealCrowd )
        return false
    end,
    started = function( self )
        if waitInMin( self, healActionDelays ) then -- $$$ temp
            meKnowledge:sendReport( eRC_WoundedPersonsHealed )
            return integration.healWoundedInCrowd( self ) -- magic
        end
        return false
    end
} ) )

method "canBeHealed" (
    function( self )
        local hasWoundedHumans = self:isWounded()
        if self:isReached() and hasWoundedHumans then
            return true
        else
            meKnowledge:sendReport( eRC_WoundedCannotBeHealed )
            if not hasWoundedHumans then
                meKnowledge:sendReport( eRC_NoWoundedVictims )
            end
            return false
        end
    end )

-- --------------------------------------------------------------------------------
-- Specific crowd transportation methods
-- --------------------------------------------------------------------------------
method "hasConcentration" ( 
    function( self, position )
        local concentration = integration.getNearbyConcentration( self, position, crowdConcentrationRadius )
        if concentration == 0 then -- no concentration found
            return false
        end
        return true
    end )

-- --------------------------------------------------------------------------------
-- Sim position methods
-- --------------------------------------------------------------------------------
method "getPosition" ( 
    function( self )
        -- The agent moves to the nearest position (not necessary the head)
        return integration.getCrowdPosition( self )
    end )

method "getPositions" ( 
    function( self )
        return { self:getPosition() }
    end )

return {}