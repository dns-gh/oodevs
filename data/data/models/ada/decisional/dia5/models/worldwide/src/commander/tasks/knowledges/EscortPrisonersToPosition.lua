return
{

    getReachable = function( self, params )
        return { params.objective }
    end,
    
    getElements = function( self, params )
        return params.prisoners
    end,
    
    getNbrFront = function( self )
        return 1
    end
}