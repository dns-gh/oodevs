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
        local lima = integration.getLimaPoint( integration.getDestinationPoint( self.source ) )
        integration.setMissionLimaFlag( lima, true )
        integration.setMissionLimaFlagSchedule( lima, true )
        integration.removeFromPointsCategory( self.source )
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
