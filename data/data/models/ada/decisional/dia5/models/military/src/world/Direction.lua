return
{
    -- -------------------------------------------------------------------------------- 
    -- High level methods 
    -- --------------------------------------------------------------------------------
    observationPriority = function( self )
        return 1
    end,

    -- -------------------------------------------------------------------------------- 
    -- Integration methods
    -- --------------------------------------------------------------------------------
    observeIt = function( self )
        return meKnowledge:orientateSensor( self )
    end,
    computePerceptionCapabilityFor = function( self, entity, position )
        return 100
    end,
    computeRelation = function( self )
        return 100
    end,
    getPerception = function( self )
        return 100
    end
}
