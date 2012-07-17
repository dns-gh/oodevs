queryImplementation "getEnemiesInRange" 
{ 
    ["execute"] = function ( params )
        local res = {}
        local enemies = {}
        local porteeMax = DEC_Tir_PorteeMaxPourTirer( params.probabilityToHit )
        enemies = DEC_Connaissances_UnitesEnnemiesVivantesDansCercle( meKnowledge:getPosition(), porteeMax )
        for _, enemy in pairs( enemies ) do
            local ennyKn =  CreateKnowledge( world_elements.Platoon, enemy )
            if not ennyKn.capture and not ennyKn:isTransported() then
              res[ #res + 1 ] = ennyKn
            end
        end
        return res
    end
}