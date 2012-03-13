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
        integration.updateBuildItSecu( self )
        return self.knowledge
    end, 
    stop = function( self )
        integration.stopBuildItSecu( self )
        return self.knowledge
    end,
} ) )

method "isBuilt" ( 
    function( self )
        if self.knowledge ~= nil then
            return integration.isConstructed( self.knowledge )
        else
            return false
        end
    end )

method "canBeCreated" ( 
    function( self )
        local result = integration.canBuildNowObjectType( DEC_GenObject_Type( self.source ) )
           and integration.hasEnoughtDotationForObjectType( DEC_GenObject_Type( self.source ) )
        if not result then -- $$$ MIA not the right place to do this report
            meKnowledge:sendReport( eRC_ConstructionObjetImpossible )
        end
        return result
    end )

-- Decontruction method: cannot deconstruct an object that is not existing
method "canBeDeconstructed" ( function( self ) return false end )
method "deconstructIt" ( function( self ) return false end )
method "removeIt" ( function( self ) return false end )

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