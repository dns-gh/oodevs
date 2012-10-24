queryImplementation "queries.getPointsAroundObject"
{ 
    ["execute"] = function ( params )
        return integration.getPositionsAroundObject( params.object )
    end
}