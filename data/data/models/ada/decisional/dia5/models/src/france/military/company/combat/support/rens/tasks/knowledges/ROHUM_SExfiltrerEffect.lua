return
{
    init = function( self, params )
        myself.leadData.areaIndex = 0 
        myself.leadData.area = {}
    end,
    
    getObjective= function( self, params )
      local area = myself.taskParams.area
      local nbFront = math.min(self:getNbrFront(), params.maxNbrFront )

      -- division de la zone en sous-zones
      local subAreas = DEC_Geometry_SplitLocalisation( myself.taskParams.area.source, nbFront, nil ) 
      subAreas = subAreas.first
      for _, area in pairs( subAreas ) do
          myself.leadData.area[#myself.leadData.area + 1] = CreateKnowledge( sword.military.world.Area, area )
      end
      
      myself.leadData.areaIndex = myself.leadData.areaIndex % #myself.leadData.area + 1
      return myself.leadData.area[ myself.leadData.areaIndex ]
    end,
}