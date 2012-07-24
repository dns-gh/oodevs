return
{
    getObjective = function( self, params )
        return { params.objective }
    end,

    getPosition = function( self, params )
        return { params.position }
    end,

    getAllie = function( self, params )
        return params.allie
    end,

    getMunition = function( self, params )
        return { params.munition }
    end,

    getInterventionType = function( self, params )
        return { params.interventionType }
    end,
    
    getNbrFront = function( self )
        return 1
    end
}