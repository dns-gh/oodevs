// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Decision.h"
#include "DEC_Tools.h"
#include "simulation_orders/MIL_MissionParameter_ABC.h"
#include "simulation_orders/MIL_ParameterType_ABC.h"
#include "Decision/DEC_DIAFunctions.h"
#include "Decision/DEC_GeometryFunctions.h"
#include "Decision/DEC_LogisticFunctions.h"
#include "Decision/DEC_MiscFunctions.h"
#include "Decision/DEC_ObjectFunctions.h"
#include "Decision/DEC_PathFunctions.h"
#include "Decision/DEC_TelepathyFunctions.h"
#include "Decision/DEC_Gen_Object.h"
#include "Decision/DEC_PathPoint.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_FragOrder.h"
#include "Entities/Orders/MIL_MissionParameterFactory.h"
#include "Entities/Orders/MIL_MissionParameterVisitor_ABC.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/DEC_Knowledge_Urban.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "protocol/protocol.h"
#include "geometry/Point2.h"
#include <boost/bind.hpp>
#include <directia/Brain.h>

// -----------------------------------------------------------------------------
// Name: ScriptRefs
// Created: LDC 2009-04-08
// -----------------------------------------------------------------------------
ScriptRefs::ScriptRefs( directia::Brain& brain  )
    : sendEvent_       ( brain.GetScriptFunction( "SendEvent" ) )
    , startEvent_      ( brain.GetScriptFunction( "StartEvent" ) )
    , stopEvents_      ( brain.GetScriptFunction( "StopEvents" ) )
    , setStateVariable_( brain.GetScriptFunction( "SetStateVariable" ) )
    , collectgarbage_  ( brain.GetScriptFunction( "collectgarbage" ) )
    , step_            ( brain.RegisterObject( std::string( "step" ) ) )
{
    // NOTHING
}

