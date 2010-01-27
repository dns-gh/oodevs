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

    -- DESTROYABLE
    destructionLevel = function() return default_engine.methods.load( "destructionLevel" ) end,
    destructionPriority = function() return default_engine.methods.load( "destructionPriority" ) end,
    isDestroyed = function() return default_engine.predicates.load( "isDestroyed" ) end,
    isDangerous = function() return default_engine.predicates.load( "isDangerous" ) end,
    destroyIt = function() return default_engine.methods.load( "destroyIt" ) end,
    dangerosityLevel = function() return default_engine.methods.load( "dangerosityLevel" ) end,

    -- COMMANDING
    isCommandingFor = function() return default_engine.methods.load( "isCommandingFor" ) end,
    isInMyTeam = function() return default_engine.methods.load( "isInMyTeam" ) end,

    --
    communicate = function() return default_engine.methods.load( "unit_communicate" ) end,


    -- Destroying
    destructionEfficiency = function() return default_engine.methods.load( "destructionEfficiency" ) end,

    isDestroyingFor = function() return default_engine.predicates.load( "terrain_analysis_isDestroyingFor" ) end,

    --Reconnoitring
    isInMyAOR = function() return default_engine.predicates.load( "isInMyAOR" ) end,


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

--    predicate "isDestroyed" {
--      dependencies = {},
--      method = function( self )
--          if DEC_Agent_EtatOps == 0 then
--            return true
--          elseif DEC_Agent_EtatOpsMajeur == 0 then
--            return true
--          end
--          return false
--      end
--  },

    getAttrition = function( self, objective, position )
    local rPH = 0.5
    local rPorteeMax    = DEC_Tir_PorteeMaxPourTirerSurUnite( objective.source, rPH )
    local rPorteeMin    = DEC_Tir_PorteeMinPourTirerSurUnite( objective.source, rPH )
        local rDistanceAEni = integration.magnitude( position, objective )
        if( rDistanceAEni > rPorteeMax or rDistanceAEni < rPorteeMin ) then return 0 end
    return 100
    end,
--    destructionPriority = function()
--      return 100
--    end,
--    destructionLevel = function( self )
--      local etatOp = ( 1 - DEC_ConnaissanceAgent_EtatOps( self.source ) )*100
--    BreakForDebug( tostring ( etatOp) )
--      return etatOp
--    end,
  getDestructionState = function( self )
    local etatOp = ( 1 - DEC_ConnaissanceAgent_EtatOps( self.source ) )*100
    BreakForDebug( tostring ( etatOp) )
      return etatOp
  end,
    canDestroyIt = function()
      return true
    end,
    canTakePosition= function()
      return true
    end,
    takeUpPosition = function()
    end,
    occupationLevel = function()
      return 100
    end,
    safeApproachIt = function()
    end,
    isSafety = function()
      return true
    end,

--    destroyIt = function( self )
--    eEtatFeu_Executer = 1
--    eEtatFeu_aucun = 0
--        if not self.actionTir then
--          BreakForDebug(" if not self.actionTir then ")
--            F_Pion_SeteEtatFeu( myself, eEtatFeu_Executer )
--            self.actionTir = DEC_StartTirDirect( self.source, 100, eTirDirectNormal, -1 )
--           -- actionCallbacks[ actionTir ] = function( arg ) self.eTir = arg end
--            --eActionTirDirect_Impossible ? $$$$ LDC what must we do in that case?
--        elseif self.eTir ==  eActionTirDirect_NoAmmo
--                or self.eTir == eActionTirDirect_NoCapacity
--                or self.eTir == eActionTirDirect_EnemyDestroyed
--                or self.eTir == eActionTirDirect_Finished then
--            BreakForDebug(" elseif self.eTir ")
--            F_Pion_SeteEtatFeu( myself, eEtatFeu_aucun )
--            DEC_StopAction( self.actionTir )
--            self.actionTir = nil
--            self.eTir = nil
--       else
--          BreakForDebug(" else ")
--       end
--    end

 destroy = function( self )
  integration.destroyIt( self )
 end,

 isOccupied = function( self )
  return true
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