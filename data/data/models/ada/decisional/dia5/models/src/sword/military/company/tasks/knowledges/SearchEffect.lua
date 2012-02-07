return
{

    getObjectivesPei = function( self, params, entity )
        return { params.objective }
    end,
     
    getDirection = function( self, params, entity )
        return params.objective
    end,
    
    getNbrFront = function( self )
        local nbrEchelon = myself.taskParams.echelonNumber or 0
        if nbrEchelon == 0 then
            nbrEchelon = 2
        end
        return integration.query.getNbrFront( nbrEchelon )
    end,
    
    getObjectives = function( self, params )
        local objective = params.objective
        if masalife.brain.core.class.isOfType( objective, sword.military.world.Area) then
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
    
    getTotalRecce = function( self )
        return true
    end,
}
