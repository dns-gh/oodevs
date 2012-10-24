return 
{
    -- -------------------------------------------------------------------------------- 
    -- Specific class methods
    -- --------------------------------------------------------------------------------
    disableIt = function( self )
        integration.disableResourceNode( self.source )
        return true
    end,
    enableIt = function( self )
        integration.enableResourceNode( self.source )
        return true
    end,
    startProduceResource = function( self )
        -- $$$ GGE : hard coded, manage the production value following unit physical abilities instead 100
        integration.increaseResourceNodeProduction( self.source, 100)
       return true
    end,
    stopProduceResource = function( self )
        -- $$$ GGE : hard coded, manage the production decrease following unit physical abilities instead 100
        integration.decreaseResourceNodeProduction( self.source, 100 )
        return true
    end,

    -- -------------------------------------------------------------------------------- 
    -- Integration methods
    -- --------------------------------------------------------------------------------
    moveToIt = function( self )
        -- NOTHING Not used yet
    end,
    destroyMoveToIt = function( self ) 
        -- NOTHING Not used yet
    end,
    observeIt = function( self )
        return meKnowledge:orientateSensor( self )
    end,
    getPosition = function( self )
        return integration.getRessourceNodePosition( self.source )
    end,
    getMyPosition = function( self )
        return CreateKnowledge( world.Point, integration.getRessourceNodePosition( self.source ) )
    end,
    getPositions = function( self )
        return integration.getUnitPositions(integration.getRessourceNodePosition( self.source ))
    end,
    getProximity = function( self, element )
        return integration.normalizedInversedDistance( self, element )
    end,
}