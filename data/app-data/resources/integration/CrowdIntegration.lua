-- -------------------------------------------------------------------------------- 
-- Change crowd attitude
-- @author DDA
-- @release 2011-05-23
-- --------------------------------------------------------------------------------

integration.setAttitude = function( eNewAttitude )
    if DEC_Population_Attitude() ~= eNewAttitude then
        integration.changeAttitude( eNewAttitude )
        meKnowledge:RC( eRC_AttitudePopulation, eNewAttitude )
    end
end

-- caller Crowd
integration.getAttitude = function( )
    return DEC_Population_Attitude()
end

integration.unitIsInCrowd = function( unit )
    return DEC_Agent_EstDansFoule( unit.source )
end

-- caller Agent
integration.getAttitudeCrowd = function( crowd )
    return DEC_GetAttitudePopulation( crowd.source )
end

-- -------------------------------------------------------------------------------- 
-- Get Crowd Informations
-- @author LMT
-- @release 2011-05-28
-- --------------------------------------------------------------------------------
integration.getCrowdInformations = function( crowd )
    DEC_ConnaissancePopulation_Reconnaitre( crowd.source )
end

-- -------------------------------------------------------------------------------- 
-- Control crowd
-- @author LMT
-- @release 2011-05-29
-- --------------------------------------------------------------------------------
integration.controlItCrowd = function( crowd )
    DEC_ConnaissancePopulation_Securiser( crowd.source )
end

integration.crowdRC = function ( ... )
    integration.report( ... )
end

-- -------------------------------------------------------------------------------- 
-- Crowd attacking unit
-- @author DDA
-- @release 2011-05-23
-- --------------------------------------------------------------------------------
integration.startAttackingIt = function( target )
   target[myself] = target[myself] or {}
    if target[myself].actionTir then
      DEC_ReprendAction( target[myself].actionTir )
      return false
    end
    target[myself].intensity = coeffAgressionForCurrentAttitude()
    target[myself].actionTir = DEC_StartTirSurPion( target[myself].intensity, target.source )
end

integration.updateAttackingIt = function( target )
   target[myself] = target[myself] or {}
   local currentIntensity = coeffAgressionForCurrentAttitude()
   if currentIntensity ~= target[myself].intensity then
     DEC__StopAction( target[myself].actionTir )
     target[myself].intensity = currentIntensity
     target[myself].actionTir = DEC_StartTirSurPion( currentIntensity, target.source )
     return false
   end
end



integration.stopAttackingIt = function( target )
    target[myself] = target[myself] or {}
    if  target[myself].actionTir then
        DEC__StopAction( target[myself].actionTir )
        target[myself].actionTir = nil
        return false
    end
end

coeffAgressionForCurrentAttitude = function()
    local coeff 
    local attitude =  DEC_Population_Attitude()
    if attitude == eAttitudePopulation_Calme then
        coeff = 0.01 
    elseif attitude == eAttitudePopulation_Agitee then
        coeff = 0.01 
    elseif attitude == eAttitudePopulation_Excitee then
        coeff = 0.33
    elseif attitude == eAttitudePopulation_Agressive then
        coeff = 1
    end
    return coeff
end

integration.fireForbiddenOnCrowd = function( self )
    return integration.getCrowdROEForAgent() == eEtatROEPopulation_EmploiForceInterdit
end

integration.wlrAllowedOnCrowd = function( self )
    return integration.getCrowdROEForAgent() == eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux
end

integration.fireAllowedOnCrowd = function( self )
    return integration.getCrowdROEForAgent() == eEtatROEPopulation_ArmesLetalesAutorisees
end

local ammoClass = {} 
ammoClass[ eEtatROEPopulation_EmploiForceInterdit ] = "alr"
ammoClass[ eEtatROEPopulation_MaintienADistanceParMoyensNonLetaux ] = "alr"
ammoClass[ eEtatROEPopulation_ArmesLetalesAutorisees ] = "mitraille"

