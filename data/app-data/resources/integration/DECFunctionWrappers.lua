
--
-- Actions
--
function DEC__StopAction( actionId )
    return _DEC__StopAction( myself, actionId )
end

function DEC_PauseAction( actionId )
    _DEC_PauseAction( myself, actionId )
end

function DEC_ReprendAction( actionId )
    _DEC_ReprendAction( myself, actionId )
end

function DEC_EtatAction( actionId )
    return _DEC_EtatAction( myself, actionId )
end

--
-- Debug
--
function BreakForDebug( message )
    _BreakForDebug( myself, message )
end

function DEC_DebugAffichePoint( point )
    _DEC_DebugAffichePoint( myself, point )
end

function DEC_DebugAffichePoints( points )
    _DEC_DebugAffichePoints( myself, points )
end

function DEC_Debug( message )
    _DEC_Debug( myself, message )
end

function DEC_Trace( message )
    -- Make sure we do nothing if this function is called while the brain is
    -- under construction (e.g., by 'LogMessage')
    -- As soon as _DEC_Trace is defined, we redefine this function for all
    -- subsequent calls
    if _DEC_Trace then
        DEC_Trace = function ( message )
            _DEC_Trace( myself, message )
        end
        DEC_Trace( message )
    end
end

function DEC_DecisionalState( key, value )
    _DEC_DecisionalState( myself, key, value )
end

--
-- Representations
--
function DEC_GetOrdersCategory()
    return _DEC_GetOrdersCategory( myself )
end

function DEC_GetPointsCategory()
    return _DEC_GetPointsCategory( myself )
end

function DEC_RemoveFromOrdersCategory( fragOrder )
    return _DEC_RemoveFromOrdersCategory( myself, fragOrder )
end

function DEC_DeleteRepresentation( fragOrder )
    _DEC_DeleteRepresentation( myself, fragOrder )
end

function DEC_RemoveFromPointsCategory( pathPoint )
    _DEC_RemoveFromPointsCategory( myself, pathPoint )
end

--
-- Orders
--
function DEC_FillMissionParameters( missionRef, mission )
    _DEC_FillMissionParameters( myself, InitTaskParameter, missionRef, mission )
end

function DEC_FinMission()
    _DEC_FinMission( myself )
end

function DEC_NouvelleMission()
    return _DEC_NouvelleMission( myself )
end

function DEC_GetLima( limaId )
    return _DEC_GetLima( myself, limaId )
end

function DEC_GetLimasFromType( limaId )
    return _DEC_GetLimasFromType( myself, limaId )
end

function DEC_ProchaineLimaHoraireNonFlagee()
    return _DEC_ProchaineLimaHoraireNonFlagee( myself )
end

function DEC_Fuseau()
    return _DEC_Fuseau( myself )
end

function DEC_SetMissionLimaFlag( limaId, flag )
    _DEC_SetMissionLimaFlag( myself, limaId, flag )
end

function DEC_GetMissionLimaFlag( limaId )
    return _DEC_GetMissionLimaFlag( myself, limaId )
end

function DEC_SetMissionLimaFlagHoraire( limaId, flag )
    _DEC_SetMissionLimaFlagHoraire( myself, limaId, flag )
end

function DEC_MRT_CreerMissionPion( pion, mission )
    -- Automaton
    return _DEC_MRT_CreerMissionPion( myself, pion, mission )
end

function DEC_MRT_Valide()
    -- Automaton
    _DEC_MRT_Valide( myself )
end

function DEC_MRT_AffecteFuseaux( pions )
    -- Automaton
    return _DEC_MRT_AffecteFuseaux( myself, pions )
end

function DEC_CDT_CreerMissionPion( pion, mission )
    -- Automaton
    return _DEC_CDT_CreerMissionPion( myself, pion, mission )
end

function DEC_CDT_DonnerMissionPion_Mission( mission )
    -- Automaton
    _DEC_CDT_DonnerMissionPion_Mission( myself, mission )
end

function DEC_CreerMissionAutomate( automate, mission )
    -- Automaton
    return _DEC_CreerMissionAutomate( myself, automate, mission )
end

function DEC_DonnerMissionAutomate_Mission( mission )
    -- Automaton
    _DEC_DonnerMissionAutomate_Mission( myself, mission )
end

function DEC_DonnerMissionAutomate( mission )
    -- Automaton
    _DEC_DonnerMissionAutomate( myself, mission )
end

function DEC_DecouperFuseau( nbrSubFuseaux )
    return _DEC_DecouperFuseau( myself, nbrSubFuseaux )
end

--
-- Knowledge objects
--
function DEC_ObjectKnowledgesInZone( localization, parameters )
    return _DEC_ObjectKnowledgesInZone( myself, localization, parameters )
end

--
-- Knowledge
--
function DEC_Connaissances_PartageConnaissancesAvec( receiver, minutes )
    _DEC_Connaissances_PartageConnaissancesAvec( myself, receiver, minutes )
end

function DEC_Connaissances_PartageConnaissancesDansZoneAvec( receiver, center, radius )
    _DEC_Connaissances_PartageConnaissancesDansZoneAvec( myself, receiver, center, radius )
end

function DEC_Knowledges_ObjectsInCircle( center, radius, filters )
    return _DEC_Knowledges_ObjectsInCircle( myself, center, radius, filters, false )
end

function DEC_Knowledges_AllObjectsInCircle( center, radius, filters )
    return _DEC_Knowledges_ObjectsInCircle( myself, center, radius, filters, true )
end

function DEC_Connaissances_ObjetsDansFuseau( type )
    return _DEC_Connaissances_ObjetsDansFuseau( myself, type )
end

function DEC_Connaissances_UnitesEnnemiesVivantesPercuesParPion( perceiver )
    return _DEC_Connaissances_UnitesEnnemiesVivantesPercuesParPion( myself, perceiver )
end

function DEC_Connaissances_Populations()
    return _DEC_Connaissances_Populations( myself )
end

--
-- Object
--
function DEC_CreerObjetSansDelais( type, localization )
    return _DEC_CreerObjetSansDelais( myself, type, localization )
end

function DEC_MagicGetOrCreateObject( type, localization )
    return _DEC_MagicGetOrCreateObject( myself, type, localization )
end

--
-- Population knowledge
--
function DEC_KnowledgePopulation_Domination( knowledgeId )
    return _DEC_KnowledgePopulation_Domination( myself, knowledgeId )
end

function DEC_ConnaissancePopulation_EstDansZone( knowledgeId, localization )
    return _DEC_ConnaissancePopulation_EstDansZone( myself, knowledgeId, localization )
end

function DEC_KnowledgePopulation_GetBarycenter( knowledgeId )
    return _DEC_KnowledgePopulation_GetBarycenter( myself, knowledgeId )
end

function DEC_Crowd_GetHumansFromAllTypes( knowledgeId )
    return _DEC_Crowd_GetHumansFromAllTypes( myself, knowledgeId )
end

--
-- Agent knowledge
--
function DEC_KnowledgeAgent_IsPerceptionLevelMax( agentKnowledge )
    return _DEC_KnowledgeAgent_IsPerceptionLevelMax( myself, agentKnowledge )
end

--
-- Geometry
--
function DEC_Geometrie_CalculerBarycentreLocalisationDansFuseau( localization )
    return _DEC_Geometrie_CalculerBarycentreLocalisationDansFuseau( myself, localization )
end

function DEC_Geometry_SplitLocalisation( localization, numParts, direction )
    return DEC_Geometry_Pion_SplitLocalisation( myself, localization, numParts, direction )
end

function DEC_Geometrie_DecoupeFuseauEnTroncons( sectionLength )
    return _DEC_Geometrie_DecoupeFuseauEnTroncons( myself, sectionLength )
end

function DEC_Geometrie_CalculerPositionObstacle( center, type, radius )
    return _DEC_Geometrie_CalculerPositionObstacle( myself, center, type, radius )
end

function DEC_Geometrie_CalculerPointArrivee()
    return _DEC_Geometrie_CalculerPointArrivee( myself )
end

function DEC_Geometrie_CalculerPointDepart()
    return _DEC_Geometrie_CalculerPointDepart( myself )
end

function DEC_Geometrie_EstPointDansFuseau( point )
    return _DEC_Geometrie_EstPointDansFuseau( myself, point )
end

function DEC_Geometrie_CalculerPositionParRapportALima( phaseLine, distanceBefore )
    return _DEC_Geometrie_CalculerPositionParRapportALima( myself, phaseLine, distanceBefore )
end

function DEC_Geometrie_CalculerPositionParRapportALimaDansFuseau( limaId, distanceBeforeLima, fuseau )
    return _DEC_Geometrie_CalculerPositionParRapportALimaDansFuseau( myself, limaId, distanceBeforeLima, fuseau )
end

function DEC_Geometrie_CalculerPositionsParRapportALima( limaId, distanceBeforeLima, numPoints )
    return _DEC_Geometrie_CalculerPositionsParRapportALima( myself, limaId, distanceBeforeLima, numPoints )
end

function DEC_Geometrie_StartCalculLignesAvantEtArriere( pionDecisionList )
    -- Automaton
    return _DEC_Geometrie_StartCalculLignesAvantEtArriere( myself, pionDecisionList )
end

function DEC_Geometrie_StartCalculAutomateLignesAvantEtArriere( automatDecisionList )
    -- Automaton
    return _DEC_Geometrie_StartCalculAutomateLignesAvantEtArriere( myself, automatDecisionList )
end

function DEC_Geometrie_CalculerPointArriveePourPion( pion )
    -- Automaton
    return _DEC_Geometrie_CalculerPointArriveePourPion( myself, pion )
