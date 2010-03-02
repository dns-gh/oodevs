defaultMethods
{
    -- DEFAULT METHODS NEEDED UNTIL SECTION INHERITE FROM
    -- net.masagroup.military.generic.world.Unit
    -- REACHABLE
    proximityLevel = function() return masalife.brain.defaultengine.methods.load( "proximityLevel") end,
    accessibilityLevel = function() return masalife.brain.defaultengine.methods.load( "accessibilityLevel") end,
    reachPriority = function() return masalife.brain.defaultengine.methods.load( "reachPriority") end,
    isDistant = function() return masalife.brain.defaultengine.methods.load( "generic_isDistant" ) end,
    isNearby = function() return masalife.brain.defaultengine.methods.load( "generic_isNearby" ) end,
    isFar = function() return masalife.brain.defaultengine.methods.load( "generic_isFar" ) end,
    isReached = function() return masalife.brain.defaultengine.methods.load( "generic_isReached" ) end,

    -- OBSERVABLE
    hostilityLevel = function() return masalife.brain.defaultengine.methods.load( "unit_hostilityLevel" ) end,
    isHostile = function () return masalife.brain.defaultengine.predicates.load( "generic_isHostile") end,
    isNeutral = function () return masalife.brain.defaultengine.methods.load( "generic_isNeutral") end,
    isFriend = function () return masalife.brain.defaultengine.methods.load( "generic_isFriend" ) end,
    perceptionLevel = function() return masalife.brain.defaultengine.methods.load( "element_perceptionLevel") end,
    observationPriority = function() return masalife.brain.defaultengine.methods.load( "generic_observationPriority" ) end,

    -- POSITIONNABLE
    isInMyAOR = function() return masalife.brain.defaultengine.predicates.load( "isInMyAOR") end,
    canTakePosition = function() return masalife.brain.defaultengine.methods.load( "canTakePosition" ) end,

    -- DESTROYABLE
    destructionLevel = function() return masalife.brain.defaultengine.methods.load( "destructionLevel" ) end,
    canDestroyIt = function() return masalife.brain.defaultengine.methods.load( "canDestroyIt" ) end,
    destructionPriority = function() return masalife.brain.defaultengine.methods.load( "destructionPriority" ) end,
    isDestroyed = function() return masalife.brain.defaultengine.predicates.load( "isDestroyed" ) end,
    isDangerous = function() return masalife.brain.defaultengine.predicates.load( "unit_isDangerous" ) end,
    destroyIt = function() return masalife.brain.defaultengine.methods.load( "destroyIt" ) end,
    dangerosityLevel = function() return masalife.brain.defaultengine.methods.load( "dangerosityLevel" ) end,

    -- NEUTRALIZABLE
    operationalLevel = function() return masalife.brain.defaultengine.methods.load( "unit_operationalLevel" ) end,
    isOperational = function() return masalife.brain.defaultengine.predicates.load( "generic_isOperational" ) end,
    canNeutralizeIt = function() return masalife.brain.defaultengine.methods.load( "unit_canNeutralizeIt" ) end,
    neutralizeIt = function() return masalife.brain.defaultengine.methods.load( "unit_neutralizeIt" ) end,
    neutralizationPriority = function() return masalife.brain.defaultengine.methods.load( "generic_neutralizationPriority" ) end,    
    
    -- COMMANDING
    isCommandingFor = function() return masalife.brain.defaultengine.methods.load( "isCommandingFor" ) end,
    isInMyTeam = function() return masalife.brain.defaultengine.methods.load( "isInMyTeam" ) end,

    --
    communicate = function() return masalife.brain.defaultengine.methods.load( "unit_communicate" ) end,

    -- REACHING
    reachEfficiency = function() return masalife.brain.defaultengine.methods.load( "reachEfficiency" ) end,
    isReachingFor = function() return masalife.brain.defaultengine.predicates.load( "isReachingFor" ) end,
    -- DESTROYING
    destructionEfficiency = function() return masalife.brain.defaultengine.methods.load( "destructionEfficiency" ) end,
    isDestroyingFor = function() return masalife.brain.defaultengine.predicates.load( "terrain_analysis_isDestroyingFor" ) end,

    -- NEUTRALIZING
    isNeutralizingFor = function() return masalife.brain.defaultengine.predicates.load( "terrain_analysis_isNeutralizingFor" ) end,
    neutralizationEfficiency = function() return masalife.brain.defaultengine.methods.load( "neutralizationEfficiency" ) end,
    

    -- RECONNOITRING
    isInMyAOR = function() return masalife.brain.defaultengine.predicates.load( "isInMyAOR" ) end,

    -- IDENTIFIABLE
    identificationPriority = function() return masalife.brain.defaultengine.methods.load( "identificationPriority" ) end,
    isDetected = function() return masalife.brain.defaultengine.methods.load( "isDetected" ) end,
    isRecognized = function() return masalife.brain.defaultengine.methods.load( "isRecognized" ) end,
    identificationLevel = function() return masalife.brain.defaultengine.methods.load( "identificationLevel" ) end,
    isIdentified = function() return masalife.brain.defaultengine.predicates.load( "isIdentified" ) end,
    -- SAFEGUARDABLE
    canRespond = function() return masalife.brain.defaultengine.methods.load( "canRespond" ) end,
    safetyPriority = function() return masalife.brain.defaultengine.methods.load( "safetyPriority" ) end,
    threatLevel = function() return masalife.brain.defaultengine.methods.load( "threatLevel" ) end,
    isThreatening = function() return masalife.brain.defaultengine.methods.load( "isThreatening" ) end,
    selfProtectFromIt = function() return masalife.brain.defaultengine.methods.load( "selfProtectFromIt" ) end,
    isThreateningOrHostile = function() return masalife.brain.defaultengine.predicates.load( "isThreateningOrHostile" ) end,
   
    -- FLEEABLE
    fleePriority = function() return masalife.brain.defaultengine.methods.load( "fleePriority" ) end,
    
}

