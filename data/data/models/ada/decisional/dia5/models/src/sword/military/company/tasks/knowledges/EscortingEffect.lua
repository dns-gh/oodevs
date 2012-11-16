return
{
    init = function( self, params, parameters )
        local entities = {}
        self.entitiesToFollow = {}
        if masalife.brain.core.class.isOfType( parameters.objective, sword.military.world.Company) then
             entities = DEC_Automate_PionsDeAutomateAvecPC(parameters.objective.source)
             for _, element in pairs (entities) do
                self.entitiesToFollow[ #self.entitiesToFollow + 1 ] = CreateKnowledge( sword.military.world.PlatoonAlly, element )
             end
        else
            self.entitiesToFollow = { parameters.objective }
        end
    end,

    getEntitiesToFollow = function( self, params )
        return self.entitiesToFollow
    end,
    
    getEntityToEscort = function( self, params )
        myself.leadData.currentEscortingEntity = myself.leadData.currentEscortingEntity or 0
        myself.leadData.currentEscortingEntity = myself.leadData.currentEscortingEntity % #self.entitiesToFollow + 1
        return self.entitiesToFollow[myself.leadData.currentEscortingEntity]
    end,

    getNbrFront = function( self )
       return #self.entitiesToFollow
    end,
}
