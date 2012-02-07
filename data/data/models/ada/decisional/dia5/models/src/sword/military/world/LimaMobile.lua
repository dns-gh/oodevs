return
{
    getLimaType = function( self )
        return self.limaType_
    end,
    takeUpPosition = function( self )
        return integration.isPosted()
    end,
    actionOnIt = function( self )
        return self:specificActionOnIt()
    end,
    clean = function( self )
        local lima = DEC_GetLimaPoint( DEC_GetDestPoint( self.source ) )
        DEC_SetMissionLimaFlag( lima, true )
        DEC_SetMissionLimaFlagHoraire( lima, true )
        DEC_RemoveFromPointsCategory( self.source )
    end,
    
    predicate "isCoordinationLigne"
    {
        dependencies = "none",
        method = function( self )
                    return false
                 end
    },
    
    predicate "isCoordinationFinish"
    {
        dependencies = "none",
        method = function( self )
                    return true
                 end
    }
}
