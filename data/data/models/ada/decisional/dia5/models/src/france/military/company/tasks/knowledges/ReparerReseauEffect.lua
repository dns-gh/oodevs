return
{
    getMeetingPoint = function( self, params )
        return integration.randomPositionOnCircle( params.meetingPoint, 100 )
    end,

    getObjective = function( self, params )
       if not myself.leadData.objectiveIndex then
            myself.leadData.objectiveIndex = 0 
       end
       myself.leadData.objectiveIndex = myself.leadData.objectiveIndex % #params.objectives + 1
       return params.objectives[myself.leadData.objectiveIndex]
    end,
}