end

function DEC_Geometrie_PosDeploiementASAOmni( positionCount, center, radius )
    return _DEC_Geometrie_PosDeploiementASAOmni( myself, positionCount, center, radius )
end

function DEC_Geometrie_CalculerPointProcheLocalisationDansFuseau( location )
    -- Agent & Automaton
    return _DEC_Geometrie_CalculerPointProcheLocalisationDansFuseau( myself, location )
end

function DEC_Geometrie_CalculerPointProcheLocalisationNonClippeeDansFuseau( location )
    return _DEC_Geometrie_CalculerPointProcheLocalisationNonClippeeDansFuseau( myself, location )
end

function DEC_Geometrie_CalculerRetardSurObjectifs( fuseau, automates, objectives )
    return _DEC_Geometrie_CalculerRetardSurObjectifs( myself, fuseau, automates, objectives )
end

function DEC_Geometrie_AdvanceAlongFuseau( pion )
    return _DEC_Geometrie_AdvanceAlongFuseau( myself, pion )
end

function DEC_Geometrie_GetPointsLimas( limaType, divider )
    return _DEC_Geometrie_GetPointsLimas( myself, limaType, divider )
end

function DEC_Geometrie_CalculerPositionAppui( agentToSupport, distance )
    return _DEC_Geometrie_CalculerPositionAppui( myself, agentToSupport, distance )
end

function DEC_Geometrie_CalculerPositionEmbuscade( ambushPosition, retreatPosition, distance )
    return _DEC_Geometrie_CalculerPositionEmbuscade( myself, ambushPosition, retreatPosition, distance )
end

function DEC_Geometrie_CalculerPositionSurete( enemyKnowledge, minDistance )
    return _DEC_Geometrie_CalculerPositionSurete( myself, enemyKnowledge, minDistance )
end

function DEC_Geometrie_CalculerPositionSureteAvecPopulation( populationKnowledgeId, minDistance )
    return _DEC_Geometrie_CalculerPositionSureteAvecPopulation( myself, populationKnowledgeId, minDistance )
end

function DEC_Geometrie_CalculerPositionSureteAvecObjectif( enemyKnowledge, minDistance, objective )
    return _DEC_Geometrie_CalculerPositionSureteAvecObjectif( myself, enemyKnowledge, minDistance, objective )
end

function DEC_Geometrie_CalculerEntreeFuseauLaPlusProche()
    return _DEC_Geometrie_CalculerEntreeFuseauLaPlusProche( myself )
end

function DEC_Geometry_SplitLocalisationSurface( localization, averageArea, direction )
    return _DEC_Geometry_SplitLocalisationSurface( myself, localization, averageArea, direction )
end

function DEC_Geometry_RecursiveSplitLocalisationSurface( localization, averageArea )
    return _DEC_Geometry_RecursiveSplitLocalisationSurface( myself, localization, averageArea )
end

function DEC_Geometrie_CalculerTrafficableBarycentreLocalisation( localization )
    return _DEC_Geometrie_CalculerTrafficableBarycentreLocalisation( myself, localization )
end

function DEC_IsPointInUrbanBlockTrafficable( point )
    return DEC_IsPointInUrbanBlockTrafficableForPlatoon( myself, point )
end

function DEC_Geometrie_GetLeavingAreaPosition( localization )
    return _DEC_Geometrie_GetLeavingAreaPosition( myself, localization )
end

--
-- Logistics
--
function DEC_Maintenance_ActiverChaine()
    -- Agent & Automaton
    _DEC_Maintenance_ActiverChaine( myself )
end

function DEC_Maintenance_DesactiverChaine()
    -- Agent & Automaton
    _DEC_Maintenance_DesactiverChaine( myself )
end

function DEC_Maintenance_ChangerPriorites( priorities )
    -- Agent & Automaton
    _DEC_Maintenance_ChangerPriorites( myself, priorities )
end

function DEC_Maintenance_Priorites()
    -- Automaton
    return _DEC_Maintenance_Priorites( myself )
end

function DEC_Maintenance_ChangerPrioritesTactiques( priorities )
    -- Agent & Automaton
    _DEC_Maintenance_ChangerPrioritesTactiques( myself, priorities )
end

function DEC_Maintenance_PrioritesTactiques()
    -- Automaton
    return _DEC_Maintenance_PrioritesTactiques( myself )
end

function DEC_Maintenance_ChangerRegimeTravail( workRate )
    -- Agent & Automaton
    _DEC_Maintenance_ChangerRegimeTravail( myself, workRate )
end

function DEC_Maintenance_RegimeTravail()
    -- Automaton
    return _DEC_Maintenance_RegimeTravail( myself )
end

function DEC_Sante_ActiverChaine()
    -- Agent & Automaton
    _DEC_Sante_ActiverChaine( myself )
end

function DEC_Sante_DesactiverChaine()
    -- Agent & Automaton
    _DEC_Sante_DesactiverChaine( myself )
end

function DEC_Sante_ActiverFonctionTri()
    -- Agent & Automaton
    _DEC_Sante_ActiverFonctionTri( myself )
end

function DEC_Sante_DesactiverFonctionTri()
    -- Agent & Automaton
    _DEC_Sante_DesactiverFonctionTri( myself )
end

function DEC_Sante_ActiverFonctionSoin()
    -- Agent & Automaton
    _DEC_Sante_ActiverFonctionSoin( myself )
end

function DEC_Sante_DesactiverFonctionSoin()
    -- Agent & Automaton
    _DEC_Sante_DesactiverFonctionSoin( myself )
end

function DEC_Sante_ChangerPriorites( priorities )
    -- Agent & Automaton
    _DEC_Sante_ChangerPriorites( myself, priorities )
end

function DEC_Sante_Priorites()
    -- Automaton
    return _DEC_Sante_Priorites( myself )
end

function DEC_Sante_ChangerPrioritesTactiques( priorities )
    -- Agent & Automaton
    _DEC_Sante_ChangerPrioritesTactiques( myself, priorities )
end

function DEC_Sante_PrioritesTactiques()
    -- Automaton
    return _DEC_Sante_PrioritesTactiques( myself )
end

function DEC_Ravitaillement_ActiverChaine()
    -- Agent & Automaton
    _DEC_Ravitaillement_ActiverChaine( myself )
end

function DEC_Ravitaillement_DesactiverChaine()
    -- Agent & Automaton
    _DEC_Ravitaillement_DesactiverChaine( myself )
end

--
-- Automaton
--

-- Accessors
function DEC_Automate_PionsAvecPC()
    return myself:DEC_Automate_PionsAvecPC()
end

function DEC_Automate_PionsSansPC()
    return _DEC_Automate_PionsSansPC( myself )
end

function DEC_Automate_PionPC()
    return _DEC_Automate_PionPC( myself )
end

function DEC_Automate_AutomatesSubordonnes()
    return _DEC_Automate_AutomatesSubordonnes( myself )
end

function DEC_Automate_PionsMelee()
    return _DEC_Automate_PionsMelee( myself )
end

function DEC_Automate_PionsGenie()
    return _DEC_Automate_PionsGenie( myself )
end

function DEC_AutomateSuperieur_EstEmbraye()
    return _DEC_AutomateSuperieur_EstEmbraye( myself )
end

function DEC_Automate_PionsAvecPCCommunication()
    return _DEC_Automate_PionsAvecPCCommunication( myself )
end

function DEC_Automate_PionsSansPCCommunication()
    return _DEC_Automate_PionsSansPCCommunication( myself )
end

-- State
function DEC_Automate_EstEmbraye()
    return myself:DEC_Automate_EstEmbraye()
end

function DEC_Automate_ROE()
    return _DEC_Automate_ROE( myself )
end

function DEC_Automate_ChangeEtatROE( state )
    _DEC_Automate_ChangeEtatROE( myself, state )
end

function DEC_Automate_ChangeEtatROEPopulation( state )
    _DEC_Automate_ChangeEtatROEPopulation( myself, state )
end

function DEC_Automate_ROEPopulation()
    return _DEC_Automate_ROEPopulation( myself )
end

function DEC_Automate_isLogistic()
    return _DEC_Automate_isLogistic( myself )
end

-- Agent accessors
function DEC_Automate_PionEstContamine( pion )
    return _DEC_Automate_PionEstContamine( myself, pion )
end

function DEC_Automate_PionEstEmpoisonne( pion )
    return _DEC_Automate_PionEstEmpoisonne( myself, pion )
end

function DEC_Automate_PionEstNeutralise( pion )
    return _DEC_Automate_PionEstNeutralise( myself, pion )
end

function DEC_Automate_PionEstTransporte( pion )
    return _DEC_Automate_PionEstTransporte( myself, pion )
end

function DEC_Automate_PionRelevePion( relieving, relieved )
    return _DEC_Automate_PionRelevePion( myself, relieving, relieved )
end

function DEC_Automate_PionPeutReleverPion( relieving, relieved )
    return _DEC_Automate_PionPeutReleverPion( myself, relieving, relieved )
end

function DEC_Automate_EstPointDansFuseauPion( point, pion )
    return _DEC_Automate_EstPointDansFuseauPion( myself, point, pion )
end

function DEC_Automate_PionPeutConstruireObjet( pion, type )
    return _DEC_Automate_PionPeutConstruireObjet( myself, pion, type )
end

function DEC_Automate_PionPeutConstruireObjetAvecLocalisation( pion, type, localization )
    return _DEC_Automate_PionPeutConstruireObjetAvecLocalisation( myself, pion, type, localization )
end

function DEC_Automate_PionPeutConstruireContournementObjet( pion, knowledgeId )
    return _DEC_Automate_PionPeutConstruireContournementObjet( myself, pion, knowledgeId )
