queryImplementation "getObstaclesInArea" 
{
    ["execute"] = function ( params )
        local allRes={}
        local obstacles = {}
        obstacles = DEC_ObjectKnowledgesInZone( params.area.source, 
            {"abattis", "anti tank obstacle", "barricade", "bridge destruction", 
             "landslide", "mines", "mined area (linear)", "mined area (scattered)", 
             "road destruction" } )
        for _, objective in pairs( obstacles ) do
            allRes[ #allRes + 1 ] = CreateKnowledge( sword.military.world.Object, objective )
        end
        return allRes
    end
}