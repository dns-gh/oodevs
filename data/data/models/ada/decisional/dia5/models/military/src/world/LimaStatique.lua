return
{
    -- OBSERVABLE
    computeRelation = function( self )
        return 0 -- MGD Maybe add area occupation later
    end,
    observeIt = function( self )
        return meKnowledge:orientateSensor( self )
    end,
    getPosition = function( self )
        return integration.getAreaPosition( self )
    end,
    getMyPosition = function( self )
      return CreateKnowledge( world.Point, self:getPosition() )
    end
}
