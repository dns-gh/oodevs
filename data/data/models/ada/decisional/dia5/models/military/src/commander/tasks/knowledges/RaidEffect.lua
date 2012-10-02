return
{
    init = function( self, params )
        myself.leadData.objectiveIndex = 0
    end,
    
    getObjectives = function( self, params, entity )     
        return { params.objective }
    end,

    getReachable = function( self, params )
          local nbFront = math.min(self:getNbrFront(), params.maxNbrFront )
          -- division de la zone en sous-zones
          if masalife.brain.core.class.isOfType( params.objective, sword.military.world.Area) then
              integration.splitArea(params.objective, nbFront )
              local min = math.min( params.maxNbrFront ,#myself.leadData.subAreas )
              myself.leadData.objectiveIndex = myself.leadData.objectiveIndex % min + 1
              return myself.leadData.subAreas[ myself.leadData.objectiveIndex ]
          else
              return params.objective
          end
    end,
}
