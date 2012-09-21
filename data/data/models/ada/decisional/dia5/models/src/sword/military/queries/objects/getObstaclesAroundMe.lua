queryImplementation "getObstaclesAroundMe" 
{
    ["execute"] = function ( params )
        local allRes={}
        local obstacles = {}
        obstacles = DEC_Knowledges_ObjectsInCircle( meKnowledge:getPosition(), params.distance,
            { eTypeObjectAbatis, eTypeObjectAntiTankObstacle, eTypeObjectBarricade, eTypeObjectBridgeDestruction, 
             eTypeObjectLandslide, eTypeObjectMines, eTypeObjectLinearMinedArea, eTypeObjectScatteredMinedArea, 
             eTypeObjectRoadDestruction } )
        for _, objective in pairs( obstacles ) do
            allRes[ #allRes + 1 ] = CreateKnowledge( sword.military.world.Object, objective )
        end
        return allRes
    end
}