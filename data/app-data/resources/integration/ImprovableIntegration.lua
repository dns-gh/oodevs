-------------------------------------------------------------------
---- IMPROVABLE INTERFACE IMPLEMENTATION
-------------------------------------------------------------------

---Informs whether or not the agent has the physical ability to perform the impovement action on the given 'object'.
-- (see authoring tool, 'Equipement' tab, 'Objects' widget). 
-- This method can only be called by an agent.
-- This implementation refers to the 'integration.startImproveIt'.
-- @param object a DirectIA object knowledge.
-- @return Boolean 'true' if the agent can improve the object knowledge.
integration.canImproveIt = function( object )
    object[ myself ] = object[ myself ] or {} 
    return object[ myself ].actionImprovementState ~= eActionObjetPasDeCapacite
end

---Informs whether or not the agent has the necessary dotation to perform the improvement of the given 'object'.
-- This method can only be called by an agent.
-- This implementation refers to the 'integration.startImproveIt'.
-- @param object a DirectIA object knowledge.
-- @return Boolean returns 'true' until the agent has enough dotation to perform the improvement action.
integration.hasEnoughtDotationForImprovement = function( object )
    object[ myself ] = object[ myself ] or {} 
    return object[ myself ].actionImprovementState ~= eActionObjetManqueDotation
end

---Informs whether or not the agent has the physical ability to perform the impovement action on the given 'object'.
-- This method can only be called by an agent.
-- @param object a DirectIA object knowledge
-- @return Boolean returns 'true' if the agent can improve the object, 'false' otherwise.
integration.agentCanImproveObject = function( object )
    return _DEC_Agent_PeutValoriserObjet( myself, object.source )
end

--- Informs if the agent has dotation to improve the provided object. The implementation of this method also
-- checks if reinforcing units are present and if they have the needed dotation.
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

--- Returns the improvement level of the given DirectIA object knowledge.
-- This method can only be called by an agent.
-- @param object a DirectIA object knowledge
-- @return Numeric the level of improvement (a percentage). A value of '0' means 'not improved at all', 
-- a value of '100' means 'totally improved'.
integration.improvementLevel = function( object )
    return DEC_ConnaissanceObjet_NiveauValorisation( object.source ) * 100
end

--- Start improving the given object.
-- This method can only be called by an agent.
-- @param object a DirectIA object knowledge
integration.startImproveIt = function( object )
    object[ myself ] = object [myself ] or {} 
    object[ myself ].actionImprovement = _DEC_StartValoriserObjet( myself, object.source )
    actionCallbacks[ object[ myself ].actionImprovement ] = function( arg ) 
        object[ myself ].actionImprovementState = arg
    end
    reportFunction( eRC_DebutValorisation, object.source )
end

--- Informs about the progress of the improvement action. The implementation displays
-- debug traces if the action cannot be performed normally.
-- This method can only be called by an agent.
-- @see integration.startImproveIt method
-- @param object a DirectIA object knowledge
integration.updateImproveIt = function( object )
    if object[ myself ].actionImprovementState == eActionObjetImpossible then
        _DEC_Trace( myself, "impossible works" ) -- the objet cannot be improved
    elseif object [ myself ].actionImprovementState == eActionObjetManqueDotation then
        _DEC_Trace( myself, "not enough dotation" ) -- the agent has no dotation left
    elseif object[ myself ].actionImprovementState == eActionObjetPasDeCapacite then
        _DEC_Trace( myself, "no capacity" ) -- the agent has no physical capability
    end
end

--- Informs about the progress of the improvement action. The implementation displays
-- debug traces if the action cannot be performed normally. It returns a boolean 
-- value upon action termination.
-- This method can only be called by an agent.
-- @see integration.startImproveIt method
-- @param object a DirectIA object knowledge
-- @returns Boolean returns 'true' once the improvement is terminated, 'false' if it fails.
integration.updateImproveItWithFeedbacks = function( object )
    if object[ myself ].actionImprovementState == eActionObjetImpossible then
        _DEC_Trace( myself, "this object cannot be improved" )
        return false
    elseif object[ myself ].actionImprovementState == eActionObjetManqueDotation then
        _DEC_Trace( myself, "not enough dotation" ) 
        return false
    elseif object[ myself ].actionImprovementState == eActionObjetPasDeCapacite then
        _DEC_Trace( myself, "the agent does not have the capacity to improve the object" )
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
        _DEC_Trace( myself, "pause work improve" )
    end
    object[ myself ].actionImprovement = _DEC__StopAction( myself, object[ myself ].actionImprovement )
end