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
#include "Decision/DEC_OrdersFunctions.h"
#include "Decision/DEC_PathFunctions.h"
#include "Decision/DEC_TelepathyFunctions.h"
#include "Decision/DEC_Gen_Object.h"
#include "Decision/DEC_PathPoint.h"
#include "Decision/DEC_KnowledgeUrbanFunctions.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_FragOrder.h"
#include "Entities/Orders/MIL_MissionParameterFactory.h"
#include "Entities/Orders/MIL_MissionParameterVisitor_ABC.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/DEC_Knowledge_Urban.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "protocol/protocol.h"
#include <geometry/Point2.h>
#include <boost/bind.hpp>
#include <directia/tools/binders/ScriptRef.h>

// -----------------------------------------------------------------------------
// Name: ScriptRefs
// Created: LDC 2009-04-08
// -----------------------------------------------------------------------------
ScriptRefs::ScriptRefs( directia::brain::Brain& brain  )
    : sendEvent_               ( brain[ "SendEvent" ] )
    , startEvent_              ( brain[ "StartEvent" ] )
    , stopEvents_              ( brain[ "StopEvents" ] )
    , setStateVariable_        ( brain[ "SetStateVariable" ] )
    , collectgarbage_          ( brain[ "collectgarbage" ] )
    , step_                    ( brain[ "step" ] )
    , callbackPerception_      ( brain[ "CallbackPerception" ] )
    , knowledgeCallbackAction_ ( brain[ "KnowledgeCallbackAction" ] )
    , removeAction_            ( brain[ "RemoveAction" ] )
    , initTaskParameter_       ( brain[ "InitTaskParameter" ] )
    , cleanBrainBeforeDeletion_( brain[ "CleanBrainBeforeDeletion" ] )
{
    // NOTHING
}