end

function DEC_Automate_PionPeutDetruireObjet( pion, knowledgeId )
    return _DEC_Automate_PionPeutDetruireObjet( myself, pion, knowledgeId )
end

function DEC_Automate_PionPeutDevaloriserObjet( pion, knowledge )
    return _DEC_Automate_PionPeutDevaloriserObjet( myself, pion, knowledge )
end

-- Logistics
function DEC_Automate_PcDeTC2()
    return _DEC_Automate_PcDeTC2( myself )
end

function DEC_DemandeDeRavitaillement()
    _DEC_DemandeDeRavitaillement( myself )
end


--
-- Crowd
--

-- Self
function DEC_GetPosition()
    return _DEC_GetPosition( myself )
end

function DEC_GetNombrePersonne()
    return _DEC_GetNombrePersonne( myself )
end

function DEC_GetNombrePersonneContaminee()
    return _DEC_GetNombrePersonneContaminee( myself )
end

-- Security
function DEC_GetUrbanBlockAngriness()
    return _DEC_GetUrbanBlockAngriness( myself )
end

function DEC_ReintegrateUrbanBlock()
    _DEC_ReintegrateUrbanBlock( myself )
end

function DEC_Population_HealWounded()
    _DEC_Population_HealWounded( myself )
end

-- Move
function DEC_HasFlow()
    return _DEC_HasFlow( myself )
end

function DEC_Population_HasReachedBlockBorder( urbanKnowledge )
    return _DEC_Population_HasReachedBlockBorder( myself, urbanKnowledge )
end

function DEC_Population_HasReachedDestination( destination )
    return _DEC_Population_HasReachedDestination( myself, destination )
end

function DEC_Population_HasReachedDestinationCompletely( destination )
    return _DEC_Population_HasReachedDestinationCompletely( myself, destination )
end

-- Decisional states
function DEC_Population_ChangeEtatDomination( dominationState )
    _DEC_Population_ChangeEtatDomination( myself, dominationState )
end

function DEC_Population_Morts()
    return _DEC_Population_Morts( myself )
end

-- Effets
function DEC_Population_RalentissementPion_ChangeVitesse( speed )
    _DEC_Population_RalentissementPion_ChangeVitesse( myself, speed )
end

function DEC_Population_RalentissementPion_VitesseParDefaut()
    _DEC_Population_RalentissementPion_VitesseParDefaut( myself )
end

function DEC_Population_ChangerAttitude( attitudeId )
    _DEC_Population_ChangerAttitude( myself, attitudeId )
end

function DEC_Population_Attitude()
    return _DEC_Population_Attitude( myself )
end

function DEC_Population_Positions()
    return _DEC_Population_Positions( myself )
end

function DEC_Population_ChangeUrbanDestructionState( state )
    _DEC_Population_ChangeUrbanDestructionState( myself, state )
end

function DEC_Population_UrbanDestructionState()
    return _DEC_Population_UrbanDestructionState( myself )
end

function DEC_Population_ChangeDemonstrationState( state )
    _DEC_Population_ChangeDemonstrationState( myself, state )
end

function DEC_Population_DemonstrationState()
    return _DEC_Population_DemonstrationState( myself )
end

-- Knowledge objects
function DEC_ObjectKnowledgesInCircle( radius, parameters )
    return _DEC_ObjectKnowledgesInCircle( myself, radius, parameters )
end

function DEC_ConnaissanceObjet_Distance( knowledge )
    return _DEC_ConnaissanceObjet_Distance( myself, knowledge )
end

function DEC_ConnaissanceObjet_PointPlusProche( knowledge )
    return _DEC_ConnaissanceObjet_PointPlusProche( myself, knowledge )
end

function DEC_ConnaissanceObjet_EstEnnemi( knowledge )
    return _DEC_ConnaissanceObjet_EstEnnemi( myself, knowledge )
end

-- Knowledge agents
function DEC_Connaissances_PionsPrenantAPartie()
    return _DEC_Connaissances_PionsPrenantAPartie( myself )
end

function DEC_Connaissances_PionsSecurisant()
    return _DEC_Connaissances_PionsSecurisant( myself )
end

-- Orders
function DEC_AssignMissionCrowdParameter( mission, parameter, knowledgeId )
    _DEC_AssignMissionCrowdParameter( myself, mission, parameter, knowledgeId )
end

function DEC_AssignMissionCrowdListParameter( knowledgeId )
    return _DEC_AssignMissionCrowdListParameter( myself, knowledgeId )
end

-- Actions
function DEC__StartDeplacement( targetPosition )
    return _DEC__StartDeplacement( myself, targetPosition )
end

function DEC_StartDeplacementItineraire( targetPositions )
    return _DEC_StartDeplacementItineraire( myself, targetPositions )
end

function DEC__StartTirSurPions( intensity )
    return _DEC__StartTirSurPions( myself, intensity )
end

function DEC__StartTirSurPion( intensity, id )
    return _DEC__StartTirSurPion( myself, intensity, id )
end

function DEC_StartTirSurPion( intensity, pion )
    return _DEC_StartTirSurPion( myself, intensity, pion )
end

function DEC_DetruireBlocUrbain( urbanObject, category )
    if not category then
        return _DEC_Population_DetruireBlocUrbain( myself, urbanObject )
    else
        return _DEC_Agent_DetruireBlocUrbain( myself, urbanObject, category )
    end
end

function DEC_StartAgresserFoule()
    return _DEC_StartAgresserFoule( myself )
end


--
-- Agent
--

-- Move
function DEC_Agent_NiveauInstallation()
    -- also supported by Population
    return _DEC_Agent_NiveauInstallation( myself )
end

-- Crowd
function DEC_Agent_EstDansFoule( pion )
    -- also supported by Population
    return _DEC_Agent_EstDansFoule( myself, pion )
end

-- NBC
function DEC_ConnaissanceObjet_DemandeDeDecontamination( knowledge )
    -- also supported by Population
    return DEC_ConnaissanceObjet_DemandeDeDecontaminationSurPion( myself, knowledge )
end

-- Actions
function DEC_StartDeplacement( path )
    return _DEC_StartDeplacement( myself, path, false )
end

function DEC_StartMovementSuspended( path )
    return _DEC_StartDeplacement( myself, path, true )
end

function DEC_StartTirDirect( enemy, percentage, firingMode, ammoDotationClass )
    return _DEC_StartTirDirect( myself, enemy, percentage, firingMode, ammoDotationClass )
end

function DEC_StartTirDirectDebarques( enemy, percentage, firingMode )
    return _DEC_StartTirDirectDebarques( myself, enemy, percentage, firingMode )
end

function DEC_StartTirDirectTransporteurs( enemy, percentage, firingMode )
    return _DEC_StartTirDirectTransporteurs( myself, enemy, percentage, firingMode )
end

function DEC__StartTirDirectSurComposantesMajeures( ammoDotationClass, enemy, percentage, firingMode )
    return _DEC__StartTirDirectSurComposantesMajeures( myself, ammoDotationClass, enemy, percentage, firingMode )
end

function DEC__StartTirSurPopulation( populationKnowledgeID, dotationClass )
    return _DEC__StartTirSurPopulation( myself, populationKnowledgeID, dotationClass )
end

function DEC_StartTirIndirectSurPosition( category, nbInterventionType, targetPosition )
    return _DEC_StartTirIndirectSurPosition( myself, category, nbInterventionType, targetPosition )
end

function DEC_StartTirIndirectSurPositionAvecDemandeur( category, nbInterventionType, targetPosition, requester )
    return _DEC_StartTirIndirectSurPositionAvecDemandeur( myself, catgeory, nbInterventionType, targetPosition, requester )
end

function DEC_StartTirIndirectSurConnaissance( category, nbInterventionType, targetKnowledgeID )
    return _DEC_StartTirIndirectSurConnaissance( myself, category, nbInterventionType, targetKnowledgeID )
end

function DEC_StartTirIndirectSurConnaissancePtr( category, nbInterventionType, targetKnowledge )
    return _DEC_StartTirIndirectSurConnaissancePtr( myself, category, nbInterventionType, targetKnowledge )
end

function DEC_StartCreateObject( genObject )
    return _DEC_StartCreateObject( myself, genObject, false )
end

function DEC_StartCreateObjectInstantaneously( genObject )
    return _DEC_StartCreateObject( myself, genObject, true )
end

function DEC_StartPrepareObject( genObject )
    return _DEC_StartPrepareObject( myself, genObject )
end

function DEC_StartDevaloriserObjet( knowledge )
    return _DEC_StartDevaloriserObjet( myself, knowledge )
end

function DEC_StartDetruireObjet( knowledge )
    return _DEC_StartDetruireObjet( myself, knowledge )
end

function DEC_StartValoriserObjet( knowledge )
    return _DEC_StartValoriserObjet( myself, knowledge )
end

function DEC_StartCreerContournementObjet( knowledge )
    return _DEC_StartCreerContournementObjet( myself, knowledge )
end

function DEC__StartOccuperObjet( knowledge )
    return _DEC__StartOccuperObjet( myself, knowledge )
end

function DEC_StartReprendreTravauxObjet( knowledge, valorizeIt )
    return _DEC_StartReprendreTravauxObjet( myself, knowledge, valorizeIt )
end

function DEC_ReparerBlocUrbain( urbanBlock )
    return _DEC_ReparerBlocUrbain( myself, urbanBlock )
end

function DEC_DeteriorateUrbanBlock( urbanBlock, percentage )
    return _DEC_DeteriorateUrbanBlock( myself, urbanBlock, percentage )
end

