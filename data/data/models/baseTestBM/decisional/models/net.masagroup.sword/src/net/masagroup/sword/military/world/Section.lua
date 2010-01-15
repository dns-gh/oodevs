defaultMethods
{
    -- DEFAULT METHODS NEEDED UNTIL SECTION INHERITE FROM 
    -- net.masagroup.military.generic.world.Unit
    -- REACHABLE
    proximityLevel = function() return default_engine.methods.load( "proximityLevel") end,
    accessibilityLevel = function() return default_engine.methods.load( "accessibilityLevel") end,
    reachPriority = function() return default_engine.methods.load( "reachPriority") end,
    isDistant = function() return default_engine.methods.load( "isDistant" ) end,
    isNearby = function() return default_engine.methods.load( "isNearby" ) end,
    isFar = function() return default_engine.methods.load( "isFar" ) end,
    isReached = function() return default_engine.methods.load( "isReached" ) end,

   -- OBSERVABLE
    hostilityLevel = function() return default_engine.methods.load( "unit_hostilityLevel" ) end,
    isHostile = function () return default_engine.predicates.load( "isHostile") end,
    isNeutral = function () return default_engine.methods.load( "isNeutral") end,
    isFriend = function () return default_engine.methods.load( "isFriend" ) end,
    perceptionLevel = function() return default_engine.methods.load( "perceptionLevel") end,
    observationPriority = function() return default_engine.methods.load( "observationPriority" ) end,
    
    -- COMMANDING
    isCommandingFor = function() return default_engine.methods.load( "isCommandingFor" ) end,
    isInMyTeam = function() return default_engine.methods.load( "isInMyTeam" ) end,
    
    -- 
    communicate = function() return default_engine.methods.load( "unit_communicate" ) end,
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

    -- INTEGRATION METHODS
    -- reachable action
    moveToIt = function( self )
        return integration.moveToIt( self )
    end,
    -- observable action
    observeIt = function( self )
        --TODO
    end,
    -- Tactical analysis integration
    computeDistance = function( self, target )
        return integration.normalizedInversedDistance( self, target )
    end,
    computeRelation = function( self )
        return 100 --TODO
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
        return 0 -- TODO
    end,
    computeReconnaissanceCapability = function(self, objective, keypoint )
    	return integration.normalizedInversedDistance( objective, keypoint )
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