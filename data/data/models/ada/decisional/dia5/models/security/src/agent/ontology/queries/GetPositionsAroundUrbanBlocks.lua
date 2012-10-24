-- ****************************************************************************
-- GetPositionsAroundUrbanBlocks
-- Return keypoints around the urban blocks
-- ****************************************************************************
queryImplementation "GetPositionsAroundUrbanBlocks" 
{
    [ "execute" ] = function ( params )
        local allRes = {}

        -- -------------------------------------------------------------------------------- 
        -- Get sim positions around each urban block
        -- --------------------------------------------------------------------------------
        for _, urbanBlock in pairs( params.urbanBlocks ) do
            for _, simPoint in pairs( urbanBlock:getPositions() ) do
                if not integration.isPointInsideUrbanBlock( simPoint, urbanBlock.source ) then -- just points around, no the barycenter.
                    allRes[ #allRes + 1 ] = CreateKnowledge( agent.ontology.classes.Position, simPoint )
                end
            end
        end

        -- NO Key points
        if not next( allRes ) then
            meKnowledge:sendReport( eRC_NoKeyPositionsFoundInArea )
        end
        -- affichePositions( allRes )
        return allRes
    end
}