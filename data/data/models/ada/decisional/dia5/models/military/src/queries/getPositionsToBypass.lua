queryImplementation "getPositionsToBypass" 
{ 
    ["execute"] = function ( params )
        local allRes = knowledgeManager.getQueryResult( "getPositionsToBypass" )
        if not next( allRes ) then
            allRes = {}
            local point, objectPosition, scaledObject
            for _, element in pairs ( params.elementsToBypass ) do
                scaledObject = integration.enlargeLocation( element:getLocalisation() , 50 )
                objectPosition = integration.computeNearestBorder( meKnowledge:getPosition(), scaledObject )
                point = CreateKnowledge( world.Point, objectPosition )
                allRes[ #allRes + 1 ] = point
            end
            knowledgeManager.setQueryResult( "getPositionsToBypass", allRes )
        end
        return allRes
    end
}