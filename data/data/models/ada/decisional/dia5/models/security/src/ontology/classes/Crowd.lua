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
        local hasWoundedHumans = integration.crowdHasWoundHumans( self )
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

method "extractVictimsFromIt" (
    function( self )
        return integration.extractVictimsFromCrowd( self )
    end )

-- --------------------------------------------------------------------------------
-- Transport
-- --------------------------------------------------------------------------------
method "canBeLoaded" ( 
    function( self )
        if integration.isTransportingCrowd() then -- $$$ MIA hack.
            return true
        end
        if self:isReached() then -- check if there is a concentration to load
            local concentration = integration.getNearbyConcentration( self, self, 100 )
            if concentration ~= nil then
                if integration.canTransportCrowd() then -- agent has physical capability?
                    return true
                end
            end
        end
        return false
    end )

method "canBeUnloaded" (
    function( self )
        if not integration.isTransportingCrowd() then
            return false
        else
            return true
        end
    end )

method "isLoaded" ( 
    function( self ) 
        return integration.isTransportingCrowd()
    end )

method "loadIt" ( masalife.brain.integration.startStopAction( 
{
    start = function( self )
        local concentration = integration.getNearbyConcentration( self, self, 100 )
        if concentration ~= nil then
            return integration.startLoadCrowd( self, concentration )
        else
            return false
        end
    end,
    started = function( self )
        return integration.startedLoadCrowd( self )
    end,
    stop = function( self )
        return integration.stopLoadCrowd( self )
    end,
} ) )

method "unloadIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self, position )
        local bodyPosition = CreateKnowledge( ontology.classes.Position, meKnowledge:getPosition() )
        return integration.startUnloadCrowd( self, bodyPosition )
    end,
    started = function( self )
        return integration.startedUnloadCrowd( self )
    end, 
    stop = function( self )
        return integration.stopUnloadCrowd( self )
    end,
} ) )

-- --------------------------------------------------------------------------------
-- Wounded
-- --------------------------------------------------------------------------------
method "isWounded" ( 
    function( self )
        return integration.crowdHasWoundHumans( self )
    end )

method "healIt" ( 
    function( self )
        return integration.healWoundedInCrowd( self )
    end )

method "canBeHealed" ( 
    function( self )
        return self:isReached() -- add physical capabilities
    end )

-- --------------------------------------------------------------------------------
-- Specific crowd transportation methods
-- --------------------------------------------------------------------------------
method "hasConcentration" ( 
    function( self, position )
        local concentration = integration.getNearbyConcentration( self, position, 100 )
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