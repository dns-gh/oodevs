local reachingDistance = 200 -- 200 meters

-- --------------------------------------------------------------------------------
-- Specific methods 
-- --------------------------------------------------------------------------------
method "isReached" ( 
    function( self )
        local simAgents = integration.getSubordinateAgentsFromCommander( meKnowledge ) -- with CP
        result = {}
        local nbrOfArrivedUnits = 0
        for i = 1, #simAgents do
            local subordinate = CreateKnowledge( commander.ontology.Subordinate, simAgents[ i ] )
            if integration.distance( self, subordinate ) < reachingDistance then
                nbrOfArrivedUnits = nbrOfArrivedUnits + 1
            end
        end
        return nbrOfArrivedUnits == #simAgents
    end )

-- --------------------------------------------------------------------------------
-- Specific integration methods 
-- --------------------------------------------------------------------------------
method "getPosition" ( 
    function( self ) 
        return integration.getPointPosition( self )
    end )

return {}