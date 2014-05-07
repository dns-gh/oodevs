-------------------------------------------------------------------
---- IMPROVABLE INTERFACE IMPLEMENTATION
-------------------------------------------------------------------

---Informs whether or not the agent has the physical ability to perform the impovement action on the given 'object'.
-- (see authoring tool, 'Equipement' tab, 'Objects' widget). 
-- This method can only be called by an agent.
-- @param object a DirectIA object knowledg.
-- @return Boolean returns 'true' if the agent can improve the object knowledge, 'false' otherwise.
-- This implementation refers to the 'integration.startImproveIt action.
integration.canImproveIt = function( object )
    object[ myself ] = object[ myself ] or {} 
    if object[ myself ] then
        return object[ myself ].actionImprovementState ~= eActionObjetPasDeCapacite
    else
        return DEC_Agent_PeutValoriserObjet( object.source )
    end
end

---Informs whether or not the agent has the necessary dotation to perform the improvement of the given 'object'.
-- This method can only be called by an agent.
-- @param object a DirectIA object knowledge.
-- @return Boolean returns 'true' if the agent has enough dotation, 'false' otherwise.
integration.hasEnoughtDotationForImprovement = function( object )
    object[ myself ] = object[ myself ] or {} 
    if object[ myself ] then
        return object[ myself ].actionImprovementState ~= eActionObjetManqueDotation
    else
        return false
    end
end

---Informs whether or not the agent has the physical ability to perform the impovement action on the given 'object'.
-- This method can only be called by an agent.
-- @param object a DirectIA object knowledge
-- @return Boolean returns 'true' if the agent can improve the object, 'false' otherwise.
integration.agentCanImproveObject = function( object )
    return DEC_Agent_PeutValoriserObjet( object.source )
end

--- Informs if the agent has dotation to improve the provided object. The implementation of this method also
-- check if reinforcing units are present and if they have the needed dotation.
-- This method can only be called by an agent.
-- @return Boolean returns 'true' if the agent has enough dotation to improve the provided object.
-- It returns 'false' otherwise.
integration.hasEnoughDotationForImprovementWithReinforcement = function( object )
    local dotations = DEC_GetAgentDotationManquantePourValoriserObjet( meKnowledge.source, object.source ) -- returns the needed quantity of dotation to get the element improved.
    if DEC_Agent_GetAgentDotation( myself, dotations.first ) >= 1 then -- until the agent has at least one dotation it can improve
        return true
    else -- No dotation left. Verify that the agent is reinforced and if reinforcing agents has the dotation.
        local reinforcingAgents = DEC_Agent_Renforts( meKnowledge.source )
        for i = 1, #reinforcingAgents do
            local nbreDotation = DEC_Agent_GetAgentDotation( reinforcingAgents[ i ], dotations.first )
            if nbreDotation >= 1 then
                return true -- at least one agent has the dotation
            end
        end
    end
    return false
end

--- Informs it the given 'object' can be improved.
-- This method can only be called by an agent.
-- @param object a DirectIA knowledge object
-- @return Boolean returns 'true' if the object is an 'Improvable' type of object, 'false' otherwise. 
-- See the 'Object' tab, 'Capacities' section in authoring tool.
integration.canBeImproved = function( object )
    return DEC_ConnaissanceObjet_PeutEtreValorise( object.source )
end

---Returns the improvement level of the given DirectIA object knowledge.
-- This method can only be called by an agent.
-- @param object a DirectIA object knowledge
-- @return Numeric the level of improvement (a percentage). A value of '0' means 'not improved at all', 
-- a value of '100' means 'totally improved'.
integration.improvementLevel = function( object )
    return DEC_ConnaissanceObjet_NiveauValorisation( object.source ) * 100
end

---To start improving the given object.
-- This method can only be called by an agent.
-- @param object a DirectIA object knowledge
integration.startImproveIt = function( object )
    object[ myself ] = object [myself ] or {} 
    object[ myself ].actionImprovement = DEC_StartValoriserObjet( object.source )
    actionCallbacks[ object[ myself ].actionImprovement ] = function( arg ) 
        object[ myself ].actionImprovementState = arg
    end
    reportFunction( eRC_DebutValorisation, object.source )
end

--- Update the improvement of the given object
-- This method can only be called by an agent.
-- @param object a DirectIA object knowledge
-- @see integration.startImproveIt method
integration.updateImproveIt = function( object ) -- used by scipio
    if object[ myself ].actionImprovementState == eActionObjetImpossible then
        DEC_Trace( "impossible works" ) 
    elseif object [ myself ].actionImprovementState == eActionObjetManqueDotation then
        DEC_Trace( "not enough dotation" ) 
    elseif object[ myself ].actionImprovementState == eActionObjetPasDeCapacite then
        DEC_Trace( "no capacity" ) 
    end
end

--- Update the improvement of the given object
-- This method can only be called by an agent.
-- @param object a DirectIA object knowledge
-- @returns Boolean returns 'true' once the improvement is terminated, 'false' if it fails.
-- @see integration.startImproveIt method
integration.updateImproveItWithFeedbacks = function( object )
    if object[ myself ].actionImprovementState == eActionObjetImpossible then
        DEC_Trace( "this object cannot be improved" )
        return false
    elseif object[ myself ].actionImprovementState == eActionObjetManqueDotation then
        DEC_Trace( "not enough dotation" ) 
        return false
    elseif object[ myself ].actionImprovementState == eActionObjetPasDeCapacite then
        DEC_Trace( "the agent has not capacity to improve the object" )
        return false
    elseif object[ myself ].actionImprovementState == eActionObjetTerminee then
        return true
    end
end

--- Stop the action of improving the given object.
-- @see integration.startBuildObjectOnLocalization and integration.updateImproveItWithFeedbacks methods
-- @param object a DirectIA object knowledge
-- @see integration.startImproveIt method
integration.stopImproveIt = function( object )
    object[ myself ] = object[ myself ] or {} 
    if object[ myself ].actionImprovementState == eActionObjetTerminee then
        reportFunction( eRC_FinValorisation, object.source )
    else
        DEC_Trace( "pause work improve" )
    end
    object[ myself ].actionImprovement = DEC__StopAction( object[ myself ].actionImprovement )
end