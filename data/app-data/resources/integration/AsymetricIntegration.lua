-------------------------------------------------------------------
---- ASYMETRIC INTERFACE IMPLEMENTATION
-------------------------------------------------------------------

local defaultPhForFiringRange = 0.9 -- used in integration.isInFiringRangeForDotation

--- Allows the agent to become stealthy or not
-- When unit is stealthy, other agents cannot perceive it.
-- Becoming visible is effective 2 ticks later if no order to stay stealthy is given during this delay
-- @see OnNewTick in CRTaskListener.lua
-- @param beStealth Boolean, whether or not the unit will be stealthy
-- @return nothing
integration.setStealth = function( beStealth )
    local stealthFactor = beStealth and 0 or 1
    myself.lastStealth = myself.lastStealth or 1
    if stealthFactor == 1 and myself.lastStealth == 0 then 
        myself.wantedVisible = true -- Ask to become visible (see OnNewTick method in CRTaskListener).
        return
    end
    if stealthFactor == 0 then -- Become invisible
        if myself.lastStealth == 1 then
            DEC_Perception_Furtivite( stealthFactor )
            myself.lastStealth = stealthFactor
        end
        myself.wantedVisible = false
        myself.stealthTick = 0 -- reinit number of ticks for OnNewTick method in CRTaskListener.
        return
    end
end

--- Returns true if agent is stealthy, false otherwise
-- @return Boolean, whether or not this entity is stealthy
integration.isStealth = function( )
    return DEC_Perception_EstFurtif()
end

--- Returns the maximum indirect firing range for a particular ammunition
-- @param dotation simulation dotation category
-- @return Float, value is the indirect firing range in meters
integration.porteePourAttentat = function( dotation )
    myself[dotation] = myself[dotation] or {}
    myself[dotation].portee = myself[dotation].portee or DEC_Tir_PorteeMaxTirIndirect( dotation )
    return myself[dotation].portee
end

--- Start an attack on target knowledge with given ammunition
-- The entity can commit a suicide attack. In this case, he will die during the attack
-- @param target Directia target knowledge (target should be a localized element, e.g. area, crowd, agent, point, urban block...)
-- @param suicide Boolean
-- @param dotation simulation resource
-- @return true
integration.startAttackIt = function( target, suicide, dotation )
    target[myself] = target[myself] or {}
    local nbIntervention = 1
    if suicide then
        nbIntervention = DEC_Agent_GetAgentDotation( myself, dotation ) 
    end 
    reportFunction(eRC_ExecutionAttentat )
    target[myself].attackAction = DEC_StartTirIndirectSurPosition( dotation, nbIntervention, target:getPosition() )
    actionCallbacks[ target[myself].attackAction ] = function( arg ) target[myself].attackState = arg end
    return true
end

--- Continue attack on the provided target knowledge
-- If action is a suicide one and is finished, the agent commits suicide
-- If action is impossible (e.g. no ammunition), a report is sent    
-- @param target Directia target knowledge (target should be a localized element, e.g. area, crowd, agent, point, urban block...)
-- @param suicide Boolean
-- @param dotation simulation resource
-- @return Boolean, whether or not firing is over
integration.updateAttackIt = function( target, suicide, dotation )
    target[myself] = target[myself] or {}
    if target[myself].attackState then
        if target[myself].attackState == eIndirectFireState_Running then
            return false
        elseif target[myself].attackState == eIndirectFireState_Finished or target[myself].attackState == eIndirectFireState_NoAmmo then
            if suicide then DEC_Suicide() end
            return true
        else
            reportFunction(eRC_MissionImpossibleReason, "Attentat impossible" )
            return true
        end
    end
    return false
end

--- Stop attack on the target knowledge
-- The simulation action of terrorist attack is stopped
-- @param target Directia target knowledge (target should be a localized element, e.g. area, crowd, agent, point, urban block...)
-- @param suicide Boolean
-- @param dotation simulation resource
-- @return true
integration.stopAttackIt = function( target, suicide, dotation )
    target[myself] = target[myself] or {}
    target[myself].attackAction = DEC__StopAction( target[myself].attackAction )
    target[myself].attackState = nil
    return true
end

--- Allows the entity to commit suicide
-- Note that this method can only be called by an agent
integration.commitSuicide = function( )
    DEC_Suicide()
end

