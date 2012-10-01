return
{  
    getMeetingPoint = function( self, params )
        if params.meetingPoint and params.meetingPoint ~= NIL then
            return { integration.randomPositionOnCircle( params.meetingPoint, 100 ) } 
        end
        return NIL
    end,
    
    getFinalMeetingPoint = function ( self, params ) -- Need to send a single element for build knowledge
        if params.meetingPoint and params.meetingPoint ~= NIL then
            return integration.randomPositionOnCircle( params.meetingPoint, 100 )
        end
        return NIL
    end,
    
    getObjectivesPei = function( self, params )
  
      local firstParam = params.objectives[ next(params.objectives) ]
      
      -- Defense de BU : chaque pion effectue occupe une portion de la zone
    if masalife.brain.core.class.isOfType( firstParam, sword.military.world.Area) then
        if not myself.leadData.currentPosition then
            myself.leadData.currentPosition = 0 
            integration.splitArea( firstParam, math.min( self:getNbrFront(), params.maxNbrFront ) )
        end
      
        myself.leadData.currentPosition = myself.leadData.currentPosition % #myself.leadData.subAreas + 1
        
        return { myself.leadData.subAreas[myself.leadData.currentPosition] }
      end
      
       -- Defense de BU : tous les pions occupent le BU
      if masalife.brain.core.class.isOfType( firstParam, sword.military.world.UrbanBlock) then
        return params.objectives 
      end

       -- Defense de points : attribution de 1 point par pion
       if not myself.leadData.pointsIndexPEI then
            myself.leadData.pointsIndexPEI = 0 
       end

       myself.leadData.pointsIndexPEI = myself.leadData.pointsIndexPEI % #params.objectives + 1

        return { params.objectives[myself.leadData.pointsIndexPEI] }
    end,
    
    getObjectives = function( self, params )
  
      local firstParam = params.objectives[ next(params.objectives) ]
      
      -- Defense de BU : chaque pion occupe une portion de la zone
      if masalife.brain.core.class.isOfType( firstParam, sword.military.world.Area) then
        if not myself.leadData.currentPosition then
            myself.leadData.currentPosition = 0 
            integration.splitArea( firstParam, math.min( self:getNbrFront(), params.maxNbrFront ) )
        end
      
        myself.leadData.currentPosition = myself.leadData.currentPosition % #myself.leadData.subAreas + 1
        
        return { myself.leadData.subAreas[myself.leadData.currentPosition] }
      end

       -- Defense de points : attribution de 1 point par pion
       if not myself.leadData.pointsIndex then
            myself.leadData.pointsIndex = 0 
       end

       myself.leadData.pointsIndex = myself.leadData.pointsIndex % #params.objectives + 1

        return { params.objectives[myself.leadData.pointsIndex] }
    end,
    
    getDirection = function( self, params, entity )     
        local mission = DEC_GetRawMission( meKnowledge.source )
        local dir = DEC_GetDirectionEnnemi( mission )
        local position = DEC_Geometrie_PositionTranslateDir( params.objectives[1]:getPosition(), dir, 1000 )
        return CreateKnowledge( sword.military.world.Point, position)
    end, 

    getWithImprovement = function( self, params )
        return params.withImprovement or false
    end,
}
