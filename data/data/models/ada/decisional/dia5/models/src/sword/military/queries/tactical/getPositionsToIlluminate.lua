queryImplementation "getPositionsToIlluminate"
{
    ["execute"] = function ( params )
        local allRes = {}
        for _, objective in pairs( params.elementsToIlluminate ) do
            allRes[ #allRes + 1 ] = CreateProxyKnowledge( sword.military.world.IlluminatingArea, objective )
        end
        return allRes
    end
}