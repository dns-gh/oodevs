-- ****************************************************************************
-- GetAllReachableDestinationInArea
-- Return keypoints in a area. Keypoints are crossroads and urban blocks
-- ****************************************************************************
queryImplementation "GetAllReachableDestinationInArea" 
{
    [ "execute" ] = function ( params )

        local allRes = knowledgeManager.getQueryResult( "GetAllReachableDestinationInArea" )
        if next( allRes ) then return allRes end

        -- -------------------------------------------------------------------------------- 
        -- Urban blocks
        -- --------------------------------------------------------------------------------
        local urbanBlocks = integration.getUrbanBlockInsideArea( params.area.source )
        for _, urbanBlock in pairs( urbanBlocks ) do
            allRes[ #allRes + 1 ] = CreateKnowledge( agent.ontology.classes.UrbanBlock, urbanBlock )
        end

        -- -------------------------------------------------------------------------------- 
        -- Crossroads
        -- $$$ MIA A revoir. Ne pas renvoyer tous les crossroads et tester si ils sont dans
        -- la zone. Appeler une fonction qui fait ça.
        -- --------------------------------------------------------------------------------
        local crossroads = integration.getCrossroads() -- filled with agent.ontology.classes.Position
        for _, crossroad in pairs( crossroads ) do
            if integration.isPointInsideLocation( crossroad:getPosition(), params.area.source ) then
                allRes[ #allRes + 1 ] = crossroad
            end
        end

        -- -------------------------------------------------------------------------------- 
        -- Occupyable objects
        -- --------------------------------------------------------------------------------
        local simObjects = integration.getObjectsKnowledgeInZoneWithCapacity( "protection", params.area )
        for _, simObject in pairs( simObjects ) do
            allRes[ #allRes + 1 ] = CreateKnowledge( agent.ontology.classes.Object, simObject )
        end

        -- -------------------------------------------------------------------------------- 
        -- Area intersection with roads
        -- --------------------------------------------------------------------------------
        local simPositions = integration.findRoadIntersectionWithZone( params.area.source )
        for _, simPos in pairs( simPositions ) do
            allRes[ #allRes + 1 ] = CreateKnowledge( agent.ontology.classes.Position, simPos )
        end

        -- NO Key points
        if not next( allRes ) then
            meKnowledge:sendReport( eRC_NoKeyPositionsFoundInArea )
        end

        knowledgeManager.setQueryResult( "GetAllReachableDestinationInArea", allRes )
        --affichePositions( allRes )
        return allRes
    end
}