--- Sets the attitude of this crowd to the given attitude.
-- May display a report.
-- This method can only be called by a crowd.
-- @see integration.getAttitude
-- @param newAttitude Integer, the new attitude among one of the following constants :
-- <ul> <li> eAttitudeCalme (quiet) </li>
-- <li> eAttitudeAgitee (agitated) </li>
-- <li> eAttitudeExcitee (excited) </li>
-- <li> eAttitudeAgressive (aggressive) </li> </ul>
integration.setAttitude = function( newAttitude )
    if DEC_Population_Attitude() ~= newAttitude then
        integration.changeAttitude( newAttitude )
        reportFunction( eRC_AttitudePopulation, newAttitude )
    end
end

--- Returns the current attitude of this crowd.
-- This method can only be called by a crowd.
-- @see integration.setAttitude
-- @return Integer, the current attitude among one of the following constants :
-- <ul> <li> eAttitudeCalme (quiet) </li>
-- <li> eAttitudeAgitee (agitated) </li>
-- <li> eAttitudeExcitee (excited) </li>
-- <li> eAttitudeAgressive (aggressive) </li> </ul>
integration.getAttitude = function( )
    return DEC_Population_Attitude()
end

--- Returns true if the given agent is inside this crowd, false otherwise.
-- This method can only be called by a crowd.
-- @param unit Directia agent
-- @return Boolean
integration.unitIsInCrowd = function( unit )
    return DEC_Agent_EstDansFoule( unit.source )
end

--- Makes this crowd display a report with the given id.
-- Any additional parameter will be used as parameters to the report.
-- This method can only be called by a crowd.
-- @param id Integer, the id of the report (see Reports.xml in the physical database).
integration.crowdRC = function ( ... )
    integration.report( ... )
end

--- Returns this crowd's aggressiveness coefficient.
-- This method can only be called by a crowd.
-- @see integration.startAttackingIt
-- @see integration.updateAttackingIt
-- @return Float between 0 and 1, 0 meaning this crowd is
-- quiet and 1 meaning that this crowd is aggressive.
local coeffAgressionForCurrentAttitude = function()
    local attitude =  DEC_Population_Attitude()
    if attitude == eAttitudePopulation_Calme then
        return 0.01 
    elseif attitude == eAttitudePopulation_Agitee then
        return 0.01 
    elseif attitude == eAttitudePopulation_Excitee then
        return 0.33
    elseif attitude == eAttitudePopulation_Agressive then
        return 1
    end
    return nil
end

--- Makes this crowd start attacking the given agent.
-- Continues the current attacking action if another one was started
-- by a previous call to this method but was not stopped by a call
-- to integration.stopAttackingIt .
-- This method can only be called by a crowd.
-- @see integration.updateAttackingIt
-- @see integration.stopAttackingIt
-- @param target Directia agent
-- @return Boolean, false if a current attacking action involving this crowd
-- and the given agent is currently occurring, true otherwise.
integration.startAttackingIt = function( target )
    target[ myself ] = target[ myself ] or {}
    if target[ myself ].actionTir then
        DEC_ReprendAction( target[ myself ].actionTir )
        return false
    end
    target[ myself ].intensity = coeffAgressionForCurrentAttitude()
    if masalife.brain.core.class.isOfType( target, integration.ontology.types.agentKnowledge) then
        local agent = DEC_Connaissance_EnAgent( target.source )
        target[ myself ].actionTir = DEC_StartTirSurPion( target[ myself ].intensity, agent )
    else
        target[ myself ].actionTir = DEC_StartTirSurPion( target[ myself ].intensity, target.source )
    end
    return true
end