-- -------------------------------------------------------------------------------- 
-- Unit engage crowd
-- @author DDA
-- @release 2011-04-28
-- --------------------------------------------------------------------------------
integration.startShootingOnCrowd = function( crowd )
    crowd[myself] = crowd[myself] or {}
    if crowd[myself].actionTir then
      DEC_ReprendAction( crowd[myself].actionTir )
      return false
    end
    crowd[myself].actionTir = DEC__StartTirSurPopulation( crowd.source, ammoClass[ integration.getCrowdROEForAgent() ] )
    actionCallbacks[ crowd[myself].actionTir ] = function( arg ) crowd[myself].eTir = arg end
end
integration.stopShootingOnCrowd = function( crowd )
     crowd[myself] = crowd[myself] or {}
     if  crowd[myself].actionTir then
         crowd[myself].actionTir = DEC__StopAction( crowd[myself].actionTir )
         crowd[myself].actionTir = nil
     end
end

-- -------------------------------------------------------------------------------- 
-- Return true if unit is in crowd
-- @author DDA
-- @release 2011-05-13
-- --------------------------------------------------------------------------------
integration.isInCrowd = function()
    return DEC_Agent_EstDansFoule()
end

-- -------------------------------------------------------------------------------- 
-- Return true the knowledge of unit is in crowd
-- @author DDA
-- @release 2011-05-13
-- --------------------------------------------------------------------------------
integration.isKnowledgeInCrowd = function( target )
    return DEC_ConnaissanceAgent_EstDansFoule( target.source )
end

integration.isKwoledgeInCrowd = integration.isKnowledgeInCrowd -- To ensure backward compatibility

integration.hasReachUrbanBlock = function( target )
    return DEC_Population_HasReachedBlockBorder( target.source )
end

integration.hasReachedDestination = function( point )
    return DEC_Population_HasReachedDestination( point.source )
end

integration.hasEntirelyReachedDestination = function( localizedElement )
    return DEC_Population_HasReachedDestinationCompletely( localizedElement:getPosition() )
end

integration.setDensitePopulationSortante = function( checkpoint, outgoingFlow )
   DEC_ConnaissanceObjet_ChangeDensitePopulationSortante( checkpoint, outgoingFlow / 300 ) -- valeur entre 0.003 et 0.03
end

integration.setNombreIndividuArmeDansFoule = function( crowd, percentageToDesarm )
    DEC_Agent_ChangerNombreIndividuArmeDansFoule( crowd, (100 - percentageToDesarm) / 100 )
end

integration.getCrowds = function()
    return DEC_Connaissances_Populations()
end

-- ******************************************************************************** 
-- CROWD AGENT
-- All methods used to compute force ratio
-- ******************************************************************************** 
-- -------------------------------------------------------------------------------- 
-- Manage interactions with units (compute the local ratio of forces = "domination").
-- --------------------------------------------------------------------------------
integration.manageInteractionsWithUnits = function( nonLethalWeaponsAuthorizedFactor, 
                                                    lethalWeaponsAuthorizedFactor )
    myself.domination = myself.domination or {}
    if not myself.domination.default then
        myself.domination.default = 1
    end
    
    -- Compute domination regarding 
    integration.FouleEstSecurisee( nonLethalWeaponsAuthorizedFactor, lethalWeaponsAuthorizedFactor )
    
    -- Compute average domination with all interacting units.
    local number = 0
    local finalDomination = 0
    for i, domination in pairs( myself.domination ) do
        finalDomination = finalDomination + domination
        number = number + 1
    end
    finalDomination = finalDomination / number
    integration.setDomination( finalDomination )

    -- report
    if finalDomination == 0 then
        if not myself.reportUnderControl then
            meKnowledge:RC( eRC_PopulationTemporairementControlee )
            myself.reportUnderControl = true
        end
    else
        myself.reportUnderControl = nil
    end
end

-- -------------------------------------------------------------------------------- 
-- ROE effects on crowd domination
-- --------------------------------------------------------------------------------
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

