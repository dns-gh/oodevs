-- --------------------------------------------------------------------------------
-- Order to agents to follow me
-- --------------------------------------------------------------------------------
method "issueMoveToOrder" (
    function( self, missionName, subordinate, waypoints )
        integration.ListenFrontElement( subordinate )
        integration.SendMessage( "Commander_MoveTo", subordinate, 
            { missionName = missionName, waypoints = waypoints }, { type = "dynamic" } )
        return true
    end )

method "sendMessage" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self, message )
        integration.displayTrace( message )
    end
} ) )

method "getPosition" (
    function( self )
        return integration.getCommanderPosition( self )
    end )

method "RC" ( 
    function( self, ... )
        integration.genericRC( ... )
    end )

return {}