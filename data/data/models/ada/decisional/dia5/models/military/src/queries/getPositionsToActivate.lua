queryImplementation "getPositionsToActivate" 
{ 
    ["execute"] = function ( params )
        local allRes = {}
        for _, element in pairs ( params.elementsToActivate ) do
            local scaledObject = DEC_Geometrie_AgrandirLocalisation( DEC_GenObjectKnowledge_Localisation( element.source ), 50 )
            local objectPosition = DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), scaledObject )
            allRes[ #allRes + 1 ] = CreateKnowledge( world.Point, objectPosition )
        end
        return allRes
    end
}