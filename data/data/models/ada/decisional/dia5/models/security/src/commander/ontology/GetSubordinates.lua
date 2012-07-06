-- ****************************************************************************
-- GetSubordinates
-- Return the subordinate units
-- ****************************************************************************
queryImplementation "GetSubordinates" 
{
    [ "execute" ] = function ( params )
        local simAgents = integration.getSubordinateAgentsFromCommander( meKnowledge ) -- with CP
        result = {}
        for i = 1, #simAgents do
            local simAgent = simAgents[ i ]
            result[ #result + 1 ] = CreateKnowledge( commander.ontology.Subordinate, simAgent )
        end
        return result
    end
}