queryImplementation "getEnemiesToSupport" 
{ 
    ["execute"] = function ( params )
        local res = {}
        local ennemies = {}
        local pH = 0.5 -- $$$ sortir en tant que paramètre?
        local porteeMax = DEC_Tir_PorteeMaxPourTirer( pH )
        for _, element in pairs( params.elementsToSupport ) do
            ennemies = DEC_Connaissances_UnitesEnnemiesVivantesDansCercle( element:getPosition(), porteeMax )
            for _, ennemy in pairs( ennemies ) do
                res[ #res + 1 ] = CreateKnowledge( sword.military.world.Platoon, ennemy )
            end
        end
        return res
    end
}