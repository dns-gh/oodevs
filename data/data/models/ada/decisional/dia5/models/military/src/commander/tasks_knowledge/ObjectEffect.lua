return
{
    getObject = function( self, params )
        return params.objective
    end,
    
    getDestination = function( self, params )
        return params.destination:getMyPosition()
    end
}
