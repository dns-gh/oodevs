-- ****************************************************************************
-- GetResourceNodesWithinArea
-- Try to find Resource nodes within an area .
--
-- ****************************************************************************
queryImplementation "GetResourceNodesWithinArea" 
{
    ["execute"] = function ( params )
        local allRes = {}
        local resourceNodes = integration.getResourceNetworkNodesInZone( meKnowledge.source, params.area.source )
        for _, resourceNode in pairs( resourceNodes ) do
            allRes[ #allRes + 1 ] = CreateKnowledge( agent.ontology.classes.ResourceNode, resourceNode )
        end
        -- affichePositions( allRes ) -- $$$ MIA: to debug.
        if not next( allRes ) then
            meKnowledge:sendReport( eRC_NoResourceNodesWithinArea )
        end
        return allRes
    end
}