function DEC__StartAnimerObjet( knowledge )
    return _DEC__StartAnimerObjet( myself, knowledge )
end

function DEC_StartDistributionObjet( knowledge, quantity )
    return _DEC_StartDistributionObjet( myself, knowledge, quantity )
end

function DEC_StartSupplyObject( knowledge, dotationTypes, quantity )
    return _DEC_StartSupplyObject( myself, knowledge, dotationTypes, quantity )
end

function DEC_StartExtractFromStockObject( knowledge, dotationTypes, quantity )
    return _DEC_StartExtractFromStockObject( myself, knowledge, dotationTypes, quantity )
end

function DEC__StartControlerZone( localization, radius, fire )
    return _DEC__StartControlerZone( myself, localization, radius, fire )
end

function DEC_StartEmbarquement()
    return _DEC_StartEmbarquement( myself )
end

function DEC_StartDebarquement()
    return _DEC_StartDebarquement( myself )
end

function DEC_StartIlluminer( enemy, ally )
    return _DEC_StartIlluminer( myself, enemy, ally )
end

function DEC_StartExtinguishObject( knowledge )
    return _DEC_StartExtinguishObject( myself, knowledge )
end

function DEC_StartInterrogateCrowd( knowledgeId )
    return _DEC_StartInterrogateCrowd( myself, knowledgeId )
end

function DEC_StartInterrogateUnit( knowledge )
    return _DEC_StartInterrogateUnit( myself, knowledge )
end

function DEC_StartTraverserReseauSouterrain( destination )
    return _DEC_StartTraverserReseauSouterrain( myself, destination )
end

function DEC_StartPreterVSRAM( start, agent, nbToLend )
    return _DEC_StartPreterVSRAM( myself, start, agent, nbToLend )
end

function DEC_StartPreterRemorqueurs( start, agent, nbToLend )
    return _DEC_StartPreterRemorqueurs( myself, start, agent, nbToLend )
end

function DEC_StartPreterComposantes( start, agent, type, nbToLend )
    return _DEC_StartPreterComposantes( myself, start, agent, type, nbToLend )
end

function DEC_StartConsumingResources( category, value, duration )
    return _DEC_StartConsumingResources( myself, category, value, duration )
end

function DEC_Start_TransportEmbarquer()
    return _DEC_Start_TransportEmbarquer( myself )
end

function DEC_Start_TransportDebarquer( position )
    return _DEC_Start_TransportDebarquer( myself, position )
end

function DEC_StartEmbarquerFouleDUneConcentration( knowledgeId, concentrationId )
    return _DEC_StartEmbarquerFouleDUneConcentration( myself, knowledgeId, concentrationId )
end

function DEC_StartDebarquerFouleSurPosition( knowledgeId, position )
    return _DEC_StartDebarquerFouleSurPosition( myself, knowledgeId, position )
end

function DEC_Start_EmergencyInfluence( activity, influence )
    return _DEC_Start_EmergencyInfluence( myself, activity, influence )
end

function DEC_Start_EmergencyTriggerActivity( activity, influence )
    return _DEC_Start_EmergencyTriggerActivity( myself, activity, influence )
end

function DEC_Start_EmergencyInfluenceInArea( activity, influence, localization )
    return _DEC_Start_EmergencyInfluenceInArea( myself, activity, influence, localization )
end

function DEC_Start_EmergencyTriggerActivityInArea( activity, influence, localization )
    return _DEC_Start_EmergencyTriggerActivityInArea( myself, activity, influence, localization )
end

function DEC_Start_EmergencyUnloadActivity( activity, influence )
    return _DEC_Start_EmergencyUnloadActivity( myself, activity, influence )
end

function DEC_Agent_ADotationPourConstruireObjet( type )
    return _DEC_Agent_ADotationPourConstruireObjet( myself, type )
end

function DEC_Agent_ADotationPourConstruireObjetSansRenforts( type )
    return _DEC_Agent_ADotationPourConstruireObjetSansRenforts( myself, type )
end

function DEC_Stock_IsExtractPossible( knowledge, dotationTypes )
    return _DEC_Stock_IsExtractPossible( myself, knowledge, dotationTypes )
end

function DEC_Stock_IsSupplyPossible( knowledge, dotationTypes )
    return _DEC_Stock_IsSupplyPossible( myself, knowledge, dotationTypes )
end

function DEC_Transport_AjouterPion( agent, transportOnlyLoadable )
    _DEC_Transport_AjouterPion( myself, agent, transportOnlyLoadable )
end

function DEC_Transport_AjouterPions( agents, transportOnlyLoadable )
    _DEC_Transport_AjouterPions( myself, agents, transportOnlyLoadable )
end

function DEC_Transport_EmbarquerDansTransporteurSansDelais( carrier )
    _DEC_Transport_EmbarquerDansTransporteurSansDelais( myself, carrier )
end

function DEC_Transport_DebarquerDeTransporteurSansDelais()
    _DEC_Transport_DebarquerDeTransporteurSansDelais( myself )
end

function DEC_Transport_Transporteur()
    return _DEC_Transport_Transporteur( myself )
end

function DEC_Transport_EmbarquerPionSansDelais( agent, transportOnlyLoadable )
    _DEC_Transport_EmbarquerPionSansDelais( myself, agent, transportOnlyLoadable )
end

function DEC_Transport_EmbarquerPionsSansDelais( agents, transportOnlyLoadable )
    _DEC_Transport_EmbarquerPionsSansDelais( myself, agents, transportOnlyLoadable )
end

function DEC_Transport_DebarquerPionSansDelais( agent )
    _DEC_Transport_DebarquerPionSansDelais( myself, agent )
end

function DEC_Transport_DebarquerPionsSansDelais( agents )
    _DEC_Transport_DebarquerPionsSansDelais( myself, agents )
end

function DEC_Transport_EstTermine()
    return _DEC_Transport_EstTermine( myself )
end

function DEC_Transport_Annuler()
    _DEC_Transport_Annuler( myself )
end

function DEC_Transport_EnCoursDeTransport()
    return _DEC_Transport_EnCoursDeTransport( myself )
end

function DEC_Agent_PeutTransporterPion( agent, transportOnlyLoadable )
    return _DEC_Agent_PeutTransporterPion( myself, agent, transportOnlyLoadable )
end

function DEC_Agent_TransportNombreAllerRetour( agent, transportOnlyLoadable )
    return _DEC_Agent_TransportNombreAllerRetour( myself, agent, transportOnlyLoadable )
end

function DEC_Prisonniers_CapturerEtEmbarquer( knowledge )
    _DEC_Prisonniers_CapturerEtEmbarquer( myself, knowledge )
end

function DEC_Prisonniers_Debarquer( knowledge )
    _DEC_Prisonniers_Debarquer( myself, knowledge )
end

function DEC_Prisonniers_DebarquerDansCamp( knowledge, campKnowledge )
    _DEC_Prisonniers_DebarquerDansCamp( myself, knowledge, campKnowledge )
end

function DEC_Prisonniers_EstEmbarque( knowledge )
    return _DEC_PrisonniersRefugies_EstEmbarque( myself, knowledge )
end

function DEC_Refugies_OrienterEtEmbarquer( knowledge )
    _DEC_Refugies_OrienterEtEmbarquer( myself, knowledge )
end

function DEC_Refugies_Debarquer( knowledge )
    _DEC_Refugies_Debarquer( myself, knowledge )
end

function DEC_Refugies_DebarquerDansCamp( knowledge, campKnowledge )
    _DEC_Refugies_DebarquerDansCamp( myself, knowledge, campKnowledge )
end

function DEC_Refugies_EstEmbarque( knowledge )
    return _DEC_PrisonniersRefugies_EstEmbarque( myself, knowledge )
end

function DEC_Orientate( direction )
    _DEC_Orientate( myself, direction )
end

-- Orders
function DEC_CreerMissionPion( mission )
    return _DEC_CreerMissionPion( myself, mission )
end

function DEC_CreerMissionPionVersPion( mission )
    return _DEC_CreerMissionPionVersPion( myself, mission )
end

function DEC_DonnerMissionPion( mission )
    _DEC_DonnerMissionPion( myself, mission )
end

function DEC_DonnerMissionPionVersPion( mission )
    _DEC_DonnerMissionPionVersPion( myself, mission )
end

-- Loading/Unloading
function DEC_Agent_EstEmbarquable()
    return _DEC_Agent_EstEmbarquable( myself )
end

function DEC_Agent_EstEmbarque()
    return _DEC_Agent_EstEmbarque( myself )
end

function DEC_Agent_EstDebarque()
    return _DEC_Agent_EstDebarque( myself )
end

function DEC_Agent_DureeEmbarquement()
    return _DEC_Agent_DureeEmbarquement( myself )
end

function DEC_Agent_DureeDebarquement()
    return _DEC_Agent_DureeDebarquement( myself )
end

function DEC_Agent_TransporteursPret()
    return _DEC_Agent_TransporteursPret( myself )
end

function DEC_LaisserTransporteursSansDelai()
    _DEC_LaisserTransporteursSansDelai( myself )
end

function DEC_RecupererTransporteursSansDelai()
    _DEC_RecupererTransporteursSansDelai( myself )
end

-- Perception
function DEC_Identification_DistanceMaxCompMajeure()
    return _DEC_Identification_DistanceMaxCompMajeure( myself )
end

function DEC_Reconnoissance_MajorComponentMinDistance()
    return _DEC_Reconnoissance_MajorComponentMinDistance( myself )
end

function DEC_Detection_Distance()
    return _DEC_Detection_Distance( myself )
end

function DEC_Perception_EstFurtif()
    return _DEC_Perception_EstFurtif( myself )