-- -------------------------------------------------------------------------------- 
-- Domination management for crowd agent
-- --------------------------------------------------------------------------------
integration.FouleEstSecurisee = function( nonLethalWeaponsAuthorizedFactor, lethalWeaponsAuthorizedFactor )
    local pions = DEC_Connaissances_PionsSecurisant()
    
    -- Reports
    if next( pions ) then
        if not myself.reportControllingUnits then
            meKnowledge:RC( eRC_UnitsTryingToControl )
            myself.reportControllingUnits = true
        end
    else 
        myself.reportControllingUnits = nil 
    end

    -- Domination computation
    local calculDomination = 0
    for _,pion in pairs( pions ) do
        calculDomination = calculDomination 
                           + S_EffetSecurisationSurDomination( DEC_ConnaissanceAgent_RoePopulation( pion ), 
                                                               nonLethalWeaponsAuthorizedFactor, 
                                                               lethalWeaponsAuthorizedFactor )
    end
    if next( pions ) then -- units try to control the crowd
        myself.domination.default = myself.domination.default + calculDomination
    else -- no unit control the crowd, domination increase to reach the default value
        myself.domination.default = myself.domination.default + g_NaturalEvolutionDomination
    end
    myself.domination.default = math.min( 1, myself.domination.default )
    myself.domination.default = math.max( myself.domination.default, 0 )
end


-- ******************************************************************************** 
-- AGENT --> control a crowd
-- All methods used to control crowds.
-- ******************************************************************************** 
-- -------------------------------------------------------------------------------- 
-- Controling crowd
-- --------------------------------------------------------------------------------
integration.startControlCrowd = function( crowd, periodicity, decreaseRate )
    myself.prevDomination   = integration.getDomination( crowd )
    myself.rTempsDebut      = getSimulationTime()
    myself.delayExceeded    = false
    myself.bDerniereChance  = false
    meKnowledge:RC( eRC_MiseEnPlaceControleDeFoule )
end
integration.updateControlCrowd = function( crowd, periodicity, decreaseRate )
    -- periodicity in SECONDE
    -- decreaseRate % per periodicity.
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
            meKnowledge:RC( eRC_SituationDifficilementTenableFaceAPopulation )
        else
            meKnowledge:RC( eRC_CannotControlTheCrowd )
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
            meKnowledge:RC( eRC_CrowdIsUnderControl )
        end
        integration.getCrowdInformations( crowd )
        return true -- returns "true" meaning the action is running.
    end
    myself.hasReport = nil
    return nil
end

-- -------------------------------------------------------------------------------- 
-- Safeguarding on crowd.
-- --------------------------------------------------------------------------------
integration.startInterveneOnCrowd = function( crowd )
    crowd.respond = true
    crowd.rTempsDebut = getSimulationTime()
end

integration.updateInterveneOnCrowd = function( crowd, periodicity )
    if crowd.respond then
        
        meKnowledge:RC( eRC_RiposteContrePopulation, crowd.source )
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

integration.stopInterveneOnCrowd = function( crowd )
    crowd.respond = nil
    crowd.rTempsDebut = nil
    integration.stopShootingOnCrowd( crowd )
end

-- crowd asks its domination
integration.getMyDomination = function( crowd )
    return DEC_GetDomination( crowd.source )
end

-- unit asks crowd's domination
integration.getDomination = function( entity )
    if masalife.brain.core.class.isOfType( entity, integration.ontology.types.population ) then
        return DEC_KnowledgePopulation_Domination( entity.source ).first
    end
    return nil
end

-- for Crowd agent:
integration.setDomination = function( domination ) 
    DEC_Population_ChangeEtatDomination( domination )
end

integration.getDangerosityLevel = function( crowd )
    return DEC_ConnaissancePopulation_Dangerosite( crowd.source )*100
end

integration.affinityLevel = function( crowd )
    return (DEC_GetAdhesionPopulation( crowd.source ) + 1 ) *50
end

integration.crowdHasFlow = function( crowd )
    return DEC_ConnaissancePopulation_HasFlow( myself, crowd.source )
end

-- -------------------------------------------------------------------------------- 
-- Crowd Demonstrates
-- @author EVH
-- @release 2011-05-06
-- $$$ MIA TODO revoir les valeurs, les sortir dans ADN
-- --------------------------------------------------------------------------------
function S_IntensiteManifestationSurPions()
    local returnValue = 0
    local attitude = DEC_Population_Attitude()
    if attitude == eAttitudePopulation_Calme then
        returnValue = 0.01
    elseif attitude == eAttitudePopulation_Agitee then
        returnValue = 0.01
    elseif attitude == eAttitudePopulation_Excitee then
        returnValue = 1
    elseif attitude == eAttitudePopulation_Agressive then
        returnValue = 1
    end
    return returnValue
