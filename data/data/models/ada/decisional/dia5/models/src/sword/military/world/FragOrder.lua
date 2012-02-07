return
{
    isROE = function( self )
        return integration.isROE( self )
    end,
    mustBePropagate = function( self )
        return integration.mustBePropagate( self )
    end,
    updateROE = function( self )
       integration.updateROE( self )
    end,
    isTask = function( self )
        return integration.isTask( self )
    end,
    startTask = function( self )
       integration.startFragOrderTask( self )
    end,
    getParameters = function( self )
        return integration.getParameters( self )
    end
}