end

function DEC_Perception_Furtivite( factor )
    _DEC_Perception_Furtivite( myself, factor )
end

-- Reinforcement
function DEC_Renforts()
    return _DEC_Renforts( myself )
end

function DEC_Renforce( target )
    return _DEC_Renforce( myself, target )
end

function DEC_AnnuleRenforcement()
    _DEC_AnnuleRenforcement( myself )
end

-- Installation
function DEC_Agent_EstInstalle()
    return _DEC_Agent_EstInstalle( myself )
end

function DEC_Agent_EstDesinstalle()
    return _DEC_Agent_EstDesinstalle( myself )
end

function DEC_Agent_SInstaller()
    _DEC_Agent_SInstaller( myself )
end

-- Deployment
function DEC_Agent_IsDeployed()
    return _DEC_Agent_IsDeployed( myself )
end

function DEC_Agent_IsUndeployed()
    return _DEC_Agent_IsUndeployed( myself )
end

function DEC_Agent_Deploy()
    _DEC_Agent_Deploy( myself )
end

function DEC_Agent_Undeploye()
    _DEC_Agent_Undeploye( myself )
end

-- Accessors
function DEC_Agent_EstPC()
    return _DEC_Agent_EstPC( myself )
end

function DEC_Agent_EstTransporte()
    return _DEC_Agent_EstTransporte( myself )
end

function DEC_Agent_EstEnVol()
    return _DEC_Agent_EstEnVol( myself )
end

function DEC_Agent_HauteurDeVol( height )
    _DEC_Agent_HauteurDeVol( myself, height )
end

function DEC_Agent_EnVille()
    return _DEC_Agent_EnVille( myself )
end

function DEC_Agent_EtatOps()
    return _DEC_Agent_EtatOps( myself )
end

function DEC_Agent_EtatOpsMajeur()
    return _DEC_Agent_EtatOpsMajeur( myself )
end

function DEC_Agent_PeutConstruireObjet( type )
    return _DEC_Agent_PeutConstruireObjet( myself, type )
end

function DEC_Agent_PeutConstruireObjetAvecLocalisation( type, location )
    return _DEC_Agent_PeutConstruireObjetAvecLocalisation( myself, type, location )
end

function DEC_Agent_PeutConstruireObjetSansRenforts( type )
    return _DEC_Agent_PeutConstruireObjetSansRenforts( myself, type )
end

function DEC_Agent_PeutConstruireObjetSansRenfortsAvecLocalisation( type, location )
    return _DEC_Agent_PeutConstruireObjetSansRenfortsAvecLocalisation( myself, type, location )
end

function DEC_Agent_PeutConstruireObjetEmbarque( type )
    return _DEC_Agent_PeutConstruireObjetEmbarque( myself, type )
end

function DEC_Agent_PeutConstruireObjetEmbarqueAvecLocalisation( type, location )
    return _DEC_Agent_PeutConstruireObjetEmbarqueAvecLocalisation( myself, type, location )
end

function DEC_Agent_PeutConstruireContournementObjet( object )
    return _DEC_Agent_PeutConstruireContournementObjet( myself, object )
end

function DEC_Agent_PeutDetruireObjet( object )
    return _DEC_Agent_PeutDetruireObjet( myself, object )
end

function DEC_Agent_PeutValoriserObjet( object )
    return _DEC_Agent_PeutValoriserObjet( myself, object )
end

function DEC_Agent_PeutDevaloriserObjet( object )
    return _DEC_Agent_PeutDevaloriserObjet( myself, object )
end

function DEC_Agent_ActiverModeDiscret()
    _DEC_Agent_ActiverModeDiscret( myself )
end

function DEC_Agent_DesactiverModeDiscret()
    _DEC_Agent_DesactiverModeDiscret( myself )
end

function DEC_Agent_EstNeutralise()
    return _DEC_Agent_EstNeutralise( myself )
end

function DEC_Agent_EstCibleTirIndirect()
    return _DEC_Agent_EstCibleTirIndirect( myself )
end

function DEC_Agent_AutomateEstEmbraye()
    return _DEC_Agent_AutomateEstEmbraye( myself )
end

function DEC_Agent_Position()
    return _DEC_Agent_Position( myself )
end

function DEC_Agent_Direction()
    return _DEC_Agent_Direction( myself )
end

function DEC_Agent_EstMort()
    return _DEC_Agent_EstMort( myself )
end

function DEC_Agent_RoePopulation()
    return _DEC_Agent_RoePopulation( myself )
end

function DEC_HasDotation( category )
    return _DEC_HasDotation( myself, category )
end

function DEC_CanUseDotation( category )
    return _DEC_CanUseDotation( myself, category )
end

function DEC_GetDotation( id )
    return _DEC_GetDotation( myself, id )
end

function DEC_Agent_IlluminateRange()
    return _DEC_Agent_IlluminateRange( myself )
end

function DEC_Agent_CanExtinguish( knowledge )
    return _DEC_Agent_CanExtinguish( myself, knowledge )
end

function DEC_Agent_CanFly()
    return DEC_Pion_CanFly( myself )
end

function DEC_Agent_AutonomieEnDeplacement()
    return _DEC_Agent_AutonomieEnDeplacement( myself )
end

function DEC_Agent_TempsPourParcourirDistanceEnLigneDroite( distance )
    return _DEC_Agent_TempsPourParcourirDistanceEnLigneDroite( myself, distance )
end

function DEC_Agent_AutomateForcerReddition( automaton )
    return _DEC_Agent_AutomateForcerReddition( myself, automaton )
end

function DEC_Agent_ChangerAmbianceEnSurete( safety )
    _DEC_Agent_ChangerAmbianceEnSurete( myself, safety )
end

function DEC_ReleverPion( agent )
    return _DEC_ReleverPion( myself, agent )
end

function DEC_ReleverUnite( agent )
    return _DEC_ReleverPion( myself, agent )
end

function DEC_PeutReleverPion( agent )
    return DEC_PeutReleverPion( myself, agent )
end

function DEC_Suicide()
    _DEC_Suicide( myself )
end

function DEC_Agent_EstDansLeFuseau()
    return _DEC_Agent_EstDansLeFuseau( myself )
end

-- NBC
function DEC_Agent_SeDecontaminer()
    _DEC_Agent_SeDecontaminer( myself )
end

function DEC_Agent_EstAgentNBC()
    return _DEC_Agent_EstAgentNBC( myself )
end

function DEC_Agent_EstContamine()
    return myself:DEC_Agent_EstContamine()
end

function DEC_Agent_EstEmpoisonne()
    return _DEC_Agent_EstEmpoisonne( myself )
end

function DEC_Agent_MettreTenueProtectionNBC()
    _DEC_Agent_MettreTenueProtectionNBC( myself )
end

function DEC_Agent_EnleverTenueProtectionNBC()
    _DEC_Agent_EnleverTenueProtectionNBC( myself )
end

function DEC_Agent_NiveauProtectionNBC()
    return _DEC_Agent_NiveauProtectionNBC( myself )
end

function DEC_Agent_ImmuniserNbc()
    _DEC_Agent_ImmuniserNbc( myself )
end

function DEC_Agent_StopImmuniserNbc()
    _DEC_Agent_StopImmuniserNbc( myself )
end

-- Blackout
function DEC_Agent_PasserEnSilenceRadio()
    _DEC_Agent_PasserEnSilenceRadio( myself )
end

function DEC_Agent_ArreterSilenceRadio()
    _DEC_Agent_ArreterSilenceRadio( myself )
end

function DEC_Agent_PasserEnSilenceRadioPartiel()
    _DEC_Agent_PasserEnSilenceRadioPartiel( myself, true )
end

function DEC_Agent_PasserEnEmissionRestreinte()
    _DEC_Agent_PasserEnSilenceRadioPartiel( myself, false )
end

-- Knowledge sharing
function DEC_EnableSharedPerception( agent )
    _DEC_EnableSharedPerception( myself, agent )
end

function DEC_DisabledSharedPerception( agent )
    _DEC_DisabledSharedPerception( myself, agent )
end

function DEC_EnableSharingKnowledges( agent )
    _DEC_EnableSharingKnowledges( myself, agent )
end

function DEC_DisabledSharingKnowledges( agent )
    _DEC_DisabledSharingKnowledges( myself, agent )
end

-- Decisional state
function DEC_Agent_ChangeEtatRapportDeForce( state )
    _DEC_Agent_ChangeEtatRapportDeForce( myself, state )
end

function DEC_Agent_ChangeEtatROE( state )
    _DEC_Agent_ChangeEtatROE( myself, state )
end

function DEC_Agent_GetEtatROE()
    return _DEC_Agent_GetEtatROE( myself )
end

function DEC_Agent_ChangeEtatROEPopulation( population )
    _DEC_Agent_ChangeEtatROEPopulation( myself, population )
end

function DEC_Agent_ChangeEtatCombatDeRencontre( state )
    _DEC_Agent_ChangeEtatCombatDeRencontre( myself, state )
end

function DEC_Agent_ChangeEtatOperationnel( state )
    _DEC_Agent_ChangeEtatOperationnel( myself, state )
end

function DEC_Agent_ChangeDisponibiliteAuTirIndirect( state )
    _DEC_Agent_ChangeDisponibiliteAuTirIndirect( myself, state )
end

-- Human factors
function DEC_FacteurHumain_Fatigue()
    return _DEC_FacteurHumain_Fatigue( myself )
end

function DEC_FacteurHumain_Moral()
    return _DEC_FacteurHumain_Moral( myself )
end

-- Hierarchy
function DEC_Pion_PionsAvecPC()
    return myself:DEC_Pion_PionsAvecPC()
