--Reaching Implementation

--  normalize the return value of DEC_ConnaissanceBlocUrbain_Traficabilite (wich is positif if the trafficability in UB is possible for major composante of the unit)
integration.urbanBlockTrafficabilityLevel = function( self, forLoadedState )
    return integration.isUrbanBlockTrafficable( self, forLoadedState )
end

integration.isReachingFor = function( self, objective )
end