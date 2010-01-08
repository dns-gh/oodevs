#include "simulation_kernel_pch.h"

#include "DEC_Decision.h"
#include "DEC_Tools.h"
#include "simulation_orders/MIL_MissionParameter_ABC.h"
#include "simulation_orders/MIL_ParameterType_ABC.h"
#include "Decision/Functions/DEC_DIAFunctions.h"
#include "Decision/Functions/DEC_GeometryFunctions.h"
#include "Decision/Functions/DEC_LogisticFunctions.h"
#include "Decision/Functions/DEC_MiscFunctions.h"
#include "Decision/Functions/DEC_ObjectFunctions.h"
#include "Decision/Functions/DEC_PathFunctions.h"
#include "Decision/Functions/DEC_TelepathyFunctions.h"
#include "Decision/DEC_Gen_Object.h"
#include "Decision/Path/DEC_PathPoint.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_FragOrder.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/Orders/MIL_MissionParameterFactory.h"
#include "Entities/Orders/MIL_MissionParameterVisitor_ABC.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Urban/Block.h"
#include "geometry/Point2.h"

#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: ScriptRefs
// Created: LDC 2009-04-08
// -----------------------------------------------------------------------------
ScriptRefs::ScriptRefs( directia::Brain& brain  )
: sendEvent_       ( brain.GetScriptFunction( "SendEvent" ) )
, startEvent_      ( brain.GetScriptFunction( "behavior_model.startTask" ) )
, stopEvents_      ( brain.GetScriptFunction( "behavior_model.stopTask" ) )
, setStateVariable_( brain.GetScriptFunction( "SetStateVariable" ) )
, collectgarbage_  ( brain.GetScriptFunction( "collectgarbage" ) )
, step_            ( brain.RegisterObject( std::string( "step" ) ) )
{
}

