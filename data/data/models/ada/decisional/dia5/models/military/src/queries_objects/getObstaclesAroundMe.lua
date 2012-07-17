queryImplementation "getObstaclesAroundMe" 
{
    ["execute"] = function ( params )
        local allRes={}
        local obstacles = {}
        obstacles = DEC_Knowledges_ObjectsInCircle( meKnowledge:getPosition(), params.distance,
            {"abattis", "anti tank obstacle", "barricade", "bridge destruction", 
             "landslide", "mines", "mined area (linear)", "mined area (scattered)", 
             "road destruction" } )
        for _, objective in pairs( obstacles ) do
            allRes[ #allRes + 1 ] = CreateKnowledge( world_elements.Object, objective )
        end
        return allRes
    end
}