namespace DEC_DecisionImpl
{

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterGeometryFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterGeometryFunctions( directia::brain::Brain& brain)
{
    brain[ "DEC_Geometrie_DecouperListePoints" ] = &DEC_GeometryFunctions::SplitListPoints;
    brain[ "DEC_Geometrie_CalculerPositionCouverture" ] = &DEC_GeometryFunctions::ComputeCoverPosition;
    brain[ "DEC_Geometrie_CalculerBarycentreAgents" ] = &DEC_GeometryFunctions::ComputeAgentsBarycenter;
    brain[ "DEC_Geometrie_PositionTranslate" ] = &DEC_GeometryFunctions::TranslatePosition;
    brain[ "DEC_Geometrie_PositionTranslateDir" ] = &DEC_GeometryFunctions::TranslatePositionInDirection;
    brain[ "DEC_Geometrie_PositionsEgales" ] = &DEC_GeometryFunctions::ComparePositions;
    brain[ "DEC_Geometrie_Distance" ] = &DEC_GeometryFunctions::Distance;//@TODO MGD Replace by ComputeDistance on shared_ptr
    brain[ "DEC_Geometrie_ConvertirPointEnLocalisation" ] = &DEC_GeometryFunctions::ConvertPointToLocalisation;
    brain[ "DEC_Geometrie_EstPointDansLocalisation" ] = &DEC_GeometryFunctions::IsPointInsideLocalisation;
    brain[ "DEC_Geometrie_CreerLocalisation" ] = &DEC_GeometryFunctions::CreateLocalisation;
    brain[ "DEC_Geometrie_CreerListePoints" ] = &DEC_GeometryFunctions::CreateListPoint;
    brain[ "DEC_Geometrie_CreerPoint" ] = &DEC_GeometryFunctions::CreatePoint;
	brain[ "DEC_Geometrie_CopiePoint" ] = &DEC_GeometryFunctions::CopyPoint;
    brain[ "DEC_Geometrie_CreerDirection" ] = &DEC_GeometryFunctions::CreateDirection;
    brain[ "DEC_Geometrie_CreerDirectionPerpendiculaire" ] = &DEC_GeometryFunctions::CreateOrthoDirection;
    brain[ "DEC_Geometrie_InverseDirection" ] = &DEC_GeometryFunctions::ReverseDirection;
    brain[ "DEC_Geometrie_CopieEtInverseDirection" ] = &DEC_GeometryFunctions::CopyAndReverseDirection;
    brain[ "DEC_Geometrie_CopieEtRotateDirection" ] = &DEC_GeometryFunctions::CopyAndRotateDirection;
    brain[ "DEC_Geometrie_CalculerDistanceLigneMoyenne" ] = &DEC_GeometryFunctions::ComputeDistanceFromMiddleLine;
    brain[ "DEC_Geometrie_CalculerBarycentreLocalisation" ] = &DEC_GeometryFunctions::ComputeLocalisationBarycenter;
    brain[ "DEC_Geometrie_DirectionMoyenne" ] = &DEC_GeometryFunctions::ComputeMeanDirection;
    brain[ "DEC_Geometrie_PositionAleatoireSurCercle" ] = &DEC_GeometryFunctions::ComputeRandomPointOnCircle;
    brain[ "DEC_Geometrie_PositionAleatoireDansCercle" ] = &DEC_GeometryFunctions::ComputeRandomPointInCircle;
    brain[ "DEC_Geometrie_CreerLocalisationCercle" ] = &DEC_GeometryFunctions::CreateCircleLocalisation;
    brain[ "DEC_Geometrie_PionEstCoordonne" ] = &DEC_GeometryFunctions::ListUncoordinatedPawns;
    brain[ "DEC_Geometrie_PourcentageTerrainCompartimente" ] = &DEC_GeometryFunctions::ComputeClosedTerrainRatioInFuseau;
    brain[ "DEC_Geometrie_PourcentageTerrainOuvert" ] = &DEC_GeometryFunctions::ComputeOpenTerrainRatioInFuseau;
    brain[ "DEC_Geometrie_TrierZonesSelonOuvertureTerrain" ] = &DEC_GeometryFunctions::SortZonesAccordingToTerrainOpening;
    brain[ "DEC_Geometrie_PourcentageZoneTerrainCompartimente" ] = &DEC_GeometryFunctions::ComputeClosedTerrainRatioInZone;
    brain[ "DEC_Geometrie_PourcentageZoneTerrainOuvert" ] = &DEC_GeometryFunctions::ComputeOpenTerrainRatioInZone;
    brain[ "DEC_Geometrie_TrierFuseauxSelonOuvertureTerrain" ] = &DEC_GeometryFunctions::SortFuseauxAccordingToTerrainOpening;
    brain[ "DEC_Geometrie_ConvertirFuseauEnLocalisation" ] = &DEC_GeometryFunctions::ConvertFuseauToLocalisation;
    brain[ "DEC_Geometrie_ProchainObjectifDansFuseau" ] = &DEC_GeometryFunctions::GetNextObjectiveInFuseau;
    brain[ "DEC_Geometrie_DistanceBetweenPoints" ] = &DEC_GeometryFunctions::ComputeDistance;
    brain[ "DEC_Geometrie_AreaSize" ] = &DEC_GeometryFunctions::ComputeAreaSize;
    brain[ "DEC_Geometrie_AreaDiameter" ] = &DEC_GeometryFunctions::ComputeAreaDiameter;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterUrbanBlockFunctions
// Created: LMT 2010-09-15
// -----------------------------------------------------------------------------
void RegisterUrbanBlockFunctions( directia::brain::Brain& brain )
{
   brain[ "DEC_ConnaissanceUrbanBlock_Barycentre" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Urban > ) >( boost::bind( &DEC_KnowledgeUrbanFunctions::GetCurrentBarycenter, _1 ) );
 
}
   

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterAreaFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterAreaFunctions( directia::brain::Brain& brain )
{
    brain[ "DEC_BMArea_Barycenter" ] = &DEC_GeometryFunctions::ComputeBarycenter;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterTimeManagementFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterTimeManagementFunctions( directia::brain::Brain& brain )
{
    // $$$$ AGE 2007-10-11: Un seul temps
    brain[ "DEC_TempsSim" ] = &DEC_DIAFunctions::GetSimTime;
    brain[ "DEC_TempsReel" ] = &DEC_DIAFunctions::GetRealTime;
    brain[ "DEC_Nuit" ] = &DEC_DIAFunctions::IsNight;
}
// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterParametersCopyFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterParametersCopyFunctions( directia::brain::Brain& brain )
{
    brain[ "DEC_Copie_Point" ] = &DEC_DIAFunctions::CopyPoint;
    brain[ "DEC_Copie_Point_Mission" ] = &DEC_DIAFunctions::CopyPointMission;
    brain[ "DEC_Copie_ListePoints_Mission" ] = &DEC_DIAFunctions::CopyPathMission;
    brain[ "DEC_Copie_PointDansListePoints_Mission" ] = &DEC_DIAFunctions::CopyPointToListPointMission;
    brain[ "DEC_Copie_Localisation" ] = &DEC_DIAFunctions::CopyLocalisation;
    brain[ "DEC_Copie_Localisation_Mission" ] = &DEC_DIAFunctions::CopyLocalisationMission;
    brain[ "DEC_Copie_ListeLocalisations_Mission" ] = &DEC_DIAFunctions::CopyLocalisationListMission;
    brain[ "DEC_Copie_LocalisationDansListeLocalisations_Mission" ] = &DEC_DIAFunctions::CopyLocalisationToLocationListMission;
    brain[ "DEC_UserTypeList_PushBack_Mission" ] = &DEC_DIAFunctions::CopyKnowledgeObjectToKnowledgeObjectListMission;
    brain[ "DEC_GenObjectList_PushBack_Mission" ] = &DEC_DIAFunctions::CopyGenObjectToGenObjectListMission;
}


// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterListsManipulationFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterListsManipulationFunctions( directia::brain::Brain& brain )
{
    brain[ "DEC_ListePoints_GetAt" ] = &DEC_DIAFunctions::ListPoint_GetAt;
    brain[ "DEC_ListePoints_Size" ] = &DEC_DIAFunctions::ListPoint_Size;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterLogisticFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterLogisticFunctions( directia::brain::Brain& brain )
{
    brain[ "DEC_NecessiteEvacuationBlesses" ] = &DEC_LogisticFunctions::HasWoundedHumansToEvacuate;
    brain[ "DEC_EvacuerBlessesVersTC2" ] = &DEC_LogisticFunctions::EvacuateWoundedHumansToTC2;
    brain[ "DEC_InterdireEvacuationAutomatiqueBlesses" ] = &DEC_LogisticFunctions::ForbidWoundedHumansAutoEvacuation;
    brain[ "DEC_AutoriserEvacuationAutomatiqueBlesses" ] = &DEC_LogisticFunctions::AllowWoundedHumansAutoEvacuation;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterEngineerObjectsFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterEngineerObjectsFunctions( directia::brain::Brain& brain )
{
    brain[ "DEC_GenObject_Type" ] = &DEC_ObjectFunctions::GetGenObjectType;
    brain[ "DEC_GenObject_Localisation" ] = &DEC_ObjectFunctions::GetGenObjectLocalisation;
    brain.Register( "DEC_GenObject_Densite",               &DEC_Gen_Object::GetDensity );
    brain[ "DEC_GenObject_TypeObstacleManoeuvre" ] = &DEC_ObjectFunctions::GetGenObjectReservedObstacle;
    brain[ "DEC_GenObject_TC2" ] = &DEC_ObjectFunctions::GetGenObjectTC2;
    brain.Register( "DEC_GenObject_DelaiActiviteMines",    &DEC_Gen_Object::GetMinesActivityTime );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterObjectsFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterObjectsFunctions( directia::brain::Brain& brain )
{
    brain[ "S_TypeObject_ToString" ] = &DEC_ObjectFunctions::ConvertTypeObjectToString;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterObjectivesFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterObjectivesFunctions( directia::brain::Brain& brain )
{
    brain.Register( "DEC_Objectif_Localisation", &DEC_Objective::GetLocalisation );
    brain.Register( "DEC_Objectif_Flag",         &DEC_Objective::Flag );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterSpecificPointsFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterSpecificPointsFunctions( directia::brain::Brain& brain )
{
    //Rep_Points
    brain[ "DEC_GetRepPoint" ] = &DEC_PathFunctions::GetRepPoint;
    brain[ "DEC_IsAvantPoint" ] = &DEC_PathFunctions::IsAvantPoint;
    brain[ "DEC_IsPoint" ] = &DEC_PathFunctions::IsPoint;
    brain[ "DEC_GetTypePoint" ] = &DEC_PathFunctions::GetTypePoint;
    brain[ "DEC_GetDestPoint" ] = &DEC_PathFunctions::GetDestPoint;
    brain[ "DEC_GetTypeLimaPoint" ] = &DEC_PathFunctions::GetTypeLimaPoint;
    brain[ "DEC_GetLimaPoint" ] = &DEC_PathFunctions::GetLimaPoint;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterTypeFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterTypeFunctions( directia::brain::Brain& brain )
{
    brain.Register( "GetType", &MIL_Mission_ABC::GetDIAType );
    brain.Register( "GetType", &DEC_Decision_ABC::GetDIAType );
    brain.Register( "GetType", &DEC_RolePion_Decision::GetDIAType );
    brain.Register( "GetType", &DEC_AutomateDecision::GetDIAType );
    brain.Register( "GetType", &DEC_PathPoint::GetDIAType );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterMissionParametersFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterMissionParametersFunctions( directia::brain::Brain& brain )
{
    brain[ "DEC_AssignMissionPionParameter" ] = &MIL_MissionParameterFactory::SetPawnParameter;
    brain[ "DEC_AssignMissionAutomatParameter" ] = &MIL_MissionParameterFactory::SetAutomatParameter;
    brain[ "DEC_AssignMissionBoolParameter" ] = &MIL_MissionParameterFactory::SetBoolParameter;
    brain[ "DEC_AssignMissionObjectKnowledgeParameter" ] = &MIL_MissionParameterFactory::SetObjectKnowledgeParameter;
    brain[ "DEC_AssignMissionNatureAtlasTypeParameter" ] = &MIL_MissionParameterFactory::SetNatureAtlasTypeParameter;
    brain[ "DEC_AssignMissionAutomatListParameter" ] = &MIL_MissionParameterFactory::SetAutomatListParameter;
    brain[ "DEC_AssignMissionAgentKnowledgeParameter" ] = &MIL_MissionParameterFactory::SetAgentKnowledgeParameter;
    brain[ "DEC_AssignMissionPathListParameter" ] = &MIL_MissionParameterFactory::SetPathListParameter;
    brain[ "DEC_AssignMissionEnumereParameter" ] = &MIL_MissionParameterFactory::SetEnumereParameter;
    brain[ "DEC_AssignMissionLocationParameter" ] = &MIL_MissionParameterFactory::SetLocationParameter;
    brain[ "DEC_AssignMissionObjectKnowledgeListParameter" ] = &MIL_MissionParameterFactory::SetObjectKnowledgeListParameter;
    brain[ "DEC_AssignMissionPointParameter" ] = &MIL_MissionParameterFactory::SetPointParameter;
    brain[ "DEC_AssignMissionAgentKnowledgeListParameter" ] = &MIL_MissionParameterFactory::SetAgentKnowledgeListParameter;
    brain[ "DEC_AssignMissionGenObjectListParameter" ] = &MIL_MissionParameterFactory::SetGenObjectListParameter;
    brain[ "DEC_AssignMissionPionListParameter" ] = &MIL_MissionParameterFactory::SetPionListParameter;
    brain[ "DEC_AssignMissionLocationListParameter" ] = &MIL_MissionParameterFactory::SetLocationListParameter;
    brain[ "DEC_AssignMissionPointListParameter" ] = &MIL_MissionParameterFactory::SetPointListParameter;
    brain[ "DEC_AssignMissionUrbanBlockParameter" ] = &MIL_MissionParameterFactory::SetUrbanBlockParameter;
    brain[ "DEC_AssignMissionDirectionParameter" ] = &MIL_MissionParameterFactory::SetDirectionParameter;
    brain[ "DEC_IsMissionAvailable" ] = &DEC_OrdersFunctions::IsMissionAvailable;

    directia::tools::binders::ScriptRef initParameterFunction = brain[ "InitTaskParameter" ];
    brain[ "DEC_FillMissionParameters" ] =
        boost::function< void( const directia::tools::binders::ScriptRef&, boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_MiscFunctions::FillMissionParameters, boost::ref(brain), initParameterFunction, _1 , _2 ) );
}
// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterDebugFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterDebugFunctions( directia::brain::Brain& brain, unsigned int id )
{
    brain[ "DEC_PointToString" ] = &DEC_DIAFunctions::PointToString;
    brain[ "DEC_DirectionToString" ] =  &DEC_DIAFunctions::DirectionToString ;
    brain[ "DEC_ItineraireToString" ] = &DEC_DIAFunctions::PathToString ;
    brain[ "BreakForDebug" ] =
        boost::function< void( const std::string& ) >( boost::bind( &DEC_DIAFunctions::BreakForDebug, id ,_1 ) ) ;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterTelepathyFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterTelepathyFunctions( directia::brain::Brain& brain )
{
    brain.Register( "SetStateVariable", &DEC_Decision_ABC::SetStateVariable );
    brain.Register( "SetambianceMission_", &DEC_Decision_ABC::SetAmbianceMission );
    brain.Register( "SetbAppuieFreinage_", &DEC_Decision_ABC::SetAppuieFreinage );
    brain.Register( "GetbDemandeOrdreConduitePoursuivre_", &DEC_Decision_ABC::GetDemandeOrdreConduitePoursuivre );
    brain.Register( "GetbEnCoursExtractionPersonnel_", &DEC_Decision_ABC::GetEnCoursExtractionPersonnel );
    brain.Register( "GetbEnExploitation_", &DEC_Decision_ABC::GetEnExploitation );
    brain.Register( "SetbEnExploitation_", &DEC_Decision_ABC::SetbEnExploitation );
    brain.Register( "GetbEnPhaseRavitaillement_", &DEC_Decision_ABC::GetEnPhaseRavitaillement );
    brain.Register( "SetbEnPhaseRavitaillement_", &DEC_Decision_ABC::SetEnPhaseRavitaillement );
    brain.Register( "GetbMiseEnOeuvre_", &DEC_Decision_ABC::GetMiseEnOeuvre );
    brain.Register( "SetbMiseEnOeuvre_", &DEC_Decision_ABC::SetMiseEnOeuvre );
    brain.Register( "GeteEtatFeu_", &DEC_Decision_ABC::GetEtatFeu );
    brain.Register( "GetlisteEnisTirAutorise_", &DEC_Decision_ABC::GetListeEnisTirAutorise );
    brain.Register( "SetlisteEnisTirAutorise_", &DEC_Decision_ABC::SetListeEnisTirAutorise );
    brain.Register( "ClearlisteEnisTirAutorise_", &DEC_Decision_ABC::ClearListeEnisTirAutorise );
    brain.Register( "GetlistePionsCoordination_", &DEC_Decision_ABC::GetListePionsCoordination );
    brain.Register( "ClearlistePionsCoordination_", &DEC_Decision_ABC::ClearListePionsCoordination );
    brain.Register( "GetobjMisEnCours_", &DEC_Decision_ABC::GetObjMisEnCours );
    brain.Register( "SetobjMisEnCours_", &DEC_Decision_ABC::SetObjMisEnCours );
    brain.Register( "GetobjectifCourant_", &DEC_Decision_ABC::GetObjectifCourant );
    brain.Register( "GetplotRavitaillementAssigne_", &DEC_Decision_ABC::GetPlotRavitaillementAssigne );
    brain.Register( "SetplotRavitaillementAssigne_", &DEC_Decision_ABC::SetPlotRavitaillementAssigne );
    brain.Register( "ClearplotsRavitaillement_", &DEC_Decision_ABC::ClearPlotsRavitaillement );
    brain.Register( "GetporteeAction_", &DEC_Decision_ABC::GetPorteeAction );
    brain.Register( "SetporteeAction_", &DEC_Decision_ABC::SetPorteeAction );
    brain.Register( "GetrNiveauAlerteRavitaillement_", &DEC_Decision_ABC::GetNiveauAlerteRavitaillement );
    brain.Register( "Getpoint_", &DEC_PathPoint::GetPos );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterItineraryFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterItineraryFunctions( directia::brain::Brain& brain )
{
    brain.Register( "DEC_Itineraire_DernierPoint", &DEC_Decision_ABC::GetLastPointOfPath );
    brain.Register( "DEC_Itineraire_ExtrapolerPosition", &DEC_Decision_ABC::ExtrapolatePosition );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterCommonUserFunctions
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
void RegisterCommonUserFunctions( directia::brain::Brain& brain/*, unsigned int id*/ )
{
    RegisterGeometryFunctions( brain );
    RegisterUrbanBlockFunctions( brain );
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
    //RegisterDebugFunctions( brain, id );
    RegisterTelepathyFunctions( brain );
    RegisterItineraryFunctions( brain );
    DEC_TelepathyFunctions::Register( brain );
    MIL_FragOrder::Register( brain );
}

typedef void (*T_Function)( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element );
typedef bool (*T_FunctionBM)( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element );

void BoolFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    bool value;
    if( element.ToBool( value ) )
        refMission[ name ] = value;
}
void EnumerationFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    int value;
    if( element.ToId( value ) )
        refMission[ name ] = value;
}
void PointFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< MT_Vector2D > value;
    if( element.ToPoint( value ) && value.get() )
        refMission[ name ] = value;
}
void PointListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPointList( value ) )
        refMission[ name ] = value;
}
bool PointFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< MT_Vector2D > value;
    if( element.ToPoint( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.Point" ] , name, value, false );     
        return true;
    }
    return false;
}

bool PointListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPointList( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.Point" ], name, value, true );
        return true;
    }
    return false;
}
void PolygonFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToPolygon( value ) && value.get() )
        refMission[ name ] = value;
}
void PolygonListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToPolygonList( value ) )
        refMission[ name ] = value;
}
bool AreaFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToPolygon( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.Area" ], name, value, false );
	    return true;
    }
    return false;
}
bool AreaListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToPolygonList( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.Area" ], name, value, true );
	    return true;
    }
    return false;
}
void LocationFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToLocation( value ) && value.get() )
        refMission[ name ] = value;
}
void LocationListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToLocationList( value ) )
        refMission[ name ] = value;
}
void PathFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPath( value ) && !value.empty() )
        refMission[ name ] = value;
}
void PathListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > > value;
    if( element.ToPathList( value ) && !value.empty() )
        refMission[ name ] = value;
}
bool PathFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPath( value ) && !value.empty() )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.Point" ], name, value, true );       
	    return true;
    }
    return false;
}
void DirectionFunction( const directia::tools::binders::ScriptRef& /*refMission*/, const std::string& /*name*/, MIL_MissionParameter_ABC& /*element*/ )
{
    // $$$$ LDC: FIXME The only existing Direction argument is dangerDirection_ which is never used by the brains.
}
bool DirectionFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< MT_Vector2D > value;
    if( element.ToDirection( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission,  brain[ "net.masagroup.sword.military.world.Direction" ] , name, value, false );     
	    return true;
    }
    return false;
}
void NatureAtlasFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    int value = 0;
    if( element.ToNatureAtlas( value ) )
        refMission[ name ] = value;
}
void AutomatFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Decision_ABC* value = 0;
    if( element.ToAutomat( value ) )
        refMission[ name ] = value;
}
bool AutomatFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Decision_ABC* value = 0;
    if( element.ToAutomat( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.Company" ], name, value, false );
	    return true;
    }
    return false;
}
void AutomatListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAutomatList( value ) )
        refMission[ name ] = value;
}
bool AutomatListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAutomatList( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.Company" ], name, value, true );
	    return true;
    }
    return false;
}
void AgentFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Decision_ABC* value = 0; // $$$$ LDC: Parfois on se sert de champs dessus comme eniEnCours_...
    if( element.ToAgent( value ) )
        refMission[ name ] = value;
}
bool AgentFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Decision_ABC* value = 0; // $$$$ LDC: Parfois on se sert de champs dessus comme eniEnCours_...
    if( element.ToAgent( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.PlatoonAlly" ], name, value, false );
	    return true;
    }
    return false;
}
void AgentListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAgentList( value ) )
        refMission[ name ] = value;
}
bool AgentListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAgentList( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.PlatoonAlly" ], name, value, true );
	    return true;
    }
    return false;
}
void AgentKnowledgeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Agent > value;
    if( element.ToAgentKnowledge( value ) && value.get() )
        refMission[ name ] = value;
}
bool AgentKnowledgeFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Agent > value;//@TODO SEE how to bind agent and knowledge agent with the same BM knowledge
    if( element.ToAgentKnowledge( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.Platoon" ], name, value, false );
	    return true;
    }
    return false;
}
void AgentKnowledgeListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > value;
    if( element.ToAgentKnowledgeList( value ) )
        refMission[ name ] = value;
}
bool AgentKnowledgeListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > value;
    if( element.ToAgentKnowledgeList( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.Platoon" ], name, value, true );
	    return true;
    }
    return false;
}
void ObjectKnowledgeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Object > value;
    if( element.ToObjectKnowledge( value ) && value && value->IsValid() )
        refMission[ name ] = value;
}
bool ObjectKnowledgeFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Object > value;
    if( element.ToObjectKnowledge( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.Object" ], name, value, false );
	    return true;
    }
    return false;
}
void ObjectKnowledgeListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > value;
    if( element.ToObjectKnowledgeList( value ) )
        refMission[ name ] = value;
}
bool ObjectKnowledgeListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > value;
    if( element.ToObjectKnowledgeList( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.Object" ], name, value, true );
        return true;
    }
    return false;
}
void PopulationKnowledgeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Knowledge_Population* value = 0;//@TODO MGD why not shared_ptr
    if( element.ToPopulationKnowledge( value ) && value )
        refMission[ name ] = value->GetID();
}
bool PopulationKnowledgeFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Knowledge_Population* value = 0;
    if( element.ToPopulationKnowledge( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.Population" ], name, value, false );//@TODO MGD Add CompositeReachable for Population?
         return true;
    }
    return false;
}

