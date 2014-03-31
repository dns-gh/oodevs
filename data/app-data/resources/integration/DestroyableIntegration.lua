--Destroyable Implementation

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


--- Manage the direct fire action, method calls at each tick during the action
-- @param target The agent to shoot with direct fire
-- @param snipe a boolean if action is used by sniper : send a report if firing is temporarily blocked in urban area
-- @return a boolean 'true' if action is finished, 'false' if action continues
integration.startedDestroyingIt = function( target, snipe )
    if target[myself] then
        if target[myself].eTir == eActionTirDirect_NoAmmo and not target[myself].noAmmo then
            target[myself].noAmmo = true
            reportFunction(eRC_TirImpossiblePlusDeMunitions )
            return true
        elseif target[myself].eTir == eActionTirDirect_NoCapacity and not target[myself].noCapacity then
            target[myself].noCapacity = true
            reportFunction(eRC_TirSansCapacite )
            return true
        elseif target[myself].eTir == eActionTirDirect_Impossible and not target[myself].noTir then
            target[myself].noTir = true
            reportFunction(eRC_TirDirectImpossible )
            return true
        elseif snipe and target[myself].eTir == eActionTirDirect_TemporarilyBlocked and not target[myself].blocked then
            target[myself].blocked = true
            reportFunction(eRC_ShootingTemporarilyBlocked )
        elseif target[myself].eTir and target[myself].eTir ~= 2  then
            g_myEnemy = target.source
            return true
        end
        return false
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
    local stateROE = integration.getROE()
    if (not integration.isInForbiddenFireArea( target ) ) and ( stateROE == eRoeStateFreeFire ) then
        return true
    end
    return false
end

integration.firePermittedForPoint = function( target )
    local stateROE = integration.getROE()
    local localisation = DEC_Geometrie_CreerLocalisationCercle( target:getPosition(), 10 )
    local object = integration.obtenirObjetProcheDe( localisation, eTypeObjectFireForbiddenArea, 10000 )
    if object then    
        local area = DEC_GenObjectKnowledge_Localisation( object )
        if (not integration.isPointInSimLocalisation(target, area)) and ( stateROE ~= eRoeStateFireByOrder ) then
            return true
        else
            return false
        end
    end
    return true
end

integration.fireNotForbbiden = function( target )
    local stateROE = integration.getROE()
    if (not integration.isInForbiddenFireArea( target ) ) and ( stateROE ~= eRoeStateFireByOrder ) then
        return true
    end
    return false
end

integration.isInForbiddenFireArea = function( target )
    local friendly = 1 -- tristate: 0 = enemy, 1 = friendly, 2 = unknown
    return DEC_KnowledgeAgent_IsInObject( eTypeObjectFiringForbiddenArea, target.source, friendly )
end

-- Renvoie l'attrition potentielle [0;100] de l'agent à partir d'une position donnée, pour un PH donné.
integration.getAttrition = function( self, target, position, pH )

    -- gestion des munitions --
    if target.eTir == eActionTirDirect_NoAmmo or target.eTir == eActionTirDirect_NoCapacity or target.eTir == eActionTirDirect_Impossible then
        return 0
    end

    -- gestion ligne de tir --
    if integration.getPerception( position, target ) == 0 then
        return 0
    end

    -- gestion de la portée --
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

integration.isFlying = function ( target )
    return DEC_ConnaissanceAgent_EstEnVol( target.source )
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

integration.forbidAmmunition = function( munitions )
    DEC_Pion_InterdireMunition( munitions )
end

integration.autoriseAmmunition = function( munitions )
    DEC_Pion_AutoriserMunition( munitions )
end

integration.autoriseAllAmmunitions = function()
    DEC_Pion_AutoriserToutesMunitions()
end

integration.getMaxRangeToFireForPH = function( pH )
    return DEC_Tir_PorteeMaxPourTirer( pH )
end

integration.getMaxRangeToFireAgentForPH = function( agent, pH )
    return DEC_Tir_PorteeMaxPourTirer( agent, pH )
end

integration.getKnowledgesUnitsEngaging = function()
    return DEC_Connaissances_UnitesPrenantAPartie()
end

integration.getKnowledgesUnitsEngagingFriend = function( friendAgent )
    return DEC_Connaissances_UnitesPrenantAPartieSurAmi( friendAgent )
end

integration.getKnowledgesDangerousUnits = function( )
    return DEC_Connaissances_UnitesEnnemiesDangereuses( )
end

integration.getKnowledgeDangerousAgent = function( agent )
    return DEC_ConnaissanceAgent_Dangerosite( agent )
end

integration.isAgentTacticallyDestroyed = function( agent )
    return DEC_ConnaissanceAgent_EstDetruitTactique( agent )
end

integration.getMaxRangeToFireOnAgent = function( agent, pH )
    return DEC_Tir_PorteeMaxPourTirerSurUnite( agent, pH )
end

integration.getMaxRangeToBeFiriedByAgent = function( agent, pH )
    return DEC_Tir_PorteeMaxPourEtreTireParUnite( agent, pH )
end
     
integration.canBeDestroyedWithMissiles = function( targetUnit, ph, speed )
    local integration = integration
    local distanceCouverte = integration.porteeMaxPourTirerSurUnitePosturesReelles( targetUnit, ph )
    local pointInterception = integration.positionInterception( targetUnit, speed )
    if not pointInterception then
        return false
    end
    local distancePointInterception = DEC_Geometrie_DistanceBetweenPoints( meKnowledge:getPosition(), pointInterception )
    local tempsInterception =  distancePointInterception / ( speed * 60 )
    if( distancePointInterception <= distanceCouverte ) then
        if targetUnit:isValid() then
            if waitInMin( meKnowledge, tempsInterception ) then
                if( integration.niTropPresNiTropLoin( targetUnit, ph ) ) then
                    return true
                end
            end
        end
    end
    return false
end
