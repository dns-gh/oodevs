queryImplementation "getEnemiesToEngageForEscort" 
{ 
    ["execute"] = function ( params )
        local res = {}
        local enemies = {}
        local DEC_ConnaissanceAgent_EstValide = DEC_ConnaissanceAgent_EstValide
        local DEC_ConnaissanceAgent_EstDetruitTactique = DEC_ConnaissanceAgent_EstDetruitTactique
        local DEC_Tir_PorteeMaxPourTirerSurUnite = DEC_Tir_PorteeMaxPourTirerSurUnite
        local DEC_ConnaissanceAgent_Position = DEC_ConnaissanceAgent_Position
        local DEC_ConnaissanceAgent_EstEnVol = DEC_ConnaissanceAgent_EstEnVol
        local DEC_Geometrie_Distance = DEC_Geometrie_Distance
        local myPosition = meKnowledge:getPosition()
        local unitToEscortPosition = params.unitToEscort:getPosition()
        local porteeMax = DEC_Tir_PorteeMaxPourTirer( params.probabilityToHit )
        enemies = DEC_Connaissances_UnitesEnnemiesVivantesDansCercle( myPosition, porteeMax ) -- filter: max range
        enemies = fusionList( enemies, DEC_Connaissances_UnitesEnnemiesVivantesDansCercle( unitToEscortPosition, porteeMax ) )
        for i = 1, #enemies do
            if DEC_ConnaissanceAgent_EstValide( enemies[i] ) and not DEC_ConnaissanceAgent_EstDetruitTactique( enemies[i] ) 
              and not DEC_ConnaissanceAgent_EstEnVol( enemies[i] ) then
                local ennyKn =  CreateKnowledge( sword.military.world.Platoon, enemies[i] )
                if not ennyKn.capture and not ennyKn:isTransported() then
                    res[ #res + 1 ] = ennyKn
                end
            end
        end
        return res
    end
}