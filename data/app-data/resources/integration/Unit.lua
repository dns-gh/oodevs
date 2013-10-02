------------------------------------------------------------------
--- IDENTIFIABLE INTERFACE IMPLEMENTATION SPECIFIC TO AN UNIT
------------------------------------------------------------------

integration.identificationPriority = function( self )
    return math.max( self:proximityLevel(), 1 ) / 100
end

integration.canIdentifyIt = function( self )
    return not self:isIdentified() and masalife.brain.knowledge.me.body:computeIdentificationCapability( self, masalife.brain.knowledge.me.body ) ~= 0
end

integration.isIdentifiedPredicate = {
    dependencies = { self = { "identificationLevel_" } },
    method = function( self )
        return self:identificationLevel() >= 66 and self:identificationLevel() <= 100
    end
}

integration.setDecisionalState = function( category, value)
    DEC_DecisionalState( category, value )
end

integration.setForceRatioState = function( value )
    DEC_Agent_ChangeEtatRapportDeForce( value )
end

integration.setOperationalState = function( value )
    DEC_Agent_ChangeEtatOperationnel( value )
end

integration.getAgentMajorOpsState = function( agent )
    return agent:DEC_Agent_EtatOpsMajeur()
end

integration.getAgentOpsState = function( agent )
    if agent then
        return agent:DEC_Agent_EtatOps()
    else
        return DEC_Agent_EtatOps()
    end
end

integration.isAgentImmobilized = function( agent )
    return DEC_Agent_EstImmobilise( agent )
end

integration.isKnowledgeAgentValid = function( agent )
    return DEC_ConnaissanceAgent_EstValide( agent )
end

integration.isKnowledgesAgentsInArea = function( area )
    return DEC_Connaissances_UnitesEnnemiesVivantesDansZone( area )
end

integration.getFriendsInArea = function( area )
    local friends = DEC_Connaissances_UnitesAmiesDansZone( area )
    local DEC_ConnaissanceAgent_EnAgent = DEC_ConnaissanceAgent_EnAgent
    for i=1,#friends do
        if DEC_ConnaissanceAgent_EnAgent( friends[ i ] ) == myself then
            friends[ i ] = nil
            break
        end
    end
    
    return friends
end

integration.getKnowledgesLivingAgentsInCircle = function( position, distance )
    return DEC_Connaissances_UnitesEnnemiesVivantesDansCercle( position, distance )
end

integration.getHeightForKnowledgeAgent = function( agent )
    return DEC_ConnaissanceAgent_Altitude( agent )
end

integration.getKnowledgeAgentOperationalState = function( agent )
    return DEC_ConnaissanceAgent_EtatOps( agent )
end

-- Allows the unit to deactivate default behaviours, such as self-protection for instance.
-- The table myself.deactivations contains all the current deactivated behaviours.
-- @param behaviour : A string describing a behaviour (e.g. "selfprotect")
integration.deactivate = function( behaviour )
    local myself = myself
    myself.deactivations = myself.deactivations or {}
    myself.deactivations[ behaviour ] = ( myself.deactivations[ behaviour ] or 0 ) + 1
end

-- Allows the unit to reactivate previously deactivated behaviours.
-- @param behaviour : A string describing a behaviour (e.g. "selfprotect")
integration.reactivate = function( behaviour )
    local myself = myself
    myself.deactivations = myself.deactivations or {}
    if not myself.deactivations[ behaviour ] then
        error("Reactivating " .. tostring( behaviour ) .. " which has not been deactivated.")
    end
    myself.deactivations[ behaviour ] = myself.deactivations[ behaviour ] - 1
    if myself.deactivations[ behaviour ] == 0 then
        myself.deactivations[ behaviour ] = nil
    end
end

-- Returns true if the provided behaviour is currently inactive, false otherwise
-- @param behaviour : A string describing a behaviour (e.g. "selfprotect")
integration.isInactive = function( behaviour )
    return myself.deactivations and myself.deactivations[ behaviour ]
end

------------------------------------------------------------------
--- SAFEGUARDABLE INTERFACE IMPLEMENTATION SPECIFIC TO AN UNIT
------------------------------------------------------------------
integration.threatLevel = function( self )
    return self:computeAggressiveness()
