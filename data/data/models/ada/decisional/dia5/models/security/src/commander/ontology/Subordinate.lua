-- --------------------------------------------------------------------------------
-- Tasks abilities
-- --------------------------------------------------------------------------------
method "canPerformTask" (
    function( self,missionName )
        return integration.isMissionAvailable( self, missionName )
    end )

return {}