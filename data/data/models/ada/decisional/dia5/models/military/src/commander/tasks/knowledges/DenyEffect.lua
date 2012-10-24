return
{

    getScoutObjectives = function( self, params, entity )
        if not myself.leadData.objIndex then
            myself.leadData.objIndex = 0 
        end
        myself.leadData.objIndex = myself.leadData.objIndex % #params.objectives + 1
        return { params.objectives[myself.leadData.objIndex] }
    end,    

    getDirection = function( self, params, entity )     
        local mission = integration.getRawMission( meKnowledge.source )
        local dir = integration.getDirectionEnemy( mission )
        local position = integration.positionTranslateDir( params.objectives[1]:getPosition(), dir, 1000 )
        return CreateKnowledge( world.Point, position)
    end,

    getObjective = function( self, params )
        return { params.objective }
    end,
     

    getPositions = function( self, params )
       if not myself.leadData.positionIndex then
            myself.leadData.positionIndex = 0 
       end

       local min = math.min( params.maxNbrFront ,#params.positions )
       myself.leadData.positionIndex = myself.leadData.positionIndex % min + 1

       return { params.positions[myself.leadData.positionIndex] }
    end,
    
    getPosition = function( self, params )
        return { params.position }
    end,

    getWithImprovement = function( self, params )
        return params.withImprovement or false
    end,
    
    getFinalMeetingPoint = function ( self, params ) -- Need to send a single element for build knowledge
        if params.meetingPoint and params.meetingPoint ~= NIL then
            return integration.randomPositionInCircle( params.meetingPoint, 100 )
        end
        return NIL
    end,
}
