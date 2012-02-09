-- --------------------------------------------------------------------------------
-- Work
-- --------------------------------------------------------------------------------
method "createIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self )
        integration.startBuildIt( self, ontology.classes.Object ) -- $$$ MIA TEMP
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

method "isBuilt" ( 
    function( self )
        if self.knowledge ~= nil then
            return integration.buildLevel( self.knowledge ) == 100
        else
            return false
        end
    end )

method "canBeCreated" ( 
    function( self )
        return integration.canBuildNowObjectType( DEC_GenObject_Type( self.source ) ) 
           and integration.hasEnoughtDotationForObjectType( DEC_GenObject_Type( self.source ) )
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