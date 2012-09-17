return
{
    -- -------------------------------------------------------------------------------- 
    -- High level methods
    -- --------------------------------------------------------------------------------
    canTakePosition = function( self )
        return integration.isInFiringRange( self.proxy ) 
               and self.proxy:isPerceived()
               and self.proxy:isNearby()
               and meKnowledge:isInCrowd()
    end,
    destructionEfficiency = function( self, objective )
         return self:proximityLevel() / 100
    end,
    neutralizationEfficiency = function( self, objective )
         return self:proximityLevel() / 100
    end,

    -- -------------------------------------------------------------------------------- 
    -- Predicates
    -- --------------------------------------------------------------------------------
    predicate "isDestroyingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return self.proxy == objective and integration.isElementInAOR( self )
        end
    },
    predicate "isNeutralizingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return self.proxy == objective
        end
    }
}