--- Makes this crowd continue attacking the given agent.
-- If this crowd has changed its attitude, the current
-- attack action will stop and a new one will start,
-- with a more appropriate aggressivity coefficient.
-- This method can only be called by a crowd.
-- @see integration.startAttackingIt
-- @see integration.stopAttackingIt
-- @param target Directia agent
-- @return Boolean, false if this crowd has a new attitude, true otherwise.
integration.updateAttackingIt = function( target )
    target[myself] = target[myself] or {}
    local currentIntensity = coeffAgressionForCurrentAttitude()
    if currentIntensity ~= target[myself].intensity then
        DEC__StopAction( target[myself].actionTir )
        target[myself].intensity = currentIntensity
        target[myself].actionTir = DEC_StartTirSurPion( currentIntensity, target.source )
        return false
    end
    return true
end


--- Makes this crowd stop attacking the given agent.
-- Stops the current attacking action if there is one.
-- This method can only be called by a crowd.
-- @see integration.startAttackingIt
-- @see integration.updateAttackingIt
-- @param target Directia agent
-- @return Boolean, false if an attacking action has been stopped,
-- true otherwise.
integration.stopAttackingIt = function( target )
    target[myself] = target[myself] or {}
    if target[myself].actionTir then
        DEC__StopAction( target[myself].actionTir )
        target[myself].actionTir = nil
        return false
    end
    return true
end

--- Returns true if this crowd has reached the border of the given urban block, false otherwise.
-- This method can only be called by a crowd.
-- @param urbanBlock Urban block knowledge
-- @return Boolean
integration.hasReachUrbanBlock = function( urbanBlock )
    return DEC_Population_HasReachedBlockBorder( urbanBlock.source )
end

--- Returns true if this crowd has entirely reached the given localized element, false otherwise.
-- For this method to return true, this crowd must not have any flow, must have only one concentration,
-- and must be on the position returned by the "getPosition" method of the given localized element.
-- This method can only be called by a crowd.
-- @param localizedElement Localized element knowledge (area, urban block, object...), defining a
-- "getPosition" method returning a simulation point.
-- @return Boolean
integration.hasEntirelyReachedDestination = function( localizedElement )
    return DEC_Population_HasReachedDestinationCompletely( localizedElement:getPosition() )
end

--- Sets the domination value to the result of integration.FouleEstSecurisee computation.
-- May display reports.
-- This method can only be called by a crowd.
-- @see integration.FouleEstSecurisee
-- @param nonLethalWeaponsAuthorizedFactor Float, the positive multiplicative factor applied on the total effect
-- on the domination value if the controlling units cannot use lethal weapons. The bigger this factor,
-- the more sensitive the crowd is to non-lethal weapons.
-- @param lethalWeaponsAuthorizedFactor Float, the positive multiplicative factor applied on the total effect
-- on the domination value if the controlling units can use lethal weapons. The bigger this factor,
-- the more sensitive the crowd is to lethal weapons.
integration.manageInteractionsWithUnits = function( nonLethalWeaponsAuthorizedFactor, 
                                                    lethalWeaponsAuthorizedFactor )
    myself.domination = myself.domination or {}
    myself.domination.default = myself.domination.default or 1
    
    -- Compute the effect on this crowd's domination value
    integration.FouleEstSecurisee( nonLethalWeaponsAuthorizedFactor, lethalWeaponsAuthorizedFactor )
    
    -- Compute average domination with all interacting units.
    local number = 0
    local finalDomination = 0
    for _, domination in pairs( myself.domination ) do
        finalDomination = finalDomination + domination
        number = number + 1
    end
    finalDomination = finalDomination / number
    integration.setDomination( finalDomination )

    -- report
    if finalDomination == 0 then
        if not myself.reportUnderControl then
            reportFunction( eRC_PopulationTemporairementControlee )
            myself.reportUnderControl = true
        end
    else
        myself.reportUnderControl = nil
    end
end

