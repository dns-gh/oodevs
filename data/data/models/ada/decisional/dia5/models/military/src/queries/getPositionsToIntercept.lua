local radius = 150 -- in meters. Defined the radius of cricle in witch we need 
                   -- to find terrain poisition to destroy elements
local nbParts = 2 -- Number of positions to harass : 2 is the minima

queryImplementation "getPositionsToIntercept"
{
    ["execute"] = function ( params )
        local blocksInCircle
        local allRes={}

        -- Urban environnement,  get all the UB in circle around elements to destroy
        for _, objective in pairs( params.objectives ) do
            local elementPosition = objective:getPosition()
            if elementPosition then
                blocksInCircle = integration.getUrbanBlockInsideCircle( elementPosition, radius )
                for _, block in pairs( blocksInCircle ) do
                    allRes[ #allRes + 1 ] = CreateKnowledge( world.UrbanBlock, block )
                end
            end
        end

        -- Area case, add positions in the area : if I had less than 2 position in UB I try to find more positions
        local nbAreas = nbParts - #allRes
        if nbAreas > 0 then
            for _, objective in pairs( params.objectives ) do
                if masalife.brain.core.class.isOfType( objective, world.Area) then
                    local subAreas = integration.geometrySplitLocalisation( objective.source, nbAreas, nil )
                    for _, subArea in pairs( subAreas.first ) do
                         local pos = integration.computeLocationBarycenter( subArea )
                         allRes[ #allRes + 1 ] = CreateKnowledge( world.Point, pos )
                    end
                end
            end
        end

        return allRes
    end
}
