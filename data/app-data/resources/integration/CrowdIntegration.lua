--- Returns the current attitude of the given crowd.
-- This method can only be called by an agent.
-- @param crowd Crowd knowledge
-- @return Integer, the current attitude among one of the following constants :
-- <ul> <li> eAttitudeCalme (quiet) </li>
-- <li> eAttitudeAgitee (agitated) </li>
-- <li> eAttitudeExcitee (excited) </li>
-- <li> eAttitudeAgressive (aggressive) </li> </ul>
integration.getAttitudeCrowd = function( crowd )
    return DEC_GetAttitudePopulation( crowd.source )
end

--- Gives this agent additional informations about the given crowd
--- (number of male/female/children, attitude...).
-- This method can only be called by an agent.
-- @param crowd Crowd knowledge
integration.getCrowdInformations = function( crowd )
    DEC_ConnaissancePopulation_Reconnaitre( crowd.source )
end

--- Makes this agent control the given crowd.
-- This method can only be called by an agent.
-- @param crowd Crowd knowledge
integration.controlItCrowd = function( crowd )
    DEC_ConnaissancePopulation_Securiser( crowd.source )
end

--- Returns true if the current rule of engagement towards crowds for this entity
--- is to not use any lethal fire, false otherwise.
-- This method can only be called by an agent.
-- @see integration.getCrowdROEForAgent
-- @return Boolean
integration.fireForbiddenOnCrowd = function()
    return integration.getCrowdROEForAgent() == eEtatROEPopulation_EmploiForceInterdit
end

--- Returns true if the current rule of engagement towards crowds for this entity
--- is to maintain distances with non-lethal fire, false otherwise.
-- This method can only be called by an agent.
-- @see integration.getCrowdROEForAgent
-- @return Boolean
integration.wlrAllowedOnCrowd = function()
    return integration.getCrowdROEForAgent() == eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux
end

--- Returns true if the current rule of engagement towards crowds for this entity
--- is to use lethal fire if necessary, false otherwise.
-- This method can only be called by an agent.
-- @see integration.getCrowdROEForAgent
-- @return Boolean
integration.fireAllowedOnCrowd = function()
    return integration.getCrowdROEForAgent() == eEtatROEPopulation_ArmesLetalesAutorisees
end

local ammoClass = {} 
ammoClass[ eEtatROEPopulation_EmploiForceInterdit ] = "alr"
ammoClass[ eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux ] = "alr"
ammoClass[ eEtatROEPopulation_ArmesLetalesAutorisees ] = "mitraille"

--- Makes this entity start engaging the given crowd.
-- Continues the current engaging action if another one was started
-- by a previous call to this method but was not stopped by a call
-- to integration.stopShootingOnCrowd .
-- This method can only be called by an agent.
-- @see integration.stopShootingOnCrowd
-- @param crowd Crowd knowledge
-- @return Boolean, false if a current engaging action involving this entity
-- and the given crowd is currently occurring, true otherwise.
integration.startShootingOnCrowd = function( crowd )
    crowd[myself] = crowd[myself] or {}
    if crowd[myself].actionTir then
        DEC_ReprendAction( crowd[myself].actionTir )
        return false
    end
    crowd[myself].actionTir = DEC__StartTirSurPopulation( crowd.source, ammoClass[ integration.getCrowdROEForAgent() ] )
    actionCallbacks[ crowd[myself].actionTir ] = function( arg ) crowd[myself].eTir = arg end
    return true
end

--- Makes this entity stop engaging the given crowd.
-- Stops the current engaging action if there is one.
-- This method can only be called by an agent.
-- @see integration.startShootingOnCrowd
-- @param crowd Crowd knowledge
integration.stopShootingOnCrowd = function( crowd )
     crowd[myself] = crowd[myself] or {}
     if  crowd[myself].actionTir then
         crowd[myself].actionTir = DEC__StopAction( crowd[myself].actionTir )
         crowd[myself].actionTir = nil
     end