masalife.brain.communication.setMessageTreatment( "Order", integration.communication.StartMissionPion )
 
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
    moveToIt = masalife.brain.integration.startStopAction( { start = integration.startMoveToIt, started = integration.updateMoveToIt, stop = integration.stopMoveToIt } ),

    -- observable action
    observeIt = function( self )
      integration.observeIt( self )
    end,
    -- Tactical analysis integration
    getPosition = function( self )
      return integration.getSectionPosition( self )
    end,
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
        return 100 -- TODO -- $$$ MIA: not used in skill yet... @MGD URGENT clean all concrete knowledge
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
      return 100 --TODO MGD ??? not do before??????
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
    
    neutralize = masalife.brain.integration.startStopAction( { start = integration.startNeutralizingIt, started = function( self, ...) end, stop = integration.stopNeutralizingIt } ),

    destroy = masalife.brain.integration.startStopAction( { start = integration.startDestroyingIt, started = function( self, ...) end, stop = integration.stopDestroyingIt } ),
    
    isOccupied = function( self )
      return true --TODO MGD
    end,
    identifyIt = function ( self )
      integration.identifyIt( self )
    end,

    getIdentificationState = function( self )
      return integration.getAutomatPerception( self )
    end,
    
    -- SAFEGUARDABLE
    respondToIt =  masalife.brain.integration.startStopAction( { start = integration.startDestroyingIt, started = function( self, ...) end, stop = integration.stopDestroyingIt } ),
    
    -- FLEEABLE
    fleeFromIt = function( self )
        -- $$$$ MIA: nothing by default.
    end,

    -- ILLUMINABLE
    illuminationPriority = function( self )
        return self:isDefinitelyIlluminated() and 0 or ( math.max( self:proximityLevel(), 1 ) ) / 100
    end,
    canIlluminateIt = function( self )
        return integration.canIlluminateIt( self )
    end,
    illuminationLevel = function( self )
        return integration.illuminationLevel( self )
    end,
    illuminateIt = masalife.brain.integration.startStopAction( { start = integration.startIlluminateIt, started = startedIlluminateIt, stop = integration.stopIlluminateIt } ),
 
    predicate "isDefinitelyIlluminated"
    {
        method = function( self )
                    return integration.isDefinitelyIlluminated( self )
                 end
    },
    -- ILLUMINATING
    illuminationEfficiency = function( self, objective )
        return computeDistance( self, objective )
    end,
    predicate "isIlluminatingFor" 
    { 
        method = function( self, objective )
                    return self:illuminationEfficiency( objective ) > 0
                 end 
    },
    -- INDIRECTFIREABLE
    canApplyFireOnIt = function( self, munition, interventionType )
        return integration.canApplyFireOnSection( self, munition, interventionType )
    end,
    applyFireOnIt = masalife.brain.integration.startStopAction( { start = integration.startApplyFireOnSection, started = integration.updateApplyFireOnSection, stop = integration.stopApplyFireOnSection } ),

    computeAggressiveness = function( self, target )
      return integration.computeAggressiveness( self, target )
    end,
    
tasks =
    {
        adder = function( self, task ) error( "unused method", 2 ) end,
        accepter = function( self, visitor )
                        for role in masalife.brain.knowledge.executeQuery( { knowledgeType = { directia.core.model.Role } } ) do
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
