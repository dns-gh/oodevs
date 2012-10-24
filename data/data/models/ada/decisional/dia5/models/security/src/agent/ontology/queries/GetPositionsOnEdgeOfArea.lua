-- ****************************************************************************
-- GetPositionsOnEdgeOfArea
-- Return positions on the edge of area 
-- ****************************************************************************
queryImplementation "GetPositionsOnEdgeOfArea" 
{
    [ "execute" ] = function ( params )

        local allRes = knowledgeManager.getQueryResult( "GetPositionsOnEdgeOfArea" )
        if not next( allRes )  then
            allRes = {}
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
            -- affichePositions( allRes )
            knowledgeManager.setQueryResult( "GetPositionsOnEdgeOfArea", allRes )
        end
        return allRes
    end
}