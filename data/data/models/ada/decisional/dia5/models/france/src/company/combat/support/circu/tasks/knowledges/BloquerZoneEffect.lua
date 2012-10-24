return
{
    init = function( self, params )
        myself.leadData.objectiveIndex = 0 
        myself.leadData.objectives = {}
    end,

    getZone = function( self, params )
      local nbFront = math.min(self:getNbrFront(), params.maxNbrFront )
      for i=1, #params.zones do
          local objective = params.zones[i]
          -- division de la zone en sous-zones
          local nbParts = math.max((nbFront /#params.zones),1)
          local subAreas = integration.geometrySplitLocalisation( objective.source, nbParts, nil ) -- TODO: voir la fonction integration.splitArea pour remplacer la fonction integration.geometrySplitLocalisation
          subAreas = subAreas.first
          for _, area in pairs( subAreas ) do
              myself.leadData.objectives[#myself.leadData.objectives + 1] = CreateKnowledge( world.Area, area )
          end
          if #subAreas == 0 then
              myself.leadData.objectives[#myself.leadData.objectives + 1] = objective -- cas ou la zone est hors limite
          end
      end
      myself.leadData.objectiveIndex = myself.leadData.objectiveIndex % #myself.leadData.objectives + 1
      return myself.leadData.objectives[ myself.leadData.objectiveIndex ]
    end,
}
