return
{

    getReachable = function( self, params )
        return { params.camp }
    end,
    
    getElements = function( self, params )
        return params.peoples
    end,
    
    getNbrFront = function( self )
        return 1
    end
}