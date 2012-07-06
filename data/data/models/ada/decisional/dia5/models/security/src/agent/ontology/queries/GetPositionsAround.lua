-- ****************************************************************************
-- GetPositionsAround
-- Return keypoints around the urban blocks
-- ****************************************************************************
queryImplementation "GetPositionsAround" 
{
    ["execute"] = function ( params )
        allRes = {}

        -- -------------------------------------------------------------------------------- 
        -- Get sim positions around each urban block
        -- --------------------------------------------------------------------------------
        for _, element in pairs( params.terrainElements ) do
            for _, simPoint in pairs( element:getPositions() ) do
                allRes[ #allRes + 1 ] = CreateKnowledge( agent.ontology.classes.Position, simPoint )
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