end

--- Returns true if this agent is inside a crowd, false otherwise.
-- This method can only be called by an agent.
-- @return Boolean
integration.isInCrowd = function()
    return DEC_Agent_EstDansFoule()
end

--- Returns true if the given agent knowledge is inside a crowd, false otherwise.
-- This method can only be called by an agent.
-- @param target Directia agent knowledge
-- @return Boolean
integration.isKnowledgeInCrowd = function( target )
    return DEC_ConnaissanceAgent_EstDansFoule( target.source )
end

--- Returns a list of all crowds known by this entity.
-- This method can only be called by an agent or a company.
-- @return List of simulation crowds
integration.getCrowds = function()
    return DEC_Connaissances_Populations()
end

--- Makes this entity start controlling the given crowd.
-- Displays a report.
-- This method can only be called by an agent.
-- @see integration.updateControlCrowd
-- @param crowd Crowd knowledge
integration.startControlCrowd = function( crowd )
    myself.prevDomination   = integration.getDomination( crowd )
    myself.rTempsDebut      = getSimulationTime()
    myself.delayExceeded    = false
    myself.bDerniereChance  = false
    reportFunction( eRC_MiseEnPlaceControleDeFoule )
end

--- Makes this entity continue controlling the given crowd.
-- The effect caused by this entity upon the given crowd is managed by
-- the integration.manageInteractionsWithUnits method (called by the crowd).
-- This method only adds this entity to the list of entities controlling this crowd,
-- displays reports, and gives information about whether
-- this entity's attempt at controlling the given crowd is working, or
-- if the crowd is out of control.
-- If the domination value has decreased less than the given decrease rate
-- over the last period, then the control fails and the method returns false. 
-- This method checks the decreasing of the domination value with the given periodicity.
-- Once the crowd is completely controlled, it gives additional informations
-- about itself.
-- This method can only be called by an agent.
-- @see integration.startControlCrowd
-- @see integration.controlItCrowd
-- @param crowd Crowd knowledge
-- @param periodicity Integer, the number of seconds between every domination value
-- decrease check.
-- @param decreaseRate Float between 0 and 100, the expected decrease rate for the
-- domination value of the controlled crowd for the controlling to be working.
-- @return Boolean, true if the given crowd's domination value has reached zero (meaning
-- that the crowd has been controlled successfully), false if the controlling has failed,
-- nil otherwise.
integration.updateControlCrowd = function( crowd, periodicity, decreaseRate )
    -- Check if the domination has decreased more than the given  on the period
    if myself.delayExceeded then
        local domination = integration.getDomination( crowd )
        if domination < ( myself.prevDomination - ( decreaseRate / 100 ) ) or domination == 0 then
            myself.prevDomination = domination
            myself.delayExceeded = false
            myself.rTempsDebut = getSimulationTime()
        elseif not myself.bDerniereChance then
            myself.bDerniereChance = true
            myself.prevDomination = domination
            myself.delayExceeded = false
            myself.rTempsDebut = getSimulationTime()
            reportFunction( eRC_SituationDifficilementTenableFaceAPopulation )
        else
            reportFunction( eRC_CannotControlTheCrowd )
            return false -- FAILED
        end
    end
    -- Check periodicity to monitor how the domination is evolving.
    if periodicity <= getSimulationTime() - myself.rTempsDebut then
        myself.delayExceeded = true -- SUCCEED
    end

    integration.controlItCrowd( crowd ) -- add the agent into the list of controlling agents 

    if integration.getDomination( crowd ) == 0 then -- crowd is under control
        if not myself.hasReport then
            myself.hasReport = true
            reportFunction( eRC_CrowdIsUnderControl )
        end
        integration.getCrowdInformations( crowd )
        return true -- returns "true" meaning the action is running.
    end
    myself.hasReport = nil
    return nil
