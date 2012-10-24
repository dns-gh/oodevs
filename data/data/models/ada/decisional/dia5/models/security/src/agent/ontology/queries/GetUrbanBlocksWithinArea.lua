-- ****************************************************************************
-- GetUrbanBlocksWithinArea
-- Try to find UBs within an area passed as parameter.
-- ****************************************************************************
queryImplementation "GetUrbanBlocksWithinArea" 
{
    ["execute"] = function ( params )
        local allRes = {}
        local urbanBlocks = integration.getUrbanBlockInsideArea( params.area.source )
        for _, urbanBlock in pairs( urbanBlocks ) do
            allRes[ #allRes + 1 ] = CreateKnowledge( agent.ontology.classes.UrbanBlock, urbanBlock )
        end
        -- affichePositions( allRes ) -- $$$ MIA: to debug. Remove for release
        if not next( allRes ) then
            meKnowledge:sendReport( eRC_NoUrbanBlocksWithinArea )
        end
        return allRes
    end
}