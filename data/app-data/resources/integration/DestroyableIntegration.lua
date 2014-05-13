-------------------------------------------------------------------
-- DESTROYABLE INTERFACE IMPLEMENTATION
-------------------------------------------------------------------

--- Starts firing at a target unit with direct fire.
-- This method can only be called by an agent.
-- @param target a DirectIA agent knowledge
integration.startDestroyingIt = function( target )
    target[ myself ] = target[ myself ] or {}
    if not target[ myself ].actionTir then
        target[ myself ].actionTir = DEC_StartTirDirect( target.source, 100, eTirDirectNormal, -1 )
        actionCallbacks[ target[ myself ].actionTir ] = function( arg ) 
            target[ myself ].eTir = arg 
        end
    end
end

--- Starts firing at a target unit, specifically with a 'ground- air missile' type of ammunition.
-- This method can only be called by an agent.
-- @param target, DirectIA agent knowledge
-- @param percentage the percentage of componants to be used to open fire.
-- @see munition class definition in 'type.lua' file.
integration.startDestroyingItWithMissileAir = function( target, percentage )
    target[ myself ] = target[ myself ] or {}
    if not target[ myself ].actionTir then
        target[ myself ].actionTir = DEC_StartTirDirect( target.source, percentage, eTirDirectNormal, eMunitionClasse_MissileAir )
        actionCallbacks[ target[ myself ].actionTir ] = function( arg ) 
            target[ myself ].eTir = arg
        end
    end
end

--- Starts firing back at a target unit.
-- This method can only be called by an agent.
-- @param target a DirectIA agent knowledge
integration.startRespondIt = function( target )
    target[ myself ] = target[ myself ] or {}
    if not target[ myself ].actionRespond then
        target[ myself ].actionRespond = DEC_StartTirDirect( target.source, 100, eTirDirectNormal, -1 )
        actionCallbacks[ target[ myself ].actionRespond ] = function( arg )
            target[ myself ].eTir = arg 
        end
    end
end

--- Manages the direct fire actions.
-- This method can only be called by an agent.
-- @see integration.startDestroyingIt and integration.startDestroyingItWithMissileAir methods
-- @param target The agent to shoot with direct fire
-- @param snipe a boolean if action is used by sniper : send a report if firing is temporarily blocked in urban area
-- @return Two return values :
-- <ul> <li> Boolean, 'true' if a fire has been shot or if it will never be possible to shoot; false otherwise </li>
-- <li> Integer, the fire action current state, among one of these possible values (or nil if there is no current action state) : </li>
-- <ul> <li> eActionTirDirect_Impossible </li>
-- <li> eActionTirDirect_EnemyDestroyed </li>
-- <li> eActionTirDirect_NoCapacity </li>
-- <li> eActionTirDirect_NoAmmo </li>
-- <li> eActionTirDirect_Running </li>
-- <li> eActionTirDirect_Finished </li>
-- <li> eActionTirDirect_TemporarilyBlocked </li>
-- <li> eActionTirDirect_None </li> </ul> </ul>
integration.startedDestroyingIt = function( target, snipe )
    if target[myself] then
        local actionState = target[myself].eTir
        if actionState == eActionTirDirect_NoAmmo and not target[myself].noAmmo then
            target[myself].noAmmo = true
            reportFunction(eRC_TirImpossiblePlusDeMunitions )
            return true, actionState
        elseif actionState == eActionTirDirect_NoCapacity and not target[myself].noCapacity then
            target[myself].noCapacity = true
            reportFunction(eRC_TirSansCapacite )
            return true, actionState
        elseif actionState == eActionTirDirect_Impossible and not target[myself].noTir then
            target[myself].noTir = true
            reportFunction(eRC_TirDirectImpossible )
            return true, actionState
        elseif snipe and actionState == eActionTirDirect_TemporarilyBlocked and not target[myself].blocked then
            target[myself].blocked = true
            reportFunction(eRC_ShootingTemporarilyBlocked )
        elseif actionState and actionState ~= eActionTirDirect_NoCapacity then
            g_myEnemy = target.source
            return true, actionState
        end
        return false, actionState
    end
    return false
end

--- Stops the fire action.
-- This method can only be called by an agent.
-- @see integration.startDestroyingIt and integration.startDestroyingItWithMissileAir methods
-- @param target a DirectIA agent knowledge
integration.stopDestroyingIt = function( target )
    target[ myself ] = target[ myself ] or {}
    if  target[ myself ].actionTir ~= nil then
        target[ myself ].actionTir = DEC__StopAction( target[ myself ].actionTir )
        target[ myself ].actionTir = nil
        target[ myself ].eTir = nil
        g_myEnemy = nil
    end