bool UrbanBlockFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Urban > value;
    if( element.ToUrbanBlock( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.UrbanBlock" ], name, value, false );  
        return true;
    }
    return false;
}

void DotationTypeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const PHY_DotationCategory* value = 0;
    if( element.ToDotationType( value ) && value )
        refMission[ name ] = value;
}
void EquipmentTypeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const PHY_ComposanteTypePion* value = 0;
    if( element.ToEquipmentType( value ) && value )
        refMission[ name ] = value;
}
void GDHFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    float value = 0;
    if( element.ToGDH( value ) )
        refMission[ name ] = value;
}
void NumericFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    float value = 0;
    if( element.ToNumeric( value ) )
        refMission[ name ] = value;
}
void GenObjectFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Gen_Object > value;
    if( element.ToGenObject( value ) && value.get() )
        refMission[ name ] = value;
}
bool GenObjectFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Gen_Object > value;
    if( element.ToGenObject( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.EngineerObject" ], name, value, true );
        return true;
    }
    return false;
}
void GenObjectListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Gen_Object > > value;
    if( element.ToGenObjectList( value ) )
        refMission[ name ] = value;
}
bool GenObjectListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Gen_Object > > value;
    if( element.ToGenObjectList( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "net.masagroup.sword.military.world.EngineerObject" ], name, value, true );
        return true;
    }
    return false;
}
void MaintenancePrioritiesFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_ComposanteTypePion* > value;
    if( element.ToMaintenancePriorities( value ) )
        refMission[ name ] = value ;
}
void MedicalPrioritiesFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_HumanWound* > value;
    if( element.ToMedicalPriorities( value ) )
        refMission[ name ] = value ;
}
void IndirectFireFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    Common::FireId fire;
    fire.set_id( 0 );
    if( element.ToIndirectFire( fire ) && fire.id() )
        refMission[ name ] = fire.id();
}
void StringFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::string value;
    if( element.ToString( value ) )
        refMission[ name ] = value;
}
void ObjectiveListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Objective > > value;
    if( element.ToObjectiveList( value ) )
        refMission[ name ] = value;
}
bool LocationCompositeFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    return PointFunctionBM( brain, knowledgeCreateFunction, refMission, name, element )
        || PathFunctionBM( brain, knowledgeCreateFunction, refMission, name, element )
        || AreaFunctionBM( brain, knowledgeCreateFunction, refMission, name, element )
        || AutomatFunctionBM( brain, knowledgeCreateFunction, refMission, name, element )
        || AgentFunctionBM( brain, knowledgeCreateFunction, refMission, name, element )
        || AgentKnowledgeFunctionBM( brain, knowledgeCreateFunction, refMission, name, element )
        || ObjectKnowledgeFunctionBM( brain, knowledgeCreateFunction, refMission, name, element )
        || PopulationKnowledgeFunctionBM( brain, knowledgeCreateFunction, refMission, name, element )
        || UrbanBlockFunctionBM( brain, knowledgeCreateFunction, refMission, name, element );
}

