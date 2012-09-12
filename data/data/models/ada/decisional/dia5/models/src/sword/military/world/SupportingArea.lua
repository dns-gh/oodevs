return
{
    -- -------------------------------------------------------------------------------- 
    -- Overrided high level methods
    -- --------------------------------------------------------------------------------
    canTakePosition = function( self )
        local distance = integration.distance( self.proxy, meKnowledge )
        if distance < self.distanceMin then
            return true
        else
            return false
        end
    end,
    supportEfficiency = function( self, objective )
        return ( self:proximityLevel() / 100 
          + meKnowledge:computeSupportCapability( objective, self ) ) / 100 
    end,

    -- -------------------------------------------------------------------------------- 
    -- Predicates
    -- --------------------------------------------------------------------------------
    predicate "isSupportingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:computeSupportCapability( objective, self ) > 0 and integration.isElementInAOR( self.proxy )
        end
    },
    computeSupportCapabilityFor = function( self, platoon, objective )
        if not self.trafficableComputed then
              self.trafficableComputed = { trafficable = platoon:canDismount()
              or 100 == integration.isPointInUrbanBlockTrafficable( self, false ) }
        end
        if self.trafficableComputed.trafficable and ( 
            integration.distance( objective:getMyPosition(), self:getMyPosition() ) < 300 ) then
            return 100 else return 0 end
    end
}
