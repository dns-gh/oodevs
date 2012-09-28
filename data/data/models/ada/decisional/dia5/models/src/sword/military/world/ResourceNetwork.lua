return 
{
    -- -------------------------------------------------------------------------------- 
    -- Specific class methods
    -- --------------------------------------------------------------------------------
    disableIt = function( self )
        DEC_ReseauRessource_DesactiverElement( self.source )
        return true
    end,
    enableIt = function( self )
        DEC_ReseauRessource_ActiverElement( self.source )
        return true
    end,
    startProduceResource = function( self )
        -- $$$ GGE : hard coded, manage the production value following unit physical abilities instead 100
        DEC_ReseauRessourceAugmenteProduction( self.source, 100)
       return true
    end,
    stopProduceResource = function( self )
        -- $$$ GGE : hard coded, manage the production decrease following unit physical abilities instead 100
        DEC_ReseauRessourceBaisseProduction( self.source, 100 )
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
        return DEC_ReseauRessource_Position( self.source )
    end,
    getMyPosition = function( self )
        return CreateKnowledge( sword.military.world.Point, DEC_ReseauRessource_Position( self.source ) )
    end,
    getPositions = function( self )
        return DEC_Geometrie_CalculerTrafficablePointPourPoint(DEC_ReseauRessource_Position( self.source ))
    end,
    getProximity = function( self, element )
        return integration.normalizedInversedDistance( self, element )
    end,
}