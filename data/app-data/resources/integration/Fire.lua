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
    fire.actionExtinguish = DEC_StartExtinguishObject( fire.source )
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
-- @param noActionReport Boolean, false if the "Fire is being extinguished" report
-- should be displayed, true otherwise.
-- @return Boolean, true if the fire is already extinguished, or if there is not enough
-- resource to extinguish it, or if the agent has no capacity to extinguish it, or if
-- the extinguishing action is over; false otherwise.
integration.updateExtinguish = function( fire, noActionReport )
    if fire.actionExtinguishState == eActionObjetImpossible then
        DEC_Trace( "Fire is already extinguished" )
        reportFunction(eRC_ImpossibleToExtinguishFire )
        return true -- $$$ MIA: TEMP should return false
    elseif fire.actionExtinguishState == eActionObjetManqueDotation then
        DEC_Trace( "not enough dotation" )
        reportFunction(eRC_ImpossibleToExtinguishFire )
        return true  -- $$$ MIA: TEMP should return false
    elseif fire.actionExtinguishState == eActionObjetPasDeCapacite then
        DEC_Trace( "no capacity" ) 
        reportFunction(eRC_ImpossibleToExtinguishFire )
        return true  -- $$$ MIA: TEMP should return false
    elseif( fire.actionExtinguishState == eActionObjetTerminee ) then
        reportFunction(eRC_ObjectExtinguished )
        return true
    end
    if not noActionReport then
        reportFunction( eRC_FireIsBeingExtinguished )
    end
    return false
end

--- Stops extinguishing the given fire.
-- This method can only be called by an agent.
-- @see integration.startExtinguish
-- @see integration.updateExtinguish
-- @param fire Object knowledge
integration.stopExtinguish = function( fire )
    fire.actionExtinguish = DEC__StopAction( fire.actionExtinguish )
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
        reportFunction( eRC_ImpossibleToExtinguishFire )
        return false
    end
end
