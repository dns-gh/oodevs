return
{
    getObject = function( self, params )
        return params.decontaminationSite
    end,
    
    getDestination = function( self, params )
        return params.meetingPoint:getMyPosition()
    end
}
