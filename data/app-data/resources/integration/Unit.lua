--- Returns the provided agent knowledge's priority to be identified
-- @param agent Directia agent knowledge
-- @return Float, the identification priority (between 0 and 1)
integration.identificationPriority = function( agent )
    return math.max( agent:proximityLevel(), 1 ) / 100
end

--- Returns true if this entity can identify the provided agent knowledge, false otherwise
-- @param agent Directia agent knowledge
-- @return Boolean, whether or not this entity can identify the agent knowledge
integration.canIdentifyIt = function( agent )
    return not agent:isIdentified() and masalife.brain.knowledge.me.body:computeIdentificationCapability( agent, masalife.brain.knowledge.me.body ) ~= 0
end

--- Deprecated
integration.isIdentifiedPredicate = {
    dependencies = { self = { "identificationLevel_" } },
    method = function( self )
        return self:identificationLevel() >= 66 and self:identificationLevel() <= 100
    end
}

--- Sets this entity's decisional state for a given category.
-- The decisional state is a table of (string, string) which can be used to store information.
-- For instance, the category can be 'Echelon', 'Contact'...
-- @param category String, the name of the category
-- @param value String, the new value of the category
integration.setDecisionalState = function( category, value)
    DEC_DecisionalState( category, value )
end

--- Sets the force ratio state to the provided value
-- @param value Integer, the force ratio state. The possible values are :
-- <ul> <li> eForceRatioStateNone </li>
-- <li> eForceRatioStateNeutral </li>
-- <li> eForceRatioStateFavorable </li>
-- <li> eForceRatioStateUnfavorable </li> </ul>
integration.setForceRatioState = function( value )
    DEC_Agent_ChangeEtatRapportDeForce( value )
end

--- Sets the operational state to the provided value
-- @param value Integer, the operational state. The possible values are :
-- <ul> <li> eEtatDestruction_None </li>
-- <li> eEtatDestruction_Tactique </li>
-- <li> eEtatDestruction_Total </li> </ul>
integration.setOperationalState = function( value )
    DEC_Agent_ChangeEtatOperationnel( value )
end

--- Returns the provided agent's major operational state
-- @param agent Simulation agent
-- @return Float (between 0 and 1)
integration.getAgentMajorOpsState = function( agent )
    return agent:DEC_Agent_EtatOpsMajeur()
end

--- Returns the provided agent's operational state
-- If no agent is provided, this function returns this entity's operational state instead
-- If no agent is provided, this function will fail if called by anything but an agent.
-- @param agent Simulation agent (optional)
-- @return Float (between 0 and 1)
integration.getAgentOpsState = function( agent )
    if agent then
        return agent:DEC_Agent_EtatOps()
    else
        return DEC_Agent_EtatOps()
    end
end

--- Returns the provided agent knowledge's operational state
-- @param agent Simulation agent knowledge
-- @return Float (between 0 and 1)
integration.getKnowledgeAgentOperationalState = function( agent )
    return DEC_ConnaissanceAgent_EtatOps( agent )
end

--- Returns true if the provided agent is immobilized, false otherwise
-- @param agent Simulation agent
-- @return Boolean, whether or not the agent is immobilized
integration.isAgentImmobilized = function( agent )
    return DEC_Agent_EstImmobilise( agent )
end

--- Returns true if the provided agent is valid, false otherwise
-- @param agent Simulation agent
-- @return Boolean, whether or not the agent is valid
integration.isAgentValid = function( agent )
    return DEC_Agent_EstValide( agent )
end

--- Returns true if the provided agent knowledge is valid, false otherwise
-- @param agent Simulation agent knowledge
-- @return Boolean, whether or not the agent knowledge is valid
integration.isKnowledgeAgentValid = function( agent )
    return DEC_ConnaissanceAgent_EstValide( agent )
end

--- Returns all the agent knowledges inside the provided area
-- @param area Simulation area
-- @return List of simulation agent knowledges
integration.getAgentKnowledgesInArea = function( area )
    return DEC_Connaissances_UnitesEnnemiesVivantesDansZone( area )
end

--- Returns all the known friendly units inside the provided area
-- This entity will not be among the returned friendly units
-- @param area Simulation area
-- @return Table of friendly simulation agent knowledges
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

--- Returns all the known friendly units inside the circle defined by
--- the provided center and distance.
-- @param position Simulation position, the center of the circle
-- @param distance Float, the radius of the circle
-- @return List of living simulation agent knowledges in the circle
integration.getKnowledgesLivingAgentsInCircle = function( position, distance )
    return DEC_Connaissances_UnitesEnnemiesVivantesDansCercle( position, distance )
