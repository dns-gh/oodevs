queryImplementation "getNearbyEnemies" 
{ 
    [ "execute" ] = function( params )

        local nearbyEnemies = {}
        local simEnemies = DEC_Connaissances_UnitesEnnemiesVivantesDansCercle( meKnowledge:getPosition(), DEC_Detection_Distance() )
        for _, simEny in pairs( simEnemies ) do
            local eny = CreateKnowledge( world_elements.Platoon, simEny )
            if eny:isNearby()  and not eny:isTransported() then
               nearbyEnemies[ #nearbyEnemies + 1 ] = eny
            end
        end
        return nearbyEnemies

    end
}