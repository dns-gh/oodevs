queryImplementation "getCrowdInArea" 
{ 
    ["execute"] = function ( params )
        local allRes = {}
        local crowds = integration.getCrowds()
        for _,crowd in pairs( crowds ) do
             if integration.isKnowledgeCrowdInsideArea( crowd, params.area.source ) then
                allRes[ #allRes + 1 ] = CreateKnowledge( world.Crowd, crowd )
             end
        end
        return allRes
    end
}