return
{
    getObjectives = function( self, params, entity )     
        return { params.objective }
    end,

    getReachable = function( self, params )
         return params.objective
    end,
}