--- Allows terrorist to attack the provided object knowledge
-- The object will be degraded following the dotation used
-- A report is sent 
-- The terrorist can commit a suicide attack. In this case, he will die during the attack
-- @param target object knowledge
-- @param suicide Boolean
-- @param dotation simulation dotation category
-- @return true
integration.attackObject = function( target, suicide, dotation )
    reportFunction(eRC_ExecutionAttentat )
    DEC_ConnaissanceObjet_Degrader( target.source, 0.5, dotation )
    if suicide then DEC_Suicide() end
    return true
end

--- Allows terrorist to assassinate officers of the provided agent knowledge 
-- A report is sent by the provided unit knowledge and by the terrorist 
-- @param unit Directia agent knowledge
-- @return true
integration.killOfficers = function( unit )
    DEC_ConnaissanceAgent_TuerOfficiers( unit.source )
    integration.SendMessage( "killOfficers", integration.getAgentFromKnowledge( unit ), emptyTable, { type = "dynamic" } )
    reportFunction(eRC_ExecutionAttentat )
    return true
end

--- Returns true if target is in range for firing with the provided dotation and the given ph, false otherwise
-- @param target Directia agent knowledge
-- @param dotation simulation dotation category
-- @param ph Float (between 0 and 1), the wanted probability to hit (optional, default value is defaultPhForFiringRange = 0.9)
-- @return Boolean, whether or not the target is in firing range
integration.isInFiringRangeForDotation = function( target, dotation, ph )
    return integration.distance( meKnowledge, target ) < DEC_Tir_PorteeMaxPourTirerSurUniteAvecMunition( target.source,
                                                                                                         ph or defaultPhForFiringRange,
                                                                                                         dotation )
end

--- Deprecated
integration.selfDestruct = integration.commitSuicide

--- Informs that the agent knowledge take into account refugees
-- A report is sent
-- @return true
integration.takeAsRefugee = function( )
    -- $$$ MIA TODO
    reportFunction(eRC_OrientationPopulationVersCampRefugies )
    return true
end

--- Informs that the agent knowledge takes into account prisoners
-- A report is sent
-- @return true
integration.takeAsPrisoner = function( )
    -- $$$ MIA TODO
    reportFunction(eRC_OrientationEnnemiRenduVersCampPrisonniers )
    return true
end

--- Informs that the agent knowledge has executed a terrorist attack
-- A report is sent
integration.attackReport = function( )
    reportFunction(eRC_ExecutionAttentat )
end

--- Allows the agent to capture the provided agent knowledges
-- The goal is to capture forcibly terrorists or units. They become 'hostage'
-- The hostages lose their freedom of action. They don't share their knowledges information with their knowledge group
-- The hostages are transported by the agent
-- A report is sent by the hostages and by the agent
-- @param units, list of DirectIA agent knowledges
-- @param message, string defined in Reports.xml
-- @see Reports.xml in physical database
-- @return true
integration.capture = function( units, message )
    if not myself.CRCaptureSomeone then return true end
    for _, unit in pairs( units ) do
        if not myself.CRCaptureSomeone[unit] and not DEC_ConnaissanceAgent_EstPrisonnier( unit.source ) then
            local agent = DEC_ConnaissanceAgent_EnAgent(unit.source)
            DEC_Agent_AutomateForcerReddition( DEC_GetAutomate( agent ))
            DEC_UnitDecisionalState( unit.source, "hostage", "true" )
            DEC_Connaissance_Transporter( myself, unit.source )
            reportFunction(message, unit.source )
            myself.CRCaptureSomeone[ unit ] = true
            unit.capture = true
            integration.SendMessage( "capture", integration.getAgentFromKnowledge( unit ), { element = myself }, { type = "dynamic" } )
            DEC_ChangerSuperieurLogistiqueConnaissance( myself, unit.source )
            integration.addToLoadedUnits( unit )
            integration.addToCapturedUnits( unit )
        end
    end
    return true
end

--- Hostages are released
-- The provided agent knowledges 'hostages' are free and can share their knowledges with their knowledge group
-- A report is sent by the hostages
-- @param units, list of DirectIA agent knowledges
-- @param message, report enum
-- @return true
integration.captureEnd = function( units, message )
    for _, unit in pairs( units ) do
            DEC_Agent_ForcerSilenceRadio( unit.source, false )
            DEC_UnitDecisionalState( unit.source, "hostage", "false" )
            myself.CRCaptureSomeone[ unit ] = false
            unit.capture = false
            integration.SendMessage( "captureEnd", integration.getAgentFromKnowledge( unit ), { element = myself }, { type = "dynamic" } )
    end
    return true
end