bool LocationCompositeListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > list;
    if( element.ToList( list ) )
    {
        knowledgeCreateFunction( refMission, "beginlist", name );
        for( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >::const_iterator it = list.begin(); it != list.end(); ++it )
        {
            PointFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it )
        || PathFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it )
        || AreaFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it )
        || AutomatFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it )
        || AgentFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it )
        || AgentKnowledgeFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it )
        || ObjectKnowledgeFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it )
        || PopulationKnowledgeFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it )
        || UrbanBlockFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it );
        }
        knowledgeCreateFunction( refMission, "endlist", name );
        return true;
    }
    return false;
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
        functors[ "Heading" ] = DirectionFunction;
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
        functorsBM[ "PointBMList" ] = PointListFunctionBM;
        functorsBM[ "PathBM" ] = PathFunctionBM;
        functorsBM[ "AreaBM" ] = AreaFunctionBM;
        functorsBM[ "AreaBMList" ] = AreaListFunctionBM;
        functorsBM[ "AutomateBM" ] = AutomatFunctionBM;
        functorsBM[ "AutomateBMList" ] = AutomatListFunctionBM;
        functorsBM[ "AgentBM" ] = AgentFunctionBM;
        functorsBM[ "AgentBMList" ] = AgentListFunctionBM;
        functorsBM[ "AgentKnowledgeBM" ] = AgentKnowledgeFunctionBM;
        functorsBM[ "AgentKnowledgeBMList" ] = AgentKnowledgeListFunctionBM;
        functorsBM[ "ObjectKnowledgeBM" ] = ObjectKnowledgeFunctionBM;
        functorsBM[ "ObjectKnowledgeBMList" ] = ObjectKnowledgeListFunctionBM;
        functorsBM[ "PopulationKnowledgeBM" ] = PopulationKnowledgeFunctionBM;
        functorsBM[ "GenObjectBM" ] = GenObjectFunctionBM;
        functorsBM[ "GenObjectBMList" ] = GenObjectListFunctionBM;
        functorsBM[ "UrbanBlockBM" ] = UrbanBlockFunctionBM;
        functorsBM[ "HeadingBM" ] = DirectionFunctionBM;
        functorsBM[ "LocationComposite" ] = LocationCompositeFunctionBM;
        functorsBM[ "LocationCompositeList" ] = LocationCompositeListFunctionBM;
    }
}


