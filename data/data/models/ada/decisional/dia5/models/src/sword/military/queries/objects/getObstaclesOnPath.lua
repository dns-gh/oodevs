local CheckEqualObjects = function( object, result )
    if #result > 0  then
        local currentRes = result[ 1 ]
        if DEC_Geometrie_PositionsEgales( DEC_Geometrie_CalculerBarycentreLocalisation(  DEC_ConnaissanceObjet_Localisation( object ) ) , DEC_Geometrie_CalculerBarycentreLocalisation( DEC_ConnaissanceObjet_Localisation( currentRes.source ) ) )  then
            return currentRes
        end
    end
end

queryImplementation "getObstaclesOnPath" 
{ 
    [ "execute" ] = function ( params )

        local result = knowledgeManager.getQueryResult( "getObstacleOnPath" )
        local currentObstacle = {}
        local object = DEC_GetNextObjectOnPath( nil, 0.0, {} )
        -- Object on the path 
        if object.first then
            -- Check if object knowledge is already in result table else create knowledge of it
            local objectToAdd = CheckEqualObjects( object.second.first, result )
            if not objectToAdd then
                local kn = CreateKnowledge( integration.ontology.types.object, object.second.first ) 
                if not kn:isFriend() then
                    currentObstacle = { kn }
                else
                    currentObstacle = { }
                end
            else
                currentObstacle = { objectToAdd }
            end
        end
        knowledgeManager.setQueryResult( "getObstacleOnPath", currentObstacle )
        return currentObstacle
    end
}
