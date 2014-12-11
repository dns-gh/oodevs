--- Returns the current burning state of the given fire, computed as the
-- fire's current heat over its maximal heat (therefore, a float between 0 and 1).
-- Returns 0 if the given object has no burning capacity.
-- @param fire Object knowledge
-- @return Float between 0 and 1
integration.getBurningState = function( fire )
    return DEC_ConnaissanceObjet_BurningLevel( fire.source )
end

--- Starts extinguishing the given fire.
-- This method can only be called by an agent.
-- @see integration.updateExtinguish
-- @see integration.stopExtinguish
-- @param fire Object knowledge
-- @return Boolean, false
integration.startExtinguish = function( fire )
    fire.actionExtinguish = _DEC_StartExtinguishObject( myself, fire.source )
    actionCallbacks[ fire.actionExtinguish ] = function( arg ) 
        fire.actionExtinguishState = arg 
    end
    return false
end

--- Continues extinguishing the given fire.
-- This method can only be called by an agent.
-- This method may display reports.
-- @see integration.startExtinguish
-- @see integration.stopExtinguish
-- @param fire Object knowledge
-- @return Boolean, true if the fire is already extinguished, or if there is not enough
-- resource to extinguish it, or if the agent has no capacity to extinguish it, or if
-- the extinguishing action is over; false otherwise.
integration.updateExtinguish = function( fire )
    if fire.actionExtinguishState == eActionObjetImpossible then
        _DEC_Trace( myself, "Fire is already extinguished" )
        reportFunction(eRC_ImpossibleToExtinguishFire )
        return true -- $$$ MIA: TEMP should return false
    elseif fire.actionExtinguishState == eActionObjetManqueDotation then
        _DEC_Trace( myself, "not enough dotation" )
        reportFunction(eRC_ImpossibleToExtinguishFire )
        return true  -- $$$ MIA: TEMP should return false
    elseif fire.actionExtinguishState == eActionObjetPasDeCapacite then
        _DEC_Trace( myself, "no capacity" ) 
        reportFunction(eRC_ImpossibleToExtinguishFire )
        return true  -- $$$ MIA: TEMP should return false
    elseif( fire.actionExtinguishState == eActionObjetTerminee ) then
        reportFunction(eRC_ObjectExtinguished )
        return true
    end
    reportOnceFunction( eRC_FireIsBeingExtinguished )
    return false
end

--- Stops extinguishing the given fire.
-- This method can only be called by an agent.
-- @see integration.startExtinguish
-- @see integration.updateExtinguish
-- @param fire Object knowledge
integration.stopExtinguish = function( fire )
    fire.actionExtinguish = _DEC__StopAction( myself, fire.actionExtinguish )
    fire.actionExtinguishState = nil
end

--- Returns true if the given object has the burning capacity, false otherwise.
-- This method displays the "eRC_ImpossibleToExtinguishFire" report if 
-- the object has no burning capacity.
-- @param object Object knowledge
-- @return Boolean
integration.canBeExtinguished = function( object )
    if DEC_ObjectKnowledge_HasCapacity( object.source, "burn" ) then
        return true
    else
        reportOnceFunction( eRC_ImpossibleToExtinguishFire )
        return false
    end
end