end

--- Stops the respond action.
-- This method can only be called by an agent.
-- @see integration.startRespondIt methods
-- @param target a DirectIA agent knowledge
integration.stopRespondIt = function( target )
    target[ myself ] = target[ myself ] or {}
    if  target[ myself ].actionRespond ~= nil then
        target[ myself ].actionRespond = DEC__StopAction( target[ myself ].actionRespond )
        target[ myself ].actionRespond = nil
        target[ myself ].eTir = nil
        g_myEnemy = nil
    end
end

--- Informs whether or not the agent has the authorization to open fire at the given target.
-- The implentation checks if the current rule of engagement is compatible and if 
-- the target is not located into a fire restricted area. 
-- This method can only be called by an agent.
-- @param target a DirectIA agent knowledge
-- @return a boolean 'true' if the agent can open fire, 'false' otherwise.
integration.firePermitted = function( target )
    local stateROE = integration.getROE()
    if ( not integration.isInForbiddenFireArea( target ) ) and ( stateROE == eRoeStateFreeFire ) then
        return true
    end
    return false
end

--- Informs if the agent has the authorization to open fire onto a given position.
-- The implentation checks if the current rule of engagement is compatible and if 
-- the target is not located into a fire restricted area. 
-- This method can only be called by an agent.
-- @param target a DirectIA 'LocalizedElement 'knowledge.
-- @return a boolean 'true' if the agent can open fire, 'false' otherwise.
integration.firePermittedForPoint = function( target )
    local stateROE = integration.getROE()
    local localisation = DEC_Geometrie_CreerLocalisationCercle( target:getPosition(), 10 )
    local object = integration.obtenirObjetProcheDe( localisation, eTypeObjectFireForbiddenArea, 10000 )
    if object then
        local area = DEC_GenObjectKnowledge_Localisation( object )
        if ( not integration.isPointInSimLocalisation( target, area ) ) 
             and ( stateROE ~= eRoeStateFireByOrder ) then
            return true
        else
            return false
        end
    end 
    return true
end

--- Informs if the agent has the authorization to open fire at the given target.
-- The implentation checks if the current rule of engagement is compatible and if 
-- the target is not located into a fire restricted area. 
-- This method can only be called by an agent.
-- @param target a DirectIA agent knowledge
-- @return a boolean 'true' if the agent can open fire, 'false' otherwise.
integration.fireNotForbbiden = function( target )
    local stateROE = integration.getROE()
    if (not integration.isInForbiddenFireArea( target ) ) and ( stateROE ~= eRoeStateFireByOrder ) then
        return true
    end
    return false
end

--- Informs if the provided target is located into a fire restricted area. 
-- The implementation tests if the target is inside a "firing forbidden area" type of object.
-- This method can only be called by an agent.
-- @param target a DirectIA agent knowledge
-- @return a boolean 'true' if the target is located inside a "firing forbidden area" type of object, 'false' otherwise.
integration.isInForbiddenFireArea = function( target )
    local friendly = 1 -- tristate: 0 = enemy, 1 = friendly, 2 = unknown
    return DEC_KnowledgeAgent_IsInObject( eTypeObjectFiringForbiddenArea, target.source, friendly )
end

