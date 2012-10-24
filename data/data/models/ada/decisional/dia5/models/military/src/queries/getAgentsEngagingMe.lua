queryImplementation "getAgentsEngagingMe" 
{ 
    [ "execute" ] = function( params )

        local agentsEngagingMe = {}
        local simEnemies = integration.getKnowledgesUnitsEngaging()
        for _, simEny in pairs( simEnemies ) do
            local eny = CreateKnowledge( world.Platoon, simEny )
            agentsEngagingMe[ #agentsEngagingMe + 1 ] = eny
        end
        return agentsEngagingMe

    end
}