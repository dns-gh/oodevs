-- ****************************************************************************
-- GetAllReachableDestinationsAroundPosition
-- Return keypoints in a circle defined by radius issued as paramter . 
-- Keypoints are crossroads and urban blocks
-- ****************************************************************************
queryImplementation "GetAllReachableDestinationsAroundPosition" 
{
    ["execute"] = function ( params )

        local allRes = knowledgeManager.getQueryResult( "GetAllReachableDestinationsAroundPosition" )
        if next( allRes ) then return allRes end

        local simLoc = DEC_Geometrie_CreerLocalisationCercle( params.position:getPosition(), params.radius )

        -- -------------------------------------------------------------------------------- 
        -- Urban blocks
        -- --------------------------------------------------------------------------------
        local urbanBlocks = DEC_Connaissances_BlocUrbainDansZone( simLoc )
        for _, urbanBlock in pairs( urbanBlocks ) do
            allRes[ #allRes + 1 ] = CreateKnowledge( agent.ontology.classes.UrbanBlock, urbanBlock )
        end

        -- -------------------------------------------------------------------------------- 
        -- Crossroads
        -- --------------------------------------------------------------------------------
        local crossroads = {}
        DEC_Crossroads( crossroads ) -- filled with military.world.classes.sim.Point
        for _, crossroad in pairs( crossroads ) do
            if DEC_Geometrie_EstPointDansLocalisation( crossroad:getPosition(), simLoc ) then
                allRes[ #allRes + 1 ] = crossroad
            end
        end

        -- NO Key points
        if not next( allRes ) then
            meKnowledge:sendReport( eRC_NoKeyPositionsFoundInArea )
        end
        knowledgeManager.setQueryResult( "GetAllReachableDestinationsAroundPosition", allRes )

        --affichePositions( allRes )
        return allRes
    end
}