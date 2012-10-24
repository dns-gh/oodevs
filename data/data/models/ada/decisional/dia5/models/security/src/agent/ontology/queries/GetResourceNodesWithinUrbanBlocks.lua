-- ****************************************************************************
-- GetResourceNodesWithinUrbanBlocks
-- Try to find Resource nodes within urban blocks.
--
-- ****************************************************************************
queryImplementation "GetResourceNodesWithinUrbanBlocks" 
{
    ["execute"] = function ( params )
        local allRes = {}
        for _, urbanBlock in pairs( params.urbanBlocks ) do
            local simUrbanBlockArea = integration.polylineUrbanBlock( urbanBlock.source )
            local simResourceNodes = integration.getResourceNetworkNodesInZone( nil, simUrbanBlockArea )
            for _, simResourceNode in pairs( simResourceNodes ) do
                allRes[ #allRes + 1 ] = CreateKnowledge( agent.ontology.classes.ResourceNode, simResourceNode )
            end
        end

        -- affichePositions( allRes ) -- $$$ MIA: to debug. Remove for release
        if not next( allRes ) then
            meKnowledge:sendReport( eRC_NoResourceNodesWithinArea )
        end
        return allRes
    end
}
