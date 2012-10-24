return
{
    getSealOffObjective = function( self, params )
        return params.objective
    end,

    getSealOffPosition = function( self, params )
        myself.leadData.sealOffIndex = myself.leadData.sealOffIndex or 0 
        if masalife.brain.core.class.isOfType( params.objective, world.UrbanBlock) then
            myself.leadData.sealOffPositions = myself.leadData.sealOffPositions or integration.locationInPoints( integration.polylineUrbanBlock( params.objective.source ) )
        else
            myself.leadData.sealOffPositions = myself.leadData.sealOffPositions or integration.locationInPoints( params.objective.source )
        end
        local number = math.floor( (#myself.leadData.sealOffPositions / meKnowledge.nbPionsMain ) +1 )

        myself.leadData.sealOffIndex = ( myself.leadData.sealOffIndex + number ) % #myself.leadData.sealOffPositions + 1
        return CreateKnowledge( world.Point, myself.leadData.sealOffPositions[myself.leadData.sealOffIndex] )
    end,

    getMeetingPoint = function( self, params )
        return params.objective:getMyPosition()
    end,

    getWithImprovement = function( self, params )
        return params.withImprovement or false
    end,
}