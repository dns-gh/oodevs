--Reaching Implementation

--  normalize the return value of DEC_ConnaissanceBlocUrbain_Traficabilite (wich is positif if the trafficability in UB is possible for major composante of the unit)
integration.urbanBlockTrafficabilityLevel = function( self, forLoadedState )
    return integration.isUrbanBlockTrafficable( self, forLoadedState )
end

integration.isReachingFor = function( self, objective )
end

getBetterReachingEfficiencyInList = function( elements, entity )
    local nPosition = #elements
    local bestPosition = -1
    local currentPositionReach = -1 
    local bestReturnPosition = nil
    local riverCost = -1 -- assumes reachEficiency is between 0 and 1
    local entityPosition = entity:getPosition()

    for i = 1, nPosition do
        local position = elements[i]
        currentPositionReach = position:reachEfficiency( entity )
        if not DEC_Geometrie_ArePointsOnSameRiverBank( entityPosition, position:getPosition() ) then
            currentPositionReach = currentPositionReach + riverCost
        end
        if currentPositionReach > bestPosition then
            bestPosition = currentPositionReach
            bestReturnPosition = position
        end
    end
    return bestReturnPosition
end
