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

integration.getAgentOpsState = function()
    return DEC_Agent_EtatOps()
end

integration.isKnowledgeAgentValid = function( agent )
    return DEC_ConnaissanceAgent_EstValide( agent )
end

integration.isKnowledgesAgentsInArea = function( area )
    return DEC_Connaissances_UnitesEnnemiesVivantesDansZone( area )
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
