return
{
    init = function( self, params )
        myself.leadData.areaIndex = 0 
        myself.leadData.area = {}
        if myself.taskParams.byNight and not integration.isNight() then
            meKnowledge:RC( eRC_AttenteConditionFavorablePourInfiltration )
        end
    end,
    
    getArea = function( self, params )
      local area = myself.taskParams.area
      local nbFrontBefore = math.min(self:getNbrFront(), params.maxNbrFront )
      local nbFront = math.max(nbFrontBefore,1)
      -- division de la zone en sous-zones
      local subAreas = integration.geometrySplitLocalisation( myself.taskParams.area.source, nbFront, nil ) 
      subAreas = subAreas.first
      for _, area in pairs( subAreas ) do
          myself.leadData.area[#myself.leadData.area + 1] = CreateKnowledge( world.Area, area )
      end
      if #subAreas == 0 then
          myself.leadData.area[#myself.leadData.area + 1] = area -- cas ou la zone est hors limite
      end
      myself.leadData.areaIndex = myself.leadData.areaIndex % #myself.leadData.area + 1
      return myself.leadData.area[ myself.leadData.areaIndex ]
    end,

    getByNight = function( self, params )
        return myself.taskParams.byNight
    end,
}