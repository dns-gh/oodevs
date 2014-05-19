--- Deprecated, use integration.isUrbanBlockTrafficable instead
integration.urbanBlockTrafficabilityLevel = integration.isUrbanBlockTrafficable

--- Deprecated
integration.isReachingFor = function( self, objective )
end

--- Returns the best position to reach the given entity among the given list of positions.
-- @param elements List of knowledges, each defining a "reachEfficiency" method returning a float between 0 and 1,
-- and a "getPosition" method returning a simulation position.
-- @param entity Knowledge to reach
-- @return Knowledge
getBetterReachingEfficiencyInList = function( elements, entity )
    local bestPosition = -math.huge
    local bestReturnPosition = nil
    local riverCost = -1 -- assumes reachEficiency is between 0 and 1
    local waterCost = -2 -- assumes reachEficiency is between 0 and 1
    local myPosition = meKnowledge:getPosition()

    for i = 1, #elements do
        local position = elements[i]
        local currentPositionReach = position:reachEfficiency( entity )
        if DEC_Geometrie_IsWaterInBetween(  myPosition, position:getPosition() ) then
            currentPositionReach = currentPositionReach + waterCost
        elseif DEC_Geometrie_IsLinearRiverInBetween(  myPosition, position:getPosition() ) then
            currentPositionReach = currentPositionReach + riverCost
        end
        if currentPositionReach > bestPosition then
            bestPosition = currentPositionReach
            bestReturnPosition = position
        end
    end
    return bestReturnPosition
end
