return
{
    init = function( self, params )
        myself.leadData.areaIndex = 0 
        myself.leadData.area = {}
    end,
    
    getObjective= function( self, params )
      if masalife.brain.core.class.isOfType( myself.taskParams.objective, world.Area) then
          local area = myself.taskParams.objective
          local nbFront = math.min(self:getNbrFront(), params.maxNbrFront )
    
          -- division de la zone en sous-zones
          local subAreas = integration.geometrySplitLocalisation( myself.taskParams.objective.source, nbFront, nil )
          subAreas = subAreas.first
          for _, area in pairs( subAreas ) do
              myself.leadData.area[#myself.leadData.area + 1] = CreateKnowledge( world.Area, area )
          end
          if #subAreas == 0 then
              myself.leadData.area[#myself.leadData.area + 1] = area -- cas ou la zone est hors limite
          end
          myself.leadData.areaIndex = myself.leadData.areaIndex % #myself.leadData.area + 1
          return myself.leadData.area[ myself.leadData.areaIndex ]
      else
          return myself.taskParams.objective
      end
    end,
    
    getByNight = function( self, params )
        return myself.taskParams.byNight
    end,
}