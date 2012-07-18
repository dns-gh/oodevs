return
{
    getReachable = function( self, params )
        return params.entities
    end,

    getObjectiveGen = function( self, params )
         return params.entities[ 1 ]:getMyPosition()
    end,
}
