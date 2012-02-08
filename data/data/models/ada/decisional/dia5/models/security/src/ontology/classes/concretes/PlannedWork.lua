-- --------------------------------------------------------------------------------
-- Action methods
-- --------------------------------------------------------------------------------
method "reachIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self )
        return integration.startMoveToIt( self, pathType ) 
    end,
    started = function( self )
        return integration.updateMoveToIt( self, pathType )
    end, 
    stop = function( self )
        return integration.deselectMoveToIt( self )
    end,
} ) )

method "createIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self )
        integration.startBuildIt( self, ontology.classes.concretes.Object )
        return self.knowledge
    end, 
    started = function( self )
        integration.updateBuildIt( self )
        return self.knowledge
    end, 
    stop = function( self )
        integration.stopBuildIt( self )
        return self.knowledge
    end,
} ) )

method "isCreated" ( 
    function( self )
        if self.knowledge ~= nil then
            return integration.buildLevel( self.knowledge ) == 100
        else
            return false
        end
    end )

-- --------------------------------------------------------------------------------
-- Specific methods
-- --------------------------------------------------------------------------------
method "getPosition" ( 
    function( self ) 
        return integration.getEngineerObjectPosition( self )
    end )

method "getPositions" ( 
    function( self ) 
        return { self:getPosition() }
    end )

return{}