#include< directia/ScriptRef.h>

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterCommonUserFunctions
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
namespace DEC_DecisionImpl
{

void RegisterCommonUserFunctions( directia::Brain& brain, unsigned int id )
{
    // Geometry
    brain.RegisterFunction( "DEC_Geometrie_DecouperListePoints",                &DEC_GeometryFunctions::SplitListPoints );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionCouverture",         &DEC_GeometryFunctions::ComputeCoverPosition );
    brain.RegisterFunction( "DEC_Geometrie_CalculerBarycentreAgents",           &DEC_GeometryFunctions::ComputeAgentsBarycenter );
    brain.RegisterFunction( "DEC_Geometrie_PositionTranslate",                  &DEC_GeometryFunctions::TranslatePosition );
    brain.RegisterFunction( "DEC_Geometrie_PositionTranslateDir",               &DEC_GeometryFunctions::TranslatePositionInDirection );
    brain.RegisterFunction( "DEC_Geometrie_PositionsEgales",                    &DEC_GeometryFunctions::ComparePositions );
    brain.RegisterFunction( "DEC_Geometrie_Distance",                           &DEC_GeometryFunctions::Distance );
    brain.RegisterFunction( "DEC_Geometrie_ConvertirPointEnLocalisation",       &DEC_GeometryFunctions::ConvertPointToLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_EstPointDansLocalisation",           &DEC_GeometryFunctions::IsPointInsideLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_CreerLocalisation",                  &DEC_GeometryFunctions::CreateLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_CreerListePoints",                   &DEC_GeometryFunctions::CreateListPoint );
    brain.RegisterFunction( "DEC_Geometrie_CreerPoint",                         &DEC_GeometryFunctions::CreatePoint );
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
    
    directia::ScriptRef initQueryFunction = brain.GetScriptFunction( "InitQueryReturn" );
    //Keypoint
    brain.RegisterFunction( "DEC_Crossroads", 
        boost::function< void( float, float, float, float, const directia::ScriptRef& ) >( boost::bind( &DEC_GeometryFunctions::GetCrossroads, initQueryFunction, _1 , _2, _3, _4, _5 ) ) ) ;
    //BMArea
    brain.RegisterFunction( "DEC_BMArea_Barycenter", &DEC_GeometryFunctions::ComputeBarycenter );

    // Time management
    // $$$$ AGE 2007-10-11: Un seul temps
    brain.RegisterFunction( "DEC_TempsSim",        &DEC_DIAFunctions::GetSimTime );
    brain.RegisterFunction( "DEC_TempsReel",       &DEC_DIAFunctions::GetRealTime );
    brain.RegisterFunction( "DEC_Nuit",            &DEC_DIAFunctions::IsNight );

    // Parameters copy
    brain.RegisterFunction( "DEC_Copie_Point",                                      &DEC_DIAFunctions::CopyPoint );
    brain.RegisterFunction( "DEC_Copie_Point_Mission",                              &DEC_DIAFunctions::CopyPointMission );
    brain.RegisterFunction( "DEC_Copie_ListePoints_Mission",                        &DEC_DIAFunctions::CopyPathMission );
    brain.RegisterFunction( "DEC_Copie_PointDansListePoints_Mission",               &DEC_DIAFunctions::CopyPointToListPointMission );
    brain.RegisterFunction( "DEC_Copie_Localisation",                               &DEC_DIAFunctions::CopyLocalisation );
    brain.RegisterFunction( "DEC_Copie_Localisation_Mission",                       &DEC_DIAFunctions::CopyLocalisationMission );
    brain.RegisterFunction( "DEC_Copie_ListeLocalisations_Mission",                 &DEC_DIAFunctions::CopyLocalisationListMission );
    brain.RegisterFunction( "DEC_Copie_LocalisationDansListeLocalisations_Mission", &DEC_DIAFunctions::CopyLocalisationToLocationListMission );

    // User type lists manipulation
    brain.RegisterFunction( "DEC_ListePoints_GetAt",     &DEC_DIAFunctions::ListPoint_GetAt );
    brain.RegisterFunction( "DEC_ListePoints_Size",      &DEC_DIAFunctions::ListPoint_Size );

    // Logistic
    brain.RegisterFunction( "DEC_NecessiteEvacuationBlesses",            &DEC_LogisticFunctions::HasWoundedHumansToEvacuate );
    brain.RegisterFunction( "DEC_EvacuerBlessesVersTC2",                 &DEC_LogisticFunctions::EvacuateWoundedHumansToTC2 );
    brain.RegisterFunction( "DEC_InterdireEvacuationAutomatiqueBlesses", &DEC_LogisticFunctions::ForbidWoundedHumansAutoEvacuation );
    brain.RegisterFunction( "DEC_AutoriserEvacuationAutomatiqueBlesses", &DEC_LogisticFunctions::AllowWoundedHumansAutoEvacuation );

    // Gen objects
    brain.RegisterFunction( "DEC_GenObject_Type",                  &DEC_ObjectFunctions::GetGenObjectType );
    brain.RegisterFunction( "DEC_GenObject_Localisation",          &DEC_ObjectFunctions::GetGenObjectLocalisation );
    brain.RegisterFunction( "DEC_GenObject_Densite",               &DEC_Gen_Object::GetDensity );
    brain.RegisterFunction( "DEC_GenObject_TypeObstacleManoeuvre", &DEC_ObjectFunctions::GetGenObjectReservedObstacle );
    brain.RegisterFunction( "DEC_GenObject_TC2",                   &DEC_ObjectFunctions::GetGenObjectTC2 );
    brain.RegisterFunction( "DEC_GenObject_DelaiActiviteMines",    &DEC_Gen_Object::GetMinesActivityTime );

    // Objects
    brain.RegisterFunction( "S_TypeObject_ToString", &DEC_ObjectFunctions::ConvertTypeObjectToString );

    // Objectives
    brain.RegisterFunction( "DEC_Objectif_Localisation", &DEC_Objective::GetLocalisation );
    brain.RegisterFunction( "DEC_Objectif_Flag",         &DEC_Objective::Flag );

    //Rep_Points
    brain.RegisterFunction( "DEC_GetRepPoint",       &DEC_PathFunctions::GetRepPoint );
    brain.RegisterFunction( "DEC_IsAvantPoint",      &DEC_PathFunctions::IsAvantPoint );
    brain.RegisterFunction( "DEC_IsPoint",           &DEC_PathFunctions::IsPoint );
    brain.RegisterFunction( "DEC_GetTypePoint",      &DEC_PathFunctions::GetTypePoint );
    brain.RegisterFunction( "DEC_GetDestPoint",      &DEC_PathFunctions::GetDestPoint );
    brain.RegisterFunction( "DEC_GetTypeLimaPoint",  &DEC_PathFunctions::GetTypeLimaPoint );
    brain.RegisterFunction( "DEC_GetLimaPoint",      &DEC_PathFunctions::GetLimaPoint );

    // __type_
    brain.RegisterFunction( "GetType", &MIL_Mission_ABC::GetDIAType );
    brain.RegisterFunction( "GetType", &DEC_Decision_ABC::GetDIAType );
    brain.RegisterFunction( "GetType", &DEC_RolePion_Decision::GetDIAType );
    brain.RegisterFunction( "GetType", &DEC_AutomateDecision::GetDIAType );
    brain.RegisterFunction( "GetType", &DEC_PathPoint::GetDIAType );

    // Mission parameters
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
    
    directia::ScriptRef initParameterFunction = brain.GetScriptFunction( "InitTaskParameter" );
    brain.RegisterFunction( "DEC_FillMissionParameters",
        boost::function< void( const directia::ScriptRef&, MIL_Mission_ABC* ) >( boost::bind( &DEC_MiscFunctions::FillMissionParameters, initParameterFunction, _1 , _2 ) ) );
    
    // Debug
    brain.RegisterFunction( "DEC_PointToString",      &DEC_DIAFunctions::PointToString );
    brain.RegisterFunction( "DEC_DirectionToString",  &DEC_DIAFunctions::DirectionToString );
    brain.RegisterFunction( "DEC_ItineraireToString", &DEC_DIAFunctions::PathToString );
    brain.RegisterFunction( "BreakForDebug",
        boost::function< void( const std::string& ) >( boost::bind( &DEC_DIAFunctions::BreakForDebug, id ,_1 ) ) );

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
    brain.RegisterFunction( "GetlistePionsCoordination_", &DEC_Decision_ABC::GetListePionsCoordination );
    brain.RegisterFunction( "GetobjMisEnCours_", &DEC_Decision_ABC::GetObjMisEnCours );
    brain.RegisterFunction( "SetobjMisEnCours_", &DEC_Decision_ABC::SetObjMisEnCours );
    brain.RegisterFunction( "GetobjectifCourant_", &DEC_Decision_ABC::GetObjectifCourant );
    brain.RegisterFunction( "GetplotRavitaillementAssigne_", &DEC_Decision_ABC::GetPlotRavitaillementAssigne );
    brain.RegisterFunction( "SetplotRavitaillementAssigne_", &DEC_Decision_ABC::SetPlotRavitaillementAssigne );
    brain.RegisterFunction( "GetporteeAction_", &DEC_Decision_ABC::GetPorteeAction );
    brain.RegisterFunction( "SetporteeAction_", &DEC_Decision_ABC::SetPorteeAction );
    brain.RegisterFunction( "GetrNiveauAlerteRavitaillement_", &DEC_Decision_ABC::GetNiveauAlerteRavitaillement );

    brain.RegisterFunction( "DEC_Itineraire_DernierPoint", &DEC_Decision_ABC::GetLastPointOfPath );
    brain.RegisterFunction( "DEC_Itineraire_ExtrapolerPosition" , &DEC_Decision_ABC::ExtrapolatePosition );

    //Representations    
    brain.RegisterFunction( "Getpoint_", &DEC_PathPoint::GetPos );

    DEC_TelepathyFunctions::Register( brain );
    
    MIL_FragOrder::Register( brain );
}

typedef void (*T_Function)( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element );
typedef void (*T_FunctionBM)( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element );

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
void PointFunctionBM( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< MT_Vector2D > value;
    if( element.ToPoint( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, std::string( "net.masagroup.sword.military.world.Point" ), name, value, value->ToStdVector(), false );//@TODO MGD fix this ugly        
    }
}
void PointListFunctionBM( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPointList( value ) )
    {
        std::vector< std::vector< MT_Float > > positions;
        for( std::vector< boost::shared_ptr< MT_Vector2D > >::iterator it = value.begin(); it!=value.end(); it++ )
        {
            positions.push_back( (*it)->ToStdVector() );
        }
        knowledgeCreateFunction( refMission, std::string( "net.masagroup.sword.military.world.Point" ), name, value, positions, true );
    }
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
void AreaFunctionBM( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToPolygon( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, std::string( "net.masagroup.sword.military.world.Area" ), name, value, value->ComputeBarycenter().ToStdVector(), false );
    }
}
void AreaListFunctionBM( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToPolygonList( value ) )
    {
        std::vector< std::vector< MT_Float > > positions;
        for( std::vector< boost::shared_ptr< TER_Localisation > >::iterator it = value.begin(); it!=value.end(); it++ )
        {
            positions.push_back( (*it)->ComputeBarycenter().ToStdVector() );
        }
        knowledgeCreateFunction( refMission, std::string( "net.masagroup.sword.military.world.Area" ), name, value, positions, true );
    }
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
void DirectionFunction( const directia::ScriptRef& /*refMission*/, const std::string& /*name*/, MIL_MissionParameter_ABC& /*element*/ )
{
    // $$$$ LDC: FIXME The only existing Direction argument is dangerDirection_ which is never used by the brains.
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
void AutomatFunctionBM( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Decision_ABC* value = 0;
    if( element.ToAutomat( value ) )
    {
        MT_Vector2D pos;
        value->GetAutomate().GetAlivePionsBarycenter( pos );
        knowledgeCreateFunction( refMission, std::string( "net.masagroup.sword.military.world.Automat" ), name, value, pos.ToStdVector(), false );
    }
}
void AutomatListFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAutomatList( value ) )
        refMission.RegisterObject( name, value );
}
void AutomatListFunctionBM( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAutomatList( value ) )
    {
        std::vector< std::vector< MT_Float > > positions;
        for( std::vector< DEC_Decision_ABC* >::iterator it = value.begin(); it!=value.end(); it++ )
        {
            MT_Vector2D pos;
            (*it)->GetAutomate().GetAlivePionsBarycenter( pos );
            positions.push_back( pos.ToStdVector() );
        }
        knowledgeCreateFunction( refMission, std::string( "net.masagroup.sword.military.world.Automat" ), name, value, positions, true );
    }     
}
void AgentFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Decision_ABC* value = 0; // $$$$ LDC: Parfois on se sert de champs dessus comme eniEnCours_...
    if( element.ToAgent( value ) )
        refMission.RegisterObject( name, value );
}
void AgentFunctionBM( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Decision_ABC* value = 0; // $$$$ LDC: Parfois on se sert de champs dessus comme eniEnCours_...
    if( element.ToAgent( value ) )
        knowledgeCreateFunction( refMission, std::string( "net.masagroup.sword.military.world.Section" ), name, value, value->GetPosition()->ToStdVector(), false );
}
void AgentListFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAgentList( value ) )
        refMission.RegisterObject( name, value );
}
void AgentListFunctionBM( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAgentList( value ) )
    {
        std::vector< std::vector< MT_Float > > positions;
        for( std::vector< DEC_Decision_ABC* >::iterator it = value.begin(); it!=value.end(); it++ )
        {
            positions.push_back( (*it)->GetPosition()->ToStdVector() );
        }
        knowledgeCreateFunction( refMission, std::string( "net.masagroup.sword.military.world.Section" ), name, value, positions, true );
    }
}
void AgentKnowledgeFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Agent > value;
    if( element.ToAgentKnowledge( value ) && value.get() )
        refMission.RegisterObject( name, value );
}
void AgentKnowledgeFunctionBM( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Agent > value;//@TODO SEE how to bind agent and knowledge agent with the same BM knowledge
    if( element.ToAgentKnowledge( value ) && value.get() )
        knowledgeCreateFunction( refMission, std::string( "net.masagroup.sword.military.world.Section" ), name, value, value->GetPosition().ToStdVector(), false );
}
void AgentKnowledgeListFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > value;
    if( element.ToAgentKnowledgeList( value ) )
        refMission.RegisterObject( name, value );
}
void AgentKnowledgeListFunctionBM( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > value;
    if( element.ToAgentKnowledgeList( value ) )
    {
        std::vector< std::vector< MT_Float > > positions;
        for( std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >::iterator it = value.begin(); it!=value.end(); it++ )
        {
            positions.push_back( (*it)->GetPosition().ToStdVector() );
        }
        knowledgeCreateFunction( refMission, std::string( "net.masagroup.sword.military.world.Section" ), name, value, positions, true );
    }
}
void ObjectKnowledgeFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Object > value;
    if( element.ToObjectKnowledge( value ) && value && value->IsValid() )
        refMission.RegisterObject( name, value );
}
void ObjectKnowledgeFunctionBM( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Object > value;
    if( element.ToObjectKnowledge( value ) && value )
        knowledgeCreateFunction( refMission, std::string( "net.masagroup.sword.military.world.Object" ), name, value, value->GetObjectKnown()->GetLocalisation().ComputeBarycenter().ToStdVector(), false );
}
void ObjectKnowledgeListFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > value;
    if( element.ToObjectKnowledgeList( value ) )
        refMission.RegisterObject( name, value );
}
void ObjectKnowledgeListFunctionBM( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > value;
    if( element.ToObjectKnowledgeList( value ) )
    {
        std::vector< std::vector< MT_Float > > positions;
        for( std::vector< boost::shared_ptr< DEC_Knowledge_Object > >::iterator it = value.begin(); it!=value.end(); it++ )
        {
            positions.push_back( (*it)->GetObjectKnown()->GetLocalisation().ComputeBarycenter().ToStdVector() );
        }
        knowledgeCreateFunction( refMission, std::string( "net.masagroup.sword.military.world.Object" ), name, value, positions, true );
    }
}
void PopulationKnowledgeFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Knowledge_Population* value = 0;
    if( element.ToPopulationKnowledge( value ) && value )
        refMission.RegisterObject( name, value->GetID() );
}
void PopulationKnowledgeFunctionBM( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Knowledge_Population* value = 0;
    if( element.ToPopulationKnowledge( value ) && value )
        knowledgeCreateFunction( refMission, std::string( "net.masagroup.sword.military.world.Population" ), name, value, false );//@TODO MGD Add CompositeReachable for Population?
}

