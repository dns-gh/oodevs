queryImplementation "queries.getPointsAroundObject" 
{ 
    ["execute"] = function ( params )
        -- $$$ MIA TODO: faire un cache...
        local allRes = {}
        local scaledObject = DEC_Geometrie_AgrandirLocalisation( DEC_GenObjectKnowledge_Localisation( params.object.source ), 50 )
        local objectPosition = DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), scaledObject )
        allRes[ 1 ] = CreateKnowledge( world.Point, objectPosition )
        return allRes
    end
}