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
         return ( self:proximityLevel() / 100 
           + meKnowledge:computeDestructionCapability( objective, self ) ) / 100
    end,
    neutralizationEfficiency = function( self, objective )
         return ( self:proximityLevel() / 100 
            + meKnowledge:computeNeutralisationCapability( objective, self ) ) / 100
    end,

    -- -------------------------------------------------------------------------------- 
    -- Predicates
    -- --------------------------------------------------------------------------------
    predicate "isDestroyingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return self.proxy == objective
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