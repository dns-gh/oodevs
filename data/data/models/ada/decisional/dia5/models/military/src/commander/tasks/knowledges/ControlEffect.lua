return
{
    init = function( self, params )
        myself.leadData.objectiveIndex = 0 
        myself.leadData.objectives = {}
        self.nbAreas = 0
        for _, objective in pairs( myself.taskParams.objectives ) do
            if masalife.brain.core.class.isOfType( objective, world.Area) then
                self.nbAreas = self.nbAreas + 1 
            end
        end
    end,

    -- Partage des objectifs: chaque pion PEI reçoit la position d'un des objectifs de la mission
    -- Si parmi les objectifs il y a des zones,  pions PEI recoivent les barycentres des zones
    
    getScoutObjectives = function( self, params, entity )
        if not myself.leadData.currentScoutObjective then
            myself.leadData.currentScoutObjective = 0
        end
        myself.leadData.currentScoutObjective = myself.leadData.currentScoutObjective  % #params.objectives + 1
        local current = params.objectives[ myself.leadData.currentScoutObjective ]
        return { current:getMyPosition() }
    end, 
       
    getDirection = function( self, params, entity, objectivePosition )
        if not params.objectives.kn then
            local mission = integration.getRawMission( meKnowledge.source )
            local dir = integration.getDirectionEnemy( mission )
            local position
            if objectivePosition then
                position = integration.positionTranslateDir( objectivePosition:getPosition(), dir, 1000 )
            else
                position = integration.positionTranslateDir( meKnowledge:getPosition(), dir, 1000 )
            end
            params.objectives.kn = CreateKnowledge( world.Point, position)
        end
        return params.objectives.kn
    end,
    
    -- Partage des objectifs: chaque pion reçoit un objectif
    -- Si parmi les objectifs il y a des zones, celles-ci sont divisées en sous-zones par rapport au nombre de pions "mains" restant
    
    getObjective = function( self, params )
      local nbFront = math.min(self:getNbrFront(), params.maxNbrFront )
      for i=1, #params.objectives do
            local objective = params.objectives[i]
          -- division de la zone en sous-zones
          if masalife.brain.core.class.isOfType( objective, world.Area) then
              local nbOtherObjectives = #params.objectives - self.nbAreas
              local nbParts = math.max((( nbFront - nbOtherObjectives )/self.nbAreas),1)
              local subAreas = integration.geometrySplitLocalisation( objective.source, nbParts, nil ) -- TODO: voir la fonction integration.splitArea pour remplacer la fonction integration.geometrySplitLocalisation
              subAreas = subAreas.first
              for _, area in pairs( subAreas ) do
                  myself.leadData.objectives[#myself.leadData.objectives + 1] = CreateKnowledge( world.Area, area )
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
    getWithImprovement = function( self, params )
        return params.withImprovement or false
    end,
}