end

integration.startDemonstrate = function( crowd )
    local intensite = S_IntensiteManifestationSurPions()
    crowd.manifIntensity = intensite
    crowd.actionSurPions = DEC__StartTirSurPions( intensite )
    DEC_Population_ChangeDemonstrationState( true )
end

integration.uptateDemonstrate = function( crowd )
    local intensite = S_IntensiteManifestationSurPions()
    if intensite ~= crowd.manifIntensity then
        crowd.actionSurPions = DEC__StopAction( crowd.actionSurPions )
        crowd.actionSurPions = DEC__StartTirSurPions( intensite )
        crowd.manifIntensity = intensite
    end
end

integration.stopDemonstrate = function( crowd )
    if crowd.actionSurPions then
        crowd.actionSurPions = DEC__StopAction( crowd.actionSurPions )
        crowd.actionSurPions = nil
    end
    DEC_Population_ChangeDemonstrationState( false )
end

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

integration.getNearbyConcentration = function ( crowd, position, distance )
    if not distance then
        distance = 300
    end
    return DEC_GetConcentrationLaPlusProche( myself, crowd.source, position:getPosition(), distance )
end

integration.getPositionConcentration = function ( crowd, concentration )
    if concentration == 0 then return {} end
    return CreateKnowledge( integration.ontology.types.point, DEC_GetPositionConcentration( myself, crowd.source, concentration ) )
end

integration.getCrowdBarycenter = function( crowd )
    return DEC_KnowledgePopulation_GetBarycenter( crowd.source )
end

integration.getNbPersonConcentration = function ( crowd, concentration )
    return DEC_GetNombrePersonnesDansConcentration( myself, crowd.source, concentration )
end

integration.crowdIsContamined = function ()
    return DEC_GetNombrePersonneContaminee() > 0
end

integration.extractVictimsFromCrowd = function( crowd )
    local position = DEC_Geometrie_CalculerPositionSureteAvecPopulation( crowd.source, 0 ) -- /!\ can returns a nil value!
    if not position then
        position = DEC_Agent_Position() -- extract wounded creating a new crowd on my own position
    end
    position = DEC_Geometrie_PositionAleatoireSurCercle( position, 50 )
    return DEC_Crowd_ExtractWoundedFromCrowd( crowd.source, position )
end

integration.extractDeadFromCrowd = function( crowd )
    local position = DEC_Geometrie_CalculerPositionSureteAvecPopulation( crowd.source, 0 ) -- /!\ can returns a nil value!
    if not position then
        position = DEC_Agent_Position() -- extract dead creating a new crowd on my own position
    end
    position = DEC_Geometrie_PositionAleatoireSurCercle( position, 50 )
    return DEC_Crowd_ExtractDeadFromCrowd( crowd.source, position )
end

integration.crowdHasWoundHumans = function( crowd )
    return integration.crowdGetHumansFromAllTypes( crowd )[2] > 0
end

integration.crowdHasOnlyWoundedHumans = function( crowd )
    local humans = integration.crowdGetHumansFromAllTypes( crowd )
    return humans[1] == 0 -- healthy
           and humans[2] > 0 -- wounded
           and humans[3] == 0 -- contaminated
           and humans[4] == 0 -- dead
end

integration.crowdHasDeadHumans = function( crowd )
    return integration.crowdGetHumansFromAllTypes( crowd )[4] > 0
end

integration.crowdHasOnlyDeadHumans = function( crowd )
    local humans = integration.crowdGetHumansFromAllTypes( crowd )
    return humans[1] == 0 -- healthy
           and humans[2] == 0 -- wounded
           and humans[3] == 0 -- contaminated
           and humans[4] > 0 -- dead
end