namespace DEC_DecisionImpl
{

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterGeometryFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterGeometryFunctions( directia::Brain& brain )
{
    brain.RegisterFunction( "DEC_Geometrie_DecouperListePoints",                &DEC_GeometryFunctions::SplitListPoints );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionCouverture",         &DEC_GeometryFunctions::ComputeCoverPosition );
    brain.RegisterFunction( "DEC_Geometrie_CalculerBarycentreAgents",           &DEC_GeometryFunctions::ComputeAgentsBarycenter );
    brain.RegisterFunction( "DEC_Geometrie_PositionTranslate",                  &DEC_GeometryFunctions::TranslatePosition );
    brain.RegisterFunction( "DEC_Geometrie_PositionTranslateDir",               &DEC_GeometryFunctions::TranslatePositionInDirection );
    brain.RegisterFunction( "DEC_Geometrie_PositionsEgales",                    &DEC_GeometryFunctions::ComparePositions );
    brain.RegisterFunction( "DEC_Geometrie_Distance",                           &DEC_GeometryFunctions::Distance ); //@TODO MGD Replace by ComputeDistance on shared_ptr
    brain.RegisterFunction( "DEC_Geometrie_ConvertirPointEnLocalisation",       &DEC_GeometryFunctions::ConvertPointToLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_EstPointDansLocalisation",           &DEC_GeometryFunctions::IsPointInsideLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_CreerLocalisation",                  &DEC_GeometryFunctions::CreateLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_CreerListePoints",                   &DEC_GeometryFunctions::CreateListPoint );
    brain.RegisterFunction( "DEC_Geometrie_CreerPoint",                         &DEC_GeometryFunctions::CreatePoint );
    brain.RegisterFunction( "DEC_Geometrie_CopiePoint",                         &DEC_GeometryFunctions::CopyPoint );
    brain.RegisterFunction( "DEC_Geometrie_CreerDirection",                     &DEC_GeometryFunctions::CreateDirection );
    brain.RegisterFunction( "DEC_Geometrie_CreerDirectionPerpendiculaire",      &DEC_GeometryFunctions::CreateOrthoDirection );
    brain.RegisterFunction( "DEC_Geometrie_InverseDirection",                   &DEC_GeometryFunctions::ReverseDirection );
    brain.RegisterFunction( "DEC_Geometrie_CopieEtInverseDirection",            &DEC_GeometryFunctions::CopyAndReverseDirection );
    brain.RegisterFunction( "DEC_Geometrie_CopieEtRotateDirection",             &DEC_GeometryFunctions::CopyAndRotateDirection );
    brain.RegisterFunction( "DEC_Geometrie_CalculerDistanceLigneMoyenne",       &DEC_GeometryFunctions::ComputeDistanceFromMiddleLine );
    brain.RegisterFunction( "DEC_Geometrie_CalculerBarycentreLocalisation",     &DEC_GeometryFunctions::ComputeLocalisationBarycenter );
    brain.RegisterFunction( "DEC_Geometrie_DirectionMoyenne",                   &DEC_GeometryFunctions::ComputeMeanDirection );
    brain.RegisterFunction( "DEC_Geometrie_PositionAleatoireSurCercle",         &DEC_GeometryFunctions::ComputeRandomPointOnCircle );
    brain.RegisterFunction( "DEC_Geometrie_PositionAleatoireDansCercle",        &DEC_GeometryFunctions::ComputeRandomPointInCircle );
    brain.RegisterFunction( "DEC_Geometrie_CreerLocalisationCercle",            &DEC_GeometryFunctions::CreateCircleLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_PionEstCoordonne",                   &DEC_GeometryFunctions::ListUncoordinatedPawns );
    brain.RegisterFunction( "DEC_Geometrie_PourcentageTerrainCompartimente",    &DEC_GeometryFunctions::ComputeClosedTerrainRatioInFuseau );
    brain.RegisterFunction( "DEC_Geometrie_PourcentageTerrainOuvert",           &DEC_GeometryFunctions::ComputeOpenTerrainRatioInFuseau );
    brain.RegisterFunction( "DEC_Geometrie_TrierZonesSelonOuvertureTerrain",    &DEC_GeometryFunctions::SortZonesAccordingToTerrainOpening );
    brain.RegisterFunction( "DEC_Geometrie_PourcentageZoneTerrainCompartimente",&DEC_GeometryFunctions::ComputeClosedTerrainRatioInZone );
    brain.RegisterFunction( "DEC_Geometrie_PourcentageZoneTerrainOuvert",       &DEC_GeometryFunctions::ComputeOpenTerrainRatioInZone );
    brain.RegisterFunction( "DEC_Geometrie_TrierFuseauxSelonOuvertureTerrain",  &DEC_GeometryFunctions::SortFuseauxAccordingToTerrainOpening );
    brain.RegisterFunction( "DEC_Geometrie_ConvertirFuseauEnLocalisation",      &DEC_GeometryFunctions::ConvertFuseauToLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_ProchainObjectifDansFuseau",         &DEC_GeometryFunctions::GetNextObjectiveInFuseau );
    brain.RegisterFunction( "DEC_Geometrie_DistanceBetweenPoints",              &DEC_GeometryFunctions::ComputeDistance );
    brain.RegisterFunction( "DEC_Geometrie_AreaSize",                           &DEC_GeometryFunctions::ComputeAreaSize );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterAreaFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterAreaFunctions( directia::Brain& brain )
{
    brain.RegisterFunction( "DEC_BMArea_Barycenter", &DEC_GeometryFunctions::ComputeBarycenter );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterTimeManagementFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterTimeManagementFunctions( directia::Brain& brain )
{
    // $$$$ AGE 2007-10-11: Un seul temps
    brain.RegisterFunction( "DEC_TempsSim",  &DEC_DIAFunctions::GetSimTime );
    brain.RegisterFunction( "DEC_TempsReel", &DEC_DIAFunctions::GetRealTime );
    brain.RegisterFunction( "DEC_Nuit",      &DEC_DIAFunctions::IsNight );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterParametersCopyFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterParametersCopyFunctions( directia::Brain& brain )
{
    brain.RegisterFunction( "DEC_Copie_Point",                                      &DEC_DIAFunctions::CopyPoint );
    brain.RegisterFunction( "DEC_Copie_Point_Mission",                              &DEC_DIAFunctions::CopyPointMission );
    brain.RegisterFunction( "DEC_Copie_ListePoints_Mission",                        &DEC_DIAFunctions::CopyPathMission );
    brain.RegisterFunction( "DEC_Copie_PointDansListePoints_Mission",               &DEC_DIAFunctions::CopyPointToListPointMission );
    brain.RegisterFunction( "DEC_Copie_Localisation",                               &DEC_DIAFunctions::CopyLocalisation );
    brain.RegisterFunction( "DEC_Copie_Localisation_Mission",                       &DEC_DIAFunctions::CopyLocalisationMission );
    brain.RegisterFunction( "DEC_Copie_ListeLocalisations_Mission",                 &DEC_DIAFunctions::CopyLocalisationListMission );
    brain.RegisterFunction( "DEC_Copie_LocalisationDansListeLocalisations_Mission", &DEC_DIAFunctions::CopyLocalisationToLocationListMission );
    brain.RegisterFunction( "DEC_UserTypeList_PushBack_Mission",                    &DEC_DIAFunctions::CopyKnowledgeObjectToKnowledgeObjectListMission );
    brain.RegisterFunction( "DEC_GenObjectList_PushBack_Mission",                   &DEC_DIAFunctions::CopyGenObjectToGenObjectListMission );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterListsManipulationFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterListsManipulationFunctions( directia::Brain& brain )
{
    brain.RegisterFunction( "DEC_ListePoints_GetAt", &DEC_DIAFunctions::ListPoint_GetAt );
    brain.RegisterFunction( "DEC_ListePoints_Size",  &DEC_DIAFunctions::ListPoint_Size );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterLogisticFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterLogisticFunctions( directia::Brain& brain )
{
    brain.RegisterFunction( "DEC_NecessiteEvacuationBlesses",            &DEC_LogisticFunctions::HasWoundedHumansToEvacuate );
    brain.RegisterFunction( "DEC_EvacuerBlessesVersTC2",                 &DEC_LogisticFunctions::EvacuateWoundedHumansToTC2 );
    brain.RegisterFunction( "DEC_InterdireEvacuationAutomatiqueBlesses", &DEC_LogisticFunctions::ForbidWoundedHumansAutoEvacuation );
    brain.RegisterFunction( "DEC_AutoriserEvacuationAutomatiqueBlesses", &DEC_LogisticFunctions::AllowWoundedHumansAutoEvacuation );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterEngineerObjectsFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterEngineerObjectsFunctions( directia::Brain& brain )
{
    brain.RegisterFunction( "DEC_GenObject_Type",                  &DEC_ObjectFunctions::GetGenObjectType );
    brain.RegisterFunction( "DEC_GenObject_Localisation",          &DEC_ObjectFunctions::GetGenObjectLocalisation );
    brain.RegisterFunction( "DEC_GenObject_Densite",               &DEC_Gen_Object::GetDensity );
    brain.RegisterFunction( "DEC_GenObject_TypeObstacleManoeuvre", &DEC_ObjectFunctions::GetGenObjectReservedObstacle );
    brain.RegisterFunction( "DEC_GenObject_TC2",                   &DEC_ObjectFunctions::GetGenObjectTC2 );
    brain.RegisterFunction( "DEC_GenObject_DelaiActiviteMines",    &DEC_Gen_Object::GetMinesActivityTime );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterObjectsFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterObjectsFunctions( directia::Brain& brain )
{
    brain.RegisterFunction( "S_TypeObject_ToString", &DEC_ObjectFunctions::ConvertTypeObjectToString );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterObjectivesFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterObjectivesFunctions( directia::Brain& brain )
{
    brain.RegisterFunction( "DEC_Objectif_Localisation", &DEC_Objective::GetLocalisation );
    brain.RegisterFunction( "DEC_Objectif_Flag",         &DEC_Objective::Flag );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterSpecificPointsFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterSpecificPointsFunctions( directia::Brain& brain )
{
    brain.RegisterFunction( "DEC_GetRepPoint",      &DEC_PathFunctions::GetRepPoint );
    brain.RegisterFunction( "DEC_IsAvantPoint",     &DEC_PathFunctions::IsAvantPoint );
    brain.RegisterFunction( "DEC_IsPoint",          &DEC_PathFunctions::IsPoint );
    brain.RegisterFunction( "DEC_GetTypePoint",     &DEC_PathFunctions::GetTypePoint );
    brain.RegisterFunction( "DEC_GetDestPoint",     &DEC_PathFunctions::GetDestPoint );
    brain.RegisterFunction( "DEC_GetTypeLimaPoint", &DEC_PathFunctions::GetTypeLimaPoint );
    brain.RegisterFunction( "DEC_GetLimaPoint",     &DEC_PathFunctions::GetLimaPoint );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterTypeFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterTypeFunctions( directia::Brain& brain )
{
    brain.RegisterFunction( "GetType", &MIL_Mission_ABC::GetDIAType );
    brain.RegisterFunction( "GetType", &DEC_Decision_ABC::GetDIAType );
    brain.RegisterFunction( "GetType", &DEC_RolePion_Decision::GetDIAType );
    brain.RegisterFunction( "GetType", &DEC_AutomateDecision::GetDIAType );
    brain.RegisterFunction( "GetType", &DEC_PathPoint::GetDIAType );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterMissionParametersFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterMissionParametersFunctions( directia::Brain& brain )
{
    brain.RegisterFunction( "DEC_AssignMissionPionParameter",                &MIL_MissionParameterFactory::SetPawnParameter );
    brain.RegisterFunction( "DEC_AssignMissionAutomatParameter",             &MIL_MissionParameterFactory::SetAutomatParameter );
    brain.RegisterFunction( "DEC_AssignMissionBoolParameter",                &MIL_MissionParameterFactory::SetBoolParameter );
    brain.RegisterFunction( "DEC_AssignMissionObjectKnowledgeParameter",     &MIL_MissionParameterFactory::SetObjectKnowledgeParameter );
    brain.RegisterFunction( "DEC_AssignMissionNatureAtlasTypeParameter",     &MIL_MissionParameterFactory::SetNatureAtlasTypeParameter );
    brain.RegisterFunction( "DEC_AssignMissionAutomatListParameter",         &MIL_MissionParameterFactory::SetAutomatListParameter );
    brain.RegisterFunction( "DEC_AssignMissionAgentKnowledgeParameter",      &MIL_MissionParameterFactory::SetAgentKnowledgeParameter );
    brain.RegisterFunction( "DEC_AssignMissionPathListParameter",            &MIL_MissionParameterFactory::SetPathListParameter );
    brain.RegisterFunction( "DEC_AssignMissionEnumereParameter",             &MIL_MissionParameterFactory::SetEnumereParameter );
    brain.RegisterFunction( "DEC_AssignMissionLocationParameter",            &MIL_MissionParameterFactory::SetLocationParameter );
    brain.RegisterFunction( "DEC_AssignMissionObjectKnowledgeListParameter", &MIL_MissionParameterFactory::SetObjectKnowledgeListParameter );
    brain.RegisterFunction( "DEC_AssignMissionPointParameter",               &MIL_MissionParameterFactory::SetPointParameter );
    brain.RegisterFunction( "DEC_AssignMissionAgentKnowledgeListParameter",  &MIL_MissionParameterFactory::SetAgentKnowledgeListParameter );
    brain.RegisterFunction( "DEC_AssignMissionGenObjectListParameter",       &MIL_MissionParameterFactory::SetGenObjectListParameter );
    brain.RegisterFunction( "DEC_AssignMissionPionListParameter",            &MIL_MissionParameterFactory::SetPionListParameter );
    brain.RegisterFunction( "DEC_AssignMissionLocationListParameter",        &MIL_MissionParameterFactory::SetLocationListParameter );
    brain.RegisterFunction( "DEC_AssignMissionPointListParameter",           &MIL_MissionParameterFactory::SetPointListParameter );
    brain.RegisterFunction( "DEC_AssignMissionUrbanBlockParameter",          &MIL_MissionParameterFactory::SetUrbanBlockParameter );
    brain.RegisterFunction( "DEC_AssignMissionDirectionParameter",           &MIL_MissionParameterFactory::SetDirectionParameter );

    directia::ScriptRef initParameterFunction = brain.GetScriptFunction( "InitTaskParameter" );
    brain.RegisterFunction( "DEC_FillMissionParameters",
        boost::function< void( const directia::ScriptRef&, boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_MiscFunctions::FillMissionParameters, boost::ref( brain ), initParameterFunction, _1 , _2 ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterDebugFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterDebugFunctions( directia::Brain& brain, unsigned int id )
{
    brain.RegisterFunction( "DEC_PointToString",      &DEC_DIAFunctions::PointToString );
    brain.RegisterFunction( "DEC_DirectionToString",  &DEC_DIAFunctions::DirectionToString );
    brain.RegisterFunction( "DEC_ItineraireToString", &DEC_DIAFunctions::PathToString );
    brain.RegisterFunction( "BreakForDebug",
        boost::function< void( const std::string& ) >( boost::bind( &DEC_DIAFunctions::BreakForDebug, id ,_1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterTelepathyFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterTelepathyFunctions( directia::Brain& brain )
{
    brain.RegisterFunction( "SetStateVariable", &DEC_Decision_ABC::SetStateVariable );
    brain.RegisterFunction( "SetambianceMission_", &DEC_Decision_ABC::SetAmbianceMission );
    brain.RegisterFunction( "SetbAppuieFreinage_", &DEC_Decision_ABC::SetAppuieFreinage );
    brain.RegisterFunction( "GetbDemandeOrdreConduitePoursuivre_", &DEC_Decision_ABC::GetDemandeOrdreConduitePoursuivre );
    brain.RegisterFunction( "GetbEnCoursExtractionPersonnel_", &DEC_Decision_ABC::GetEnCoursExtractionPersonnel );
    brain.RegisterFunction( "GetbEnExploitation_", &DEC_Decision_ABC::GetEnExploitation );
    brain.RegisterFunction( "SetbEnExploitation_", &DEC_Decision_ABC::SetbEnExploitation );
    brain.RegisterFunction( "GetbEnPhaseRavitaillement_", &DEC_Decision_ABC::GetEnPhaseRavitaillement );
    brain.RegisterFunction( "SetbEnPhaseRavitaillement_", &DEC_Decision_ABC::SetEnPhaseRavitaillement );
    brain.RegisterFunction( "GetbMiseEnOeuvre_", &DEC_Decision_ABC::GetMiseEnOeuvre );
    brain.RegisterFunction( "SetbMiseEnOeuvre_", &DEC_Decision_ABC::SetMiseEnOeuvre );
    brain.RegisterFunction( "GeteEtatFeu_", &DEC_Decision_ABC::GetEtatFeu );
    brain.RegisterFunction( "GetlisteEnisTirAutorise_", &DEC_Decision_ABC::GetListeEnisTirAutorise );
    brain.RegisterFunction( "SetlisteEnisTirAutorise_", &DEC_Decision_ABC::SetListeEnisTirAutorise );
    brain.RegisterFunction( "ClearlisteEnisTirAutorise_", &DEC_Decision_ABC::ClearListeEnisTirAutorise );
    brain.RegisterFunction( "GetlistePionsCoordination_", &DEC_Decision_ABC::GetListePionsCoordination );
    brain.RegisterFunction( "ClearlistePionsCoordination_", &DEC_Decision_ABC::ClearListePionsCoordination );
    brain.RegisterFunction( "GetobjMisEnCours_", &DEC_Decision_ABC::GetObjMisEnCours );
    brain.RegisterFunction( "SetobjMisEnCours_", &DEC_Decision_ABC::SetObjMisEnCours );
    brain.RegisterFunction( "GetobjectifCourant_", &DEC_Decision_ABC::GetObjectifCourant );
    brain.RegisterFunction( "GetplotRavitaillementAssigne_", &DEC_Decision_ABC::GetPlotRavitaillementAssigne );
    brain.RegisterFunction( "SetplotRavitaillementAssigne_", &DEC_Decision_ABC::SetPlotRavitaillementAssigne );
    brain.RegisterFunction( "ClearplotsRavitaillement_", &DEC_Decision_ABC::ClearPlotsRavitaillement );
    brain.RegisterFunction( "GetporteeAction_", &DEC_Decision_ABC::GetPorteeAction );
    brain.RegisterFunction( "SetporteeAction_", &DEC_Decision_ABC::SetPorteeAction );
    brain.RegisterFunction( "GetrNiveauAlerteRavitaillement_", &DEC_Decision_ABC::GetNiveauAlerteRavitaillement );
    brain.RegisterFunction( "Getpoint_", &DEC_PathPoint::GetPos );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterItineraryFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterItineraryFunctions( directia::Brain& brain )
{
    brain.RegisterFunction( "DEC_Itineraire_DernierPoint", &DEC_Decision_ABC::GetLastPointOfPath );
    brain.RegisterFunction( "DEC_Itineraire_ExtrapolerPosition" , &DEC_Decision_ABC::ExtrapolatePosition );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterCommonUserFunctions
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
void RegisterCommonUserFunctions( directia::Brain& brain, unsigned int id )
{
    RegisterGeometryFunctions( brain );
    RegisterAreaFunctions( brain );
    RegisterTimeManagementFunctions( brain );
    RegisterParametersCopyFunctions( brain );
    RegisterListsManipulationFunctions( brain );
    RegisterLogisticFunctions( brain );
    RegisterEngineerObjectsFunctions( brain );
    RegisterObjectsFunctions( brain );
    RegisterObjectivesFunctions( brain );
    RegisterSpecificPointsFunctions( brain );
    RegisterTypeFunctions( brain );
    RegisterMissionParametersFunctions( brain );
    RegisterDebugFunctions( brain, id );
    RegisterTelepathyFunctions( brain );
    RegisterItineraryFunctions( brain );
    DEC_TelepathyFunctions::Register( brain );
    MIL_FragOrder::Register( brain );
}

typedef void ( *T_Function )( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element );
typedef void ( *T_FunctionBM )( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element );

void BoolFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    bool value;
    if( element.ToBool( value ) )
        refMission.RegisterObject( name, value );
}
void EnumerationFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    int value;
    if( element.ToId( value ) )
        refMission.RegisterObject( name, value );
}
void PointFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< MT_Vector2D > value;
    if( element.ToPoint( value ) && value.get() )
        refMission.RegisterObject( name, value );
}
void PointListFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPointList( value ) )
        refMission.RegisterObject( name, value );
}
void PointFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< MT_Vector2D > value;
    if( element.ToPoint( value ) && value.get() )
        knowledgeCreateFunction( refMission,  brain.GetScriptVariable( "net.masagroup.sword.military.world.Point" ) , name, value, false );
}
void PointListFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPointList( value ) )
        knowledgeCreateFunction( refMission, brain.GetScriptVariable( "net.masagroup.sword.military.world.Point" ), name, value, true );
}
void PolygonFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToPolygon( value ) && value.get() )
        refMission.RegisterObject( name, value );
}
void PolygonListFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToPolygonList( value ) )
        refMission.RegisterObject( name, value );
}
void AreaFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToPolygon( value ) && value.get() )
        knowledgeCreateFunction( refMission, brain.GetScriptVariable( "net.masagroup.sword.military.world.Area" ), name, value, false );
}
void AreaListFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToPolygonList( value ) )
        knowledgeCreateFunction( refMission, brain.GetScriptVariable( "net.masagroup.sword.military.world.Area" ), name, value, true );
}
void LocationFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToLocation( value ) && value.get() )
        refMission.RegisterObject( name, value );
}
void LocationListFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToLocationList( value ) )
        refMission.RegisterObject( name, value );
}
void PathFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPath( value ) && !value.empty() )
        refMission.RegisterObject( name, value );
}
void PathListFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > > value;
    if( element.ToPathList( value ) && !value.empty() )
        refMission.RegisterObject( name, value );
}
void PathFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPath( value ) && !value.empty() )
        knowledgeCreateFunction( refMission, brain.GetScriptVariable( "net.masagroup.sword.military.world.Point" ), name, value, true );
}
void DirectionFunction( const directia::ScriptRef& /*refMission*/, const std::string& /*name*/, MIL_MissionParameter_ABC& /*element*/ )
{
    // $$$$ LDC: FIXME The only existing Direction argument is dangerDirection_ which is never used by the brains.
}
void DirectionFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< MT_Vector2D > value;
    if( element.ToDirection( value ) && value.get() )
        knowledgeCreateFunction( refMission,  brain.GetScriptVariable( "net.masagroup.sword.military.world.Direction" ) , name, value, false );
}
void NatureAtlasFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    int value = 0;
    if( element.ToNatureAtlas( value ) )
        refMission.RegisterObject( name, value );
}
void AutomatFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Decision_ABC* value = 0;
    if( element.ToAutomat( value ) )
        refMission.RegisterObject( name, value );
}
void AutomatFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Decision_ABC* value = 0;
    if( element.ToAutomat( value ) )
        knowledgeCreateFunction( refMission, brain.GetScriptVariable( "net.masagroup.sword.military.world.Company" ), name, value, false );
}
void AutomatListFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAutomatList( value ) )
        refMission.RegisterObject( name, value );
}
void AutomatListFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAutomatList( value ) )
        knowledgeCreateFunction( refMission, brain.GetScriptVariable( "net.masagroup.sword.military.world.Company" ), name, value, true );
}
void AgentFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Decision_ABC* value = 0; // $$$$ LDC: Parfois on se sert de champs dessus comme eniEnCours_...
    if( element.ToAgent( value ) )
        refMission.RegisterObject( name, value );
}
void AgentFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Decision_ABC* value = 0; // $$$$ LDC: Parfois on se sert de champs dessus comme eniEnCours_...
    if( element.ToAgent( value ) )
        knowledgeCreateFunction( refMission, brain.GetScriptVariable( "net.masagroup.sword.military.world.PlatoonAlly" ), name, value, false );
}
void AgentListFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAgentList( value ) )
        refMission.RegisterObject( name, value );
}
void AgentListFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAgentList( value ) )
        knowledgeCreateFunction( refMission, brain.GetScriptVariable( "net.masagroup.sword.military.world.PlatoonAlly" ), name, value, true );
}
void AgentKnowledgeFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Agent > value;
    if( element.ToAgentKnowledge( value ) && value.get() )
        refMission.RegisterObject( name, value );
}
void AgentKnowledgeFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Agent > value;//@TODO SEE how to bind agent and knowledge agent with the same BM knowledge
    if( element.ToAgentKnowledge( value ) && value.get() )
        knowledgeCreateFunction( refMission, brain.GetScriptVariable( "net.masagroup.sword.military.world.Platoon" ), name, value, false );
}
void AgentKnowledgeListFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > value;
    if( element.ToAgentKnowledgeList( value ) )
        refMission.RegisterObject( name, value );
}
void AgentKnowledgeListFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > value;
    if( element.ToAgentKnowledgeList( value ) )
        knowledgeCreateFunction( refMission, brain.GetScriptVariable( "net.masagroup.sword.military.world.Platoon" ), name, value, true );
}
void ObjectKnowledgeFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Object > value;
    if( element.ToObjectKnowledge( value ) && value && value->IsValid() )
        refMission.RegisterObject( name, value );
}
void ObjectKnowledgeFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Object > value;
    if( element.ToObjectKnowledge( value ) && value )
        knowledgeCreateFunction( refMission, brain.GetScriptVariable( "net.masagroup.sword.military.world.Object" ), name, value, false );
}
void ObjectKnowledgeListFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > value;
    if( element.ToObjectKnowledgeList( value ) )
        refMission.RegisterObject( name, value );
}
void ObjectKnowledgeListFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > value;
    if( element.ToObjectKnowledgeList( value ) )
        knowledgeCreateFunction( refMission, brain.GetScriptVariable( "net.masagroup.sword.military.world.Object" ), name, value, true );
}
void PopulationKnowledgeFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Knowledge_Population* value = 0;//@TODO MGD why not shared_ptr
    if( element.ToPopulationKnowledge( value ) && value )
        refMission.RegisterObject( name, value->GetID() );
}
void PopulationKnowledgeFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Knowledge_Population* value = 0;
    if( element.ToPopulationKnowledge( value ) && value )
        knowledgeCreateFunction( refMission, brain.GetScriptVariable( "net.masagroup.sword.military.world.Population" ), name, value, false );//@TODO MGD Add CompositeReachable for Population?
}

void UrbanBlockFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Urban > value;
    if( element.ToUrbanBlock( value ) && value )
        knowledgeCreateFunction( refMission, brain.GetScriptVariable( "net.masagroup.sword.military.world.UrbanBlock" ), name, value, false );
}

void DotationTypeFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const PHY_DotationCategory* value = 0;
    if( element.ToDotationType( value ) && value )
        refMission.RegisterObject( name, value );
}
void EquipmentTypeFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const PHY_ComposanteTypePion* value = 0;
    if( element.ToEquipmentType( value ) && value )
        refMission.RegisterObject( name, value );
}
void GDHFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    float value = 0;
    if( element.ToGDH( value ) )
        refMission.RegisterObject( name, value );
}
void NumericFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    float value = 0;
    if( element.ToNumeric( value ) )
        refMission.RegisterObject( name, value );
}
void GenObjectFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Gen_Object > value;
    if( element.ToGenObject( value ) && value.get() )
        refMission.RegisterObject( name, value );
}
void GenObjectFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Gen_Object > value;
    if( element.ToGenObject( value ) && value.get() )
        knowledgeCreateFunction( refMission, brain.GetScriptVariable( "net.masagroup.sword.military.world.EngineerObject" ), name, value, true );
}
void GenObjectListFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Gen_Object > > value;
    if( element.ToGenObjectList( value ) )
        refMission.RegisterObject( name, value );
}
void GenObjectListFunctionBM( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Gen_Object > > value;
    if( element.ToGenObjectList( value ) )
        knowledgeCreateFunction( refMission, brain.GetScriptVariable( "net.masagroup.sword.military.world.EngineerObject" ), name, value, true );
}
void MaintenancePrioritiesFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    T_MaintenancePriorityVector value;
    if( element.ToMaintenancePriorities( value ) )
        refMission.RegisterObject( name, value );
}
void MedicalPrioritiesFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    T_MedicalPriorityVector value;
    if( element.ToMedicalPriorities( value ) )
        refMission.RegisterObject( name, value );
}
void IndirectFireFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    Common::MsgUnitFire fire;
    fire.set_oid( 0 );
    if( element.ToIndirectFire( fire ) && fire.oid() )
        refMission.RegisterObject( name, fire.oid() );
}
void StringFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::string value;
    if( element.ToString( value ) )
        refMission.RegisterObject( name, value );
}
void ObjectiveListFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Objective > > value;
    if( element.ToObjectiveList( value ) )
        refMission.RegisterObject( name, value );
}

