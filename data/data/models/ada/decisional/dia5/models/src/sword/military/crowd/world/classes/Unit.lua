predicate "isOperational"
{
    dependencies = {},
    method = function( self )
        return self:operationalLevel() ~= 0
    end 
}

return
{
    -- -------------------------------------------------------------------------------- 
    -- Integration methods
    -- --------------------------------------------------------------------------------
    getPosition = function( self )
        return DEC_Agent_PositionPtr( self.source )
    end,
    
    operationalLevel = function( self )
        return self.source:DEC_Agent_EtatOpsMajeur() * 100
    end,
    
    isReached = function( self )
        return integration.unitIsInCrowd( self )
    end,

    -- -------------------------------------------------------------------------------- 
    -- Specific action on units for crowd
    -- --------------------------------------------------------------------------------
    attackIt = masalife.brain.integration.startStopAction(
    { 
        start = integration.startAttackingIt,
        started = integration.updateAttackingIt,
        stop = integration.stopAttackingIt 
    } ),
}