--- Returns the effect that a single unit (with the given rule of engagement towards crowds) has on
--- the domination value of a crowd when trying to control it.
-- A crowd's domination value is a float value between 0 and 1, 0 meaning
-- the crowd is fully controlled, and 1 meaning it is not controlled at all.
-- This method can only be called by a crowd.
-- @param nonLethalWeaponsAuthorizedFactor Float, the positive multiplicative factor applied on the total effect
-- on the domination value if the controlling units cannot use lethal weapons. The bigger this factor,
-- the more sensitive the crowd is to non-lethal weapons.
-- @param lethalWeaponsAuthorizedFactor Float, the positive multiplicative factor applied on the total effect
-- on the domination value if the controlling units can use lethal weapons. The bigger this factor,
-- the more sensitive the crowd is to lethal weapons.
-- @return Negative float, the computed domination differential (between -0.1 and 0).
function S_EffetSecurisationSurDomination( roe, nonLethalWeaponsAuthorizedFactor, lethalWeaponsAuthorizedFactor )
    local nbrPersonne = DEC_GetNombrePersonne()
    local valeurNominale = -10
    if nbrPersonne ~= 0 then
        valeurNominale = -10 / nbrPersonne
    end
    local attitude = DEC_Population_Attitude()
    
    if attitude == eAttitudePopulation_Calme then
        -- NOTHING
    elseif attitude == eAttitudePopulation_Agitee then
        valeurNominale = valeurNominale / 20
    elseif attitude == eAttitudePopulation_Excitee then
        valeurNominale = valeurNominale / 50
    elseif attitude == eAttitudePopulation_Agressive then
        valeurNominale = valeurNominale / 100
    end
    
    if roe == eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux then
        valeurNominale = valeurNominale * nonLethalWeaponsAuthorizedFactor
    elseif roe == eEtatROEPopulation_ArmesLetalesAutorisees then
        valeurNominale = valeurNominale * lethalWeaponsAuthorizedFactor
    end
    
    return math.max( -0.1, valeurNominale )
end

--- Computes the effect that all the units trying to control this crowd have
--- on the domination value of this crowd.
-- A crowd's domination value is a float value between 0 and 1, 0 meaning
-- the crowd is fully controlled, and 1 meaning it is not controlled at all.
-- If no one is currently trying to control this crowd, then the computed effect on this crowd's
-- domination value will be positive (meaning the crowd will be less controlled than before).
-- When not controlled by units, crowds see their domination values naturally increase.
-- The computed effect is set in the myself.domination.default variable.
-- May display a report.
-- This method can only be called by a crowd.
-- @see integration.manageInteractionsWithUnits
-- @see S_EffetSecurisationSurDomination
-- @param nonLethalWeaponsAuthorizedFactor Float, the positive multiplicative factor applied on the total effect
-- on the domination value if the controlling units cannot use lethal weapons. The bigger this factor,
-- the more sensitive the crowd is to non-lethal weapons.
-- @param lethalWeaponsAuthorizedFactor Float, the positive multiplicative factor applied on the total effect
-- on the domination value if the controlling units can use lethal weapons. The bigger this factor,
-- the more sensitive the crowd is to lethal weapons.
integration.FouleEstSecurisee = function( nonLethalWeaponsAuthorizedFactor, lethalWeaponsAuthorizedFactor )
    local pions = DEC_Connaissances_PionsSecurisant()
    
    -- Reports
    if #pions > 0 then
        if not myself.reportControllingUnits then
            reportFunction( eRC_UnitsTryingToControl )
            myself.reportControllingUnits = true
        end
    else 
        myself.reportControllingUnits = nil 
    end

    -- Domination computation
    local calculDomination = 0
    for i = 1, #pions do
        calculDomination = calculDomination 
                           + S_EffetSecurisationSurDomination( DEC_ConnaissanceAgent_RoePopulation( pions[i] ), 
                                                               nonLethalWeaponsAuthorizedFactor, 
                                                               lethalWeaponsAuthorizedFactor )
    end
    if #pions > 0 then -- units try to control the crowd
        myself.domination.default = myself.domination.default + calculDomination
    else -- no unit control the crowd, domination increase to reach the default value
        myself.domination.default = myself.domination.default + g_NaturalEvolutionDomination
    end
    myself.domination.default = math.max( math.min( 1, myself.domination.default ), 0 )
end

