-- --------------------------------------------------------------------------------
-- Tasks abilities
-- --------------------------------------------------------------------------------
method "canPerformTask" (
    function( self,missionName )
        return integration.isMissionAvailable( self, missionName )
    end )

-- --------------------------------------------------------------------------------
-- Specific integration methods 
-- --------------------------------------------------------------------------------
method "getPosition" ( 
    function( self ) 
        return integration.getSubordinatePosition( self )
    end )

return {}