end

--- Returns the flying height for the provided agent knowledge
-- @param agent Simulation agent knowledge
-- @return Float
integration.getHeightForKnowledgeAgent = function( agent )
    return DEC_ConnaissanceAgent_Altitude( agent )
end

--- Returns the standard flying height of an agent (in meters)
-- Returns 0 if the agent cannot fly
-- @param agent The agent whose standard flying height will be returned
-- @return An integer, the standard flying height as defined in the physical database
integration.getStandardFlyingHeight = function( agent )
    return DEC_Agent_GetStandardFlyingHeight( agent.source )
end

--- Returns the tactical flying height of an agent (in meters)
-- Returns 0 if the agent cannot fly
-- @param agent The agent whose tactical flying height will be returned
-- @return An integer, the tactical flying height as defined in the physical database
integration.getTacticalFlyingHeight = function( agent )
    return DEC_Agent_GetTacticalFlyingHeight( agent.source )
end

--- Allows the unit to deactivate default behaviours, such as self-protection for instance.
-- The table myself.deactivations contains all the current deactivated behaviours.
-- @param behaviour String describing a behaviour (e.g. "selfprotect")
integration.deactivate = function( behaviour )
    local myself = myself
    myself.deactivations = myself.deactivations or {}
    myself.deactivations[ behaviour ] = ( myself.deactivations[ behaviour ] or 0 ) + 1
end

--- Allows the unit to reactivate previously deactivated behaviours.
-- @param behaviour String describing a behaviour (e.g. "selfprotect")
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

--- Returns true if the provided behaviour is currently inactive, false otherwise
-- @param behaviour String describing a behaviour (e.g. "selfprotect")
integration.isInactive = function( behaviour )
    return myself.deactivations and myself.deactivations[ behaviour ]
end

--- Returns the threat level of the provided agent knowledge
-- @param agent Directia agent knowledge
-- @return Float (between 0 and 100)
integration.threatLevel = function( agent )
    return agent:computeAggressiveness()
end

--- Returns this entity's priority to flee from the provided agent knowledge
-- @param agent Directia agent knowledge
-- @return Float, the fleeing priority (between 0 and 1)
integration.fleePriority = function( agent )
    return math.max( agent:proximityLevel(), 1 ) / 100
end

--- Deprecated
integration.isThreateningOrHostilePredicate = {
     dependencies = "none",
        method = function( self )
            return self:isHostile() or self:isThreatening()
        end
    }

--- Returns the name of the provided agent knowledge
-- @param agent Directia agent knowledge
-- @return String
integration.getName = function( agent )
    return DEC_GetSzName( agent.source )
end

--- Returns the name of the provided agent
-- @param agent Simulation agent
-- @return String, the name of the agent
integration.getMilName = function( agent )
    return DEC_Pion_GetMilPionName( agent )
end

--- Returns the number of energy dotation for the provided agent 
-- @param agent Simulation agent
-- @return Double, the number of energy resources
integration.getEnergyDotationNumber = function( agent )
    return DEC_Agent_GetFuelDotationNumber( agent )
end

--- Returns the maximum capacity of the energy resources
-- @param agent Simulation agent
-- @return Double, the number of the maximum capacity of energy dotation
integration.getEnergyDotationCapacity = function( agent )
    return DEC_Agent_GetFuelDotationCapacity( agent)
end

--- Supply fuel resources for the provided agent
-- @param agent Simulation agent
-- @param ratio between 0 and 1 (if value is 1 the maximum capacity for fuel resources is reached, if value is 0 the fuel resources is 0) 
integration.resupplyFuel = function( agent, ratio )
    DEC_Agent_ResupplyFuel( agent, ratio )
end

--- Returns a list of all detected agent knowledges in
-- this entity's current area of responsibility.
-- This method can only be called by an agent.
-- @return List of agent knowledges
integration.unitesDetecteesDansFuseau = function()
    return DEC_Connaissances_UnitesDetecteesDansFuseau()
end

--- Returns all the known wounded or dead units inside the circle defined by
--- the provided center and distance.
-- @param position Simulation position, the center of the circle
-- @param distance Float, the radius of the circle
-- @return List of all known wounded or dead simulation agent knowledges in the circle
integration.getWoundedOrDeadUnitsInCircle = function( position, radius )
    return DEC_Connaissances_UnitesBlesseesOuTueesDansCercle( position, radius )
end

