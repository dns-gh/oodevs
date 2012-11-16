return
{
    init = function( self, params, parameters )
        local entities = {}
        self.entitiesToFollow = {}
        if masalife.brain.core.class.isOfType( parameters.objective, sword.military.world.Company) then
             --entities = DEC_Automate_PionsDeAutomateAvecPC(parameters.objective.source)
             self.entitiesToFollow = integration.getEntitiesFromAutomat(parameters.objective, "none", true)
             for _, element in pairs (self.entitiesToFollow) do
                --self.entitiesToFollow[ #self.entitiesToFollow + 1 ] = CreateKnowledge( sword.military.world.PlatoonAlly, element )
                DEC_Trace("unité à escorter: "..tostring(DEC_GetSzName(element.source)))
             end
        else
        DEC_Trace("Si on escorte une compagnie ON NE DEVRAIT PAS ETRE LA")
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
