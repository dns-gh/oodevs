return
{
    init = function( self, params )
        myself.leadData.areaIndex = 0 
        myself.leadData.areas = {}
    end,
    
    getPosition = function( self, params )
        local objective = params.position
        local nbFrontBefore = math.min(self:getNbrFront(), params.maxNbrFront )
        local nbFront = math.max(nbFrontBefore,1)

        if masalife.brain.core.class.isOfType( objective, world.Area) then
              local subAreas = integration.geometrySplitLocalisation( objective.source, nbFront, nil )
              subAreas = subAreas.first
              myself.leadData.objectiveIndex = 0 
              for _, area in pairs( subAreas ) do
                  myself.leadData.areas[#myself.leadData.areas + 1] = CreateKnowledge( world.Area, area )
              end
              if #subAreas == 0 then
                  myself.leadData.areas[#myself.leadData.areas + 1] = objective -- cas ou la zone est hors limite
              end
              myself.leadData.areaIndex = myself.leadData.areaIndex % nbFront + 1
              return myself.leadData.areas[ myself.leadData.areaIndex ]
        else
            return params.position
        end
    end,
}