end

------------------------------------------------------------------
--- FLEEABLE INTERFACE IMPLEMENTATION SPECIFIC TO AN UNIT
------------------------------------------------------------------
integration.fleePriority = function( self )
    return math.max( self:proximityLevel(), 1 ) / 100
end

------------------------------------------------------------------
--- COMMANDING INTERFACE IMPLEMENTATION SPECIFIC TO AN UNIT
-------------------------------------------------------------------


integration.unit_commandEfficiency = function( self, individualTask, leadTask, maxNormalization, taskParams, threat )
local effects =
{
    -- Mvt effects
    "reachEffectLevel",
    "occuppyEffectLevel",
    "closeToEffectLevel",

    -- Engagement effects
    "engageEffectLevel",
    "neutralizeEffectLevel",
    "destroyEffectLevel",

    -- Observation effects
    "perceiveEffectLevel",
    "identifyEffectLevel",

     -- Reconnaissance
    "reconnaissanceEffectLevel",

    --safeguard
    "safeApproachEffectLevel",
    "selfProtectionEffectLevel",

    -- Mount / Dismount
    "mountEffectLevel",
    "dismountEffectLevel",

    -- load / unload
    "loadEffectLevel",
    "unloadEffectLevel",

    -- threats resistance
    "threatResistanceEffectLevel"
}
    -- Verify the validity of parameters
    if not leadTask or not individualTask:parametercompatible( leadTask ) then return 0 end

    local cloneEffects = function( taskEffects )
        local res = {}
        for _, level in ipairs( effects ) do
            res[ level ] = taskEffects[ level ] or 0
        end
        return res
    end
    if individualTask.computeEffectsLevels == nil then return 0 end
    local individualTaskEffects = individualTask:computeEffectsLevels()
    local leadTaskEffects = leadTask:computeEffectsLevels()
    local ponderateTaskEffects = cloneEffects( individualTaskEffects )
    if not next( ponderateTaskEffects ) then return 0 end
    leadTask:ponderateEffectsForCommandEfficiency( self, taskParams, threat, ponderateTaskEffects )

    local res, default = 0, 0
    for _, level in ipairs( effects ) do
        local l1 = ( leadTaskEffects[ level ] or default ) / 100
        local l2 = ( ponderateTaskEffects[ level ] or default ) / 100
        res = res + l1 * ( l1 < l2 and l2 - l1 or l1 - l2 )
    end
    return maxNormalization - res / #effects
end

integration.unit_communicate = function( self, task, objectives )
    integration.SendMessage( "Order", self, { order = task.name, params = objectives } )
end

integration.isThreateningOrHostilePredicate = {
     dependencies = "none",
        method = function( self )
            return self:isHostile() or self:isThreatening()
        end
    }

integration.getName = function( agent )
    return DEC_GetSzName( agent.source )
end

integration.getWoundedOrDeadUnitsInCircle = function( position, radius )
    local woundedUnitsInCircle = {}
    woundedUnitsInCircle = DEC_Connaissances_UnitesBlesseesOuTueesDansCercle( position, radius )
    return woundedUnitsInCircle
end

integration.getCiviliansInArea = function( area )
    return DEC_Connaissances_UnitesCivilesDansZone( area.source )
end

integration.getWoundedInArea = function( area )
    return DEC_Connaissances_UnitesBlesseesDansZone( area.source )
end

integration.getFiringDistanceToEngageMe = function( platoon, ph )
    return DEC_Tir_PorteeMaxPourEtreTireParUnite( platoon.source, ph)
end

-- Friend is flying?
integration.agentIsFlying = function( teammate )
    return teammate.source:DEC_Agent_EstEnVol()
end

integration.agentCanFly = function( teammate )
    return DEC_Agent_PionCanFly( teammate.source )
end

integration.getCollidingCrowds = function( self )
    local simCrowds = DEC_Connaissances_CollisionsPopulations()
    local crowds = {}
    for i = 1, #simCrowds do
        crowds[ i ] = CreateKnowledge( integration.ontology.types.population, simCrowds[ i ] )
    end
    return crowds
end