end

function DEC_Pion_PionsSansPC()
    return _DEC_Pion_PionsSansPC( myself )
end

function DEC_Pion_PionsAvecPCCommunication()
    return myself:DEC_Pion_PionsAvecPCCommunication()
end

function DEC_Pion_PionsSansPCCommunication()
    return _DEC_Pion_PionsSansPCCommunication( myself )
end

function DEC_Pion_PionPC()
    return _DEC_Pion_PionPC( myself )
end

function DEC_Pion_ChangeAutomate( arg1, arg2 )
    -- TODO: Rewrite?
    if not arg2 then
        _DEC_Pion_ChangeAutomate( myself, arg1 )
    else
        _DEC_Pion_ChangeAutomate( arg1, arg2 )
    end
end

function DEC_Agent_PositionInterception( knowledge )
    return _DEC_Agent_PositionInterception( myself, knowledge )
end

-- Knowledge objects
function DEC_ConnaissanceObjet_EstUnEnnemi( knowledge )
    return _DEC_ConnaissanceObjet_EstUnEnnemi( myself, knowledge )
end

function DEC_ConnaissanceObjet_EstUnAllie( knowledge )
    return _DEC_ConnaissanceObjet_EstUnAllie( myself, knowledge )
end

function DEC_ConnaissanceObjet_Reconnaitre( knowledge )
    _DEC_ConnaissanceObjet_Reconnaitre( myself, knowledge )
end

function DEC_ConnaissanceObjet_PeutEtreOccupe( knowledge )
    return _DEC_ConnaissanceObjet_PeutEtreOccupe( myself, knowledge )
end

function DEC_ConnaissanceObjet_PeutEtreAnime( knowledge )
    return _DEC_ConnaissanceObjet_PeutEtreAnime( myself, knowledge )
end

function DEC_ConnaissanceObjet_Degrader( knowledge, factor, dotation )
    if not dotation then
        return _DEC_Population_ConnaissanceObjet_Degrader( myself, knowledge, factor )
    else
        return _DEC_Agent_ConnaissanceObjet_Degrader( myself, knowledge, factor, dotation )
    end
end

function DEC_ConnaissanceObjet_NiveauDePerceptionCourant( knowledge )
    return _DEC_ConnaissanceObjet_NiveauDePerceptionCourant( myself, knowledge )
end

function DEC_Agent_TempsPourDegagerUnObjet( knowledge )
    return _DEC_Agent_TempsPourDegagerUnObjet( myself, knowledge )
end

function DEC_DecontaminerZone( location )
    _DEC_DecontaminerZone( myself, location )
end

-- Population knowledge accessors
function DEC_ConnaissancePopulation_Attitude( knowledgeId )
    return _DEC_ConnaissancePopulation_Attitude( myself, knowledgeId )
end

function DEC_ConnaissancePopulation_Reconnaitre( knowledgeId )
    _DEC_ConnaissancePopulation_Reconnaitre( myself, knowledgeId )
end

function DEC_ConnaissancePopulation_EstReconnu( knowledgeId )
    return _DEC_ConnaissancePopulation_EstReconnu( myself, knowledgeId )
end

function DEC_ConnaissancePopulation_EstPercueParUnite( knowledgeId )
    return _DEC_ConnaissancePopulation_EstPercueParUnite( myself, knowledgeId )
end

function DEC_ConnaissancePopulation_Dangerosite( knowledgeId )
    return _DEC_ConnaissancePopulation_Dangerosite( myself, knowledgeId )
end

function DEC_ConnaissancePopulation_PointPlusProche( knowledgeId )
    return _DEC_ConnaissancePopulation_PointPlusProche( myself, knowledgeId )
end

function DEC_ConnaissancePopulation_Securiser( knowledgeId )
    _DEC_ConnaissancePopulation_Securiser( myself, knowledgeId )
end

function DEC_ConnaissancePopulation_PointSecurisation( knowledgeId )
    return _DEC_ConnaissancePopulation_PointSecurisation( myself, knowledgeId )
end

function DEC_ConnaissancePopulation_EstEnnemi( knowledgeId )
    return _DEC_ConnaissancePopulation_EstEnnemi( myself, knowledgeId )
end

function DEC_ConnaissancePopulation_Exterminer( knowledgeId, surface, dotation )
    return _DEC_ConnaissancePopulation_Exterminer( myself, knowledgeId, surface, dotation )
end

function DEC_GetAdhesionPopulation( knowledgeId )
    return _DEC_GetAdhesionPopulation( myself, knowledgeId )
end

function DEC_GetAttitudePopulation( knowledgeId )
    return _DEC_GetAttitudePopulation( myself, knowledgeId )
end

function DEC_StartHidingInCrowd( knowledgeId )
    _DEC_StartHidingInCrowd( myself, knowledgeId )
end

function DEC_StopHidingInCrowd( knowledgeId )
    _DEC_StopHidingInCrowd( myself, knowledgeId )
end

function DEC_GetAgentsHiddenInCrowd( knowledgeId )
    return _DEC_GetAgentsHiddenInCrowd( myself, knowledgeId )
end

function DEC_Crowd_ExtractWoundedFromCrowd( knowledgeId, position )
    return _DEC_Crowd_ExtractWoundedFromCrowd( myself, knowledgeId, position )
end

function DEC_Crowd_ExtractDeadFromCrowd( knowledgeId, position )
    return _DEC_Crowd_ExtractDeadFromCrowd( myself, knowledgeId, position )
end

function DEC_Crowd_HealWoundedHumans( knowledgeId )
    return _DEC_Crowd_HealWoundedHumans( myself, knowledgeId )
end

function DEC_ConnaissancePopulation_Verrouiller( knowledgeId )
    _DEC_ConnaissancePopulation_Verrouiller( myself, knowledgeId )
end

function DEC_ConnaissancePopulation_Deverrouiller( knowledgeId )
    _DEC_ConnaissancePopulation_Deverrouiller( myself, knowledgeId )
end

function DEC_ObtenirRenseignementCritiqueSurFoule( knowledgeId )
    return _DEC_ObtenirRenseignementCritiqueSurFoule( myself, knowledgeId )
end

function DEC_Agent_ChangerNombreIndividuArmeDansFoule( knowledgeId, armedIndividuals )
    _DEC_Agent_ChangerNombreIndividuArmeDansFoule( myself, knowledgeId, armedIndividuals )
end

function DEC_Connaissances_BlocUrbain( results )
    local blocks = _DEC_Connaissances_BlocUrbain( myself )
    InitQueryReturn( results, integration.ontology.types.urbanBlock, blocks, true )
end

function DEC_Connaissances_BlocUrbainPourPosition( position )
    return _DEC_Connaissances_BlocUrbainPourPosition( myself, position )
end

function DEC_Connaissances_BlocUrbainDansZone( location )
    return _DEC_Connaissances_BlocUrbainDansZone( myself, location )
end

function DEC_Connaissances_BlocUrbainDansOuIntersectentZone( location )
    return _DEC_Connaissances_BlocUrbainDansOuIntersectentZone( myself, location )
end

-- Urban block
function DEC_ConnaissanceUrbanBlock_NiveauDeReconnaissanceCourant( urbanObject )
    return _DEC_ConnaissanceUrbanBlock_NiveauDeReconnaissanceCourant( myself, urbanObject )
end

function DEC_ConnaissanceBlocUrbain_Traficabilite( urbanObject )
    return _DEC_ConnaissanceBlocUrbain_Traficabilite( myself, urbanObject )
end

function DEC_ConnaissanceBlocUrbain_RapForLocal( urbanObject )
    return _DEC_ConnaissanceBlocUrbain_RapForLocal( myself, urbanObject )
end

function DEC_Connaissances_UnitesEnnemiesVivantesDansBlocUrbain( urbanObject )
    return _DEC_Connaissances_UnitesEnnemiesVivantesDansBlocUrbain( myself, urbanObject )
end

-- Agent knowledge
function DEC_ConnaissanceAgent_AttritionPotentielle( knowledge, position )
    return _DEC_ConnaissanceAgent_AttritionPotentielle( myself, knowledge, position )
end

function DEC_ConnaissanceAgent_Dangerosite( knowledge )
    return _DEC_ConnaissanceAgent_Dangerosite( myself, knowledge )
end

function DEC_ConnaissanceAgent_NiveauDePerceptionCourant( knowledge )
    return _DEC_ConnaissanceAgent_NiveauDePerceptionCourant( myself, knowledge )
end

function DEC_ConnaissanceAgent_EstPercuParUnite( knowledge )
    return _DEC_ConnaissanceAgent_EstPercuParUnite( myself, knowledge )
end

function DEC_ConnaissanceAgent_EstUnEnnemi( knowledge )
    return _DEC_ConnaissanceAgent_EstUnEnnemi( myself, knowledge )
end

function DEC_ConnaissanceAgent_EstUnAllie( knowledge )
    return _DEC_ConnaissanceAgent_EstUnAllie( myself, knowledge )
end

function DEC_ConnaissanceAgent_PercoitUnite( knowledge )
    return _DEC_ConnaissanceAgent_PercoitUnite( myself, knowledge )
end

function DEC_ConnaissanceAgent_EstAPorteDeCapteurDansCone( knowledge, direction, angle )
    return _DEC_ConnaissanceAgent_EstAPorteDeCapteurDansCone( myself, knowledge, direction, angle )
end

function DEC_ConnaissanceAgent_PeutEtreIllumine( knowledge )
    return _DEC_ConnaissanceAgent_PeutEtreIllumine( myself, knowledge )
end

