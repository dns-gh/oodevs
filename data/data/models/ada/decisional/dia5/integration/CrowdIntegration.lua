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
    DEC_RC( ... )
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
integration.isKwoledgeInCrowd = function( target )
    return DEC_ConnaissanceAgent_EstDansFoule( target.source )
end

integration.hasReachUrbanBlock = function( target )
    return DEC_Population_HasReachedBlockBorder( target.source )
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

integration.FouleEstSecurisee = function( nonLethalWeaponsAuthorizedFactor, lethalWeaponsAuthorizedFactor )
    local pions = DEC_Connaissances_PionsSecurisant()
    local calculDomination = 0
    for _,pion in pairs( pions ) do
        calculDomination = calculDomination + S_EffetSecurisationSurDomination( DEC_ConnaissanceAgent_RoePopulation( pion ), nonLethalWeaponsAuthorizedFactor, lethalWeaponsAuthorizedFactor )
    end
    if next( pions ) then
        myself.domination.default = myself.domination.default + calculDomination
    else
        myself.domination.default = myself.domination.default + g_NaturalEvolutionDomination
    end
    myself.domination.default = math.min( 1, myself.domination.default )
    myself.domination.default = math.max( myself.domination.default, 0 )
end

-- crowd asks its domination
integration.getMyDomination = function( crowd )
    return DEC_GetDomination( crowd.source )
end

-- unit asks crowd's domination
integration.getDomination = function( entity )
    if masalife.brain.core.class.isOfType( entity, sword.military.world.Crowd) then
        return DEC_KnowledgePopulation_Domination( entity.source ).first
    end
    return nil
end

integration.setDomination = function( domination )
    if masalife.brain.core.class.isOfType( meKnowledge, sword.military.crowd.world.classes.CrowdBody ) then 
        DEC_Population_ChangeEtatDomination( domination )
    end
end

integration.getDangerosityLevel = function( crowd )
    return DEC_ConnaissancePopulation_Dangerosite( crowd.source )*100
end

integration.affinityLevel = function( crowd )
    return (DEC_GetAdhesionPopulation( crowd.source ) + 1 ) *50
end

integration.crowdHasFlow = function( crowd )
    return DEC_ConnaissancePopulation_HasFlow( crowd.source )
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
end

integration.startAgressCrowd = function( self )
    self.actionOnCrowd = DEC_StartAgresserFoule()
    self.attitude = DEC_Population_Attitude()
end

integration.updateAgressCrowd = function( self )
   self.actionOnCrowd = self.actionOnCrowd or {}
   local currentAttitude = DEC_Population_Attitude()
   if currentAttitude ~= self.attitude then
     DEC__StopAction( self.actionOnCrowd )
     self.actionOnCrowd = DEC_StartAgresserFoule()
     self.attitude = currentAttitude
   end
end

integration.stopAgressCrowd = function( self )
    if self.actionOnCrowd then
        self.actionOnCrowd = DEC__StopAction(self.actionOnCrowd)
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

integration.getNearbyConcentration = function ( crowd, loadingPoint, distance )
    --$$$ hard coded 300
    return DEC_GetConcentrationLaPlusProche( myself, crowd.source, loadingPoint.source, 300 )
end

integration.getPositionConcentration = function ( crowd, concentration )
    if concentration == 0 then return {} end
    return CreateKnowledge( sword.military.world.Point, DEC_GetPositionConcentration( myself, crowd.source, concentration ) )
end

integration.getNbPersonConcentration = function ( crowd, concentration )
    return DEC_GetNombrePersonnesDansConcentration( myself, crowd.source, concentration )
end

integration.crowdIsContamined = function ()
    return DEC_GetNombrePersonneContaminee() > 0
end

integration.changeAttitude = function( attitude )
    DEC_Population_ChangerAttitude( attitude )
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
        hiddenAgents[ #hiddenAgents + 1 ] = CreateKnowledge( sword.military.world.PlatoonAlly, simAgent )
    end
    return hiddenAgents
end
