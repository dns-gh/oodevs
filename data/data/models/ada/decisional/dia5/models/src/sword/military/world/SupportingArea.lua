return
{
    -- -------------------------------------------------------------------------------- 
    -- Overrided high level methods
    -- --------------------------------------------------------------------------------
    canTakePosition = function( self )
        local pH = 0.5
        local maxRange = DEC_Tir_PorteeMaxPourTirer( pH )
        -- supporting position distance is less than 1/2 of fire range
        local distance = integration.distance( self.proxy, meKnowledge )
        if distance < maxRange/2 then
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
            return meKnowledge:computeSupportCapability( objective, self ) > 0
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
