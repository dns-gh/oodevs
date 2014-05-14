--- Returns true if this entity can illuminate the given agent knowledge.
-- @param unit DirectIA agent knowledge
-- @return Boolean
integration.canIlluminateIt = function( unit )
  return DEC_ConnaissanceAgent_PeutEtreIllumine( unit.source )
end

--- Returns true if this entity has illumination capacity, false otherwise.
-- This method can only be called by an agent
-- @return Boolean
integration.canIlluminate = function()
    return DEC_ConnaissanceAgent_PeutIlluminer()
end

--- Returns true if the given agent knowledge is definitively illuminated, false otherwise.
-- This method can only be called by an agent.
-- @param target DirectIA agent knowledge
-- @return Boolean
integration.isDefinitelyIlluminated = function( target )
    return DEC_ConnaissanceAgent_EstDefinitivementIllumine( target.source )
end

--- Returns the illumination level of the given agent knowledge :
-- <ul> <li> 100 if the given agent knowledge is currently illuminated </li>
-- <li> 0 otherwise </li> </ul>
-- @param agent DirectIA Agent knowledge
-- @return Integer
integration.illuminationLevel = function( agent )
    return DEC_ConnaissanceAgent_EstIllumine( agent.source ) and 100 or 0
end

--- Starts illuminating the given agent knowledge, in order for it to become known
-- to the first agent of the allies list parameter.
-- This method displays a report.
-- This method can only be called by an agent.
-- @see integration.startedIlluminateIt
-- @see integration.stopIlluminateIt
-- @param target DirectIA agent knowledge, the target to illuminate
-- @param allies List of DirectIA agent knowledges (only the first element of the list
-- will be taken into account)
-- @return Boolean, false
integration.startIlluminateIt = function( target, allies )
    target[myself] = target[myself] or {}
    target[myself].actionIlluminate = DEC_StartIlluminer( target.source, allies[1].source  )
    reportFunction(eRC_IllumineCible )
    actionCallbacks[ target[myself].actionIlluminate ] = function( arg ) target[myself].actionState = arg end
    return false
end

--- Continues illuminating the given agent knowledge.
-- This method can only be called by an agent.
-- @see integration.startIlluminateIt
-- @see integration.stopIlluminateIt
-- @param target DirectIA agent knowledge
-- @return Boolean, whether or not the illumination action is finished.
integration.startedIlluminateIt = function( target ) 
    if target[myself].actionState == eActionIllumination_Finished then -- IndirectFire hit target
        return true
    end
    return false
end

--- Stops illuminating the given agent knowledge.
-- This method can only be called by an agent.
-- @see integration.startIlluminateIt
-- @see integration.startedIlluminateIt
-- @param target DirectIA agent knowledge
-- @return Boolean, false
integration.stopIlluminateIt = function( target )
    target[myself] = target[myself] or {}
    target[myself].actionIlluminate = DEC__StopAction( target[myself].actionIlluminate )
    return false
end

--- Returns true if the given agent has illumination resources, false otherwise.
-- This method can only be called by an agent
-- @param agent DirectIA agent
-- @return Boolean
integration.canAgentIlluminate = function( agent )
    return DEC_Agent_PeutIllumine( agent )
end

