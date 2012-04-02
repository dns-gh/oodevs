-------------------------------------------------------------------------------
-- Queries Predicate Tools Implementation :
-- @author DDA
-- @created 2010-06-16
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Math�matiques Appliqu�es SA (MASA)
-------------------------------------------------------------------------------
integration.setStealth = function( beStealth )
    local stealthFactor = beStealth and 0 or 1
    if myself.lastStealth ~= stealthFactor then
        DEC_Perception_Furtivite( stealthFactor )
        myself.lastStealth = stealthFactor
    end
 end

integration.isStealth = function( self )
    return DEC_Perception_EstFurtif()
end

integration.porteePourAttentat = function( dotation )
    myself[dotation] = myself[dotation] or {}
    myself[dotation].portee = myself[dotation].portee or DEC_Tir_PorteeMaxTirIndirect( dotation )
    return myself[dotation].portee
end

--Terrorist attack integration
integration.startAttackIt = function( target, suicide, dotation )
    target[myself] = target[myself] or {}
    local nbIntervention = 1
    if suicide then nbIntervention = 10 end -- $$$$ Hard coded value (same as max amout of ammo)
    meKnowledge:RC( eRC_ExecutionAttentat )
    target[myself].attackAction = DEC_StartTirIndirectSurPosition( dotation, nbIntervention, target:getPosition() )
    actionCallbacks[ target[myself].attackAction ] = function( arg ) target[myself].attackState = arg end
    return true
end

integration.updateAttackIt = function( target, suicide, dotation )
    target[myself] = target[myself] or {}
    if target[myself].attackState then
        if target[myself].attackState == eIndirectFireState_Running then
            return false
        elseif target[myself].attackState == eIndirectFireState_Finished or target[myself].attackState == eIndirectFireState_NoAmmo then
            if suicide then DEC_Suicide() end
            return true
        else
            meKnowledge:RC( eRC_MissionImpossibleReason, "Attentat impossible" )
            return true
        end
    end
    return false
end

integration.commitSuicide = function( self )
    DEC_Suicide()
end

integration.stopAttackIt = function( target, suicide, dotation )
    target[myself] = target[myself] or {}
    target[myself].attackAction = DEC__StopAction( target[myself].attackAction )
    target[myself].attackState = nil
    return true
end


--Terrorist attack on object integration
integration.attackObject = function( target, suicide, dotation )
    meKnowledge:RC( eRC_ExecutionAttentat )
    DEC_ConnaissanceObjet_Degrader( target.source, 0.5, dotation )
    if suicide then DEC_Suicide() end
    return true
end

--Terrorist Officer assassination integration
integration.killOfficers = function( unit )
    DEC_ConnaissanceAgent_TuerOfficiers( unit.source )
    meKnowledge:RC( eRC_ExecutionAttentat )
    return true
end

--Firing Range for specific dotation
integration.isInFiringRangeForDotation = function( target, dotation )
    return integration.distance( meKnowledge, target ) < DEC_Tir_PorteeMaxPourTirerSurUniteAvecMunition( target.source, 0.9, dotation )
end

--Suicide
integration.selfDestruct = function( agent )
    DEC_Suicide()
end

--refugee
integration.takeAsRefugee = function( unit )
    -- $$$ MIA TODO
    DEC_RC( eRC_OrientationPopulationVersCampRefugies )
    return true
end

--prisoner
integration.takeAsPrisoner = function( unit )
    -- $$$ MIA TODO
    DEC_RC( eRC_OrientationEnnemiRenduVersCampPrisonniers )
    return true
end

integration.attackReport = function( self )
    meKnowledge:RC( eRC_ExecutionAttentat )
end

-- capture de terroristes d�tect�s
-- changemement de leur �tat en "otage"

integration.capture = function( units, message )

    if not myself.CRCaptureSomeone then return true end

    for _, unit in pairs( units ) do
        if not myself.CRCaptureSomeone[unit] then
            DEC_Agent_ForcerSilenceRadio( unit.source, true )
            DEC_UnitDecisionalState( unit.source, "hostage", "true" )
            DEC_Connaissance_Transporter( myself, unit.source )
            meKnowledge:RC( message, unit.source )
            myself.CRCaptureSomeone[ unit ] = true
            unit.capture = true
            integration.SendMessage( "capture", integration.getAgentFromKnowledge( unit ), { element = myself }, { type = "dynamic" } )
            DEC_ChangerSuperieurLogistiqueConnaissance( myself, unit.source )
        end
    end

    return true
end

integration.dropUnit = function( unit )
    DEC_Prisonniers_Debarquer(unit.source)
    return true
end

integration.shareKnowledge = function( dest, delay )
    meKnowledge:RC( eRC_DebutLiaison )
    DEC_Connaissances_PartageConnaissancesAvecConnaissanceAgent( myself, dest.source, delay )
end

integration.shareKnowledgeAgent = function( dest, delay )
    meKnowledge:RC( eRC_DebutLiaison )
    DEC_Connaissances_PartageConnaissancesAvec(DEC_GetAutomate( dest.source ), delay )
end

integration.shareKnowledgeAgentFromAutomat = function( dest, delay )
    meKnowledge:RC( eRC_DebutLiaison )
    DEC_Connaissances_PartageConnaissancesAvec(dest.source, delay )
end


-- --------------------------------------------------------------------------------
--  Launch projectile on a target 
-- $$$ MIA: same thing than the "integration.startAttackIt" for terrorist but without 
-- the terrorist attack conotation.
-- $$$ MIA : to merge with militaty
-- --------------------------------------------------------------------------------
integration.startLaunchProjectile = function( target, dotation, quantity )
    target[ myself ] = target[ myself ] or {}
    local nbIntervention = quantity
    target[ myself ].attackAction = DEC_StartTirIndirectSurPosition( dotation, quantity, target:getPosition() )
    actionCallbacks[ target[ myself ].attackAction ] = function( arg ) 
        target[ myself ].attackState = arg 
    end
    return false
end

integration.updateLaunchProjectile = function( target, dotation )
    target[ myself ] = target[ myself ] or {}
    if target[ myself ].attackState then
        if target[ myself ].attackState == eIndirectFireState_Running then
            return false
        elseif target[ myself ].attackState == eIndirectFireState_Finished then
            meKnowledge:sendReport( eRC_TirExecute )
            return true
        elseif target[ myself ].attackState == eIndirectFireState_NoAmmo then
            meKnowledge:sendReport( eRC_TirImpossiblePlusDeMunitions )
            return true
        elseif eIndirectFireState_Impossible then
            meKnowledge:sendReport( eRC_TirIndirectImpossible )
            return true
        elseif eIndirectFireState_NoCapacity then
            meKnowledge:sendReport( eRC_TirIndirectNoCapacity )
            return true
        end
    end
    return false
end

integration.stopLaunchProjectile = function( target, dotation )
    target[ myself ] = target[ myself ] or {}
    target[ myself ].attackAction = DEC__StopAction( target[ myself ].attackAction )
    target[ myself ].attackState = nil
    return true
end