std::map< std::string, T_Function > functors;
std::map< std::string, T_FunctionBM > functorsBM;

void InitFunctions()
{
    if( functors.empty() )
    {
        functors[ "bool" ] = BoolFunction;
        functors[ "string" ] = StringFunction;
        functors[ "enumeration" ] = EnumerationFunction;
        functors[ "datetime" ] = GDHFunction;
        functors[ "Point" ] = PointFunction;
        functors[ "PointList" ] = PointListFunction;
        functors[ "Polygon" ] = PolygonFunction;
        functors[ "PolygonList" ] = PolygonListFunction;
        functors[ "Location" ] = LocationFunction;
        functors[ "LocationList" ] = LocationListFunction;
        functors[ "Path" ] = PathFunction;
        functors[ "PathList" ] = PathListFunction;
        functors[ "Direction" ] = DirectionFunction;
        functors[ "NatureAtlas" ] = NatureAtlasFunction;
        functors[ "Automate" ] = AutomatFunction;
        functors[ "AutomateList" ] = AutomatListFunction;
        functors[ "Agent" ] = AgentFunction;
        functors[ "AgentList" ] = AgentListFunction;
        functors[ "AgentKnowledge" ] = AgentKnowledgeFunction;
        functors[ "AgentKnowledgeList" ] = AgentKnowledgeListFunction;
        functors[ "ObjectKnowledge" ] = ObjectKnowledgeFunction;
        functors[ "ObjectKnowledgeList" ] = ObjectKnowledgeListFunction;
        functors[ "PopulationKnowledge" ] = PopulationKnowledgeFunction;
        functors[ "DotationType" ] = DotationTypeFunction;
        functors[ "EquipmentType" ] = EquipmentTypeFunction;
        functors[ "GDH" ] = GDHFunction;
        functors[ "Numeric" ] = NumericFunction;
        functors[ "GenObject" ] = GenObjectFunction;
        functors[ "GenObjectList" ] = GenObjectListFunction;
        functors[ "MaintenancePriorities" ] = MaintenancePrioritiesFunction;
        functors[ "MedicalPriorities" ] = MedicalPrioritiesFunction;
        functors[ "IndirectFire" ] = IndirectFireFunction;
        functors[ "ObjectiveList" ] = ObjectiveListFunction;

        functorsBM[ "PointBM" ] = PointFunctionBM;
        functorsBM[ "PointListBM" ] = PointListFunctionBM;
        functorsBM[ "PathBM" ] = PathFunctionBM;
        functorsBM[ "AreaBM" ] = AreaFunctionBM;
        functorsBM[ "AreaListBM" ] = AreaListFunctionBM;
        functorsBM[ "AutomateBM" ] = AutomatFunctionBM;
        functorsBM[ "AutomateListBM" ] = AutomatListFunctionBM;
        functorsBM[ "AgentBM" ] = AgentFunctionBM;
        functorsBM[ "AgentListBM" ] = AgentListFunctionBM;
        functorsBM[ "AgentKnowledgeBM" ] = AgentKnowledgeFunctionBM;
        functorsBM[ "AgentKnowledgeListBM" ] = AgentKnowledgeListFunctionBM;
        functorsBM[ "ObjectKnowledgeBM" ] = ObjectKnowledgeFunctionBM;
        functorsBM[ "ObjectKnowledgeListBM" ] = ObjectKnowledgeListFunctionBM;
        functorsBM[ "PopulationKnowledgeBM" ] = PopulationKnowledgeFunctionBM;
        functorsBM[ "GenObjectBM" ] = GenObjectFunctionBM;
        functorsBM[ "GenObjectListBM" ] = GenObjectListFunctionBM;
        functorsBM[ "UrbanBlockBM" ] = UrbanBlockFunctionBM;
        functorsBM[ "DirectionBM" ] = DirectionFunctionBM;
    }
}

