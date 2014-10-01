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

--- Starts firing at a target unit, specifically with a 'ground - air missile' type of ammunition.
-- This method can only be called by an agent.
-- @param target, DirectIA agent knowledge
-- @param percentage the percentage of components to be used to open fire.
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

--- Manages the feedbacks of a running fire action.
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

--- Stops a fire action.
-- This method can only be called by an agent.
-- @see integration.startDestroyingIt and integration.startDestroyingItWithMissileAir methods
-- @param target the unit fired upon (a DirectIA agent knowledge) 
integration.stopDestroyingIt = function( target )
    target[ myself ] = target[ myself ] or {}
    if  target[ myself ].actionTir ~= nil then
        target[ myself ].actionTir = DEC__StopAction( target[ myself ].actionTir )
        target[ myself ].actionTir = nil
        target[ myself ].eTir = nil
        g_myEnemy = nil
    end
end

--- Stops a respond action.
-- This method can only be called by an agent.
-- @see integration.startRespondIt methods
-- @param target the unit fired upon (a DirectIA agent knowledge) 
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
-- Checks that the current rule of engagement is "free fire" and that the target is not located inside a fire restricted area. 
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
-- Checks that the current rule of engagement is not "fire upon order" and that 
-- the position is not located inside a fire restricted area.
-- This method can only be called by an agent.
-- @param target a DirectIA 'LocalizedElement 'knowledge.
-- @return a boolean 'true' if the agent can open fire, 'false' otherwise.
integration.firePermittedForPoint = function( target )
    local stateROE = integration.getROE()
    local friendly = 1 -- tristate: 0 = enemy, 1 = friendly, 2 = unknown
    if DEC_IsPointInObject( myself, target:getPosition(), "fire-forbidden", friendly ) then
        reportOnceFunction( eRC_FireNotPermittedDueToForbiddenArea )
        return false
    end
    if ( stateROE == eRoeStateFireByOrder ) then
        reportOnceFunction( eRC_FireNotPermittedDueToROE )
        return false
    end
    return true
end

--- Informs if the agent has the authorization to open fire at the given target.
-- Checks if the current rule of engagement is not "fire upon order" and if
-- the target is not located inside a fire restricted area. 
-- This method can only be called by an agent.
-- @param target a DirectIA agent knowledge
-- @return a boolean 'true' if the agent can open fire, 'false' otherwise.
integration.fireNotForbbiden = function( target )
    local stateROE = integration.getROE()
    if( not integration.isInForbiddenFireArea( target ) ) and ( stateROE ~= eRoeStateFireByOrder ) then
        return true
    end
    return false
end

--- Informs if the provided target is located inside a fire restricted area. 
-- Tests if the target is inside a "firing forbidden area" type of object of a friendly side.
-- Always returns false if the target is dead.
-- This method can only be called by an agent.
-- @param target a DirectIA agent knowledge
-- @return a boolean 'true' if the target is located inside a "firing forbidden area" type of object, 'false' otherwise or if the target is dead.
integration.isInForbiddenFireArea = function( target )
    local friendly = 1 -- tristate: 0 = enemy, 1 = friendly, 2 = unknown
    return DEC_KnowledgeAgent_IsInObjectWithCapacity( "fire-forbidden", target.source, friendly )
end

--- Returns the estimation of the agent attrition power regarding the provided target, position and PH (Probability to Hit).
-- This method can only be called by an agent.
-- @param target a DirectIA agent knowledge.
-- @param position a knowledge
-- @param pH the probability to hit.
-- @return a number between 0 et 100. A value of '0' means that the agent cannot engage 
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
integration.getAgentIdFromKnowledge = function( agentKnowledge )
    return DEC_GetAgentIdFromKnowledge( agentKnowledge.source )
end

--- Informs whether the provided agent knowledge is moving or not.
-- @param target a DirectIA agent knowledge.
-- @return boolean returns 'true' if the target is moving, 'false' otherwise.
integration.isMoving = function ( target )
    return DEC_ConnaissanceAgent_EstEnMouvement( target.source )
end

--- Informs whether the provided agent knowledge is flying or not.
-- @param target a DirectIA agent knowledge.
-- @return boolean returns 'true' if the target is flying, 'false' otherwise.
integration.isFlying = function ( target )
    return DEC_ConnaissanceAgent_EstEnVol( target.source )
end

