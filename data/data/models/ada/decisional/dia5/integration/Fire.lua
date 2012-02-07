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
        meKnowledge:RC( eRC_ImpossibleToExtinguishFire )
        return true -- $$$ MIA: TEMP should return false
    elseif fire.actionExtinguishState == eActionObjetManqueDotation then
        DEC_Trace( "not enough dotation" )
        meKnowledge:RC( eRC_ImpossibleToExtinguishFire )
        return true  -- $$$ MIA: TEMP should return false
    elseif fire.actionExtinguishState == eActionObjetPasDeCapacite then
        DEC_Trace( "no capacity" ) 
        meKnowledge:RC( eRC_ImpossibleToExtinguishFire )
        return true  -- $$$ MIA: TEMP should return false
    elseif( fire.actionExtinguishState == eActionObjetTerminee ) then
        meKnowledge:RC( eRC_ObjectExtinguished )
        return true
    end
    return false
end

integration.stopExtinguish = function( fire )

    fire.actionExtinguish = DEC__StopAction( fire.actionExtinguish )
    fire.actionExtinguishState = nil
end

integration.canBeExtinguished = function( self )
    --- $$$ MIA TODO
    return true 
end