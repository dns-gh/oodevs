return
{
    init = function( self, params )
        myself.leadData.objectiveIndex = 0
        myself.leadData.entitiesToSupport = {}
    end,

    getEntitiesToSupport = function( self, params )
        if myself.leadData.objectiveIndex == 0 then
            myself.leadData.entitiesToSupport = integration.filterPionWithEchelon( integration.getEntitiesFromAutomatCommunication( params.objective, "none", true), eEtatEchelon_First )
            if #myself.leadData.entitiesToSupport < 1 then
                myself.leadData.entitiesToSupport = integration.getEntitiesFromAutomatCommunication( params.objective, "none", true)
            end
            myself.leadData.nbEntities = #myself.leadData.entitiesToSupport
        end
        local nbFront = self:getNbrFront()
        local nextObjectives = {}
        myself.leadData.nbPions = myself.leadData.nbPions or nbFront
        local nbcible = math.max( ( myself.leadData.nbEntities / myself.leadData.nbPions ), 1 )
        while #nextObjectives < nbcible do
            myself.leadData.objectiveIndex = myself.leadData.objectiveIndex % #myself.leadData.entitiesToSupport + 1
            nextObjectives[ #nextObjectives + 1 ]= myself.leadData.entitiesToSupport[ myself.leadData.objectiveIndex ]
        end
        myself.leadData.nbPions = myself.leadData.nbPions - 1
        myself.leadData.nbEntities = myself.leadData.nbEntities - #nextObjectives
        return nextObjectives
    end,
    
        
    getEntitiesToFollow = function( self, params )
        local entitiesToFollow = integration.filterPionWithEchelon( integration.getEntitiesFromAutomat( meKnowledge, "none", true ), eEtatEchelon_First )
         if next(entitiesToFollow) then
          return entitiesToFollow
        else -- par défaut on renvoit le PC
          return {integration.query.getPCUnit()}
        end
    end,

    getDirection = function( self, params )
        return params.direction
    end,

    getSupportingPosition = function( self, params )
        if not myself.leadData.positionsIndex then
            myself.leadData.positionsIndex = 0
        end
        myself.leadData.positionsIndex = myself.leadData.positionsIndex % #params.positions + 1
        return params.positions[ myself.leadData.positionsIndex ]
    end,
    
    getNbrFront = function( self )
      local nbrEchelon = myself.taskParams.echelonNumber or 0
      if nbrEchelon == NIL or nbrEchelon == 0 then
        nbrEchelon = 1 -- default value
      end
      return integration.query.getNbrFront( nbrEchelon )
    end
}
