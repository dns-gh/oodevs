return
{
    getEntities = function( self, params )
        return params.entities
    end,
    
    -- point du stationnement
    getReachable = function( self, params )
        if params.meetingPoint and params.meetingPoint ~= NIL then
           return integration.randomPositionOnCircle( params.meetingPoint, 100 )
        end
        return params.objective
    end,
}
