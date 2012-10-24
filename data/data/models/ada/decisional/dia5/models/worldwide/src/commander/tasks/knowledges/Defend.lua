return
{
    getMeetingPoint = function( self, params )
        if params.meetingPoint and params.meetingPoint ~= NIL then
            return { integration.randomPositionInCircle( params.meetingPoint, 100 ) } 
        end
        return NIL
    end,

    getFinalMeetingPoint = function ( self, params ) -- Need to send a single element for build knowledge
        if params.meetingPoint and params.meetingPoint ~= NIL then
            return integration.randomPositionInCircle( params.meetingPoint, 100 )
        end
        return NIL
    end,

    getScoutObjectives = function( self, params )
        local firstParam = params.objectives[ next( params.objectives ) ]
        -- The objective is an area: each units occupy a sub-area
        if masalife.brain.core.class.isOfType( firstParam, world.Area) then
            if not myself.leadData.currentPosition then
                myself.leadData.currentPosition = 0 
                integration.splitArea( firstParam, math.min( self:getNbrFront(), params.maxNbrFront ) )
            end
            myself.leadData.currentPosition = myself.leadData.currentPosition % #myself.leadData.subAreas + 1
            return { myself.leadData.subAreas[myself.leadData.currentPosition] }
        end
        -- The objective is a urban block, units occupy the urban block
        if masalife.brain.core.class.isOfType( firstParam, world.UrbanBlock ) then
            return params.objectives 
        end
        -- The objective is a set of points: each unit occupies a point
        if not myself.leadData.pointsIndexPEI then
            myself.leadData.pointsIndexPEI = 0 
        end
        myself.leadData.pointsIndexPEI = myself.leadData.pointsIndexPEI % #params.objectives + 1
        return { params.objectives[ myself.leadData.pointsIndexPEI ] }
    end,

    getObjectives = function( self, params )
        local firstParam = params.objectives[ next( params.objectives ) ]
        -- The objective is an area: each units occupy a sub-area
        if masalife.brain.core.class.isOfType( firstParam, world.Area ) then
            if not myself.leadData.currentPosition then
                myself.leadData.currentPosition = 0 
                integration.splitArea( firstParam, math.min( self:getNbrFront(), params.maxNbrFront ) )
            end
            myself.leadData.currentPosition = myself.leadData.currentPosition % #myself.leadData.subAreas + 1
            return { myself.leadData.subAreas[ myself.leadData.currentPosition ] }
        end
        -- The objective is a set of points: each units occupy a point
        if not myself.leadData.pointsIndex then
            myself.leadData.pointsIndex = 0 
        end
        myself.leadData.pointsIndex = myself.leadData.pointsIndex % #params.objectives + 1
        return { params.objectives[ myself.leadData.pointsIndex ] }
    end,

    getDirection = function( self, params, entity )     
        local mission = integration.getRawMission( meKnowledge.source )
        local dir = integration.getDirectionEnemy( mission )
        local position = integration.positionTranslateDir( params.objectives[ 1 ]:getPosition(), dir, 1000 )
        return CreateKnowledge( world.Point, position )
    end, 

    getWithImprovement = function( self, params )
        return params.withImprovement or false
    end,
    getWithReinforcement = function( self, params )
        return false
    end
}
