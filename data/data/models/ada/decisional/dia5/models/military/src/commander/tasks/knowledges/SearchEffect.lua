return
{

    getScoutObjectives = function( self, params, entity )
        return { params.objective }
    end,
     
    getDirection = function( self, params, entity )
        return params.objective
    end,
    
    getNbrFront = function( self )
        local nbrEchelon = myself.taskParams.echelonNumber or 0
        if nbrEchelon == NIL or nbrEchelon == 0 then
            nbrEchelon = 2
        end
        return integration.query.getNbrFront( nbrEchelon )
    end,
    
    getObjectives = function( self, params )
        local objective = params.objective
        if masalife.brain.core.class.isOfType( objective, world.Area) then
            if not myself.leadData.currentPosition then
                myself.leadData.currentPosition = 0 
                integration.splitArea( params.objective, math.min( self:getNbrFront(), params.maxNbrFront ) )
            end
            myself.leadData.currentPosition = myself.leadData.currentPosition % #myself.leadData.subAreas + 1
            objective = myself.leadData.subAreas[myself.leadData.currentPosition]
        end
        return { objective }
    end,
    
    getSealOffObjective = function( self, params )
        return params.objective
    end,
    
    getSealOffPosition = function( self, params )
        myself.leadData.sealOffIndex = myself.leadData.sealOffIndex or 0 
        if masalife.brain.core.class.isOfType( params.objective, world.UrbanBlock) then
            myself.leadData.sealOffPositions = myself.leadData.sealOffPositions or integration.locationInPoints(integration.polylineUrbanBlock( params.objective.source ))
        else
            myself.leadData.sealOffPositions = myself.leadData.sealOffPositions or integration.locationInPoints( params.objective.source )
        end
        local number = math.floor( (#myself.leadData.sealOffPositions / meKnowledge.nbPionsMain ) +1 )

        myself.leadData.sealOffIndex = ( myself.leadData.sealOffIndex + number ) % #myself.leadData.sealOffPositions + 1
        return CreateKnowledge( world.Point, myself.leadData.sealOffPositions[myself.leadData.sealOffIndex] )
    end,
    
    getTotalRecce = function( self )
        return true
    end,
}
