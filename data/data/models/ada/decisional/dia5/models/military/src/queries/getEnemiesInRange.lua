queryImplementation "getEnemiesInRange" 
{ 
    ["execute"] = function ( params )
        local res = {}
        local enemies = {}
        local DEC_ConnaissanceAgent_EstValide = DEC_ConnaissanceAgent_EstValide
        local DEC_ConnaissanceAgent_EstDetruitTactique = DEC_ConnaissanceAgent_EstDetruitTactique
        local DEC_Tir_PorteeMaxPourTirerSurUnite = DEC_Tir_PorteeMaxPourTirerSurUnite
        local DEC_ConnaissanceAgent_Position = DEC_ConnaissanceAgent_Position
        local DEC_ConnaissanceAgent_EstEnVol = DEC_ConnaissanceAgent_EstEnVol
        local DEC_ConnaissanceAgent_Altitude = DEC_ConnaissanceAgent_Altitude
        local DEC_Geometrie_Distance3D = DEC_Geometrie_Distance3D
        local DEC_Geometrie_Distance = DEC_Geometrie_Distance
        local myPosition = meKnowledge:getPosition()
        local porteeMax = DEC_Tir_PorteeMaxPourTirer( params.probabilityToHit )
        enemies = DEC_Connaissances_UnitesEnnemiesVivantesDansCercle( myPosition, porteeMax ) -- First filter: max range
        for _, enemy in pairs( enemies ) do
            if DEC_ConnaissanceAgent_EstValide( enemy ) and not DEC_ConnaissanceAgent_EstDetruitTactique( enemy ) then
                local rDistanceAEni = 0 
                local porteeMaxEn = DEC_Tir_PorteeMaxPourTirerSurUnite( enemy, params.probabilityToHit ) -- range depends on enemy volume
                local enyPosition = DEC_ConnaissanceAgent_Position( enemy )
                if DEC_ConnaissanceAgent_EstEnVol( enemy ) then -- flying enemy: compute 3D distance
                    local altitude = DEC_ConnaissanceAgent_Altitude( enemy )
                    rDistanceAEni = DEC_Geometrie_Distance3D( myPosition, 0, enyPosition, altitude )
                else -- ground enemy: casual distance
                    rDistanceAEni = DEC_Geometrie_Distance( myPosition, enyPosition )
                end
                if rDistanceAEni <= porteeMaxEn then
                    local ennyKn =  CreateKnowledge( world.Platoon, enemy )
                    if not ennyKn.capture and not ennyKn:isTransported() then
                        res[ #res + 1 ] = ennyKn
                    end
                end
            end
        end
        return res
    end
}