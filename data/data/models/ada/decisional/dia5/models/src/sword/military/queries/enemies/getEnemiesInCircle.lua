queryImplementation "getEnemiesInCircle"
{ 
    [ "execute" ] = function( params )
        local res = {}
        local ennemies = {}
        ennemies = DEC_Connaissances_UnitesEnnemiesVivantesDansCercle( meKnowledge:getPosition(), params.distance )        
       
        for _, ennemy in pairs( ennemies ) do
            local eny = CreateKnowledge( sword.military.world.Platoon, ennemy )
            if not eny:isTransported() then
                res[ #res + 1 ] = eny
            end
        end
        return res
    end
}