queryImplementation "getFlyingEnemies" 
{ 
    [ "execute" ] = function( params )

        local listeUnitesEnnemiesVivantesPercues = integration.knowledgePerceivedLivingEnemiesAgents()
        local result = {}
        for i=1, #listeUnitesEnnemiesVivantesPercues do
            if integration.isAgentFlying( listeUnitesEnnemiesVivantesPercues[i] ) then
                result[#result + 1] = CreateKnowledge( world.Platoon, listeUnitesEnnemiesVivantesPercues[i] )
            end
        end
        return result

    end
}