queryImplementation "getObstaclesAroundMe" 
{
    ["execute"] = function ( params )
        local allRes={}
        local obstacles = {}
        obstacles = integration.getKnowledgesObjectsInCircle( meKnowledge:getPosition(), params.distance,
            { eTypeObjectAbatis, eTypeObjectAntiTankObstacle, eTypeObjectBarricade, eTypeObjectBridgeDestruction, 
             eTypeObjectLandslide, eTypeObjectMines, eTypeObjectScatteredMinedArea, eTypeObjectLinearMinedArea, 
             eTypeObjectRoadDestruction } )
        for _, objective in pairs( obstacles ) do
            allRes[ #allRes + 1 ] = CreateKnowledge( world.Object, objective )
        end
        return allRes
    end
}