end

--- Starts periodically firing on the given crowd.
-- @param crowd Crowd knowledge
-- @see integration.updateInterveneOnCrowd
-- @see integration.stopInterveneOnCrowd
integration.startInterveneOnCrowd = function( crowd )
    crowd.respond = true
    crowd.rTempsDebut = getSimulationTime()
end

--- Continues periodically firing on the given crowd.
-- This entity will engage the crowd with the given periodicity.
-- This method can only be called by an agent.
-- @see integration.startInterveneOnCrowd
-- @see integration.stopInterveneOnCrowd
-- @param crowd Crowd knowledge
-- @param periodicity Integer, the firing periodicity (in minutes)
-- @return Boolean, true
integration.updateInterveneOnCrowd = function( crowd, periodicity )
    if crowd.respond then
        reportFunction( eRC_RiposteContrePopulation, crowd.source )
        integration.startShootingOnCrowd( crowd )
        crowd.rTempsDebut = getSimulationTime()
        crowd.respond = false
    else
        integration.stopShootingOnCrowd( crowd )
    end
    if ( periodicity * 60 ) <= ( getSimulationTime() - crowd.rTempsDebut ) then
        crowd.respond = true
    end
    return true
end

--- Stops firing on the given crowd.
-- Ceases fire against the given crowd.
-- @see integration.startInterveneOnCrowd
-- @see integration.updateInterveneOnCrowd
-- @param crowd Crowd knowledge
integration.stopInterveneOnCrowd = function( crowd )
    crowd.respond = nil
    crowd.rTempsDebut = nil
    integration.stopShootingOnCrowd( crowd )
end

--- Returns the given crowd's domination.
-- Returns nil if the given entity is not a crowd.
-- This method can only be called by an agent or a company.
-- @param entity Directia knowledge (nominally a crowd one).
-- @return Float, the given crowd's domination between 0 and 1 (0 meaning the crowd
-- is fully controlled, 1 meaning it is not controlled at all); or nil if the "entity" parameter
-- is not a crowd.
integration.getDomination = function( entity )
    if masalife.brain.core.class.isOfType( entity, integration.ontology.types.population ) then
        return DEC_KnowledgePopulation_Domination( entity.source ).first
    end
    return nil
end

--- Returns the dangerosity level of the given crowd.
-- This method can only be called by an agent.
-- @param crowd Crowd knowledge
-- @return Float, the dangerosity level of the given crowd between 0 and 100
integration.getDangerosityLevel = function( crowd )
    return DEC_ConnaissancePopulation_Dangerosite( crowd.source )*100
end

--- Returns the affinity level of the given crowd towards this entity's side.
-- This method can only be called by an agent.
-- @param crowd Crowd knowledge
-- @return Float, the affinity level of the given crowd between 0 and 100.
integration.affinityLevel = function( crowd )
    return ( DEC_GetAdhesionPopulation( crowd.source ) + 1 ) * 50
end

--- Returns true if the given crowd has a flow, false otherwise.
-- This method can only be called by an agent.
-- @param crowd Crowd knowledge
-- @return Boolean
integration.crowdHasFlow = function( crowd )
    return DEC_ConnaissancePopulation_HasFlow( myself, crowd.source )
end

--- Returns true if the given agent is of a civilian type, false otherwise.
-- @param pion Directia agent
-- @return Boolean
integration.startAgressCrowd = function( self )
    self.actionOnCrowd = DEC_StartAgresserFoule()
end

integration.stopAgressCrowd = function( self )
    if self.actionOnCrowd then
        self.actionOnCrowd = DEC__StopAction(self.actionOnCrowd)
        self.actionOnCrowd = nil
    end
end

integration.startAgress = function( self )
    integration.startAgressCrowd( self )
    self.manifIntensity = S_IntensiteManifestationSurPions()
    self.actionSurPions = DEC__StartTirSurPions( self.manifIntensity )
