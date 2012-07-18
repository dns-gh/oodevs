queryImplementation "getObstaclesInArea" 
{
    ["execute"] = function ( params ) -- Return avoidable and constructor object
        local allRes={}
        local obstaclesConstructor = {}
        local obstaclesAvoidable = {}
        obstaclesConstructor = integration.getObjectsKnowledgeInZoneWithCapacity( "constructor", params.area)
        obstaclesAvoidable = integration.getObjectsKnowledgeInZoneWithCapacity( "avoidable", params.area)
        for _, objective in pairs( obstaclesAvoidable ) do
            if exists(obstaclesConstructor, objective) then
                 allRes[ #allRes + 1 ] = CreateKnowledge( integration.ontology.types.object, objective )
            end
        end
        return allRes
    end
}