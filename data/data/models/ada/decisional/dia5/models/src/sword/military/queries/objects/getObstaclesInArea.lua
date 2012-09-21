queryImplementation "getObstaclesInArea" 
{
    ["execute"] = function ( params )
        local allRes={}
        local obstacles = {}
        obstacles = DEC_ObjectKnowledgesInZone( params.area.source, 
            { eTypeObjectAbatis, eTypeObjectAntiTankObstacle, eTypeObjectBarricade, eTypeObjectBridgeDestruction, 
             eTypeObjectLandslide, eTypeObjectMines, eTypeObjectScatteredMinedArea, eTypeObjectLinearMinedArea, 
             eTypeObjectRoadDestruction } )
        for _, objective in pairs( obstacles ) do
            allRes[ #allRes + 1 ] = CreateKnowledge( sword.military.world.Object, objective )
        end
        return allRes
    end
}