end

integration.stopAgress = function( self )
    integration.stopAgressCrowd( self )
    if self.actionSurPions then
        self.actionSurPions = DEC__StopAction( self.actionSurPions )
        self.actionSurPions = nil
    end
end

integration.isCivilian = function( pion )
    local typePion = DEC_ConnaissanceAgent_GetMilPionType( pion.source )
    if typePion == "Pion Civilian"
    or typePion == "Pion REFUGIE"
    or typePion == "Pion Organization"
    or typePion == "Pion Notable"
    or typePion == "Pion Journalist"
    then
        return true
    else
        return false
    end
end

--- Returns the id of the given crowd's concentration that is the closest to the given position.
-- Only concentrations located under the given distance of the position will be considered.
-- If there is no such concentration, this method returns 0.
-- @param crowd Crowd knowledge
-- @param position Localized element knowledge defining a "getPosition" method returning a simulation point.
-- @param distance Float, the maximum distance between the position and the considered concentration.
-- @return Integer, the id of the closest concentration
integration.getNearbyConcentration = function ( crowd, position, distance )
    distance = distance or 300
    return DEC_GetConcentrationLaPlusProche( myself, crowd.source, position:getPosition(), distance )
end

--- Returns the given crowd's concentration position.
-- If the given concentration id equals 0, then this method will return an empty table instead.
-- @param crowd Crowd knowledge
-- @param concentration Integer, the concentration id
-- @return Point knowledge, or an empty table if the concentration id equals 0
integration.getPositionConcentration = function ( crowd, concentration )
    if concentration == 0 then return {} end
    return CreateKnowledge( integration.ontology.types.point, DEC_GetPositionConcentration( myself, crowd.source, concentration ) )
end

--- Returns the given crowd's barycenter.
-- This method can only be called by an agent or a company.
-- @param crowd Crowd knowledge
-- @return Simulation point
integration.getCrowdBarycenter = function( crowd )
    return DEC_KnowledgePopulation_GetBarycenter( crowd.source )
end

--- Returns the number of humans in the given concentration.
-- @param crowd Crowd knowledge
-- @param concentration Integer, the concentration id
-- @return Integer, the number of humans in the concentration
integration.getNbPersonConcentration = function ( crowd, concentration )
    return DEC_GetNombrePersonnesDansConcentration( myself, crowd.source, concentration )
end

--- Extracts wounded humans from the given crowd.
-- This method creates a concentration on the given position
-- with all the wounded humans of the given crowd.
-- If no position is provided, then the concentration is created instead on a 
-- safe position near the given crowd (i.e. a position at the given distance of
-- the periphery of the given crowd at the opposite of the current danger direction).
-- This method does nothing if there is no wounded humans in the crowd.
-- This method can only be called by an agent.
-- @param crowd Crowd knowledge
-- @param position Simulation point
-- @param distance Float, the distance between the extraction position and the
-- periphery of the given crowd (in meters, optional, 50 by default).
-- @return Boolean, whether or not the extraction occurred successfully.
integration.extractVictimsFromCrowd = function( crowd, position, distance )
    position = position or DEC_Geometrie_CalculerPositionSureteAvecPopulation( crowd.source, 0 ) -- /!\ can returns a nil value!
    if not position then
        position = DEC_Agent_Position() -- extract wounded creating a new crowd on my own position
    end
    position = DEC_Geometrie_PositionAleatoireSurCercle( position, distance or 50 )
    return DEC_Crowd_ExtractWoundedFromCrowd( crowd.source, position )
end