--- Returns a list of captured units ('hostages') by the agent
-- @return list of DirectIA agent knowledges
integration.getCapturedUnits = function()
    return myself.capturedUnits 
end

--- Allows the unit to drop off the provided agent knowledge
-- The agent knowledge is instantaneously dropped off at the location of the unit
-- The agent knowledge is always an 'hostage' without any freedom of action
-- A report is sent by the unit 
-- @param unit Directia agent knowledge
-- @return true
integration.dropUnit = function( unit )
    DEC_Prisonniers_Debarquer(unit.source)
    reportFunction(eRC_TerroristDropped, unit.source )
    integration.removeFromLoadedUnits( unit )
    integration.removeFromCapturedUnits( unit )
    return true
end

--- Allows the unit to share knowledges information (e.g. objects, units...) with the provided agent knowledge
-- Knowledges sharing starts after a delay. If the delay is 0, the sharing is instantaneous 
-- @param dest Directia agent knowledge
-- @param delay Float delay in minutes
integration.shareKnowledge = function( dest, delay )
    DEC_Connaissances_PartageConnaissancesAvecConnaissanceAgent( myself, dest.source, delay )
end

--- Allows the unit to share knowledges information (e.g. objects knowledges, units knowledges) with the automat of the provided agent knowledge
-- Knowledges sharing starts after a delay. If the delay is 0, the sharing is instantaneous 
-- @param dest Directia agent knowledge
-- @param delay Float delay in minutes
integration.shareKnowledgeAgent = function( dest, delay )
    DEC_Connaissances_PartageConnaissancesAvec(DEC_GetAutomate( dest.source ), delay )
end

--- Allows the automat to share knowledges information (e.g. objects knowledges, units knowledges) with the provided agent knowledge
-- Knowledges sharing starts after a delay. If the delay is 0, the sharing is instantaneous 
-- @param dest Directia agent knowledge
-- @param delay Float delay in minutes
integration.shareKnowledgeAgentFromAutomat = function( dest, delay )
    DEC_Connaissances_PartageConnaissancesAvec(dest.source, delay )
end

--- Allows the unit to Launch projectile on the provided localized element knowledge
-- @param target Directia target knowledge (target should be a localized element, e.g. area, crowd, agent, point, urban block...)
-- @param dotation simulation dotation category
-- @param quantity Float number of iterations for a shoot
-- @return false 
integration.startLaunchProjectile = function( target, dotation, quantity )
-- $$$ MIA: same thing than the "integration.startAttackIt" for terrorist but without the terrorist attack conotation.
-- $$$ MIA : to merge with militaty
    target[ myself ] = target[ myself ] or {}
    local nbIntervention = quantity
    target[ myself ].attackAction = DEC_StartTirIndirectSurPosition( dotation, quantity, target:getPosition() )
    actionCallbacks[ target[ myself ].attackAction ] = function( arg ) 
        target[ myself ].attackState = arg 
    end
    return false
end

--- Continue launching projectile on the provided target knowledge
-- A report is sent following the firing    
-- @param target Directia target knowledge (target should be a localized element, e.g. area, crowd, agent, point, urban block...)
-- @param dotation simulation dotation category
-- @return Boolean, following the state of the firing action
integration.updateLaunchProjectile = function( target, dotation )
    target[ myself ] = target[ myself ] or {}
    if target[ myself ].attackState then
        if target[ myself ].attackState == eIndirectFireState_Running then
            return false
        elseif target[ myself ].attackState == eIndirectFireState_Finished then
            reportOnceFunction( eRC_TirExecute )
            return true
        elseif target[ myself ].attackState == eIndirectFireState_NoAmmo then
            reportOnceFunction( eRC_TirImpossiblePlusDeMunitions )
            return true
        elseif eIndirectFireState_Impossible then
            reportOnceFunction( eRC_TirIndirectImpossible )
            return true
        elseif eIndirectFireState_NoCapacity then
            reportOnceFunction( eRC_TirIndirectNoCapacity )
            return true
        end
    end
    return false
end

--- Stop launching projectile on the provided target knowledge
-- @param target Directia target knowledge (target should be a localized element, e.g. area, crowd, agent, point, urban block...)
-- @param dotation simulation dotation category
-- @return true
integration.stopLaunchProjectile = function( target, dotation )
    target[ myself ] = target[ myself ] or {}
    target[ myself ].attackAction = DEC__StopAction( target[ myself ].attackAction )
    target[ myself ].attackState = nil
    return true
end