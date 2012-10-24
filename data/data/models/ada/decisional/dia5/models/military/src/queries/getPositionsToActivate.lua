queryImplementation "getPositionsToActivate" 
{ 
    ["execute"] = function ( params )
        local allRes = {}
        for _, element in pairs ( params.elementsToActivate ) do
            local scaledObject = integration.enlargeLocation( integration.getKnowledgeGenObjectLocation( element.source ), 50 )
            local objectPosition = integration.computeNearestBorder( meKnowledge:getPosition(), scaledObject )
            allRes[ #allRes + 1 ] = CreateKnowledge( world.Point, objectPosition )
        end
        return allRes
    end
}