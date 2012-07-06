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
            if DEC_Geometrie_DistanceBetweenPoints( self.source, DEC_Automate_PionPosition( simAgents[ i ] ) ) < reachingDistance then
                nbrOfArrivedUnits = nbrOfArrivedUnits + 1
            end
        end
        return nbrOfArrivedUnits == #simAgents
    end )

return {}