--- Extracts dead humans from the given crowd.
-- This method creates a concentration on the given position
-- with all the dead humans of the given crowd.
-- If no position is provided, then the concentration is created instead on a 
-- safe position near the given crowd (i.e. a position at the given distance of
-- the periphery of the given crowd at the opposite of the current danger direction).
-- This method does nothing if there is no dead humans in the crowd.
-- This method can only be called by an agent.
-- @param crowd Crowd knowledge
-- @param position Simulation point
-- @param distance Float, the distance between the extraction position and the
-- periphery of the given crowd (in meters, optional, 50 by default).
-- @return Boolean, whether or not the extraction occurred successfully.
integration.extractDeadFromCrowd = function( crowd, position, distance )
    position = position or DEC_Geometrie_CalculerPositionSureteAvecPopulation( crowd.source, 0 ) -- /!\ can returns a nil value!
    if not position then
        position = DEC_Agent_Position() -- extract dead creating a new crowd on my own position
    end
    position = DEC_Geometrie_PositionAleatoireSurCercle( position, distance or 50 )
    return DEC_Crowd_ExtractDeadFromCrowd( crowd.source, position )
end

--- Returns true if the given crowd has wounded humans, false otherwise.
-- This method can only be called by an agent or by a company.
-- @see integration.crowdGetHumansFromAllTypes
-- @param crowd Crowd knowledge
-- @return Boolean
integration.crowdHasWoundHumans = function( crowd )
    return integration.crowdGetHumansFromAllTypes( crowd )[2] > 0
end

--- Returns true if the given crowd only has wounded humans, false otherwise.
-- This method can only be called by an agent or by a company.
-- @see integration.crowdGetHumansFromAllTypes
-- @param crowd Crowd knowledge
-- @return Boolean
integration.crowdHasOnlyWoundedHumans = function( crowd )
    local humans = integration.crowdGetHumansFromAllTypes( crowd )
    return humans[1] == 0 -- healthy
           and humans[2] > 0 -- wounded
           and humans[3] == 0 -- contaminated
           and humans[4] == 0 -- dead
end

--- Returns true if the given crowd has dead humans, false otherwise.
-- This method can only be called by an agent or by a company.
-- @see integration.crowdGetHumansFromAllTypes
-- @param crowd Crowd knowledge
-- @return Boolean
integration.crowdHasDeadHumans = function( crowd )
    return integration.crowdGetHumansFromAllTypes( crowd )[4] > 0
end

--- Returns true if the given crowd only has dead humans, false otherwise.
-- This method can only be called by an agent or by a company.
-- @see integration.crowdGetHumansFromAllTypes
-- @param crowd Crowd knowledge
-- @return Boolean
integration.crowdHasOnlyDeadHumans = function( crowd )
    local humans = integration.crowdGetHumansFromAllTypes( crowd )
    return humans[1] == 0 -- healthy
           and humans[2] == 0 -- wounded
           and humans[3] == 0 -- contaminated
           and humans[4] > 0 -- dead
end

--- Returns the composition of the given crowd.
-- This method can only be called by an agent or by a company.
-- @param crowd Crowd knowledge
-- @return List with the following values : 
-- <ul> <li> 1 : number of healthy humans </li>
-- <li> 2 : number of wounded humans </li>
-- <li> 3 : number of contaminated humans </li>
-- <li> 4 : number of dead humans </li> </ul>
integration.crowdGetHumansFromAllTypes = function( crowd )
    return DEC_Crowd_GetHumansFromAllTypes( crowd.source )
end

--- Instantaneously heals all the wounded humans in the crowd (making them healthy).
-- This method can only be called by an agent.
-- @param crowd Crowd knowledge
-- @return Boolean, whether or not the healing was successful
integration.healWoundedInCrowd = function( crowd )
    return DEC_Crowd_HealWoundedHumans( crowd.source )
end

--- Returns the number of wounded humans in the given crowd.
-- This method can only be called by an agent or by a company.
-- @see integration.crowdGetHumansFromAllTypes
-- @param crowd Crowd knowledge
-- @return Integer
integration.getNbWoundedInCrowd = function( crowd )
    return integration.crowdGetHumansFromAllTypes( crowd )[2]
end