class RegisterMissionParameterVisitor : public MIL_MissionParameterVisitor_ABC
{
public:
    RegisterMissionParameterVisitor( directia::brain::Brain& brain, const directia::tools::binders::ScriptRef& refMission, directia::tools::binders::ScriptRef& knowledgeCreateFunction ) 
        : brain_( brain )
        , refMission_( refMission )
        , knowledgeCreateFunction_( knowledgeCreateFunction )
    {}
    virtual ~RegisterMissionParameterVisitor()
    {}

    virtual void Accept( const std::string& dianame, const MIL_ParameterType_ABC& type, MIL_MissionParameter_ABC& element )
    {
        std::map< std::string, T_Function >::iterator itFind = functors.find( type.GetName() );
        if( itFind != functors.end() )
            functors[ type.GetName() ]( refMission_, dianame, element );
        else if( !!knowledgeCreateFunction_ )
            functorsBM[ type.GetName() ]( brain_, knowledgeCreateFunction_, refMission_, dianame, element );
    }

private:
     directia::brain::Brain& brain_;
     const directia::tools::binders::ScriptRef& refMission_;
     directia::tools::binders::ScriptRef& knowledgeCreateFunction_;
};

// -----------------------------------------------------------------------------
// Name: RegisterMissionParameters
// Created: LDC 2009-05-04
// -----------------------------------------------------------------------------
void RegisterMissionParameters( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    InitFunctions();
    RegisterMissionParameterVisitor visitor( brain, refMission, knowledgeCreateFunction );
    mission->Visit( visitor );
}

