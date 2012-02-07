return
{
    getSealOffObjective = function( self, params )
        return params.objective
    end,

    getMeetingPoint = function( self, params )
        return params.objective:getMyPosition()
    end,

    getWithImprovement = function( self, params )
        return params.withImprovement or false
    end,
}