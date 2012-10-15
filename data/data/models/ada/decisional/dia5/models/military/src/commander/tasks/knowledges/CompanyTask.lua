return
{
    init = function( self )
      --NOTHING
    end,
    setMustBeEvaluate = function( self, value )
        --NOTHING
    end,
    
    getMeetingPoint = function( self, params )
        if params.meetingPoint and params.meetingPoint ~= NIL then
            return { integration.randomPositionInCircle( params.meetingPoint, 50 ) }  -- Need to send list of elements
        end
        return NIL
    end,
    
    getFinalMeetingPoint = function ( self, params ) -- Need to send a single element
        if params.meetingPoint and params.meetingPoint ~= NIL then
            return integration.randomPositionInCircle( params.meetingPoint, 100 )
        end
        return NIL
    end,
    
    mustBeEvaluate = function( self )
        return false
    end,
    
    getEntitiesToSupport = function( self, params )
        local entitiesToSupport = integration.filterPionWithEchelon( integration.getEntitiesFromAutomat( meKnowledge, "none", true ), eEtatEchelon_First )
        if next(entitiesToSupport) then
          return entitiesToSupport
        else -- par défaut on renvoit le PC
          return {integration.query.getPCUnit()}
        end
    end,    
    
    getEntitiesToFollow = function( self, params )
        local entitiesToFollow = integration.filterPionWithEchelon( integration.getEntitiesFromAutomat( meKnowledge, "none", true ), eEtatEchelon_First )
         if next(entitiesToFollow) then
          return entitiesToFollow
        else -- par défaut on renvoit le PC
          return {integration.query.getPCUnit()}
        end
    end,
    
    getObstaclesPlan = function( self, params )
        return params.obstacles or {}
    end,
    
    getNbrFront = function( self )
      local nbrEchelon = myself.taskParams.echelonNumber or 0
      return integration.query.getNbrFront( nbrEchelon )
    end,
    
    computeEffectsLevels = function()
        return nil -- TODO MGD merge with masalife approach if possible
    end,
        
    getObstaclesPlanForMelee = function( self, params )
      if integration.query.getNbrGEN () == 0 then -- on ne renvoit des obstacles ssi l'automate ne contient pas de pions GEN
          return params.obstacles or {}
      end
      return {}
    end,
    
    getEntityToReinforce = function( self, params )
        myself.leadData.objectiveReinforces = myself.leadData.objectiveReinforces or self:getEntitiesToSupport( params )
        myself.leadData.objectiveReinforceIndex =  myself.leadData.objectiveReinforceIndex or 0
        myself.leadData.objectiveReinforceIndex = myself.leadData.objectiveReinforceIndex % #myself.leadData.objectiveReinforces + 1
        return myself.leadData.objectiveReinforces[myself.leadData.objectiveReinforceIndex]
    end,
}