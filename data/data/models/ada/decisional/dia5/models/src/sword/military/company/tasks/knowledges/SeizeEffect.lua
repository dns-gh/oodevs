return
{   

    getPeiObjectives = function( self, params, entity )     
        return params.objectives 
    end,
    
    getReachable = function( self, params )
         local objective = params.objectives[ next( params.objectives ) ]
         if masalife.brain.core.class.isOfType( objective, sword.military.world.Area ) then
           if not myself.leadData.currentPosition then
              myself.leadData.currentPosition = 0 
              integration.splitArea( objective, math.min( self:getNbrFront(), params.maxNbrFront ) )
           end
          myself.leadData.currentPosition = myself.leadData.currentPosition % #myself.leadData.subAreas + 1
          return myself.leadData.subAreas[myself.leadData.currentPosition]
        else
         return objective
        end
    end,
    
    getObjectiveGen = function( self, params )
        local objective = self:getReachable( params )
        return { objective:getMyPosition() }
    end,
    
    getNbrFront = function( self )
      local nbrEchelon = myself.taskParams.echelonNumber or 0
      if nbrEchelon == 0 then
        nbrEchelon = 2 -- default value
      end
      return integration.query.getNbrFront( nbrEchelon )
    end
}
