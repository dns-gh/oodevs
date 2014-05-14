--- Returns true if the given target is within indirect fire range,
-- in order to be fired at by this entity with the given munition; false otherwise.
-- This method can only be called by an agent
-- @param target Any DirectIA knowledge defining a "getPosition" method returning a simulation point
-- @param munition Resource type
-- @return Boolean
integration.isOnRangeFor = function( target, munition )
    local dist = integration.distance( meKnowledge, target )
    return dist > DEC_Tir_PorteeMinTirIndirect_SansTesterEtatMunitions( munition ) 
         and dist < DEC_Tir_PorteeMaxTirIndirect_SansTesterEtatMunitions( munition )
end

--- Returns true if this entity has a launcher for the given munition, false otherwise.
-- This method can only be called by an agent
-- @param munition Resource type
-- @return Boolean
integration.hasLauncherFor = function( munition )
    return DEC_Tir_PorteeMaxTirIndirect_SansTesterEtatMunitions( munition ) ~= -1
end

--- Starts indirectly firing on the given agent knowledge with the given munition
-- This method can only be called by an agent.
-- @see integration.updateApplyFireOnPlatoon
-- @see integration.stopApplyFireOnPlatoon
-- @param target DirectIA agent knowledge
-- @param munition Resource type
-- @param quantity The quantity of projectiles to use for each shot
-- @return Boolean, false
integration.startApplyFireOnPlatoon = function( target, munition, quantity )
    target[myself] = target[myself] or {}
    target[myself].firstTime = true
    target[myself].actionIndirectFire = DEC_StartTirIndirectSurConnaissancePtr( munition, quantity, target.source )
    actionCallbacks[ target[myself].actionIndirectFire ] = function( arg ) target[myself].eIndirectFireState = arg end
    return false
end

--- Continues indirectly firing on the given agent knowledge
-- May display a report.
-- This method can only be called by an agent.
-- @see integration.startApplyFireOnPlatoon
-- @see integration.stopApplyFireOnPlatoon
-- @param target DirectIA agent knowledge
-- @return Boolean, whether or not the action is over.
integration.updateApplyFireOnPlatoon = function( target )
    if target[myself].eIndirectFireState == eIndirectFireState_Running then
        if target[myself].firstTime then
            reportFunction(eRC_TirIndirectSurCible, target.source )
            target[myself].firstTime = false
        end
        return false
    else
        if target[myself].eIndirectFireState == eIndirectFireState_Finished then
            return true
        elseif target[myself].eIndirectFireState then -- eIndirectFireState_NoCapacity, eIndirectFireState_Impossible, eIndirectFireState_NoAmmo
            integration.updateApplyFireImpossible( target )
            return true
        else
            return false
        end
    end
end

--- Stops indirectly firing on the given agent knowledge
-- May display a report
-- This method can only be called by an agent.
-- @see integration.startApplyFireOnPlatoon
-- @see integration.updateApplyFireOnPlatoon
-- @param target DirectIA agent knowledge
integration.stopApplyFireOnPlatoon = function( target )
    if target[myself].eIndirectFireState == eIndirectFireState_Finished then
        reportFunction(eRC_FinTirIndirectSurCible, target.source )
    end
    target[myself].actionIndirectFire = DEC__StopAction( target[myself].actionIndirectFire )
    target[myself].eIndirectFireState = nil
end

--- Starts indirectly firing on the given point with the given munition
-- This method can only be called by an agent.
-- @see integration.updateApplyFireOnPoint
-- @see integration.stopApplyFireOnPoint
-- @param point Point knowledge
-- @param munition Resource type
-- @param quantity The quantity of projectiles to use for each shot
-- @return Boolean, false
integration.startApplyFireOnPoint = function( point, munition, interventionType )
    point[myself] = point[myself] or {}
    point[myself].firstTime = true
    point[myself].actionIndirectFire = DEC_StartTirIndirectSurPosition( munition, interventionType, point.source )
    actionCallbacks[ point[myself].actionIndirectFire ] = function( arg ) point[myself].eIndirectFireState = arg end
    return false
end

--- Continues indirectly firing on the given point
-- May display a report.
-- This method can only be called by an agent.
-- @see integration.startApplyFireOnPoint
-- @see integration.stopApplyFireOnPoint
-- @param point Point knowledge
-- @return Boolean, whether or not the action is over.
integration.updateApplyFireOnPoint = function( point )
    if point[myself].eIndirectFireState == eIndirectFireState_Running then
        if point[myself].firstTime then
            reportFunction(eRC_PremierCoupParti )
            point[myself].firstTime = false
        end
        return false
    else
        if point[myself].eIndirectFireState == eIndirectFireState_Finished then
            if point[myself].firstTime then -- in this case the firing is finished at the first tick: send report
                reportFunction( eRC_PremierCoupParti )
                point[myself].firstTime = false
            end
            return true
        elseif point[myself].eIndirectFireState then -- eIndirectFireState_NoCapacity, eIndirectFireState_Impossible, eIndirectFireState_NoAmmo
            integration.updateApplyFireImpossible( point )
            return true
        else
            return false
        end
    end
end

--- Stops indirectly firing on the given point
-- May display a report
-- This method can only be called by an agent.
-- @see integration.startApplyFireOnPoint
-- @see integration.updateApplyFireOnPoint
-- @param point Point knowledge
integration.stopApplyFireOnPoint = function( point )
    if point[myself].eIndirectFireState == eIndirectFireState_Finished then
        reportFunction(eRC_TirExecute )
    end
    point[myself].actionIndirectFire = DEC__StopAction( point[myself].actionIndirectFire )
    point[myself].eIndirectFireState = nil