function DEC_ConnaissanceAgent_PeutIlluminer()
    return _DEC_ConnaissanceAgent_PeutIlluminer( myself )
end

function DEC_KnowledgeAgent_IsInObject( objectType, knowledge, isFriend )
    return _DEC_KnowledgeAgent_IsInObject( myself, objectType, knowledge, isFriend )
end

function DEC_KnowledgeAgent_IsInObjectWithCapacity( capacity, knowledge, isFriend )
    return _DEC_KnowledgeAgent_IsInObjectWithCapacity( myself, capacity, knowledge, isFriend )
end

-- Knowledge
function DEC_RapportDeForceLocal()
    return _DEC_RapportDeForceLocal( myself )
end

function DEC_Connaissances_UnitesEnnemiesDangereuses()
    return _DEC_Connaissances_UnitesEnnemiesDangereuses( myself )
end

function DEC_Connaissances_UnitesDetecteesDansFuseau()
    return _DEC_Connaissances_UnitesDetecteesDansFuseau( myself )
end

function DEC_Connaissances_UnitesDetecteesDansZone( zone )
    return _DEC_Connaissances_UnitesDetecteesDansZone( myself, zone )
end

function DEC_Connaissances_UnitesPrenantAPartie()
    return _DEC_Connaissances_UnitesPrenantAPartie( myself )
end

function DEC_Connaissances_UnitesEnnemiesVivantesPercues()
    return _DEC_Connaissances_UnitesEnnemiesVivantesPercues( myself )
end

function DEC_Connaissances_Observable( results )
    local enemies = DEC_Knowledge_GetEnemies( myself )
    InitQueryReturn( results, integration.ontology.types.agentKnowledge, enemies, true )
    
    local objects = DEC_Knowledge_GetObjects
    InitQueryReturn( results, integration.ontology.types.object, objects, true )
    
    local blocks = DEC_Knowledge_GetUrbanBlocks( myself )
    InitQueryReturn( results, integration.ontology.types.urbanBlock, blocks, true )
end

function DEC_Connaissances_Destroyable( results )
    local enemies = DEC_Knowledge_GetEnemies( myself )
    InitQueryReturn( results, integration.ontology.types.agentKnowledge, enemies, true )
end

function DEC_Connaissances_Identifiable( results )
    local enemies = DEC_Knowledge_GetEnemies( myself )
    InitQueryReturn( results, integration.ontology.types.agentKnowledge, enemies, true )    
end

function DEC_Connaissances_UnitesEnnemiesVivantesPercuesDansFuseau()
    return _DEC_Connaissances_UnitesEnnemiesVivantesPercuesDansFuseau( myself )
end

function DEC_Connaissances_UnitesEnnemiesVivantesDansFuseau()
    return _DEC_Connaissances_UnitesEnnemiesVivantesDansFuseau( myself )
end

function DEC_Connaissances_UnitesEnnemiesVivantesDansZone( zone )
    return _DEC_Connaissances_UnitesEnnemiesVivantesDansZone( myself, zone )
end

function DEC_Connaissances_UnitesBlesseesDansZone( zone )
    return _DEC_Connaissances_UnitesBlesseesDansZone( myself, zone )
end

function DEC_Connaissances_UnitesCivilesDansZone( zone )
    return _DEC_Connaissances_UnitesCivilesDansZone( myself, zone )
end

function DEC_Connaissances_UnitesEnnemiesVivantesDansCercle( center, radius )
    return _DEC_Connaissances_UnitesEnnemiesVivantesDansCercle( myself, center, radius )
end

function DEC_Connaissances_UnitesBlesseesOuTueesDansCercle( center, radius )
    return _DEC_Connaissances_UnitesBlesseesOuTueesDansCercle( myself, center, radius )
end

function DEC_Connaissances_RefugiesAProximite( radius )
    return _DEC_Connaissances_RefugiesAProximite( myself, radius )
end

function DEC_Connaissances_TerroristsAProximite( radius )
    return _DEC_Connaissances_TerroristsAProximite( myself, radius )
end

function DEC_Connaissances_UnitesRenduesAProximite( radius )
    return _DEC_Connaissances_UnitesRenduesAProximite( myself, radius )
end

function DEC_Connaissances_PresenceEnnemieDansCercle( center, radius )
    return _DEC_Connaissances_PresenceEnnemieDansCercle( myself, center, radius )
end

function DEC_Connaissances_UnitesAmiesDansZone( zone )
    return _DEC_Connaissances_UnitesAmiesDansZone( myself, zone )
end

function DEC_ObjectKnowledgesIntersectingInZone( location, parameters )
    return _DEC_ObjectKnowledgesIntersectingInZone( myself, location, parameters )
end

function DEC_Connaissances_CollisionsObjets()
    return _DEC_Connaissances_CollisionsObjets( myself )
end

function DEC_Connaissances_CollisionsDesastres()
    return _DEC_Connaissances_CollisionsDesastres( myself )
end

function DEC_Connaissances_Desastres()
    return _DEC_Connaissances_Desastres( myself )
end

function DEC_Connaissances_CollisionsObjetsDeType( objectType )
    return _DEC_Connaissances_CollisionsObjetsDeType( myself, objectType )
end

function DEC_Connaissances_ObjetLePlusProche( type )
    return _DEC_Connaissances_ObjetLePlusProche( myself, type )
end

function DEC_Connaissances_ObjetAmiLePlusProche( type )
    return _DEC_Connaissances_ObjetAmiLePlusProche( myself, type )
end

function DEC_Connaissances_CollisionsPopulations()
    return _DEC_Connaissances_CollisionsPopulations( myself )
end

function DEC_Connaissances_PopulationsPrenantAPartie()
    return _DEC_Connaissances_PopulationsPrenantAPartie( myself )
end

function DEC_Connaissances_EstPrisAPartieParPopulation( knowledgeId )
    return _DEC_Connaissances_EstPrisAPartieParPopulation( myself, knowledgeId )
end

-- Logistics
function DEC_RecupererComposantes( target, type, numToGetBack )
    _DEC_RecupererComposantes( myself, target, type, numToGetBack )
end

function DEC_RecupererVSRAM( target, numToGetBack )
    _DEC_RecupererVSRAM( myself, target, numToGetBack )
end

function DEC_RecupererRemorqueurs( target, numToGetBack )
    _DEC_RecupererRemorqueurs( myself, target, numToGetBack )
end

function DEC_Pion_PcDeTC2()
    return _DEC_Pion_PcDeTC2( myself )
end

function DEC_ChangeValeurDotations2( dotationType, capacityFactor )
    _DEC_ChangeValeurDotations( myself, dotationType, capacityFactor, -1 )
end

function DEC_ChangeValeurDotations3( dotationType, capacityFactor, ammoDotationClassId )
    _DEC_ChangeValeurDotations( myself, dotationType, capacityFactor, ammoDotationClassId )
end

function DEC_Ravitaillement_Convoi_DeplacementVersRavitailleurEffectue()
    _DEC_Ravitaillement_Convoi_DeplacementVersRavitailleurEffectue( myself )
end

function DEC_Ravitaillement_Convoi_DeplacementVersTransporteurEffectue()
    _DEC_Ravitaillement_Convoi_DeplacementVersTransporteurEffectue( myself )
end

function DEC_Ravitaillement_Convoi_DeplacementVersDestinataireEffectue()
    _DEC_Ravitaillement_Convoi_DeplacementVersDestinataireEffectue( myself )
end

function DEC_Ravitaillement_Convoi_FinMission()
    _DEC_Ravitaillement_Convoi_FinMission( myself )
end

function DEC_Ravitaillement_Convoi_ActionCourante()
    return _DEC_Ravitaillement_Convoi_ActionCourante( myself )
end

function DEC_Ravitaillement_Convoi_DestinataireCourant()
    return _DEC_Ravitaillement_Convoi_DestinataireCourant( myself )
end

function DEC_Ravitaillement_Convoi_Ravitailleur()
    return _DEC_Ravitaillement_Convoi_Ravitailleur( myself )
end

function DEC_Ravitaillement_Convoi_Transporteur()
    return _DEC_Ravitaillement_Convoi_Transporteur( myself )
end

function DEC_Ravitaillement_Convoi_ItineraireVersProchaineDestination()
    return _DEC_Ravitaillement_Convoi_ItineraireVersProchaineDestination( myself )
end

function DEC_Ravitaillement_Convoi_EstFluxPousse()
    return _DEC_Ravitaillement_Convoi_EstFluxPousse( myself )
end

function DEC_CreateBreakdown( type, breakdown )
    return _DEC_CreateBreakdown( myself, type, breakdown )
end

-- Misc
function DEC_ModulationVitesseCourante( factor )
    _DEC_ModulationVitesseCourante( myself, factor )
end

function DEC_ModulationVitesseMax( factor )
    _DEC_ModulationVitesseMax( myself, factor )
end

-- Path
function DEC_CreerItineraire( endPoint, pathType )
    return _DEC_CreerItineraire( myself, endPoint, pathType )
end

function DEC_CreerItineraireBM( endPoint, pathType )
    return _DEC_CreerItineraireBM( myself, endPoint, pathType )
end

function DEC_CreerItineraireListe( points, pathType )
    return _DEC_CreerItineraireListe( myself, points, pathType )
end

function DEC_Itineraire_EstEnMouvementSur( path )
    return _DEC_Itineraire_EstEnMouvementSur( myself, path )
end

function DEC_GetNextObjectOnPath( oId, oDistance, params )
    return _DEC_GetNextObjectOnPath( myself, oId, oDistance, params )
end

function DEC_GetNextObjectOnPathWithBypassed( oId, oDistance, params )
    return _DEC_GetNextObjectOnPathWithBypassed( myself, oId, oDistance, params )
