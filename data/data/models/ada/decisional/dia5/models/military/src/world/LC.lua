return
{
    specificActionOnIt = function( self )
        --NOTHING
    end,
    
    predicate "isCoordinationLigne"
    {
        dependencies = "none",
        method = function( self )
                    return true
                 end
    },
    
    predicate "isCoordinationFinish"
    {
        dependencies = "none",
        method = function( self )
                    local order = integration.query.getFirstFragOrderFromType( "Rep_OrderConduite_Poursuivre" )
                    return integration.computePositionsRelativeToLima( integration.getLimaPoint( integration.getDestinationPoint( self.source ) ) ) or order
                 end
    }
}
