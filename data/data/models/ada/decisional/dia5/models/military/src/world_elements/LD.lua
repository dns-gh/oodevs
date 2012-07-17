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
                    local order = integration.query.getFirstFragOrderFromType( "Rep_OrderConduite_Deboucher" )
                    return DEC_GetMissionLimaFlag( DEC_GetLimaPoint( DEC_GetDestPoint( self.source ) ) ) or order
                 end
    }
}
