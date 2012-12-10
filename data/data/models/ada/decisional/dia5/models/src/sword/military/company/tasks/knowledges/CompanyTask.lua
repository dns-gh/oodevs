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
            return { integration.randomPositionOnCircle( params.meetingPoint, 100 ) }  -- Need to send list of elements
        end
        return NIL
    end,
    
    getFinalMeetingPoint = function ( self, params ) -- Need to send a single element
        if params.meetingPoint and params.meetingPoint ~= NIL then
            return integration.randomPositionOnCircle( params.meetingPoint, 100 )
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
    
   getEntitiesToSupportDynamic = function( self, params )
        if not myself.leadData.objectiveDynamicSupportIndex then
            myself.leadData.entitiesToSupport = {}
            myself.leadData.objectiveDynamicSupportIndex = 0
            local entityToSupport
            if params.objective and params.objective ~= nil
             and ( masalife.brain.core.class.isOfType( params.objective, integration.ontology.types.automat)
             or masalife.brain.core.class.isOfType( params.objective, integration.ontology.types.agent) ) then
               entityToSupport = params.objective
            else
               entityToSupport = meKnowledge
            end
            myself.leadData.entitiesToSupport = integration.filterPionWithEchelon( integration.getEntitiesFromAutomatCommunication( entityToSupport, "none", true), eEtatEchelon_First )
            if #myself.leadData.entitiesToSupport < 1 then
                myself.leadData.entitiesToSupport = integration.getEntitiesFromAutomatCommunication( entityToSupport, "none", true)
            end
            myself.leadData.nbEntities = #myself.leadData.entitiesToSupport
        end
        local nbrEchelon = myself.taskParams.echelonNumber or 0
        if nbrEchelon == NIL or nbrEchelon == 0 then
            nbrEchelon = 1 -- default value
        end
        local nbFront = integration.query.getNbrFront( nbrEchelon )
        local nextObjectives = {}
        myself.leadData.nbPions = myself.leadData.nbPions or nbFront
        local nbcible = math.max( ( myself.leadData.nbEntities / myself.leadData.nbPions ), 1 )
        while #nextObjectives < nbcible do
            myself.leadData.objectiveDynamicSupportIndex = myself.leadData.objectiveDynamicSupportIndex % #myself.leadData.entitiesToSupport + 1
            nextObjectives[ #nextObjectives + 1 ]= myself.leadData.entitiesToSupport[ myself.leadData.objectiveDynamicSupportIndex ]
        end
        return nextObjectives
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