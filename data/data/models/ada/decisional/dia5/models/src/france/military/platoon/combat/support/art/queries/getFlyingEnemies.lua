queryImplementation "getFlyingEnemies" 
{ 
    [ "execute" ] = function( params )

        local listeUnitesEnnemiesVivantesPercues = DEC_Connaissances_UnitesEnnemiesVivantesPercues()
        local result = {}
        for i=1, #listeUnitesEnnemiesVivantesPercues do
            if DEC_ConnaissanceAgent_EstEnVol( listeUnitesEnnemiesVivantesPercues[i] ) then
                result[#result + 1] = CreateKnowledge( world.Platoon, listeUnitesEnnemiesVivantesPercues[i] )
            end
        end
        return result

    end
}