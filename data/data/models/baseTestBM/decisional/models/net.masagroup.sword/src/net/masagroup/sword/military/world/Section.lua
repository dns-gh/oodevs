defaultMethods
{
    -- DEFAULT METHODS NEEDED UNTIL SECTION INHERITE FROM
    -- net.masagroup.military.generic.world.Unit
    -- REACHABLE
    proximityLevel = function() return default_engine.methods.load( "proximityLevel") end,
    accessibilityLevel = function() return default_engine.methods.load( "accessibilityLevel") end,
    reachPriority = function() return default_engine.methods.load( "reachPriority") end,
    isDistant = function() return default_engine.methods.load( "generic_isDistant" ) end,
    isNearby = function() return default_engine.methods.load( "generic_isNearby" ) end,
    isFar = function() return default_engine.methods.load( "generic_isFar" ) end,
    isReached = function() return default_engine.methods.load( "generic_isReached" ) end,

    -- OBSERVABLE
    hostilityLevel = function() return default_engine.methods.load( "unit_hostilityLevel" ) end,
    isHostile = function () return default_engine.predicates.load( "generic_isHostile") end,
    isNeutral = function () return default_engine.methods.load( "generic_isNeutral") end,
    isFriend = function () return default_engine.methods.load( "generic_isFriend" ) end,
    perceptionLevel = function() return default_engine.methods.load( "element_perceptionLevel") end,
    observationPriority = function() return default_engine.methods.load( "generic_observationPriority" ) end,

    -- POSITIONNABLE
    isInMyAOR = function() return default_engine.predicates.load( "isInMyAOR") end,
    canTakePosition = function() return default_engine.methods.load( "canTakePosition" ) end,

    -- DESTROYABLE
    destructionLevel = function() return default_engine.methods.load( "destructionLevel" ) end,
    canDestroyIt = function() return default_engine.methods.load( "canDestroyIt" ) end,
    destructionPriority = function() return default_engine.methods.load( "destructionPriority" ) end,
    isDestroyed = function() return default_engine.predicates.load( "isDestroyed" ) end,
    isDangerous = function() return default_engine.predicates.load( "isDangerous" ) end,
    destroyIt = function() return default_engine.methods.load( "destroyIt" ) end,
    dangerosityLevel = function() return default_engine.methods.load( "dangerosityLevel" ) end,

    -- NEUTRALIZABLE
    operationalLevel = function() return default_engine.methods.load( "unit_operationalLevel" ) end,
    isOperational = function() return default_engine.predicates.load( "generic_isOperational" ) end,
    canNeutralizeIt = function() return default_engine.methods.load( "unit_canNeutralizeIt" ) end,
    neutralizeIt = function() return default_engine.methods.load( "unit_neutralizeIt" ) end,
    neutralizationPriority = function() return default_engine.methods.load( "generic_neutralizationPriority" ) end,    
    
    -- COMMANDING
    isCommandingFor = function() return default_engine.methods.load( "isCommandingFor" ) end,
    isInMyTeam = function() return default_engine.methods.load( "isInMyTeam" ) end,

    --
    communicate = function() return default_engine.methods.load( "unit_communicate" ) end,


    -- DESTROYING
    destructionEfficiency = function() return default_engine.methods.load( "destructionEfficiency" ) end,
    isDestroyingFor = function() return default_engine.predicates.load( "terrain_analysis_isDestroyingFor" ) end,

    -- NEUTRALIZING
    isNeutralizingFor = function() return default_engine.predicates.load( "terrain_analysis_isNeutralizingFor" ) end,
    neutralizationEfficiency = function() return default_engine.methods.load( "neutralizationEfficiency" ) end,
    

    -- RECONNOITRING
    isInMyAOR = function() return default_engine.predicates.load( "isInMyAOR" ) end,

    -- IDENTIFIABLE
    identificationPriority = function() return default_engine.methods.load( "identificationPriority" ) end,
    isDetected = function() return default_engine.methods.load( "isDetected" ) end,
    isRecognized = function() return default_engine.methods.load( "isRecognized" ) end,
    identificationLevel = function() return default_engine.methods.load( "identificationLevel" ) end,
    isIdentified = function() return default_engine.predicates.load( "isIdentified" ) end,

}

communication.setMessageTreatment( "Order", integration.communication.StartMissionPion )
 
return
{
    -- $$$ MIA: temp, to move in default military implementation
    isTotallyPerceived = function( self )
        return self:perceptionLevel() == 100
    end,
    isPartiallyPerceived = function( self )
      return self:perceptionLevel() > 25
    end,

    -- POSITIONNABLE
    takeUpPosition = function( self )
        return integration.takeUpPosition( self )
    end,

    -- INTEGRATION METHODS
    -- reachable action
    moveToIt = behavior_model.integration.startStopAction( { start = integration.startMoveToIt, started = function( self, ...) end, stop = integration.stopMoveToIt } ),

    -- observable action
    observeIt = function( self )
      integration.observeIt( self )
    end,
    -- Tactical analysis integration
    computeDistance = function( self, target )
        return integration.normalizedInversedDistance( self, target )
    end,
    computeRelation = function( self )
        return integration.computeRelationAgent( self )
    end,
    computePerceptionCapability = function( self, objective, keypoint )
        if( integration.magnitude( keypoint, objective ) > 1000 ) then
            return 0
        else
            --@TODO MGD Possible update : use true perception possibility
            return integration.normalizedInversedDistance( objective, keypoint )
        end
    end,
    computeMovementCapability = function( self )
        return 100 -- TODO -- $$$ MIA: not used in skill yet...
    end,
    getPerception = function( self )
        return integration.getAgentPerception( self )
    end,
    -- RECONNOITRABLE --
    computeReconnaissanceCapability = function(self, objective )
       return objective:isReached() and 100 or 0 --@TODO Add physic like fantassin number
    end,
    setFront = function( self )
        --TODO
    end,
    setSupport = function( self )
        --TODO
    end,
    commandEfficiency= function( self )
        --TODO
    end,

    getAttrition = function( self, target, position )
     return integration.getAttrition( self, target, position )
    end,

    occupationLevel = function()
      return 100
    end,
    safeApproachIt = function()
    end,
    isSafety = function()
      return true
    end,
    getOperationalCapacity = function( self )
      return integration.getOperationalCapacity( self )
    end,
    
    firePermitted = function( self )
      return integration.firePermitted( self )
    end,
   
    getDestructionState = function( self )
      return integration.getDestructionState( self )
    end,
    
    getAttritionForNeutralization = function( self, target, position )
      return integration.getAttritionForNeutralization( self, target, position )
    end,
    
    neutralize = behavior_model.integration.startStopAction( { start = integration.startNeutralizingIt, started = function( self, ...) end, stop = integration.stopNeutralizingIt } ),

    destroy = behavior_model.integration.startStopAction( { start = integration.startDestroyingIt, started = function( self, ...) end, stop = integration.stopDestroyingIt } ),

    isOccupied = function( self )
      return true
    end,
    identifyIt = function ( self )
      integration.identifyIt( self )
    end,

    getIdentificationState = function( self )
      return integration.getAutomatPerception( self )
    end,


tasks =
    {
        adder = function( self, task ) error( "unused method", 2 ) end,
        accepter = function( self, visitor )
                        for role in kBase.executeQuery( { knowledgeType = { directia.core.model.Role } } ) do
                            if role.body == self then
                              return role.tasks:accept( visitor )
                            end
                        end
                   end,
        remover = function( self, task )
                      error( "unused method", 2 )
                  end
     }
}
