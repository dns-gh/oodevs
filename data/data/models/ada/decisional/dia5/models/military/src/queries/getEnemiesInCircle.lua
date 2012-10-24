queryImplementation "getEnemiesInCircle"
{ 
    [ "execute" ] = function( params )
        local res = {}
        local ennemies = {}
        ennemies = integration.getKnowledgesLivingAgentsInCircle( meKnowledge:getPosition(), params.distance )        
       
        for _, ennemy in pairs( ennemies ) do
            local eny = CreateKnowledge( world.Platoon, ennemy )
            if not eny:isTransported() then
                res[ #res + 1 ] = eny
            end
        end
        return res
    end
}