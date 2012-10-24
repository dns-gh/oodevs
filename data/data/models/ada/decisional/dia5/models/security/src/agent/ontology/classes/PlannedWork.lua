-- --------------------------------------------------------------------------------
-- Work
-- --------------------------------------------------------------------------------
method "buildIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self )
        integration.startBuildIt( self, agent.ontology.classes.Object )
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

method "createIt" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self )
        integration.startBuildItInstantaneously( self, agent.ontology.classes.Object )
        return self.knowledge
    end,
    started = function( self )
        return self.knowledge
    end, 
    stop = function( self )
        return self.knowledge
    end
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
    function( self, instantaneously )
        if instantaneously then 
            return true -- magic action
        else
            local result = integration.canBuildNowObjectType( integration.getGenObjectType( self.source ) )
               and integration.hasEnoughtDotationForObjectType( integration.getGenObjectType( self.source ) )
            if not result then -- $$$ MIA not the right place to do this report
                meKnowledge:sendReport( eRC_ConstructionObjetImpossible )
            end
            return result
        end
    end )

method "getDestinationForWork" ( 
    function( self ) 
        local simPosition = integration.getPlannedObjectNearestBorderPosition( self )
        return CreateKnowledge( agent.ontology.classes.Position, simPosition )
    end )

-- Decontruction method: cannot deconstruct an object that is not existing
method "canBeDeconstructed" ( function( self, instantaneously ) return false end )
method "deconstructIt" ( function( self ) return false end )
method "removeIt" ( function( self ) return false end )
method "isDeconstructed" ( function( self ) return false end )

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