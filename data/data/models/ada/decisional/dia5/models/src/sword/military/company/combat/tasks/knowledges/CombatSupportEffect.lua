return
{
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