class RegisterMissionParameterVisitor : public MIL_MissionParameterVisitor_ABC
{
public:
    RegisterMissionParameterVisitor( const directia::Brain& brain, const directia::ScriptRef& refMission, directia::ScriptRef& knowledgeCreateFunction )
        : brain_                  ( brain )
        , refMission_             ( refMission )
        , knowledgeCreateFunction_( knowledgeCreateFunction )
    {
        // NOTHING
    }
    virtual ~RegisterMissionParameterVisitor()
    {
        // NOTHING
    }

    virtual void Accept( const std::string& dianame, const MIL_ParameterType_ABC& type, MIL_MissionParameter_ABC& element )
    {
        std::map< std::string, T_Function >::iterator itFind = functors.find( type.GetName() );
        if( itFind != functors.end() )
            functors[ type.GetName() ]( refMission_, dianame, element );
        else if( !!knowledgeCreateFunction_ )
            functorsBM[ type.GetName() ]( brain_, knowledgeCreateFunction_, refMission_, dianame, element );
    }

private:
     const directia::Brain& brain_;
     const directia::ScriptRef& refMission_;
     directia::ScriptRef& knowledgeCreateFunction_;
};

// -----------------------------------------------------------------------------
// Name: RegisterMissionParameters
// Created: LDC 2009-05-04
// -----------------------------------------------------------------------------
void RegisterMissionParameters( const directia::Brain& brain, directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    InitFunctions();
    RegisterMissionParameterVisitor visitor( brain, refMission, knowledgeCreateFunction );
    mission->Visit( visitor );
}

}
