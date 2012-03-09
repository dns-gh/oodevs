queryImplementation "getAgentsEngagingMe" 
{ 
    [ "execute" ] = function( params )

        local agentsEngagingMe = {}
        local simEnemies = DEC_Connaissances_UnitesPrenantAPartie()
        for _, simEny in pairs( simEnemies ) do
            local eny = CreateKnowledge( sword.military.world.Platoon, simEny )
            agentsEngagingMe[ #agentsEngagingMe + 1 ] = eny
        end
        return agentsEngagingMe

    end
}