--- Returns all the known civilians in the provided area
-- @param area Area knowledge
-- @return List of all known civilian simulation units in the area
integration.getCiviliansInArea = function( area )
    return DEC_Connaissances_UnitesCivilesDansZone( area.source )
end

--- Returns all the known wounded units in the provided area
-- @param area Area knowledge
-- @return List of all the known wounded simulation units in the area
integration.getWoundedInArea = function( area )
    return DEC_Connaissances_UnitesBlesseesDansZone( area.source )
end

--- Returns a list of all terrorist agent knowledges at the given distance of this entity.
-- This method can only be called by an agent.
-- @param distance Float, the distance (in meters, 600 by default).
-- @return List of agent knowledges
integration.getNearbyTerrorists = function( distance )
    local terrorists = DEC_Connaissances_TerroristsAProximite( distance or 600 )    
    local newResult = {}
    for i = 1, #terrorists do
        newResult[ i ] = CreateKnowledge( integration.ontology.types.agentKnowledge, terrorists[ i ] )
    end
    return newResult
end

--- Returns the firing distance necessary for the provided platoon to be
--- able to shoot this entity with the given probability to hit
-- @param platoon Directia agent knowledge
-- @param ph Float, the probability to hit
-- @return Float, the distance. 0 if it is impossible to fire. -1 if the agent knowledge is invalid
integration.getFiringDistanceToEngageMe = function( platoon, ph )
    return DEC_Tir_PorteeMaxPourEtreTireParUnite( platoon.source, ph )
end

--- Returns true if the provided agent is flying, false otherwise
-- @param teammate Directia agent knowledge
-- @return Boolean, whether or not the agent knowledge is flying
integration.agentIsFlying = function( teammate )
    return teammate.source:DEC_Agent_EstEnVol()
end

--- Returns true if the provided agent can fly, false otherwise
-- @param teammate Directia agent knowledge
-- @return Boolean, whether or not the agent knowledge can fly
integration.agentCanFly = function( teammate )
    return DEC_Agent_PionCanFly( teammate.source )
end

--- Returns all the crowds knowledges colliding with this entity
-- @return List of crowd knowledges
integration.getCollidingCrowds = function()
    local simCrowds = DEC_Connaissances_CollisionsPopulations()
    local crowds = {}
    for i = 1, #simCrowds do
        crowds[ i ] = CreateKnowledge( integration.ontology.types.population, simCrowds[ i ] )
    end
    return crowds
end

--- Returns true if the given agent is neutralized, false otherwise.
-- @param unit Directia agent knowledge
-- @return Boolean
integration.UnitIsNeutralized = function( unit )
    return unit.source:DEC_Agent_EtatOpsMajeur() == 0
end

--- Returns true if the given agent is dead, false otherwise.
-- @param unit Directia agent knowledge
-- @return Boolean
integration.UnitIsDead = function( unit )
    return unit.source:DEC_Agent_EstMort()
end

--- Returns the value of the boolean variable with the given name
-- from the provided agent's brain.
-- @param agent Simulation agent
-- @param varName String, the variable's name
-- @return Boolean
integration.getBoolVariable = function( agent, varName )
    return agent:GetBoolVariable( varName )
end
------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

-- Deprecated
integration.unit_commandEfficiency = function( commander, individualTask, leadTask, maxNormalization, taskParams, threat )
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
    threat = threat or {}
    if ponderateTaskEffects.threatResistanceEffectLevel ~= nil then
        local resistance = 0
        for _, threateningElement in ipairs( threat ) do
            resistance = resistance + ( 100 - meKnowledge:computeEstimateAttritionOnMe( threateningElement ) )
        end
        ponderateTaskEffects.threatResistanceEffectLevel = ponderateTaskEffects.threatResistanceEffectLevel * resistance / 100
    else
        error( "Impossible to compute resistance under threats capability for the task "..leadTask.name, 2 )
    end
    

    local res, default = 0, 0
    for _, level in ipairs( effects ) do
        local l1 = ( leadTaskEffects[ level ] or default ) / 100
        local l2 = ( ponderateTaskEffects[ level ] or default ) / 100
        res = res + l1 * ( l1 < l2 and l2 - l1 or l1 - l2 )
    end
    return maxNormalization - res / #effects
end

-- Deprecated
integration.unit_communicate = function( receiver, task, objectives )
    integration.SendMessage( "Order", receiver, { order = task.name, params = objectives } )
end

integration.isKnowledgesAgentsInArea = integration.getAgentKnowledgesInArea