end

function DEC_Itineraire_ExtrapolerPosition( time, boundOnPath )
    return myself:DEC_Itineraire_ExtrapolerPosition( time, boundOnPath )
end

-- Perception
function DEC_Perception_ActiverCoupsDeSonde()
    _DEC_Perception_ActiverCoupsDeSonde( myself )
end

function DEC_Perception_DesactiverCoupsDeSonde()
    _DEC_Perception_DesactiverCoupsDeSonde( myself )
end

function DEC_Perception_ActiverSenseursSurDecision()
    _DEC_Perception_ActiverSenseursSurDecision( myself )
end

function DEC_Perception_DesactiverSenseursSurDecision()
    _DEC_Perception_DesactiverSenseursSurDecision( myself )
end

function DEC_Perception_ActiverSenseurs()
    _DEC_Perception_ActiverSenseurs( myself, true )
end

function DEC_Perception_DesactiverSenseurs()
    _DEC_Perception_ActiverSenseurs( myself, false )
end

function DEC_Perception_ActiverRadar( radarClass )
    _DEC_Perception_ActiverRadar( myself, radarClass )
end

function DEC_Perception_DesactiverRadar( radarClass )
    _DEC_Perception_DesactiverRadar( myself, radarClass )
end

function DEC_Perception_ActiverRadarSurLocalisation( radarClass, localization )
    return _DEC_Perception_ActiverRadarSurLocalisation( myself, radarClass, localization )
end

function DEC_Perception_ActiverRadarSurPointPtr( radarClass, point )
    return _DEC_Perception_ActiverRadarSurPointPtr( myself, radarClass, point )
end

function DEC_Perception_DesactiverRadarSurLocalisation( radarClass, id )
    _DEC_Perception_DesactiverRadarSurLocalisation( myself, radarClass, id )
end

function DEC_Perception_ActiverPerceptionTirsIndirect( localization )
    return _DEC_Perception_ActiverPerceptionTirsIndirect( myself, localization )
end

function DEC_Perception_DesactiverPerceptionTirsIndirect( id )
    _DEC_Perception_DesactiverPerceptionTirsIndirect( myself, id )
end

function DEC_Perception_ActiverReconnaissanceLocalisation( localization )
    return _DEC_Perception_ActiverReconnaissanceLocalisation( myself, localization )
end

function DEC_Perception_ActivateLocationProgressiveRecce( localization, speedGrowth )
    return _DEC_Perception_ActivateLocationProgressiveRecce( myself, localization, speedGrowth )
end

function DEC_Perception_DesactiverReconnaissanceLocalisation( id )
    _DEC_Perception_DesactiverReconnaissanceLocalisation( myself, id )
end

function DEC_Perception_ActiverReconnaissanceDansBlocUrbain( urbanBlock )
    return _DEC_Perception_ActiverReconnaissanceDansBlocUrbain( myself, urbanBlock )
end

function DEC_Perception_DesactiverReconnaissanceDansBlocUrbain( id )
    _DEC_Perception_DesactiverReconnaissanceDansBlocUrbain( myself, id )
end

function DEC_Perception_ActiverDetectionObjetLocalisation( localization, center, speedGrowth )
    return _DEC_Perception_ActiverDetectionObjetLocalisation( myself, localization, center, speedGrowth )
end

function DEC_Perception_DesactiverDetectionObjetLocalisation( id )
    _DEC_Perception_DesactiverDetectionObjetLocalisation( myself, id )
end

function DEC_Perception_ActiverReconnaissancePoint( center, size, speedGrowth )
    return _DEC_Perception_ActiverReconnaissancePoint( myself, center, size, speedGrowth )
end

function DEC_Perception_DesactiverReconnaissancePoint( id )
    _DEC_Perception_DesactiverReconnaissancePoint( myself, id )
end

function DEC_Perception_VisionVerrouilleeSurDirection( direction )
    _DEC_Perception_VisionVerrouilleeSurDirection( myself, direction )
end

function DEC_Perception_VisionVerrouilleeSurPoint( point )
    _DEC_Perception_VisionVerrouilleeSurPoint( myself, point )
end

function DEC_Perception_VisionVerrouilleeSurPointPtr( point )
    _DEC_Perception_VisionVerrouilleeSurPointPtr( myself, point )
end

function DEC_Perception_VisionNormale()
    _DEC_Perception_VisionNormale( myself )
end

function DEC_Perception_ActiverObserveurTir()
    _DEC_Perception_ActiverObserveurTir( myself )
end

function DEC_Perception_DesactiverObserveurTir()
    _DEC_Perception_DesactiverObserveurTir( myself )
end

function DEC_Perception_PointEstVisible( point )
    return _DEC_Perception_PointEstVisible( myself, point )
end

function DEC_Connaissances_IdentifierToutesUnitesDansZone( location )
    _DEC_Connaissances_IdentifierToutesUnitesDansZone( myself, location )
end

function DEC_GetPerception( point, target )
    return _DEC_GetPerception( myself, point, target )
end

function DEC_Perception_ActiverModeEnregistrement()
    _DEC_Perception_ActiverModeEnregistrement( myself )
end

function DEC_Perception_DesactiverModeEnregistrement()
    _DEC_Perception_DesactiverModeEnregistrement( myself )
end

function DEC_ALAT_ActiverReconnaissance( location )
    _DEC_ALAT_ActiverReconnaissance( myself, location )
end

function DEC_ALAT_DesactiverReconnaissance()
    _DEC_ALAT_DesactiverReconnaissance( myself )
end

function DEC_ALAT_ReconnaissanceNonVuTerminee()
    return _DEC_ALAT_ReconnaissanceNonVuTerminee( myself )
end

function DEC_Perception_ActiverSurveillance( localization )
    return _DEC_Perception_ActiverSurveillance( myself, localization )
end

function DEC_Perception_DesactiverSurveillance( id )
    _DEC_Perception_DesactiverSurveillance( myself, id )
end

-- Fire
function DEC_Tir_PorteeMaxPourTirerSurUnite( knowledge, ph )
    return _DEC_Tir_PorteeMaxPourTirerSurUnite( myself, knowledge, ph )
end

function DEC_Tir_PorteeMaxPourTirerSurUniteAvecMunition( knowledge, ph, dotation )
    return _DEC_Tir_PorteeMaxPourTirerSurUniteAvecMunition( myself, knowledge, ph, dotation )
end

function DEC_Tir_PorteeMinPourTirerSurUnite( knowledge, ph )
    return _DEC_Tir_PorteeMinPourTirerSurUnite( myself, knowledge, ph )
end

function DEC_Tir_PorteeMaxPourTirerSurUnitePosturesReelles( knowledge, ph )
    return _DEC_Tir_PorteeMaxPourTirerSurUnitePosturesReelles( myself, knowledge, ph )
end

function DEC_Tir_PorteeMinPourTirerSurUnitePosturesReelles( knowledge, ph )
    return _DEC_Tir_PorteeMinPourTirerSurUnitePosturesReelles( myself, knowledge, ph )
end

function DEC_Tir_PorteeMaxPourTirerDebarqueSurUnite( knowledge, ph )
    return _DEC_Tir_PorteeMaxPourTirerDebarqueSurUnite( myself, knowledge, ph )
end

function DEC_Tir_PorteeMaxPourEtreTireParUnite( knowledge, ph )
    return _DEC_Tir_PorteeMaxPourEtreTireParUnite( myself, knowledge, ph )
end

function DEC_Tir_PorteeMaxTirIndirect( dotation )
    return _DEC_Tir_PorteeMaxTirIndirect( myself, dotation )
end

function DEC_Tir_PorteeMinTirIndirect( dotation )
    return _DEC_Tir_PorteeMinTirIndirect( myself, dotation )
end

function DEC_Tir_PorteeMaxTirIndirect_SansTesterEtatMunitions( dotation )
    return _DEC_Tir_PorteeMaxTirIndirect_SansTesterEtatMunitions( myself, dotation )
end

function DEC_Tir_PorteeMinTirIndirect_SansTesterEtatMunitions( dotation )
    return _DEC_Tir_PorteeMinTirIndirect_SansTesterEtatMunitions( myself, dotation )
end

function DEC_Tir_PorteeTheoriqueMaxTirIndirect( dotation )
    return _DEC_Tir_PorteeTheoriqueMaxTirIndirect( myself, dotation )
end

function DEC_Tir_PorteeTheoriqueMinTirIndirect( dotation )
    return _DEC_Tir_PorteeTheoriqueMinTirIndirect( myself, dotation )
end

function DEC_Tir_LancerFumigeneSurConnaissance( target )
    return _DEC_Tir_LancerFumigeneSurConnaissance( myself, target )
end

function DEC_Tir_MunitionPourTirIndirect( indirectFireDotationClassId, target )
    return _DEC_Tir_MunitionPourTirIndirect( myself, indirectFireDotationClassId, target )
end

function DEC_Pion_InterdireMunition( dotationCategories )
    _DEC_Pion_InterdireMunition( myself, dotationCategories )
end

function DEC_Pion_AutoriserMunition( dotationCategories )
    _DEC_Pion_AutoriserMunition( myself, dotationCategories )
end

function DEC_Pion_AutoriserToutesMunitions()
    _DEC_Pion_AutoriserToutesMunitions( myself )
end

-- Terrain
function DEC_FindSafetyPositions( radius, safetyDistance )
    return _DEC_FindSafetyPositions( myself, radius, safetyDistance )
end

function DEC_Crossroads( results )
    local points = _DEC_Crossroads( myself )
    InitQueryReturn( results, integration.ontology.types.point, points, true )
end

