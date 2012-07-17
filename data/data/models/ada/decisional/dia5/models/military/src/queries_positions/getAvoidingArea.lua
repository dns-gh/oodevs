queryImplementation "getAvoidingArea" 
{ 
    ["execute"] = function ( params )
        local allRes = {}
        for _, objective in pairs( params.objects ) do
            allRes[ #allRes + 1 ] = CreateProxyKnowledge( 
                world_elements.AvoidingArea, objective )
        end
        return allRes
    end
}