integration.crowdGetHumansFromAllTypes = function( crowd )
    -- [1] = healthy, [2] = wounded, [3] = contaminated, [4] = dead
    return DEC_Crowd_GetHumansFromAllTypes( crowd.source )
end

integration.healWoundedInCrowd = function( crowd )
    return DEC_Crowd_HealWoundedHumans( crowd.source )
end

integration.getNbWoundedInCrowd = function( crowd )
    return integration.crowdGetHumansFromAllTypes( crowd )[2]
end

-- Crowd point of vue. Only "Crowd" agent can use this function 
integration.damageObject = function( object, damageFactor )
    DEC_ConnaissanceObjet_Degrader( object.source, damageFactor )
end
integration.UnitIsNeutralized = function( unit )
    return unit.source:DEC_Agent_EtatOpsMajeur() == 0
end

integration.UnitIsDead = function( unit )
    return unit.source:DEC_Agent_EstMort()
end

integration.changeAttitude = function( attitude )
    DEC_Population_ChangerAttitude( attitude )
end

integration.canObjectFilterCrowds = function( object )
    return DEC_ObjectKnowledge_HasCapacity( object.source, "population-filter" )
end

integration.getKnowledgesCrowdsEngaging = function()
    return DEC_Connaissances_PopulationsPrenantAPartie()
end

integration.isKnowledgeCrowdInsideArea = function( crowd, area )
    return DEC_ConnaissancePopulation_EstDansZone( crowd, area )
end

integration.lockCrowdKnowledge = function( crowd )
    DEC_ConnaissancePopulation_Verrouiller( crowd.source )
end

integration.unlockCrowdKnowledge = function( crowd )
    DEC_ConnaissancePopulation_Deverrouiller( crowd.source )
end

integration.isKnowledgeCrowdValid = function( crowd )
    return DEC_CrowdKnowledge_IsValid( myself, crowd )
end

integration.startHidingInCrowd = function( crowd )
    DEC_StartHidingInCrowd( crowd.source )
end

integration.stopHidingInCrowd = function( crowd )
    DEC_StopHidingInCrowd( crowd.source )
end

integration.getAgentsHiddenInCrowd = function( crowd )
    local hiddenAgents = {}
    local simAgents = DEC_GetAgentsHiddenInCrowd( crowd.source ) 
    for _, simAgent in pairs( simAgents ) do
        hiddenAgents[ #hiddenAgents + 1 ] = CreateKnowledge( integration.ontology.types.agent, simAgent )
    end
    return hiddenAgents
end

integration.getHeadPosition = function( crowd )
    if DEC_ConnaissancePopulation_HasFlow( myself, crowd.source ) then
        local pointEnTete = DEC_ConnaissancePopulation_PointEnTeteDeFlux( myself, crowd.source )
        crowd.getPointEnTeteResult = crowd.getPointEnTeteResult or pointEnTete
        if DEC_Geometrie_DistanceBetweenPoints( crowd.getPointEnTeteResult,  pointEnTete) > 30 then
            crowd.getPointEnTeteResult = pointEnTete
        end
        return crowd.getPointEnTeteResult
    else
        return integration.getCrowdPosition( crowd )
    end
end

integration.getCrowdPosition = function( crowd )
    if crowd == meKnowledge then
        return DEC_GetPosition()
    else
        local pointLePlusProche = DEC_ConnaissancePopulation_PointPlusProche( crowd.source )
        crowd.getCrowdPositionResult =  crowd.getCrowdPositionResult or pointLePlusProche
        if DEC_Geometrie_DistanceBetweenPoints( pointLePlusProche, crowd.getCrowdPositionResult ) > 30 then
            crowd.getCrowdPositionResult = pointLePlusProche
        end
        return crowd.getCrowdPositionResult
    end
end

integration.crowdAgentHasFlow = function()
    return DEC_HasFlow()
end

--- Return the crowd's brain from a crowd knowledge id
-- @param crowd id of the crowd knowledge for the unit
-- @param unit simulation brain of the unit having the crowd knowledge
-- @author LMT
-- @release 2013-11-22
integration.getKnowledgeBrain = function( unit, crowd )
    return DEC_GetKnowledgeBrain( unit, crowd )
end