require "debug"
-- -------------------------------------------------------------------------------- 
-- Predicates
-- --------------------------------------------------------------------------------
predicate "isReachingFor"
{
    dependencies = "none",
    method = function( self, objective )
        return objective:getProximity( self ) > 0
    end,
}
predicate "isInMyAOR"
{
    dependencies = "none",
    method = function( self ) 
        return true 
    end,
}

return
{
    -- -------------------------------------------------------------------------------- 
    -- Decisionnal methods
    -- --------------------------------------------------------------------------------
    reachEfficiency =function( source, target )
        return ( ( source:proximityLevel() / 100 + source:getProximity( target ) ) ) / 100
    end,
    occupationLevel = function( self )
        return 0         -- not useed yet
    end,
    isOccupied = function( self )
        return false         -- not useed yet
    end,

    -- -------------------------------------------------------------------------------- 
    -- Integration methods
    -- --------------------------------------------------------------------------------
    moveToIt = function( self, pathType )
        return integration.moveToItItinerary( self ) -- spécifique à cet objet.
    end,
    destroyMoveToIt = function( self )
        return meKnowledge:destroyMoveToItIntegration( self )
    end,

    -- observable action
    observeIt = function( self )
        return meKnowledge:orientateSensor( self:getMyPosition() )
    end,
    
    -- -------------------------------------------------------------------------------- 
    -- Specific class methods used by integration methods
    -- --------------------------------------------------------------------------------
    getMyPosition = function( self )
        return self -- itinerary is a reaching position. Assumption 
    end,
    getPosition = function( self )
        return integration.getItineraryPosition( self ) -- nil if the pathfind is not found
    end,
    getPositions = function( self )
        return self:getPosition()
    end,
    getProximity = function( self, reachable )
        return integration.normalizedInversedDistance( self, reachable )
    end,
    getTraficabilityLevelFor = function( self, forLoadedState )
       return true
    end,
    canTakePosition = function( self )
        return false
    end,
    takeUpPosition = function( self )
        return integration.takeUpPosition( self )
    end,
    isContaminated = function( self )
        
    end
}