--- Returns true if the given object is mined (i.e. its improvement
-- level is strictly greater than 0%), false otherwise.
-- @param object Object knowledge
-- @return Boolean
integration.isMined = function( object )
    return DEC_ConnaissanceObjet_EstValorise( object.source ) == eTristate_True
end

--- Returns true if the given object is fully mined (i.e. its
-- improvement level equals 100%), false otherwise
-- @param object Object knowledge
-- @return Boolean
integration.isFullyMined = function( object )
    return DEC_ObjectKnowledge_IsFullMined( object.source ) == eTristate_True
end

--- Returns true if this agent can demine the given object, false otherwise.
-- This method can only be called by an agent
-- @param object Object knowledge
-- @return Boolean
integration.canDemineIt = function( object )
    return DEC_Agent_PeutDevaloriserObjet( object.source )
end

--- Starts demining the given object.
-- Displays a report.
-- This method can only be called by an agent.
-- @see integration.updateDemineIt
-- @see integration.stopDemineIt
-- @param object Object knowledge
integration.startDemineIt = function( object )
    object[myself] = object[myself] or {} 
    object[myself].actionDemine = DEC_StartDevaloriserObjet( object.source )
    actionCallbacks[ object[myself].actionDemine ] = function( arg ) object[myself].actionDemineState = arg end    
    reportFunction( eRC_DebutDevalorisation, object.source )
end

--- Continues demining the given object.
-- May display traces.
-- @see integration.startDemineIt
-- @see integration.stopDemineIt
-- @param object Object knowledge
-- @return Boolean, whether or not the demining action is happening without any issue.
integration.updateDemineIt = function( object )
    if object[myself].actionDemineState == eActionObjetImpossible then
        DEC_Trace( "impossible works" )
        return false 
    elseif object[myself].actionDemineState == eActionObjetManqueDotation then
        DEC_Trace( "not enough dotation" ) 
        return false
    elseif object[myself].actionDemineState == eActionObjetPasDeCapacite then
        DEC_Trace( "no capacity" )
        return false 
    end
    return true
end

--- Stops demining the given object.
-- May display reports.
-- This method can only be called by an agent.
-- @see integration.startDemineIt
-- @see integration.updateDemineIt
-- @param object Object knowledge
-- @return Boolean, whether or not the action is over.
integration.stopDemineIt = function( object )
    object[myself] = object[myself] or {} 
    object[myself].actionDemine = DEC__StopAction( object[myself].actionDemine )
    if object[myself].actionDemineState == eActionObjetTerminee then
        reportFunction( eRC_FinDevalorisation, object.source )
        object[myself].actionDemineState = nil
        return true
    else
        object[myself].actionDemineState = nil
        DEC_Trace( "pause work demine" )
        return false
    end
end

--- Returns true if this entity can remove the given object, false otherwise.
-- This method can only be called by an agent.
-- @param object Object knowledge
-- @return Boolean, whether or not this agent can remove the given object.
-- This method returns false if the given knowledge is not an object.
integration.canRemoveIt = function( object )
    if masalife.brain.core.class.isOfType( object, integration.ontology.types.object ) then
        return DEC_Agent_PeutDetruireObjet( object.source )
    elseif masalife.brain.core.class.isOfType( object, integration.ontology.types.urbanBlock ) then
        return true -- decreasing the structural state of urban blocks don't require specific physical ability.
    end
    return false
end

--- Returns the removal level of the given object.
-- For instance, an object with a construction level of 60% has
-- a removal level of 40%.
-- @param object Object knowledge
-- @return Float between 0 and 100
integration.removalLevel = function( object )
    return ( 100 - DEC_ConnaissanceObjet_NiveauConstruction( object.source ) * 100 )
end

--- Starts removing the given object.
-- Displays a report.
-- This method can only be called by an agent.
-- @see integration.updateRemoveIt
-- @see integration.stopRemoveIt
-- @param object Object knowledge
integration.startRemoveIt = function( object )
    object[myself] = object[myself] or {} 
    object[myself].actionRemove = DEC_StartDetruireObjet( object.source )
    actionCallbacks[ object[myself].actionRemove ] = function( arg ) object[myself].actionRemoveState = arg end
    reportFunction(eRC_DebutDegagement, object.source )
end