--- Returns true if the given object has the capacity "population-filter"
--- defined in the physical database (allowing it to filter crowds), false otherwise.
-- @param object Object knowledge
-- @return Boolean
integration.canObjectFilterCrowds = function( object )
    return DEC_ObjectKnowledge_HasCapacity( object.source, "population-filter" )
end

--- Returns a list of all the crowds that are attacking this entity.
-- This method can only be called by an agent.
-- @return List of simulation crowds
integration.getKnowledgesCrowdsEngaging = function()
    return DEC_Connaissances_PopulationsPrenantAPartie()
end

--- Returns true if the given crowd is in the provided area, false otherwise.
-- This method can only be called by an agent or by a company.
-- @param crowd Simulation crowd
-- @param area Simulation area
-- @preturn Boolean
integration.isKnowledgeCrowdInsideArea = function( crowd, area )
    return DEC_ConnaissancePopulation_EstDansZone( crowd, area )
end

--- Locks the given crowd knowledge in order to guarantee that
--- it will not be forgotten by this entity, as long as the knowledge
--- remains locked (i.e. until the integration.unlockCrowdKnowledge
--- method is called on the given crowd).
-- This method is most notably used for crowd transporting, because
-- a transported crowd is not perceived and its knowledge may be forgotten.
-- This method can only be called by an agent.
-- @see integration.unlockCrowdKnowledge
-- @param crowd Crowd knowledge
integration.lockCrowdKnowledge = function( crowd )
    DEC_ConnaissancePopulation_Verrouiller( crowd.source )
end

--- Unlocks the given crowd knowledge in order to allow again its
--- potential loss by this entity.
-- This method does nothing if the given crowd knowledge was not
-- previously locked by a call to the integration.lockCrowdKnowledge method.
-- This method can only be called by an agent.
-- @see integration.lockCrowdKnowledge
-- @param crowd Crowd knowledge
integration.unlockCrowdKnowledge = function( crowd )
    DEC_ConnaissancePopulation_Deverrouiller( crowd.source )
end

--- Returns true if the given crowd knowledge is valid, false otherwise.
-- @param crowd Simulation crowd
-- @return Boolean
integration.isKnowledgeCrowdValid = function( crowd )
    return DEC_CrowdKnowledge_IsValid( myself, crowd )
end

--- Makes this entity start hiding in the given crowd.
-- This entity will be among the returned entities of
-- the integration.getAgentsHiddenInCrowd method as
-- long as this entity remains hidden in the given crowd
-- (i.e. until the integration.stopHidingInCrowd method
-- is called).
-- This method can only be called by an agent.
-- @see integration.stopHidingInCrowd
-- @see integration.getAgentsHiddenInCrowd
-- @param crowd Crowd knowledge
integration.startHidingInCrowd = function( crowd )
    DEC_StartHidingInCrowd( crowd.source )
end

--- Makes this entity stop hiding in the given crowd.
-- This method can only be called by an agent.
-- @see integration.startHidingInCrowd
-- @param crowd Crowd knowledge
integration.stopHidingInCrowd = function( crowd )
    DEC_StopHidingInCrowd( crowd.source )
end