--- Returns the given crowd's domination, or this crowd's domination
--- if no parameter was provided.
-- This method can only be called by a crowd.
-- @param crowd Crowd knowledge (optional)
-- @return Float, the given crowd's domination (or this crowd's domination if no
-- parameter was provided) between 0 and 1 (0 meaning the crowd
-- is fully controlled, 1 meaning it is not controlled at all)
integration.getMyDomination = function( crowd )
    return DEC_GetDomination( crowd and crowd.source or myself )
end

--- Sets the domination of this crowd to the given value.
-- This method can only be called by a crowd.
-- @param domination Float, this crowd's new domination value between 0 and 1
-- (0 meaning the crowd is fully controlled, 1 meaning it is not controlled at all).
integration.setDomination = function( domination ) 
    DEC_Population_ChangeEtatDomination( domination )
end

--- Returns the intensity level of this crowd's protesting.
-- This method can only be called by a crowd.
-- @see integration.startDemonstrate
-- @see integration.updateDemonstrate
-- @return Float between 0 and 1
function S_IntensiteManifestationSurPions()
    local attitude = DEC_Population_Attitude()
    if attitude == eAttitudePopulation_Calme then
        return 0.01
    elseif attitude == eAttitudePopulation_Agitee then
        return 0.01
    elseif attitude == eAttitudePopulation_Excitee then
        return 1
    elseif attitude == eAttitudePopulation_Agressive then
        return 1
    end
    return 0
end

--- Makes this crowd start demonstrating.
-- This method can only be called by a crowd.
-- @see S_IntensiteManifestationSurPions
-- @see integration.updateDemonstrate
-- @see integration.stopDemonstrate
integration.startDemonstrate = function()
    local intensite = S_IntensiteManifestationSurPions()
    meKnowledge.manifIntensity = intensite
    meKnowledge.actionSurPions = DEC__StartTirSurPions( intensite )
    DEC_Population_ChangeDemonstrationState( true )
end

--- Makes this crowd continue demonstrating.
-- If the intensity level of this crowd's protesting has changed,
-- a new demonstrating action is started.
-- This method can only be called by a crowd.
-- @see S_IntensiteManifestationSurPions
-- @see integration.startDemonstrate
-- @see integration.stopDemonstrate
integration.updateDemonstrate = function()
    local intensite = S_IntensiteManifestationSurPions()
    if intensite ~= meKnowledge.manifIntensity then
        meKnowledge.actionSurPions = DEC__StopAction( meKnowledge.actionSurPions )
        meKnowledge.actionSurPions = DEC__StartTirSurPions( intensite )
        meKnowledge.manifIntensity = intensite
    end
end

--- Makes this crowd stop demonstrating.
-- This method can only be called by a crowd.
-- @see integration.startDemonstrate
-- @see integration.updateDemonstrate
integration.stopDemonstrate = function()
    if meKnowledge.actionSurPions then
        meKnowledge.actionSurPions = DEC__StopAction( meKnowledge.actionSurPions )
        meKnowledge.actionSurPions = nil
    end
    DEC_Population_ChangeDemonstrationState( false )
end

--- Returns true if this crowd is contaminated, false otherwise.
-- This method can only be called by a crowd.
-- @return Boolean
integration.crowdIsContamined = function ()
    return DEC_GetNombrePersonneContaminee() > 0
end

--- Makes this crowd damage the given object.
-- This method can only be called by a crowd.
-- @param object Object knowledge
-- @param damageFactor Float between 0 and 1, the damage caused to the given object,
-- 0 meaning no damage and 1 meaning total destruction.
-- @return Integer, 0 if the damaging was successful, 1 otherwise.
integration.damageObject = function( object, damageFactor )
    return DEC_ConnaissanceObjet_Degrader( object.source, damageFactor )
end

--- Returns true if this crowd has a flow, false otherwise.
-- This method can only be called by a crowd.
-- @return Boolean
integration.crowdAgentHasFlow = function()
    return DEC_HasFlow()
end
