return
{

    -- -------------------------------------------------------------------------------- 
    -- Overrided decisional methods
    -- --------------------------------------------------------------------------------
    approachEfficiency = integration.approachEfficiency,
    -- REACHING
    reachEfficiency = integration.reachEfficiency,
    computeRelation = function( self )
        return 0 -- MGD Maybe add area occupation later
    end,
    canTakePosition = function( self )
        self.distanceMin = self.distanceMin or 0
        if integration.distance( self.proxy, meKnowledge ) < self.distanceMin
           or self:isReached() then
            return true
        else
            return false
        end
    end,
    loadEfficiency = function( self, objective )
        return self.proxy == objective and 100 or 0
    end,
    reinforcementEfficiency = function( self, objective )
        return integration.normalizedInversedDistance( objective, self ) / 100
    end,
 
    -- -------------------------------------------------------------------------------- 
    -- Predicates
    -- --------------------------------------------------------------------------------
    predicate "isReachingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return meKnowledge:
                   computeMovementCapability( objective, self ) > 0
        end
    },
    predicate "isApproachingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            if myself.location ~= self and self == objective then
                return true
            else
                local distance = myself.location 
                and myself.location:getProximity( objective )
                or objective:proximityLevel()
                return myself.location ~= self
                       and self:getProximity( objective ) > distance
            end
        end
    },

    -- -------------------------------------------------------------------------------- 
    -- Integration methods
    -- --------------------------------------------------------------------------------
    observeIt = function( self )
        return meKnowledge:orientateSensor( self.proxy )
    end,
    moveToIt = masalife.brain.integration.startStopAction( 
    {
        start = integration.startMoveToItArea,
        started = integration.updateMoveToItArea, 
        stop = integration.stopMoveToIt
     } ),
    getTraficabilityLevelFor = function( self, forLoadedState )
        -- Renvoie vrai si le point n'est pas dans un BU ou dans un BU trafficable
        local pos = self:getPosition()
        if not pos then
          return 0
        end
        return integration.isPointInUrbanBlockTrafficableForProxy( self:getMyPosition(), forLoadedState )
    end,
    takeUpPosition = function( self )
        return integration.takeUpPosition( self.proxy )
    end,
    getPosition = function( self )
        return self.proxy:getPosition()
    end,
    getMyPosition = function( self )
        local pos = self:getPosition() 
        if not pos then
            return meKnowledge:getMyPosition()
        end
        return CreateKnowledge( world_elements.Point, pos )
    end,
    getPositions = function( self )
        return integration.getPointPositionsForProxy( self:getMyPosition() )
    end,
    getProximity = function( self, objective )
        return integration.normalizedInversedDistance( self.proxy, objective )
    end,
    getPerception = function( self )
        return self.proxy:proximityLevel()
    end,
    destroyMoveToIt = function( self )
        return meKnowledge:destroyMoveToItIntegration( self )
    end
}
