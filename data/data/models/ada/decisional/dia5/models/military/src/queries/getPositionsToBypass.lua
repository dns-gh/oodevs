queryImplementation "getPositionsToBypass" 
{ 
    ["execute"] = function ( params )
        local allRes = knowledgeManager.getQueryResult( "getPositionsToBypass" )
        if not next( allRes ) then
            allRes = {}
            local point, objectPosition, scaledObject
            for _, element in pairs ( params.elementsToBypass ) do     
                scaledObject = DEC_Geometrie_AgrandirLocalisation(  element:getLocalisation() , 50 )
                objectPosition = DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), scaledObject )
                point = CreateKnowledge( world.Point, objectPosition )
                allRes[ #allRes + 1 ] = point
            end
            knowledgeManager.setQueryResult( "getPositionsToBypass", allRes )
        end
        return allRes
    end
}