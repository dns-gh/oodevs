return
{

    init = function( self, params )
        myself.leadData.pionCourant = 0
        myself.leadData.pionCourantSite = 0
        myself.leadData.NombreTotalSitesAConstruire = 0
        myself.leadData.deployPositionIndex = 1
    end,

    getObjective = function( self, params )
    
      local nbFront = math.min(self:getNbrFront(), params.maxNbrFront )
      myself.leadData.pionCourant = myself.leadData.pionCourant + 1
      local result = {}
      local areasKn = {}
      local nbAreasToAdd = nbFront % #params.objectives
      local addArea = 1
      local nbParts = math.max( math.floor( nbFront /#params.objectives), 1 ) -- arrondi inférieur
      for i=1, #params.objectives do                      -- parcourrir toutes les zones en paramètre
          local objective = params.objectives[i]
          if #params.objectives <= nbFront then           -- moins de zones que de pions
              -- division de la zone en sous-zones
              if nbAreasToAdd == 0 then
                addArea = 0
              else
                nbAreasToAdd = nbAreasToAdd - 1
              end
              -- calcul du nombre de sous-zones suivant le reste de la division entière entre nbFront et #params.objectives
              local subAreas = DEC_Geometry_SplitLocalisation( objective.source, nbParts + addArea, nil ) -- TODO: voir la fonction integration.splitArea pour remplacer la fonction DEC_Geometry_SplitLocalisation
              subAreas = subAreas.first
              for _, area in pairs( subAreas ) do
                  areasKn[#areasKn + 1] = CreateKnowledge( sword.military.world.Area, area )
              end
              if #subAreas == 0 then
                  areasKn[#areasKn + 1] = objective -- cas ou la zone est hors limite
              end
          else                                              -- nb de zones  >= nb de pions
              areasKn[#areasKn + 1] = CreateKnowledge( sword.military.world.Area, objective.source )
          end
      end
       -- Distribtion des sous-zones aux pions
       if #params.objectives <= nbFront then                -- nb de zone <= nb de pions: chaque pion recoit une sous-zone
           result[#result+1] = areasKn[ myself.leadData.deployPositionIndex ]
           myself.leadData.deployPositionIndex = myself.leadData.deployPositionIndex + 1 
           myself.leadData.NombreTotalSitesAConstruire = myself.leadData.NombreTotalSitesAConstruire + 2 -- On ajoute 2 à la liste des sites (2 sites par zone)
       else                                                 -- nb de zone > nb de pions: chaque pion recoit une ou plusieurs sous-zone
           for i=1, #areasKn do               
              if  i % nbFront ==  myself.leadData.pionCourant % nbFront then  
                  result[#result+1] = areasKn[ myself.leadData.deployPositionIndex ]     
                  myself.leadData.deployPositionIndex = myself.leadData.deployPositionIndex + 1      
                  myself.leadData.NombreTotalSitesAConstruire = myself.leadData.NombreTotalSitesAConstruire + 2 -- On ajoute 2 à la liste des sites (2 sites par zone)          
              end
           end
       end
      return result
    end,
    
    getFirePositions = function( self, params )
        
        if not meKnowledge.constructedObjects then
          return {}
        end
        
        myself.leadData.pionCourantSite = myself.leadData.pionCourantSite + 1 

        local nbrFront = params.maxNbrFront
        local objectsFromAutomat = {}
        local objectsInArea = {}
        
        -- tous les objets de la même zone sont stockés dans objectsInArea
        for i=1, #params.objectives do
            local objective = params.objectives[i]
            objectsInArea[i] = {}
            for j=1, #meKnowledge.constructedObjects do
                local objet = meKnowledge.constructedObjects[j]
                if integration.isPointInLocalisation(objet, params.objectives[i]) then
                    objectsInArea[i][#objectsInArea[i]+1] = objet
                end               
             end 
        end        
        if #params.objectives >= nbrFront then
            for i=1, #params.objectives do
                if i % nbrFront == myself.leadData.pionCourantSite % nbrFront then
                  -- on donne tous les objets d'une zone ou plusieurs zones à un pion
                  objectsFromAutomat = fusionList( objectsFromAutomat, objectsInArea[i] )
                end
            end
        else
          for i=1, #meKnowledge.constructedObjects do
             if i % nbrFront == myself.leadData.pionCourantSite % nbrFront then
              -- distribution des objets indépendamment des zones. Optimisation à faire: donner des objets d'une même zone au même pion
              if meKnowledge.constructedObjects[i].alreadyGiven == false then -- l'objet ne doit pas avoir été donné à un autre pion
                objectsFromAutomat[ #objectsFromAutomat + 1 ] = meKnowledge.constructedObjects[i]
                meKnowledge.constructedObjects[i].alreadyGiven = true
              end
            end
          end
        end
        return objectsFromAutomat
    end,
    
    getMeetingPoint = function ( self, params ) -- Need to send a list for moveAlong knowledge
        if params.meetingPoint and params.meetingPoint ~= NIL then
            return { integration.randomPositionOnCircle( params.meetingPoint, 100 ) }
        end
        return NIL
    end,

    getChangeAutomatically = function ( self, params )
        return params.changeAutomatically
    end,
}