#ifndef PLATFORM
#define PLATFORM _vc80
#endif

#ifndef PLUGIN
#ifdef NDEBUG
#define PLUGIN46( plugin ) "'plugin_" + plugin + BOOST_PP_STRINGIZE( PLATFORM ) + "-mt-4_6.plugin',"
#define PLUGIN( plugin ) "'plugin_" + plugin + BOOST_PP_STRINGIZE( PLATFORM ) + "-mt.plugin',"
#else
#define PLUGIN46( plugin ) "'plugin_" + plugin + BOOST_PP_STRINGIZE( PLATFORM ) + "-mt-gd-4_6.plugin',"
#define PLUGIN( plugin ) "'plugin_" + plugin + BOOST_PP_STRINGIZE( PLATFORM ) + "-mt-gd.plugin',"
#endif
#endif

namespace
{
    std::map< std::string, boost::shared_ptr< directia::brain::Brain > > brainTable;
}

bool CreateBrain(boost::shared_ptr< directia::brain::Brain >& pArchetypeBrain, boost::shared_ptr< directia::brain::Brain >& pBrain, const std::string& includePath, const std::string& brainFile )
{
    pArchetypeBrain = brainTable[brainFile];

    if( !pArchetypeBrain.get() )
    {
        std::string plugins;
        if(brainFile == "BMAutomat.bms" || brainFile == "BMPion.bms")
        {
            plugins = std::string( "plugins={" )
                + PLUGIN( "masalife_brain" )
                + PLUGIN( "knowledge" )
                + PLUGIN( "communication" )
                + PLUGIN( "services" )
                + PLUGIN( "default_engine" );
            std::string brainInit = plugins
                + "} cwd='" + includePath + "'";
		    pBrain.reset( new directia::brain::Brain( brainInit ) );
            pArchetypeBrain = pBrain;
            return true;
        }
        else
        {
            plugins = std::string( "plugins={" )
                + PLUGIN46( "eventmanager" )
                + PLUGIN46( "motivation" );
        std::string brainInit = plugins
            + "} cwd='" + includePath + "'";
        pArchetypeBrain.reset( new directia::brain::Brain( brainInit ) );
        brainTable[brainFile] = pArchetypeBrain;
	    pBrain.reset( new directia::brain::Brain( *pArchetypeBrain ) );
        return true;
        }
    }
    else
        pBrain.reset( new directia::brain::Brain( *pArchetypeBrain ) );
    return false;
}

void IncludeFile( directia::brain::Brain& brain, const std::string& brainFile ,const std::string& includePath,const std::string& type,const std::string& groupName )
{
    brain[ "include" ]( brainFile ,includePath, type, groupName );
}

}
