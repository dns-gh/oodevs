-------------------------------------------------------------------------------
-- IndirectFireable Implementation : 
-- Regroup function to manage artillery field
-- @author MGD
-- @created 2010-01-22
-- @modified MGD 2010-01-27
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------

--- Indicate if the unit can fire at the target distance with corresponding munition 
-- @param munition The id of a munition
-- @param target The targeted integration.ontology.types.agent knowledge
-- @return bool
-- @author MGD
-- @created 2010-01-27
integration.isOnRangeFor = function( target, munition)
  local dist = integration.distance( meKnowledge, target )
  if dist > DEC_Tir_PorteeMaxTirIndirect_SansTesterEtatMunitions( munition ) then
    return false
  end
  if dist < DEC_Tir_PorteeMinTirIndirect_SansTesterEtatMunitions( munition ) then
    return false
  end
  return true
end

--- Indicate if the unit has launcher fo corresponding munition 
-- @param munition The id of a munition
-- @return bool
-- @author MGD
-- @created 2010-12-22
integration.hasLauncherFor = function( munition )
    return DEC_Tir_PorteeMaxTirIndirect_SansTesterEtatMunitions( munition ) ~= -1
end

--- Start an IndirectFire action on a target
-- @param target The targeted integration.ontology.types.agent knowledge
-- @param munition The id of a munition
-- @param interventionType The number of IT to fire
-- @author MGD
-- @created 2010-01-15
-- @modified MGD 2010-01-27
integration.startApplyFireOnPlatoon = function( target, munition, interventionType )
  target[myself] = target[myself] or {}
  target[myself].firstTime = true
  target[myself].actionIndirectFire = DEC_StartTirIndirectSurConnaissancePtr( munition, interventionType, target.source )
  actionCallbacks[ target[myself].actionIndirectFire ] = function( arg ) target[myself].eIndirectFireState = arg end
  return false
end

--- Start an IndirectFire action on a point
-- @param target The targeted integration.ontology.types.point knowledge
-- @param munition The id of a munition
-- @param interventionType The number of IT to fire
-- @author MGD
-- @created 2010-04-30
integration.startApplyFireOnPoint = function( point, munition, interventionType )
  point[myself] = point[myself] or {}
  point[myself].firstTime = true
  point[myself].actionIndirectFire = DEC_StartTirIndirectSurPosition( munition, interventionType, point.source )
  actionCallbacks[ point[myself].actionIndirectFire ] = function( arg ) point[myself].eIndirectFireState = arg end
  return false
end

--- Update an IndirectFire action on a point
-- @param point The targeted integration.ontology.types.agent knowledge
-- @author MGD
-- @created 2010-01-27
-- @modified LMT 2011-05-20
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
                meKnowledge:RC( eRC_PremierCoupParti )
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

--- Update an IndirectFire action on a target
-- @param target The targeted integration.ontology.types.agent knowledge
-- @author MGD
-- @created 2010-01-27
-- @modified LMT 2011-05-20
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

-- @created LMT 2011-05-20
integration.updateApplyFireImpossible = function( target )
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

--- Stop an IndirectFire action on a target
-- @param target The targeted integration.ontology.types.agent knowledge
-- @author MGD
-- @created 2010-01-15
-- @modified MGD 2010-01-27
integration.stopApplyFireOnPlatoon = function( target )
  if target[myself].eIndirectFireState == eIndirectFireState_Finished then
        reportFunction(eRC_FinTirIndirectSurCible, target.source )
  end
  target[myself].actionIndirectFire = DEC__StopAction( target[myself].actionIndirectFire )
  target[myself].eIndirectFireState = nil
end

--- Stop an IndirectFire action on a point
-- @param target The targeted integration.ontology.types.point knowledge
-- @author MGD
-- @created 2010-01-15
-- @modified MGD 2010-01-27
integration.stopApplyFireOnPoint = function( point )
  if point[myself].eIndirectFireState == eIndirectFireState_Finished then
        reportFunction(eRC_TirExecute )
  end
  point[myself].actionIndirectFire = DEC__StopAction( point[myself].actionIndirectFire )
  point[myself].eIndirectFireState = nil
end

integration.sendfireAvailable = function( bAvailable )
    if bAvailable then
        DEC_Agent_ChangeDisponibiliteAuTirIndirect( eFireAvailability_PretAuTir )
    else
        DEC_Agent_ChangeDisponibiliteAuTirIndirect( eFireAvailability_Indisponible )
    end
end

integration.isUnderIndirectFire = function()
    return DEC_Agent_EstCibleTirIndirect()
end

integration.munitionPourTirIndirect = function( position ) -- Be careful : if unit is load return is always nil
    return DEC_Tir_MunitionPourTirIndirect( eObus_Explosif, position ) 
end

integration.getMortarUnitsToNeutralize = function( friend )
    local res = {}
    local unitsAttackingFriend = integration.getKnowledgesUnitsEngagingFriend( friend.source )
    local unitsDangerous = integration.getKnowledgesDangerousUnits()
    res = fusionList( unitsAttackingFriend, unitsDangerous )
    return res
end

integration.findEnemyToMortarIndirectFire = function( targets )
    if not targets or #targets < 1 then
        return {}
    else
        local DEC_ConnaissanceAgent_EstUnEnnemi = DEC_ConnaissanceAgent_EstUnEnnemi
        local DEC_ConnaissanceAgent_EstDetruitTactique = DEC_ConnaissanceAgent_EstDetruitTactique
        local DEC_ConnaissanceAgent_EstEnVol = DEC_ConnaissanceAgent_EstEnVol
        local minDistance = 10000 -- why : because it's more than mortar range
        local result = {}
        local integration = integration
        for i = 1, #targets do
            local dotation = integration.munitionPourTirIndirect( targets[i]:getPosition() )
            if DEC_ConnaissanceAgent_EstUnEnnemi( targets[i].source ) and not DEC_ConnaissanceAgent_EstDetruitTactique( targets[i].source ) and not DEC_ConnaissanceAgent_EstEnVol( targets[i].source ) and 
              integration.isOnRangeFor( targets[i], dotation ) then
                local currentDistance = integration.distance( meKnowledge, targets[i] )
                if currentDistance < minDistance then -- return the nearest enemy.
                    result = targets[i]
                    minDistance = currentDistance
                end
            end
        end
        return result
    end
end

integration.launchDREB = function()
    return DEC_Tir_LancerFumigeneSurPosition(myself, meKnowledge:getPosition())
end

integration.launchSmokesOnEni = function( eni )
    DEC_Tir_LancerFumigeneSurConnaissance( eni )
end

integration.getMaxRangeIndirectFireWithoutSelectAmmo = function()
    return DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition()
end
