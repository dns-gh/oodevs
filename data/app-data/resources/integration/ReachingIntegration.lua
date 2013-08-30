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
    for i = 1, nPosition do
        local position = elements[i]
        currentPositionReach = position:reachEfficiency( entity )
        local distance = integration.distance( position, entity)
        if currentPositionReach > bestPosition then
            bestPosition = currentPositionReach
            bestReturnPosition = position
        end
    end
    return bestReturnPosition
end