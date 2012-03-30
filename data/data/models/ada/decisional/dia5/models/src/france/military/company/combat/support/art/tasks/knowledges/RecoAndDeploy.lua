return
{

    init = function( self, params )
        myself.leadData.objectives = {}
        myself.leadData.pionCourant = 0
        myself.leadData.NombreTotalSitesAConstruire = 0
    end,

    getObjective = function( self, params )
    
      myself.leadData.deployPositionIndex = 0 
      local nbFront = math.min(self:getNbrFront(), params.maxNbrFront )
      myself.leadData.pionCourant = myself.leadData.pionCourant + 1
      local result = {}
      for i=1, #params.objectives do
        local objective = params.objectives[i]
        -- division de la zone en sous-zones
        local nbParts = math.max((nbFront /#params.objectives),1)
        
        local subAreas = DEC_Geometry_SplitLocalisation( objective.source, nbParts, nil ) -- TODO: voir la fonction integration.splitArea pour remplacer la fonction DEC_Geometry_SplitLocalisation
        subAreas = subAreas.first
        for _, area in pairs( subAreas ) do
            myself.leadData.objectives[#myself.leadData.objectives + 1] = CreateKnowledge( sword.military.world.Area, area )
        end
        
        myself.leadData.deployPositionIndex = myself.leadData.deployPositionIndex % #myself.leadData.objectives + 1
        if #params.objectives > nbFront then
            if i % nbFront == myself.leadData.pionCourant % nbFront then
               result[#result+1] = myself.leadData.objectives[ myself.leadData.deployPositionIndex ]
               myself.leadData.NombreTotalSitesAConstruire = myself.leadData.NombreTotalSitesAConstruire + 2 -- On ajoute 2 à la liste des sites (2 sites par zone)
            end
        else
            if i % #params.objectives == myself.leadData.pionCourant % #params.objectives then
               result[#result+1] = myself.leadData.objectives[ myself.leadData.deployPositionIndex ] 
               myself.leadData.NombreTotalSitesAConstruire = myself.leadData.NombreTotalSitesAConstruire + 2  -- On ajoute 1 à la liste des sites (2 sites par zone)
            end
        end
      end
      return result
    end,
    
    getFirePositions = function( self, params )
    
        local objectsFromAutomat = {}
        local zone = self:getObjective( params )
        for i=1, #meKnowledge.constructedObjects do
          local objet = meKnowledge.constructedObjects[i]
          for j=1, #zone do 
              if integration.isPointInLocalisation(objet, zone[j]) then
                  if meKnowledge.constructedObjects[i].alreadyGiven == false then -- l'objet ne doit pas avoir été donné à un autre pion'
                      objectsFromAutomat[ #objectsFromAutomat + 1 ] = meKnowledge.constructedObjects[i]
                      meKnowledge.constructedObjects[i].alreadyGiven = true
                      break
                  end
              end
          end
        end
        return objectsFromAutomat
    end,
    
    getMeetingPoint = function ( self, params ) -- Need to send a list for moveAlong knowledge
        if params.meetingPoint and params.meetingPoint ~= NIL then
            return { integration.randomPositionInCircle( params.meetingPoint, 100 ) }
        end
        return NIL
    end,
}
