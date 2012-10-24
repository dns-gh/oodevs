queryImplementation "getNearbyEnemies" 
{ 
    [ "execute" ] = function( params )

        local nearbyEnemies = {}
        local simEnemies = integration.getKnowledgesLivingAgentsInCircle( meKnowledge:getPosition(), integration.getDetectionDistance() )
        for _, simEny in pairs( simEnemies ) do
            local eny = CreateKnowledge( world.Platoon, simEny )
            if eny:isNearby()  and not eny:isTransported() then
               nearbyEnemies[ #nearbyEnemies + 1 ] = eny
            end
        end
        return nearbyEnemies

    end
}