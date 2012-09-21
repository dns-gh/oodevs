--Destroyable Implementation
eTirDirectNormal = 0
eTirDirectLibre = 1

local eActionTirDirect_Impossible = 0
local eActionTirDirect_EnemyDestroyed = 1
local eActionTirDirect_NoCapacity = 2
local eActionTirDirect_NoAmmo = 3
local eActionTirDirect_Running = 4
local eActionTirDirect_Finished = 5
local eActionTirDirect_TemporarilyBlocked = 6
local eActionTirDirect_None = 7



integration.startDestroyingIt = function( target )
  target[myself] = target[myself] or {}
  if not target[myself].actionTir then
    -- @TODO DDA: replace by DEC__StartTirDirectSurComposantesMajeures with the right ennemies ammoDotationClass
    target[myself].actionTir = DEC_StartTirDirect( target.source, 100, eTirDirectNormal, -1 )
    actionCallbacks[ target[myself].actionTir ] = function( arg ) target[myself].eTir = arg end
  end
end
integration.startDestroyingItWithMissileAir = function( target, percentage )
  target[myself] = target[myself] or {}
  if not target[myself].actionTir then
    target[myself].actionTir = DEC_StartTirDirect( target.source, percentage, eTirDirectNormal, eMunitionClasse_MissileAir )
    actionCallbacks[ target[myself].actionTir ] = function( arg ) target[myself].eTir = arg end
  end
end
-- respond actions
integration.startRespondIt = function( target )
  target[myself] = target[myself] or {}
  if not target[myself].actionRespond then
    target[myself].actionRespond = DEC_StartTirDirect( target.source, 100, eTirDirectNormal, -1 )
    actionCallbacks[ target[myself].actionRespond ] = function( arg ) target[myself].eTir = arg end
  end
end

integration.startedDestroyingIt = function( target )
    if target[myself].eTir == eActionTirDirect_NoAmmo and not target[myself].noAmmo then
        target[myself].noAmmo = true
        meKnowledge:RC( eRC_TirImpossiblePlusDeMunitions )
        return true
    elseif target[myself].eTir == eActionTirDirect_NoCapacity and not target[myself].noCapacity then
        target[myself].noCapacity = true
        meKnowledge:RC( eRC_TirSansCapacite )
        return true
    elseif target[myself].eTir == eActionTirDirect_Impossible and not target[myself].noTir then
        target[myself].noTir = true
        meKnowledge:RC( eRC_TirDirectImpossible )
        return true
    elseif target[myself] and target[myself].eTir and target[myself].eTir ~= 2  then
        g_myEnemy = target.source
        return true
    end
  return false
end

integration.stopDestroyingIt = function( target )
    target[myself] = target[myself] or {}
    if  target[myself].actionTir ~= nil then
        target[myself].actionTir = DEC__StopAction( target[myself].actionTir )
        target[myself].actionTir = nil
        target[myself].eTir = nil
        g_myEnemy = nil
    end
end
integration.stopRespondIt = function( target )
    target[myself] = target[myself] or {}
    if  target[myself].actionRespond ~= nil then
        target[myself].actionRespond = DEC__StopAction( target[myself].actionRespond )
        target[myself].actionRespond = nil
        target[myself].eTir = nil
        g_myEnemy = nil
    end
end

integration.firePermitted = function( target )
    local stateROE = DEC_Agent_GetEtatROE()
    if (not integration.isInForbiddenFireArea( target ) ) and ( stateROE == eRoeStateFreeFire ) then
        return true
    end
    return false
end

integration.fireNotForbbiden = function( target )
    local stateROE = DEC_Agent_GetEtatROE()
    if (not integration.isInForbiddenFireArea( target ) ) and ( stateROE ~= eRoeStateFireByOrder ) then
        return true
    end
    return false
end

integration.isInForbiddenFireArea = function( target )
    local friendly = 1 -- tristate: 0 = enemy, 1 = friendly, 2 = unknown
    return DEC_KnowledgeAgent_IsInObject( eTypeObjectZoneInterditeTir, target.source, friendly )
end

-- Renvoie l'attrition potentielle [0;100] de l'agent � partir d'une position donn�e, pour un PH donn�.
integration.getAttrition = function( self, target, position, pH )

    -- gestion des munitions --
    if target.eTir == eActionTirDirect_NoAmmo or target.eTir == eActionTirDirect_NoCapacity or target.eTir == eActionTirDirect_Impossible then
        return 0
    end

    -- gestion ligne de tir --
    if integration.getPerception( position, target ) == 0 then
        return 0
    end

    -- gestion de la port�e --
    local porteeMax    = DEC_Tir_PorteeMaxPourTirerSurUnitePosturesReelles( target.source, pH )
    local porteeMin    = DEC_Tir_PorteeMinPourTirerSurUnitePosturesReelles( target.source, pH )
    local distanceAEni = integration.distance( position, target )
    if( distanceAEni > porteeMax or distanceAEni < porteeMin ) then
        return 0 
    end
    return DEC_ConnaissanceAgent_AttritionPotentielle( target.source, position:getPosition() ) * 100
end

integration.getAgentIdFromKnowledge = function( agentKnowledge )
    return DEC_GetAgentIdFromKnowledge( agentKnowledge.source )
end

integration.isMoving = function ( target )
    return DEC_ConnaissanceAgent_EstEnMouvement( target.source )
end

integration.porteeMaxPourTirerSurUnitePosturesReelles = function( eni, ph )
    return DEC_Tir_PorteeMaxPourTirerSurUnitePosturesReelles( eni.source, ph )
end

integration.niTropPresNiTropLoin = function( eni, ph )
    local rPorteeMin = DEC_Tir_PorteeMinPourTirerSurUnitePosturesReelles( eni.source, ph )
    local rDistanceAEni = integration.distance( meKnowledge, eni)
    local bTropProche = rDistanceAEni < rPorteeMin


    local rPorteeMax = DEC_Tir_PorteeMaxPourTirerSurUnitePosturesReelles( eni.source, ph )
    local rDistanceAEni = DEC_Geometrie_Distance3D( meKnowledge:getPosition(), 0, eni:getPosition(), DEC_ConnaissanceAgent_Altitude( eni.source ) )
    local bTropLoin =  rDistanceAEni > rPorteeMax
    
    return ( not ( bTropProche or bTropLoin ))
end
