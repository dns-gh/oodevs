return
{
    init = function( self, params )
        myself.leadData.objectiveIndex = 0 
        myself.leadData.objectives = {}
        self.nbAreas = 0
        for _, objective in pairs( myself.taskParams.areas ) do
            if masalife.brain.core.class.isOfType( objective, sword.military.world.Area) then
                self.nbAreas = self.nbAreas + 1 
            end
        end
    end,

    getArea = function( self, params )
      local nbFront = math.min(self:getNbrFront(), params.maxNbrFront )
      for i=1, #params.areas do
            local objective = params.areas[i]
          -- division de la zone en sous-zones
          if masalife.brain.core.class.isOfType( objective, sword.military.world.Area) then
              local nbOtherObjectives = #params.areas - self.nbAreas
              local nbParts = math.max((( nbFront - nbOtherObjectives )/self.nbAreas),1)
              local subAreas = DEC_Geometry_SplitLocalisation( objective.source, nbParts, nil ) -- TODO: voir la fonction integration.splitArea pour remplacer la fonction DEC_Geometry_SplitLocalisation
              subAreas = subAreas.first
              for _, area in pairs( subAreas ) do
                  myself.leadData.objectives[#myself.leadData.objectives + 1] = CreateKnowledge( sword.military.world.Area, area )
              end
              if #subAreas == 0 then
                  myself.leadData.objectives[#myself.leadData.objectives + 1] = objective -- cas ou la zone est hors limite
              end
          else
              myself.leadData.objectives[#myself.leadData.objectives + 1] = objective
          end
      end
      local min = math.min( params.maxNbrFront ,#myself.leadData.objectives )
      myself.leadData.objectiveIndex = myself.leadData.objectiveIndex % min + 1
      local nextObjective = myself.leadData.objectives[ myself.leadData.objectiveIndex ]
      return nextObjective
    end,
}