-- ****************************************************************************
-- GetObjectsAroundPosition
-- Return keypoints around the urban blocks
-- ****************************************************************************
queryImplementation "GetObjectsAroundPosition" 
{
    ["execute"] = function ( params )
        local allRes={}

        local simObjects = integration.getKnowledgesObjectsInCircle( params.position.source, params.distance, { "Barrage", "CheckPoint" } )
        for _, simObject in pairs( simObjects ) do
            allRes[ #allRes + 1 ] = CreateKnowledge( agent.ontology.classes.Object, simObject )
        end

        -- No object
        if not next( allRes ) then
            meKnowledge:sendReport( eRC_NoObjectsInTheZone )
        end
        return allRes
    end
}