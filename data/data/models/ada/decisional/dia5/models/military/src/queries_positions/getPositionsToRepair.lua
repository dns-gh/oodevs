queryImplementation "getPositionsToRepair" 
{ 
    ["execute"] = function ( params )
        local allRes = {}
        for _, objective in pairs( params.elementsToRepair ) do
            if masalife.brain.core.class.isOfType( objective, world_elements.UrbanBlock ) then
                allRes[ #allRes + 1 ] = objective:getMyPosition()
            else
                allRes[ #allRes + 1 ] = CreateProxyKnowledge( world_elements.AvoidingArea, objective )
            end
        end
        return allRes
    end
}