--- Returns the estimation of the agent attrition power regarding the provided target, position and PH (Probability to Hit).
-- This method can only be called by an agent.
-- @param target a DirectIA agent knowledge.
-- @param position a knowledge
-- @param pH the probability to hit.
-- @return numeric a percentage of attrition power. A value of '0' means that the agent cannot engage 
-- the target from the position.
integration.getAttrition = function( self, target, position, pH )

    -- check ammunition --
    if target.eTir == eActionTirDirect_NoAmmo or target.eTir == eActionTirDirect_NoCapacity or target.eTir == eActionTirDirect_Impossible then
        return 0
    end

    -- check line of sight --
    if integration.getPerception( position, target ) == 0 then
        return 0
    end

    -- check the range (distance between the position and the target --
    local porteeMax    = DEC_Tir_PorteeMaxPourTirerSurUnitePosturesReelles( target.source, pH )
    local porteeMin    = DEC_Tir_PorteeMinPourTirerSurUnitePosturesReelles( target.source, pH )
    local distanceAEni = integration.distance( position, target )
    if( distanceAEni > porteeMax or distanceAEni < porteeMin ) then
        return 0 
    end
    return DEC_ConnaissanceAgent_AttritionPotentielle( target.source, position:getPosition() ) * 100
end

--- Returns the simulation agent ID from a DirectIA agent knowledge.
-- @param target a DirectIA agent knowledge.
-- @return numeric the simulation agent ID
-- the target from the positgion
integration.getAgentIdFromKnowledge = function( agentKnowledge )
    return DEC_GetAgentIdFromKnowledge( agentKnowledge.source )
end

--- Informs whether the provided agent knowledge is moving or not.
-- This method can only be called by an agent.
-- @param target a DirectIA agent knowledge.
-- @return boolean returns 'true' if the target is moving, 'false' otherwise.
integration.isMoving = function ( target )
    return DEC_ConnaissanceAgent_EstEnMouvement( target.source )
end

--- Informs whether the provided agent knowledge is flying or not.
-- This method can only be called by an agent.
-- @param target a DirectIA agent knowledge.
-- @return boolean returns 'true' if the target is flying, 'false' otherwise.
integration.isFlying = function ( target )
    return DEC_ConnaissanceAgent_EstEnVol( target.source )
end

--- Returns the maximum range (in meters) of the agent weapons systems, regarding the given target and PH (Probability to Hit). 
-- The range takes into account the current posture of the agent.
-- This method can only be called by an agent.
-- @param eni a DirectIA agent knowledge.
-- @param ph the probability to hit.
--  @return numeric the distance in meters.
integration.porteeMaxPourTirerSurUnitePosturesReelles = function( eni, ph )
    return DEC_Tir_PorteeMaxPourTirerSurUnitePosturesReelles( eni.source, ph )
end

--- Informs if the agent is in range to engage the given target, regarding the given PH (Probability to Hit).
-- This implementation checks if the maximum and minimum range.
-- The range takes into account the current posture of the agent.
-- This method can only be called by an agent.
-- @param eni a DirectIA agent knowledge.
-- @param ph the probability to hit.
-- @return boolean returns 'true' if the agent is in range, 'false' otherwise.
integration.niTropPresNiTropLoin = function( eni, ph )

    -- check if eni is not to close.
    local rPorteeMin = DEC_Tir_PorteeMinPourTirerSurUnitePosturesReelles( eni.source, ph )
    local rDistanceAEni = integration.distance( meKnowledge, eni)
    local bTropProche = rDistanceAEni < rPorteeMin

    -- check if eni is not to far.
    local rPorteeMax = DEC_Tir_PorteeMaxPourTirerSurUnitePosturesReelles( eni.source, ph )
    local rDistanceAEni = DEC_Geometrie_Distance3D( meKnowledge:getPosition(), 0, eni:getPosition(), DEC_ConnaissanceAgent_Altitude( eni.source ) )
    local bTropLoin =  rDistanceAEni > rPorteeMax

    return ( not ( bTropProche or bTropLoin ) )
end

--- Forbid the use of the given type of ammunition. See dotation categories in authoring tool.
-- <ul> <li> Possible values are: </li>
-- <li> eMunitionClasse_Obus = 0 </li>
-- <li> eMunitionClasse_MissileSol = 1 </li>
-- <li> eMunitionClasse_MissileAir = 2 </li>
-- <li> eMunitionClasse_Mitraille = 3 </li> </ul>
-- This method can only be called by an agent.
-- @param munitions numeric the type of ammunition
integration.forbidAmmunition = function( munitions )
    DEC_Pion_InterdireMunition( munitions )
end

--- Authorize the use of the given type of ammunition. See dotation categories in authoring tool.
-- <ul> <li> Possible values are: </li>
-- <li> eMunitionClasse_Obus = 0 </li>
-- <li> eMunitionClasse_MissileSol = 1 </li>
-- <li> eMunitionClasse_MissileAir = 2 </li>
-- <li> eMunitionClasse_Mitraille = 3 </li> </ul>
-- This method can only be called by an agent.
-- @param munitions numeric the type of ammunition
integration.autoriseAmmunition = function( munitions )
    DEC_Pion_AutoriserMunition( munitions )
end

--- Authorize the use of all types of ammunition. See dotation categories in authoring tool.
integration.autoriseAllAmmunitions = function()
    DEC_Pion_AutoriserToutesMunitions()
end

--- Returns the maximum range (in meters) of the agent weapons systems regarding the given PH (Probability to Hit). 
-- See 'weapon systems' tab in authoring tool.
-- This method can only be called by an agent.
-- @param ph the probability to hit.
-- @return numeric the distance in meters.
integration.getMaxRangeToFireForPH = function( pH )
    return DEC_Tir_PorteeMaxPourTirer( pH )
end

--- Returns the maximum range (in meters) of the agent weapons systems, regarding the given target and PH (Probability to Hit). 
-- The returned range does not take into account the current posture of the agent.
-- This method can only be called by an agent.
-- @param agent a simulation agent knowledge.
-- @param ph the probability to hit.
-- @return numeric the distance in meters.
integration.getMaxRangeToFireAgentForPH = function( agent, pH )
    return DEC_Tir_PorteeMaxPourTirer( agent, pH )
end

--- Returns the list of agents knowledge that are currently firing at the agent.
-- This method can only be called by an agent.
-- @return table containing the simulation agents knowledge engaging the agent.
integration.getKnowledgesUnitsEngaging = function()
    return DEC_Connaissances_UnitesPrenantAPartie()
end

--- Returns the list of agents knowledge that are currently firing at the given 'friendAgent'.
-- This method can only be called by an agent.
-- @param friendAgent a simulation agent knowledge.
-- @return table containing the knowledge agents engaging the agent.
integration.getKnowledgesUnitsEngagingFriend = function( friendAgent )
    return DEC_Connaissances_UnitesPrenantAPartieSurAmi( friendAgent )
end

--- Returns the list of hostile agents knowledge that are considered as 'dangerous' for the agent.
-- A 'dangerous' unit is a unit which PH onto the agent is striclty greater than 0. This means that 
-- all returned agents knowledge has the capability to open fire at the agent.
-- Only enemies are returned considering the diplomatie matrix. 
-- This method can only be called by an agent.
-- @return table the knowledge agents that can engage the agent.
integration.getKnowledgesDangerousUnits = function()
    return DEC_Connaissances_UnitesEnnemiesDangereuses( )
end

--- Returns the dangerosity of the given agent knowledge.
-- A 'dangerous' unit is an unit which PH onto the agent is striclty greater than 0. This means that 
-- the agent knowledge has to capability to open fire at the agent.
-- This method can only be called by an agent.
-- @param agent a simulation agent knowledge.
-- @return numeric, a value between 0 and 1, the dangerosity of the given agent. A value of '0' means not dangerous at all,
-- a value of '1' means very dangerous.
integration.getKnowledgeDangerousAgent = function( agent )
    return DEC_ConnaissanceAgent_Dangerosite( agent )
end

--- Informs whether or not the given agent knowledge is tactically destroyed.
-- An agent is considered as not 'operational' ("tactically destroyed") if all its 
-- major equipments are destroyed (see 'Equipements' definitions, 'Units' tab ). 
-- This method can only be called by an agent.
-- @param agent a simulation agent knowledge.
-- @return boolean returns 'true' tactically destroyed, 'false' otherwise.
integration.isAgentTacticallyDestroyed = function( agent )
    return DEC_ConnaissanceAgent_EstDetruitTactique( agent )
end

--- Returns the maximum range (in meters) allowing the agent to open fire at the given agent knowledge, regarding the given PH.
-- This method can only be called by an agent.
-- @param agent a simulation agent knowledge.
-- @param pH numeric the probability to hit.
-- @return numeric returns the maximum range distance (in meters).
integration.getMaxRangeToFireOnAgent = function( agent, pH )
    return DEC_Tir_PorteeMaxPourTirerSurUnite( agent, pH )
end

--- Returns the maximum range (in meters) allowing the given  agent knowledge to open fire at the agent, regarding the given PH.
-- This method can only be called by an agent.
-- @param agent a simulation agent knowledge.
-- @param pH numeric the probability to hit.
-- @return numeric returns the maximum range distance (in meters).
integration.getMaxRangeToBeFiriedByAgent = function( agent, pH )
    return DEC_Tir_PorteeMaxPourEtreTireParUnite( agent, pH )
end

--- Returns whether or not the agent can open fire with regarding the given targetUnit, ph and targetSpeed.
-- The implementation estimate the future position of the given target (using the 'targetSpeed' parameter). If 
-- this position is in range regarding weapon systems, the method returns 'true' once the 'targetUnit' occupies the future position.
-- This method can only be called by an agent.
-- @param targetUnit a DirectIA agent knowledge.
-- @param pH numeric the probability to hit.
-- @param targetSpeed km/h the estimation of target speed.
-- @return boolean returns true when the agent can open fire, false otherwise.
integration.canBeDestroyedWithMissiles = function( targetUnit, ph, targetSpeed )
    local integration = integration
    local distanceCouverte = integration.porteeMaxPourTirerSurUnitePosturesReelles( targetUnit, ph )
    local pointInterception = integration.positionInterception( targetUnit, targetSpeed )
    if not pointInterception then
        return false
    end
    local distancePointInterception = DEC_Geometrie_DistanceBetweenPoints( meKnowledge:getPosition(), pointInterception )
    if( distancePointInterception <= distanceCouverte ) then
        if targetUnit:isValid() then
                if( integration.niTropPresNiTropLoin( targetUnit, ph ) ) then
                    return true
                end
            end
        end
    return false
end