--- Continues removing the given object.
-- May display traces.
-- @see integration.startRemoveIt
-- @see integration.stopRemoveIt
-- @param object Object knowledge
-- @return Boolean, whether or not the object removal is happening without any issue.
integration.updateRemoveIt = function( object )
    if object[myself].actionRemoveState == eActionObjetImpossible then
        DEC_Trace( "impossible works" )
        return false 
    elseif object[myself].actionRemoveState == eActionObjetManqueDotation then
        DEC_Trace( "not enough dotation" )
        return false 
    elseif object[myself].actionRemoveState == eActionObjetPasDeCapacite then
        DEC_Trace( "no capacity" )
        return false 
    end
    return true
end

--- Stops removing the given object.
-- May display reports.
-- This method can only be called by an agent.
-- @see integration.startRemoveIt
-- @see integration.updateRemoveIt
-- @param object Object knowledge
-- @return Boolean, whether or not the action is over.
integration.stopRemoveIt = function( object )
    object[myself] = object[myself] or {}
    object[myself].actionRemove = DEC__StopAction( object[myself].actionRemove )
    if object[myself].actionRemoveState == eActionObjetTerminee then
        reportFunction(eRC_FinDegagement )
        object[myself].actionRemoveState = nil
        return true
    else
        object[myself].actionRemoveState = nil
        DEC_Trace( "pause work remove" )
        return false
    end
end

--- Returns true if this entity is inside the object or less than distance from it, false otherwise.
-- This method can only be called by entities defining a "getPosition" method returning a simulation point.
-- @param object Object knowledge
-- @param distance Float, distance in meters (optional, 100 meters by default)
-- @return Boolean
integration.isInEffectArea = function( object, distance )
    return DEC_ConnaissanceObjet_PointEstProcheZoneEffet( meKnowledge:getPosition(), object.source, distance or 100 )
end

--- Returns true if the given object is avoidable, false otherwise.
-- The "avoidable" capacity is set in the authoring tool.
-- @param object Object knowledge
-- @return Boolean
integration.isAvoidable = function( object )
    return DEC_ObjectKnowledge_HasCapacity( object.source, "avoidable" )
end

--- Continues removing the given object.
-- May display reports.
-- @see integration.startRemoveIt
-- @see integration.stopRemoveItSecu
-- @param object Object knowledge
-- @return Boolean, whether or not the object removal has encountered an issue.
integration.updateRemoveItSecu = function( object )
    if object[myself].actionRemoveState == eActionObjetImpossible then
        reportOnceFunction( eRC_ConstructionObjetImpossible )
        DEC_Trace( "impossible works" )
        return true 
    elseif object[ myself ].actionRemoveState == eActionObjetManqueDotation then
        DEC_Trace( "not enough dotation" )
        reportOnceFunction( eRC_PasDotationConstructionObjet )
        return true 
    elseif object[ myself ].actionRemoveState == eActionObjetPasDeCapacite then
        DEC_Trace( "no capacity" )
        reportOnceFunction( eRC_PasDotationConstructionObjet )
        return true 
    elseif object[ myself ].actionRemoveState == eActionObjetTerminee then
        reportFunction(eRC_FinDegagement )
        return true
    end
    return false
end

--- Stops removing the given object.
-- May display a report.
-- This method can only be called by an agent.
-- @see integration.startRemoveIt
-- @see integration.updateRemoveItSecu
-- @param object Object knowledge
-- @return Boolean, true
integration.stopRemoveItSecu = function( object )
    if not object[ myself ].actionRemoveState == eActionObjetTerminee then
        reportFunction(eRC_FinDegagement )
    end
    object[ myself ] = object[ myself ] or {} 
    object[ myself ].actionRemove = DEC__StopAction( object[myself].actionRemove )
    return true
end

--- Starts deconstructing the given urban block.
-- Displays a report.
-- This method can only be called by an agent.
-- @see integration.updateRemoveItSecu
-- @see integration.stopRemoveItSecu
-- @param urbanBlock Urban block knowledge
integration.startDecontructItUrbanBlock = function( urbanBlock )
    urbanBlock[ myself ] = urbanBlock[ myself ] or {} 
    urbanBlock[ myself ].actionRemove = DEC_DeteriorateUrbanBlock( urbanBlock.source, 0 )
    actionCallbacks[ urbanBlock[ myself ].actionRemove ] = function( arg ) 
        urbanBlock[ myself ].actionRemoveState = arg 
    end
    reportFunction(eRC_DebutTravaux )
end

------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

--- Deprecated
integration.canRemoveItSecu = integration.canRemoveIt

--- Deprecated
integration.isInAvoidanceArea = function( object )
    return DEC_ConnaissanceObjet_PointEstDansZoneEvitement( meKnowledge:getPosition(), object.source )
end