--- Returns the maximum range of the agent weapon systems allowing to hit the given target with a certain probability.
-- Returns 0 if the target cannot be hit at any range.
-- The range takes into account the current posture of both agents.
-- This method can only be called by an agent.
-- @param eni a DirectIA agent knowledge.
-- @param ph the probability to hit.
--  @return numeric the distance in meters.
integration.porteeMaxPourTirerSurUnitePosturesReelles = function( eni, ph )
    return DEC_Tir_PorteeMaxPourTirerSurUnitePosturesReelles( eni.source, ph )
end

--- Informs if the agent is in range to engage the given target, regarding the given PH (Probability to Hit).
-- Checks if the target distance is in between maximum and minimum range.
-- The range takes into account the current posture of both agents.
-- This method can only be called by an agent.
-- @param eni a DirectIA agent knowledge.
-- @param ph the probability to hit.
-- @param position the position from which to compute the range.
-- @return boolean returns 'true' if the agent is in range, 'false' otherwise.
integration.niTropPresNiTropLoin = function( eni, ph, position )
    local currentPosition = position or meKnowledge -- if position is not issued, compute the range from current agent's position.

    -- check if eni is not too close.
    local rPorteeMin = DEC_Tir_PorteeMinPourTirerSurUnitePosturesReelles( eni.source, ph )
    local rDistanceAEni = integration.distance( currentPosition, eni)
    local bTropProche = rDistanceAEni < rPorteeMin

    -- check if eni is not too far.
    local rPorteeMax = DEC_Tir_PorteeMaxPourTirerSurUnitePosturesReelles( eni.source, ph )
    local rDistanceAEni = DEC_Geometrie_Distance3D( currentPosition:getPosition(), 0, eni:getPosition(), DEC_ConnaissanceAgent_Altitude( eni.source ) )
    local bTropLoin =  rDistanceAEni > rPorteeMax

    return ( not ( bTropProche or bTropLoin ) )
end

--- Forbid the use of the given types of ammunition. See dotation categories in authoring tool.
-- <ul> Possible values are: 
-- <li> eMunitionClasse_Obus = 0 (shell) </li>
-- <li> eMunitionClasse_MissileSol = 1  (ground to ground missile)</li>
-- <li> eMunitionClasse_MissileAir = 2 (ground to air missile)</li>
-- <li> eMunitionClasse_Mitraille = 3 (bullets)</li> </ul>
-- This method can only be called by an agent.
-- @param munitions the liste of the type to be fordiden
integration.forbidAmmunition = function( munitions )
    DEC_Pion_InterdireMunition( munitions )
end

--- Authorize the use of the given types of ammunition. See dotation categories in authoring tool.
-- <ul> Possible values are:
-- <li> eMunitionClasse_Obus = 0 (shell) </li>
-- <li> eMunitionClasse_MissileSol = 1  (ground to ground missile)</li>
-- <li> eMunitionClasse_MissileAir = 2 (ground to air missile)</li>
-- <li> eMunitionClasse_Mitraille = 3 (bullets)</li> </ul>
-- This method can only be called by an agent.
-- @param munitions munitions list of types to be forbidden
integration.autoriseAmmunition = function( munitions )
    DEC_Pion_AutoriserMunition( munitions )
end

--- Authorize the use of all types of ammunition. See dotation categories in authoring tool.
-- This method can only be called by an agent
integration.autoriseAllAmmunitions = function()
    DEC_Pion_AutoriserToutesMunitions()
end

--- Returns the maximum range of the agent weapon systems allowing to hit with a certain probability.
-- The range returned is computed with regards every volume, and the longest range is returned.
-- Returns 0 if the hit probability cannot be attained. 
-- See 'weapon systems' tab in authoring tool.
-- This method can only be called by an agent.
-- @param ph the probability to hit.
-- @return numeric the distance in meters.
integration.getMaxRangeToFireForPH = function( pH )
    if not pH then -- Scipio compatibility
        if integration.isAgentInsideTown() then
            pH = PH_TIR_ZURB
        else
            pH = PH_TIR_ZO
        end
    end
    return DEC_Tir_PorteeMaxPourTirer( pH )
end

