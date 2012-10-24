queryImplementation "getEnemiesInArea"
{
    ["execute"] = function ( params )
        local res = {}
        local units = {}
        for _, objective in pairs( params.areas ) do
            if masalife.brain.core.class.isOfType( objective, world.Area ) then
                units = integration.isKnowledgesAgentsInArea( objective.source )
            end
        end
        for _, enemy in pairs( units ) do
            local  eny = CreateKnowledge( world.Platoon, enemy )
            if not eny:isTransported() then
              res[ #res + 1 ] = eny
            end
        end
        return res
    end
}