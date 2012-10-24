queryImplementation "getDangerousEnemies" 
{ 
    [ "execute" ] = function( params )

        local dangerousEnemies = {}
        local simEnemies = integration.getKnowledgesDangerousUnits( )
        for _, simEny in pairs( simEnemies ) do
            local eny = CreateKnowledge( world.Platoon, simEny )
            if eny:isNearby() and not eny:isTransported() then
               dangerousEnemies[ #dangerousEnemies + 1 ] = eny
            end
        end
        return dangerousEnemies

    end
}