--- Returns the maximum range of the agent weapon systems allowing to hit the given target with a certain probability.
-- Returns 0 if the target cannot be hit at any range. 
-- The returned range does not take into account the current posture of the agent.
-- This method can only be called by an automat.
-- @param agent a simulation agent knowledge.
-- @param ph the probability to hit.
-- @return numeric the distance in meters.
integration.getMaxRangeToFireAgentForPH = function( agent, pH )
    if not pH then -- Scipio compatibility
        local unit = CreateKnowledge(integration.ontology.types.agent, agent)
        if integration.AgentIsInTown( unit ) then
            pH = PH_TIR_ZURB
        else
            pH = PH_TIR_ZO
        end
    end
    return DEC_Tir_PorteeMaxPourTirer( agent, pH )
end

--- Returns the list of agent knowledges that are currently firing at the agent.
-- This method can only be called by an agent.
-- @return table containing the simulation agent knowledges engaging the agent.
integration.getKnowledgesUnitsEngaging = function()
    return DEC_Connaissances_UnitesPrenantAPartie()
end

--- Returns the list of agent knowledges that are currently firing at the given 'friendAgent'.
-- This method can only be called by an agent.
-- @param friendAgent a simulation agent knowledge.
-- @return table containing the agent knowledges engaging the agent.
integration.getKnowledgesUnitsEngagingFriend = function( friendAgent )
    return DEC_Connaissances_UnitesPrenantAPartieSurAmi( friendAgent )
end

--- Returns the list of hostile agent knowledges that are considered 'dangerous' for the agent.
-- A 'dangerous' unit is a unit which has the capability to open fire at the target.
-- Only enemies are returned. 
-- This method can only be called by an agent.
-- @return table the agent knowledges that can engage the agent.
integration.getKnowledgesDangerousUnits = function()
    return DEC_Connaissances_UnitesEnnemiesDangereuses()
end

--- Returns the dangerosity of the given agent knowledge.
-- A 'dangerous' unit is a unit which has the capability to open fire at the target.
-- This method can only be called by an agent.
-- @param agent a simulation agent knowledge.
-- @return numeric, a value between 0 and 1, the dangerosity of the given agent. A value of '0' means not dangerous at all,
-- a value of '1' means very dangerous.
integration.getKnowledgeDangerousAgent = function( agent )
    return DEC_ConnaissanceAgent_Dangerosite( agent )
end

--- Informs whether or not the given agent knowledge is tactically destroyed.
-- An agent is considered as not 'operational' ("tactically destroyed") when all its major equipements are out of order. 
-- @param agent a simulation agent knowledge.
-- @return boolean returns 'true' tactically destroyed, 'false' otherwise.
integration.isAgentTacticallyDestroyed = function( agent )
    return DEC_ConnaissanceAgent_EstDetruitTactique( agent )
end

--- Returns the maximum range (in meters) allowing the agent to open fire at the given agent knowledge, 
-- with a probability to hit superior or equal to the given pH (expressed as a percentage).
-- This method can only be called by an agent.
-- @param agent a simulation agent knowledge.
-- @param pH numeric the probability to hit.
-- @return numeric returns the maximum range distance (in meters).
integration.getMaxRangeToFireOnAgent = function( agent, pH )
    if not pH then -- Scipio compatibility
        if integration.isAgentInsideTown() then
            pH = PH_TIR_ZURB
        else
            pH = PH_TIR_ZO
        end
    end
    return DEC_Tir_PorteeMaxPourTirerSurUnite( agent, pH )
end

--- Returns the maximum range of the agent weapon systems allowing to hit the given target with a certain probability.
-- Returns 0 if the target cannot be hit at any range.
-- This method can only be called by an agent.
-- @param agent a simulation agent knowledge.
-- @param pH numeric the probability to hit.
-- @return numeric returns the maximum range distance (in meters).
integration.getMaxRangeToBeFiriedByAgent = function( agent, pH )
    return DEC_Tir_PorteeMaxPourEtreTireParUnite( agent, pH )
end


--- Estimates the interception position of the given target (using the 'projectileSpeed' parameter) and 
-- returns true if such a position exists and is in range of the weapon systems.
-- This method can only be called by an agent.
-- @param targetUnit a DirectIA agent knowledge.
-- @param pH numeric the probability to hit.
-- @param projectileSpeed The speed of the ammunition in km/h. See weapon systems in the physical model.
-- @return boolean returns true when the agent can open fire, false otherwise.
integration.canBeDestroyedWithMissiles = function( targetUnit, ph, projectileSpeed )
    local integration = integration
    local distanceCouverte = integration.porteeMaxPourTirerSurUnitePosturesReelles( targetUnit, ph )
    local pointInterception = integration.positionInterception( targetUnit, projectileSpeed )
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