void UrbanBlockFunctionBM( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    urban::Block* value = 0;
    if( element.ToUrbanBlock( value ) && value )
    {
        const geometry::Point2f& point = value->GetFootprint()->Barycenter();
        std::vector< float > position;
        position.push_back( point.X() );
        position.push_back( point.Y() );
        position.push_back( 0 );
        knowledgeCreateFunction( refMission, std::string( "net.masagroup.sword.military.world.UrbanBlock" ), name, value, position, false );//@TODO MGD fix this ugly        
    }
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
void GenObjectFunctionBM( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAgentList( value ) )
        knowledgeCreateFunction( refMission, std::string( "net.masagroup.sword.military.world.Object" ), name, value, true );
}
void GenObjectListFunction( const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Gen_Object > > value;
    if( element.ToGenObjectList( value ) )
        refMission.RegisterObject( name, value );
}
void GenObjectListFunctionBM( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Gen_Object > > value;
    if( element.ToGenObjectList( value ) )
        knowledgeCreateFunction( refMission, std::string( "net.masagroup.sword.military.world.Object" ), name, value, true );
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
    int value = 0;
    if( element.ToIndirectFire( value ) && value )
        refMission.RegisterObject( name, value );
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
    }
}


class RegisterMissionParameterVisitor : public MIL_MissionParameterVisitor_ABC
{
public:
    RegisterMissionParameterVisitor( const directia::ScriptRef& refMission, directia::ScriptRef& knowledgeCreateFunction ) 
        : refMission_( refMission )
        , knowledgeCreateFunction_( knowledgeCreateFunction )
    {}
    virtual ~RegisterMissionParameterVisitor()
    {}

    virtual void Accept( const std::string& dianame, const MIL_ParameterType_ABC& type, MIL_MissionParameter_ABC& element )
    {
        std::map< std::string, T_Function >::iterator itFind = functors.find( type.GetName() );
        if( itFind != functors.end() )
            functors[ type.GetName() ]( refMission_, dianame, element );
        else
            functorsBM[ type.GetName() ]( knowledgeCreateFunction_, refMission_, dianame, element );
    }

private:
     const directia::ScriptRef& refMission_;
     directia::ScriptRef& knowledgeCreateFunction_;
};

// -----------------------------------------------------------------------------
// Name: RegisterMissionParameters
// Created: LDC 2009-05-04
// -----------------------------------------------------------------------------
void RegisterMissionParameters( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, MIL_Mission_ABC& mission )
{
    InitFunctions();
    RegisterMissionParameterVisitor visitor( refMission, knowledgeCreateFunction );
    mission.Visit( visitor );
}

}
