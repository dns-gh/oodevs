return
{

    init = function( self, params )
        myself.leadData.deployPositionIndex = 0 
        myself.leadData.objectives = {}
        
        myself.leadData.pionCourant = 0
    end,

    getObjective = function( self, params )
      local nbFront = math.min(self:getNbrFront(), params.maxNbrFront )
      for i=1, #params.objectives do
        local objective = params.objectives[i]
        -- division de la zone en sous-zones
        local nbParts = math.max((nbFront /#params.objectives),1)
        local subAreas = DEC_Geometry_SplitLocalisation( objective.source, nbParts, nil ) -- TODO: voir la fonction integration.splitArea pour remplacer la fonction DEC_Geometry_SplitLocalisation
        subAreas = subAreas.first
        for _, area in pairs( subAreas ) do
            myself.leadData.objectives[#myself.leadData.objectives + 1] = CreateKnowledge( sword.military.world.Area, area )
        end
      end
      myself.leadData.deployPositionIndex = myself.leadData.deployPositionIndex % #myself.leadData.objectives + 1
      return { myself.leadData.objectives[ myself.leadData.deployPositionIndex ] }
    end,
    
    getFirePositions = function( self, params )
    
        if not meKnowledge.constructedObjects then
          return {}
        end
        
        myself.leadData.pionCourant = myself.leadData.pionCourant + 1  
        local nbrFront = params.maxNbrFront
        local objectsFromAutomat = {}
       
        if #meKnowledge.constructedObjects > nbrFront then -- plus de PAA que de pions
          for i=1, #meKnowledge.constructedObjects do
            if i % nbrFront == myself.leadData.pionCourant % nbrFront then
              objectsFromAutomat[ #objectsFromAutomat + 1 ] = meKnowledge.constructedObjects[i]
            end
          end
        else                                              -- moins ou nombre égal de PAA que de pions
          for i=1, nbrFront do
            if i % #meKnowledge.constructedObjects == myself.leadData.pionCourant % #meKnowledge.constructedObjects then
              objectsFromAutomat[ #objectsFromAutomat + 1 ] = meKnowledge.constructedObjects[i]
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