--- Returns a list of all the agents hidden in the given crowd.
-- This method can only be called by an agent.
-- @param crowd Crowd knowledge
-- @return List of directia agents
integration.getAgentsHiddenInCrowd = function( crowd )
    local hiddenAgents = {}
    local simAgents = DEC_GetAgentsHiddenInCrowd( crowd.source ) 
    for i = 1, #simAgents do
        hiddenAgents[ #hiddenAgents + 1 ] = CreateKnowledge( integration.ontology.types.agent, simAgents[i] )
    end
    return hiddenAgents
end

--- Returns the given crowd's head position.
-- If the given crowd has several flows, the position of the first created flow's head is returned.
-- If the given crowd has no flow, then this method will return this crowd's position.
-- Otherwise, this method will return the position at the head of this crowd's flow
-- (or the position returned at the previous call of this method, if it is located
-- at less than the given refresh distance from the head of this crowd's flow).
-- This method can only be called by an agent.
-- @param crowd Crowd knowledge
-- @param refreshDistance Float (optional), the minimum distance between the head of
-- the given crowd's flow and the previously returned position to ensure that
-- the newly returned position is the current head position (distance in meters, 30 by default).
-- @return Simulation position
integration.getHeadPosition = function( crowd, refreshDistance )
    if DEC_ConnaissancePopulation_HasFlow( myself, crowd.source ) then
        refreshDistance = refreshDistance or 30
        local pointEnTete = DEC_ConnaissancePopulation_PointEnTeteDeFlux( myself, crowd.source )
        if not crowd.getPointEnTeteResult then
            crowd.getPointEnTeteResult = pointEnTete
        elseif DEC_Geometrie_DistanceBetweenPoints( crowd.getPointEnTeteResult,  pointEnTete ) > refreshDistance then
            crowd.getPointEnTeteResult = pointEnTete
        end
        return crowd.getPointEnTeteResult
    else
        return integration.getCrowdPosition( crowd )
    end
end

--- Returns the given crowd's position.
-- If this method is called by an agent, this method will return the closest
-- position to the given crowd (or the position returned at the previous call of
-- this method, if it is located at less that the given refresh distance from this
-- crowd's position).
-- This method can only be called by an agent or by a crowd.
-- @param crowd Crowd knowledge
-- @param refreshDistance Float (optional), the minimum distance between this crowd's position
-- and the previously returned position to ensure that the newly returned position
-- is the current crowd's position (distance in meters, 30 by default).
-- @return Simulation position
integration.getCrowdPosition = function( crowd, refreshDistance )
    if crowd == meKnowledge then
        return DEC_GetPosition()
    else
        refreshDistance = refreshDistance or 30
        local pointLePlusProche = DEC_ConnaissancePopulation_PointPlusProche( crowd.source )
        crowd.getCrowdPositionResult =  crowd.getCrowdPositionResult or pointLePlusProche
        if DEC_Geometrie_DistanceBetweenPoints( pointLePlusProche, crowd.getCrowdPositionResult ) > refreshDistance then
            crowd.getCrowdPositionResult = pointLePlusProche
        end
        return crowd.getCrowdPositionResult
    end
end

--- Return the crowd's brain from a crowd knowledge id
-- @param crowd Simulation crowd
-- @param unit Simulation agent currently having the crowd knowledge
-- @return Simulation crowd brain
integration.getKnowledgeBrain = function( unit, crowd )
    return DEC_GetKnowledgeBrain( unit, crowd )
end

------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

--- Deprecated
integration.isKwoledgeInCrowd = integration.isKnowledgeInCrowd


--- Deprecated
integration.hasReachedDestination = function( point )
    return DEC_Population_HasReachedDestination( point.source )
end

--- Deprecated
integration.setDensitePopulationSortante = function( checkpoint, outgoingFlow )
   DEC_ConnaissanceObjet_ChangeDensitePopulationSortante( checkpoint, outgoingFlow / 300 ) -- valeur entre 0.003 et 0.03
end

--- Deprecated
integration.setNombreIndividuArmeDansFoule = function( crowd, percentageToDesarm )
    DEC_Agent_ChangerNombreIndividuArmeDansFoule( crowd, (100 - percentageToDesarm) / 100 )
end

--- Deprecated
integration.uptateDemonstrate = integration.updateDemonstrate

--- Deprecated, use integration.setAttitude instead
integration.changeAttitude = function( attitude )
    DEC_Population_ChangerAttitude( attitude )
end

--- Deprecated
-- Allows the unit to channel crowd in an area
-- @param area Area knowledge
integration.channelIt = function ( area )
    DEC_Agent_CanaliserPopulation( area.source )
end
