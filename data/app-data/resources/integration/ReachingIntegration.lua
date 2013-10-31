--Reaching Implementation

--  normalize the return value of DEC_ConnaissanceBlocUrbain_Traficabilite (wich is positif if the trafficability in UB is possible for major composante of the unit)
integration.urbanBlockTrafficabilityLevel = function( self, forLoadedState )
    return integration.isUrbanBlockTrafficable( self, forLoadedState )
end

integration.isReachingFor = function( self, objective )
end

getBetterReachingEfficiencyInList = function( elements, entity )
    local nPosition = #elements
    local bestPosition = -math.huge
    local bestReturnPosition = nil
    local riverCost = -1 -- assumes reachEficiency is between 0 and 1
    local waterCost = -2 -- assumes reachEficiency is between 0 and 1
    local myPosition = meKnowledge:getPosition()

    for i = 1, nPosition do
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
