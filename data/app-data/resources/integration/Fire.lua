-- --------------------------------------------------------------------------------
--  State of a fire
-- --------------------------------------------------------------------------------
integration.getBurningState = function( fire )
    return DEC_ConnaissanceObjet_BurningLevel( fire.source )
end

-- --------------------------------------------------------------------------------
-- Action to extinguish a fire
-- --------------------------------------------------------------------------------
integration.startExtinguish = function( fire )

    fire.actionExtinguish = DEC_StartExtinguishObject( fire.source )
    actionCallbacks[ fire.actionExtinguish ] = function( arg ) 
        fire.actionExtinguishState = arg 
    end
    return false
end

integration.updateExtinguish = function( fire )
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
    meKnowledge:sendReport( eRC_FireIsBeingExtinguished )
    return false
end

integration.stopExtinguish = function( fire )

    fire.actionExtinguish = DEC__StopAction( fire.actionExtinguish )
    fire.actionExtinguishState = nil
end

integration.canBeExtinguished = function( object )
    if DEC_ObjectKnowledge_HasCapacity( object.source, "burn" ) then
        return true
    else
        meKnowledge:sendReport( eRC_ImpossibleToExtinguishFire )
        return false
    end
end
