
-- Actions
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

-- Debug
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

-- Representations
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

-- Orders
function DEC_FinMission()
    _DEC_FinMission( myself )
end

-- Knowledge objects
function DEC_ObjectKnowledgesInZone( localization, parameters )
    return _DEC_ObjectKnowledgesInZone( myself, localization, parameters )
end


--
-- Populations
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

function DEC_DetruireBlocUrbain( urbanObject )
    return _DEC_DetruireBlocUrbain( myself, urbanObject )
end

function DEC_StartAgresserFoule()
    return _DEC_StartAgresserFoule( myself )
end


--
-- Agents
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