end

--- Displays an appropriate report depending on the state of a current indirect fire action.
-- This method should not be used if this entity has no ongoing indirect fire action.
-- @see integration.updateApplyFireOnPlatoon
-- @see integration.updateApplyFireOnPoint
-- @param target DirectIA agent knowledge or Point knowledge
integration.updateApplyFireImpossible = function( target )
    if not target[myself] then
        return
    end
    if target[myself].eIndirectFireState == eIndirectFireState_Impossible then
        reportFunction(eRC_TirIndirectTargetNotIlluminated )
    end
    if target[myself].eIndirectFireState == eIndirectFireState_NoCapacity then
        reportFunction(eRC_TirIndirectNoCapacity )
    end
    if target[myself].eIndirectFireState == eIndirectFireState_NoAmmo then
        reportFunction(eRC_TirImpossiblePlusDeMunitions )
    end
end

--- Sets this entity's current availability to provide indirect fire
-- This method can only be called by an agent.
-- @param bAvailable Boolean, the entity's new availability
integration.sendfireAvailable = function( bAvailable )
    if bAvailable then
        DEC_Agent_ChangeDisponibiliteAuTirIndirect( eFireAvailability_PretAuTir )
    else
        DEC_Agent_ChangeDisponibiliteAuTirIndirect( eFireAvailability_Indisponible )
    end
end

--- Returns true if this agent is under indirect fire, false otherwise
-- This method can only be called by an agent.
-- @return Boolean
integration.isUnderIndirectFire = function()
    return DEC_Agent_EstCibleTirIndirect()
end

--- Returns a munition type appropriate to apply direct fire on the given position (i.e.
-- a munition type with the given position within range).
-- If no position is provided, the range constraint will not be taken into account.
-- This method can only be called by an agent
-- @param position Point knowledge (optional)
-- @return Resource type
integration.munitionPourTirIndirect = function( position ) -- Be careful : if unit is load return is always nil
    return DEC_Tir_MunitionPourTirIndirect( eObus_Explosif, position ) 
end

--- Returns a list of potential targets for indirect fire :
-- <ul> <li> Enemies engaging the given ally </li>
-- <li> Enemies deemed dangerous </li> <ul>
-- This method can only be called by an agent
-- @see integration.getKnowledgesUnitsEngagingFriend
-- @see integration.getKnowledgesDangerousUnits
-- @param friend DirectIA agent
-- @return List of simulation agents
integration.getMortarUnitsToNeutralize = function( friend )
    local res = {}
    local unitsAttackingFriend = integration.getKnowledgesUnitsEngagingFriend( friend.source )
    local unitsDangerous = integration.getKnowledgesDangerousUnits()
    return fusionList( unitsAttackingFriend, unitsDangerous )
end

--- Filters the given list of agent knowledges to keep only those who could
-- be appropriate targets for indirect fire.
-- In order for them to be chosen, the targets must be :
-- <ul> <li> Enemies </li>
-- <li> Operational </li>
-- <li> Non-flying agents </li>
-- <li> Within range </li> </ul>
-- This method also guarantees there will be no duplicates in the returned list.
-- This method can only be called by an agent.
-- @param targets List of agent knowledges
-- @return List of agent knowledges
integration.findEnemyToMortarIndirectFire = function( targets )
    if not targets or #targets < 1 then
        return {}
    else
        local DEC_ConnaissanceAgent_EstUnEnnemi = DEC_ConnaissanceAgent_EstUnEnnemi
        local DEC_ConnaissanceAgent_EstDetruitTactique = DEC_ConnaissanceAgent_EstDetruitTactique
        local DEC_ConnaissanceAgent_EstEnVol = DEC_ConnaissanceAgent_EstEnVol
        local result = {}
        for i = 1, #targets do
            local dotation = integration.munitionPourTirIndirect( targets[i]:getPosition() )
            if DEC_ConnaissanceAgent_EstUnEnnemi( targets[i].source ) and not DEC_ConnaissanceAgent_EstDetruitTactique( targets[i].source ) and not DEC_ConnaissanceAgent_EstEnVol( targets[i].source ) and 
              integration.isOnRangeFor( targets[i], dotation ) then
                if not exists( result, targets[i] ) then
                    result[ #result + 1 ] = targets[i]
                end
            end
        end
        return result
    end
end

--- Launches smoke rounds on this entity's current position.
-- This method can only be called by an agent.
-- @return Float, the duration of the smoke screen (in ticks)
integration.launchDREB = function()
    return DEC_Tir_LancerFumigeneSurPosition(myself, meKnowledge:getPosition())
end

--- Launches smoke rounds on the given agent's current position.
-- This method can only be called by an agent.
-- @param eni Simulation agent knowledge
-- @return Float, the duration of the smoke screen (in ticks)
integration.launchSmokesOnEni = function( eni )
    return DEC_Tir_LancerFumigeneSurConnaissance( eni )
end

--- Returns the greatest maximal indirect fire range of all
-- of this entity's components.
-- This method can only be called by an agent.
-- @return Float, the maximum range (in meters)
integration.getMaxRangeIndirectFireWithoutSelectAmmo = function()
    return DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition()
end
