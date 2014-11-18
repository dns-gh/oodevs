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
#include "Entities/Orders/MIL_MissionParameter_ABC.h"
#include "Entities/Orders/MIL_ParameterType_ABC.h"
#include "Decision/DEC_ActionFunctions.h"
#include "Decision/DEC_AgentFunctions.h"
#include "Decision/DEC_AutomateFunctions.h"
#include "Decision/DEC_CommunicationFunctions.h"
#include "Decision/DEC_DIAFunctions.h"
#include "Decision/DEC_EntityFunctions.h"
#include "Decision/DEC_FireFunctions.h"
#include "Decision/DEC_GeometryFunctions.h"
#include "Decision/DEC_KnowledgeAgentFunctions.h"
#include "Decision/DEC_KnowledgeFunctions.h"
#include "Decision/DEC_KnowledgeObjectFunctions.h"
#include "Decision/DEC_KnowledgePopulationFunctions.h"
#include "Decision/DEC_LogisticFunctions.h"
#include "Decision/DEC_MiscFunctions.h"
#include "Decision/DEC_ObjectFunctions.h"
#include "Decision/DEC_Objective.h"
#include "Decision/DEC_OrdersFunctions.h"
#include "Decision/DEC_PathFunctions.h"
#include "Decision/DEC_PerceptionFunctions.h"
#include "Decision/DEC_PopulationFunctions.h"
#include "Decision/DEC_ResourceNetworkFunctions.h"
#include "Decision/DEC_TelepathyFunctions.h"
#include "Decision/DEC_Gen_Object.h"
#include "Decision/DEC_UrbanObjectFunctions.h"
#include "Decision/DEC_TerrainFunctions.h"
#include "Decision/DEC_Logger.h"
#include "DEC_ResourceNetwork.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_FragOrder.h"
#include "Entities/Orders/MIL_MissionParameterFactory.h"
#include "Entities/Orders/MIL_MissionParameterVisitor_ABC.h"
#include "Entities/Orders/MIL_OrderTypeParameter.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "simulation_terrain/TER_PathPoint.h"
#include <directia/tools/binders/ScriptRef.h>
#include <geometry/Point2.h>

// -----------------------------------------------------------------------------
// Name: ScriptRefs
// Created: LDC 2009-04-08
// -----------------------------------------------------------------------------
ScriptRefs::ScriptRefs( sword::Brain& brain  )
    : startEvent_             ( brain.GetScriptRef( "StartEvent" ) )
    , stopEvents_             ( brain.GetScriptRef( "StopEvents" ) )
    , setStateVariable_       ( brain.GetScriptRef( "SetStateVariable" ) )
    , collectgarbage_         ( brain.GetScriptRef( "collectgarbage" ) )
    , step_                   ( brain.GetScriptRef( "step" ) )
    , callbackPerception_     ( brain.GetScriptRef( "CallbackPerception" ) )
    , knowledgeCallbackAction_( brain.GetScriptRef( "KnowledgeCallbackAction" ) )
    , removeAction_           ( brain.GetScriptRef( "RemoveAction" ) )
    , initTaskParameter_      ( brain.GetScriptRef( "InitTaskParameter" ) )
    , activateBrainDebug_     ( brain.GetScriptRef( "ActivateBrainDebug" ) )
{
    // NOTHING
}

namespace DEC_DecisionImpl
{

void RegisterMethods( sword::Brain& brain )
{
    // DEC_Decision_ABC
    brain.RegisterMethod( "DEC_Agent_EtatOps", &DEC_Decision_ABC::GetOperationalState );
    brain.RegisterMethod( "DEC_Agent_EtatOpsMajeur", &DEC_Decision_ABC::GetMajorOperationalState );
    brain.RegisterMethod( "DEC_Agent_EstPC", &DEC_Decision_ABC::IsPC );
    brain.RegisterMethod( "DEC_Agent_EstTransporte", &DEC_Decision_ABC::IsTransported );
    brain.RegisterMethod( "DEC_Agent_EstEnVol", &DEC_Decision_ABC::IsFlying );
    brain.RegisterMethod( "DEC_Agent_EstEnMouvement", &DEC_Decision_ABC::IsMoving );
    brain.RegisterMethod( "DEC_Agent_EstNeutralise", &DEC_Decision_ABC::IsNeutralized );
    brain.RegisterMethod( "DEC_Agent_AutomateEstEmbraye", &DEC_Decision_ABC::IsAutomateEngaged );
    brain.RegisterMethod( "DEC_Agent_Position", &DEC_Decision_ABC::GetPosition );
    brain.RegisterMethod( "DEC_Agent_EstMort", &DEC_Decision_ABC::IsDead );
    brain.RegisterMethod( "DEC_Agent_EstContamine", &DEC_Decision_ABC::IsContaminated );
    brain.RegisterMethod( "DEC_Agent_IsDecontaminationFinished", &DEC_Decision_ABC::IsDecontaminationFinished );
    brain.RegisterMethod( "DEC_Agent_EstBrouille", &DEC_Decision_ABC::IsJammed );
    brain.RegisterMethod( "DEC_Agent_EstEnSilenceRadioEmission", &DEC_Decision_ABC::IsInEmissionBlackout );
    brain.RegisterMethod( "DEC_Agent_EstEnSilenceRadioReception", &DEC_Decision_ABC::IsInReceptionBlackout );
    brain.RegisterMethod( "DEC_Pion_PionsAvecPC", &DEC_Decision_ABC::GetPionsWithPC );
    brain.RegisterMethod( "DEC_Pion_PionsAvecPCCommunication", &DEC_Decision_ABC::GetCommunicationPionsWithPC );
    brain.RegisterMethod( "DEC_Automate_PionsAvecPC", &DEC_Decision_ABC::GetPionsWithPC );
    brain.RegisterMethod( "DEC_Automate_PionsAvecPCCommunication", &DEC_Decision_ABC::GetCommunicationPionsWithPC );
    brain.RegisterMethod( "DEC_Automate_EstEmbraye", &DEC_Decision_ABC::IsAutomateEngaged );
    brain.RegisterMethod( "GetBoolVariable", &DEC_Decision_ABC::GetVariable< bool > );
    brain.RegisterMethod( "SetStateVariable", &DEC_Decision_ABC::SetStateVariable );
    brain.RegisterMethod( "SetambianceMission_", &DEC_Decision_ABC::SetAmbianceMission );
    brain.RegisterMethod( "SetbAppuieFreinage_", &DEC_Decision_ABC::SetAppuieFreinage );
    brain.RegisterMethod( "GetbDemandeOrdreConduitePoursuivre_", &DEC_Decision_ABC::GetDemandeOrdreConduitePoursuivre );
    brain.RegisterMethod( "GetbEnCoursExtractionPersonnel_", &DEC_Decision_ABC::GetEnCoursExtractionPersonnel );
    brain.RegisterMethod( "GetbEnExploitation_", &DEC_Decision_ABC::GetEnExploitation );
    brain.RegisterMethod( "SetbEnExploitation_", &DEC_Decision_ABC::SetbEnExploitation );
    brain.RegisterMethod( "GetbEnPhaseRavitaillement_", &DEC_Decision_ABC::GetEnPhaseRavitaillement );
    brain.RegisterMethod( "SetbEnPhaseRavitaillement_", &DEC_Decision_ABC::SetEnPhaseRavitaillement );
    brain.RegisterMethod( "GetbMiseEnOeuvre_", &DEC_Decision_ABC::GetMiseEnOeuvre );
    brain.RegisterMethod( "SetbMiseEnOeuvre_", &DEC_Decision_ABC::SetMiseEnOeuvre );
    brain.RegisterMethod( "GeteEtatFeu_", &DEC_Decision_ABC::GetEtatFeu );
    brain.RegisterMethod( "GetlisteEnisTirAutorise_", &DEC_Decision_ABC::GetListeEnisTirAutorise );
    brain.RegisterMethod( "SetlisteEnisTirAutorise_", &DEC_Decision_ABC::SetListeEnisTirAutorise );
    brain.RegisterMethod( "ClearlisteEnisTirAutorise_", &DEC_Decision_ABC::ClearListeEnisTirAutorise );
    brain.RegisterMethod( "GetlistePionsCoordination_", &DEC_Decision_ABC::GetListePionsCoordination );
    brain.RegisterMethod( "ClearlistePionsCoordination_", &DEC_Decision_ABC::ClearListePionsCoordination );
    brain.RegisterMethod( "GetptSauvegarde_", &DEC_Decision_ABC::GetPtSauvegarde );
    brain.RegisterMethod( "GetlisteZonesTir_", &DEC_Decision_ABC::GetListeZonesTir );
    brain.RegisterMethod( "ClearlisteZonesTir_", &DEC_Decision_ABC::ClearListeZonesTir );
    brain.RegisterMethod( "GetobjMisEnCours_", &DEC_Decision_ABC::GetObjMisEnCours );
    brain.RegisterMethod( "SetobjMisEnCours_", &DEC_Decision_ABC::SetObjMisEnCours );
    brain.RegisterMethod( "GetsuppliesLoaded_", &DEC_Decision_ABC::GetSuppliesLoaded );
    brain.RegisterMethod( "SetsuppliesLoaded_", &DEC_Decision_ABC::SetSuppliesLoaded );
    brain.RegisterMethod( "GetobjectifCourant_", &DEC_Decision_ABC::GetObjectifCourant );
    brain.RegisterMethod( "GetplotRavitaillementAssigne_", &DEC_Decision_ABC::GetPlotRavitaillementAssigne );
    brain.RegisterMethod( "SetplotRavitaillementAssigne_", &DEC_Decision_ABC::SetPlotRavitaillementAssigne );
    brain.RegisterMethod( "ClearplotsRavitaillement_", &DEC_Decision_ABC::ClearPlotsRavitaillement );
    brain.RegisterMethod( "GetporteeAction_", &DEC_Decision_ABC::GetPorteeAction );
    brain.RegisterMethod( "SetporteeAction_", &DEC_Decision_ABC::SetPorteeAction );
    brain.RegisterMethod( "GetrNiveauAlerteRavitaillement_", &DEC_Decision_ABC::GetNiveauAlerteRavitaillement );
    brain.RegisterMethod( "Getpoint_", &TER_PathPoint::GetPos );
    brain.RegisterMethod( "Setmunitions_", &DEC_Decision_ABC::SetMunition );
    brain.RegisterMethod( "Getmunitions_", &DEC_Decision_ABC::GetMunition );
    brain.RegisterMethod( "SetnbIT_", &DEC_Decision_ABC::SetNbIt );
    brain.RegisterMethod( "GetnbIT_", &DEC_Decision_ABC::GetNbIt );
    brain.RegisterMethod( "Setcible_", &DEC_Decision_ABC::SetTarget );
    brain.RegisterMethod( "Getcible_", &DEC_Decision_ABC::GetTarget );
    brain.RegisterMethod( "DEC_Itineraire_DernierPoint", &DEC_Decision_ABC::GetLastPointOfPath );
    brain.RegisterMethod( "DEC_Itineraire_ExtrapolerPosition", &DEC_Decision_ABC::ExtrapolatePosition );

    // DEC_AutomateDecision
    brain.RegisterMethod( "DEC_Automate_PionEstNeutralise", &DEC_AutomateDecision::IsPionNeutralized );

    // Elongation factor concept was removed but we keep the DEC function around
    // to avoid breaking old models. This should be removed after their migration.
    brain.RegisterFunction( "DEC_Elongation" , std::function< void(double) >( []( double ) {} ));

    // MT_Vector2D
    brain.RegisterMethod( "DEC_Geometrie_X", &MT_Vector2D::GetX );
    brain.RegisterMethod( "DEC_Geometrie_Y", &MT_Vector2D::GetY );

    // DEC_Knowledge_Agent
    brain.RegisterMethod( "DEC_AgentKnowledge_Relevance", &DEC_Knowledge_Agent::GetRelevance );

    // DEC_ResourceNetwork
    brain.RegisterMethod( "GetTypeName", &DEC_ResourceNetwork::GetTypeName );

    // DEC_Gen_Object
    brain.RegisterMethod( "DEC_GenObject_Densite", &DEC_Gen_Object::GetDensity );
    brain.RegisterMethod( "DEC_GenObject_DelaiActiviteMines", &DEC_Gen_Object::GetMinesActivityTime );
    brain.RegisterMethod( "DEC_GenObject_DateActivationMines", &DEC_Gen_Object::GetActivationTime );
    brain.RegisterMethod( "DEC_GenObject_Name", &DEC_Gen_Object::GetName );
    brain.RegisterMethod( "DEC_GenObject_HasCapacity", &DEC_Gen_Object::HasCapacity );

    // DEC_Objective
    brain.RegisterMethod( "DEC_Objectif_Localisation", &DEC_Objective::GetLocalisation );
    brain.RegisterMethod( "DEC_Objectif_Flag", &DEC_Objective::Flag );

    // GetDIAType variants
    brain.RegisterMethod( "GetType", &MIL_Mission_ABC::GetDIAType );
    brain.RegisterMethod( "GetType", &DEC_Decision_ABC::GetDIAType );
    brain.RegisterMethod( "GetType", &DEC_RolePion_Decision::GetDIAType );
    brain.RegisterMethod( "GetType", &DEC_AutomateDecision::GetDIAType );
    brain.RegisterMethod( "GetType", &TER_PathPoint::GetDIAType );

    // MIL_MissionParameterFactory
    brain.RegisterFunction( "DEC_AssignMissionPionParameter", &MIL_MissionParameterFactory::SetPawnParameter );
    brain.RegisterFunction( "DEC_AssignMissionAutomatParameter", &MIL_MissionParameterFactory::SetAutomatParameter );
    brain.RegisterFunction( "DEC_AssignMissionBoolParameter", &MIL_MissionParameterFactory::SetBoolParameter );
    brain.RegisterFunction( "DEC_AssignMissionObjectKnowledgeParameter", &MIL_MissionParameterFactory::SetObjectKnowledgeParameter );
    brain.RegisterFunction( "DEC_AssignMissionNatureAtlasTypeParameter", &MIL_MissionParameterFactory::SetNatureAtlasTypeParameter );
    brain.RegisterFunction( "DEC_AssignMissionAutomatListParameter", &MIL_MissionParameterFactory::CreateAutomat );
    brain.RegisterFunction( "DEC_AssignMissionAgentKnowledgeParameter", &MIL_MissionParameterFactory::SetAgentKnowledgeParameter );
    brain.RegisterFunction( "DEC_AssignMissionPathListParameter", &MIL_MissionParameterFactory::CreatePathConst );
    brain.RegisterFunction( "DEC_AssignMissionEnumereParameter", &MIL_MissionParameterFactory::SetEnumereParameter );
    brain.RegisterFunction( "DEC_AssignMissionLocationParameter", &MIL_MissionParameterFactory::SetLocationParameter );
    brain.RegisterFunction( "DEC_AssignMissionObjectKnowledgeListParameter", &MIL_MissionParameterFactory::CreateObjectKnowledge );
    brain.RegisterFunction( "DEC_AssignMissionPointParameter", &MIL_MissionParameterFactory::SetPointParameter );
    brain.RegisterFunction( "DEC_AssignMissionAgentKnowledgeListParameter", &MIL_MissionParameterFactory::CreateAgentKnowledge );
    brain.RegisterFunction( "DEC_AssignMissionGenObjectParameter", &MIL_MissionParameterFactory::SetGenObjectParameter );
    brain.RegisterFunction( "DEC_AssignMissionGenObjectListParameter", &MIL_MissionParameterFactory::CreateGenObject );
    brain.RegisterFunction( "DEC_AssignMissionPionListParameter", &MIL_MissionParameterFactory::CreatePion );
    brain.RegisterFunction( "DEC_AssignMissionLocationListParameter", &MIL_MissionParameterFactory::CreateLocation );
    brain.RegisterFunction( "DEC_AssignMissionPointListParameter", &MIL_MissionParameterFactory::CreatePoint );
    brain.RegisterFunction( "DEC_AssignMissionUrbanBlockParameter", &MIL_MissionParameterFactory::SetUrbanBlockParameter );
    brain.RegisterFunction( "DEC_AssignMissionUrbanBlockListParameter", &MIL_MissionParameterFactory::CreateUrbanBlock );
    brain.RegisterFunction( "DEC_AssignMissionDirectionParameter", &MIL_MissionParameterFactory::SetDirectionParameter );
    brain.RegisterFunction( "DEC_AssignMissionDotationTypeParameter", &MIL_MissionParameterFactory::SetDotationTypeParameter );
    brain.RegisterFunction( "DEC_AssignMissionNumericTypeParameter", &MIL_MissionParameterFactory::SetNumericTypeParameter );
    brain.RegisterFunction( "DEC_AssignMissionResourceNetworkParameter", &MIL_MissionParameterFactory::SetResourceNetworkNodeParameter );
    brain.RegisterFunction( "DEC_AssignMissionResourceNetworkListParameter", &MIL_MissionParameterFactory::CreateResourceNetworkNode );
    brain.RegisterFunction( "DEC_AssignMissionResourceNetworkTypeParameter", &MIL_MissionParameterFactory::SetResourceNetworkTypeParameter );
    brain.RegisterFunction( "DEC_AssignMissionResourceNetworkTypeListParameter", &MIL_MissionParameterFactory::CreateResourceNetworkType );
    brain.RegisterFunction( "DEC_AssignMissionPathConstParameter", &MIL_MissionParameterFactory::AssignPathConst );
    brain.RegisterFunction( "DEC_AssignMissionListParameter", &MIL_MissionParameterFactory::AssignMissionListParameter );
    brain.RegisterFunction( "_DEC_AssignMissionCrowdParameter", &MIL_MissionParameterFactory::SetCrowdKnowledgeParameter );
    brain.RegisterFunction<boost::shared_ptr<MIL_MissionParameter_ABC>, DEC_Decision_ABC*, int>( "_DEC_AssignMissionCrowdListParameter", &MIL_MissionParameterFactory::CreatePopulationKnowledge );

    MIL_FragOrder::Register( brain );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterCommonUserFunctions
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
void RegisterCommonUserFunctions( sword::Brain& brain, bool isMasalife )
{
    DEC_ActionFunctions::Register( brain );
    DEC_AgentFunctions::Register( brain );
    DEC_AutomateFunctions::Register( brain );
    DEC_CommunicationFunctions::Register( brain );
    DEC_DIAFunctions::Register( brain );
    DEC_EntityFunctions::Register( brain );
    DEC_FireFunctions::Register( brain );
    DEC_GeometryFunctions::Register( brain );
    DEC_KnowledgeAgentFunctions::Register( brain );
    DEC_KnowledgeFunctions::Register( brain );
    DEC_KnowledgeObjectFunctions::Register( brain );
    DEC_KnowledgePopulationFunctions::Register( brain );
    DEC_LogisticFunctions::Register( brain );
    DEC_MiscFunctions::Register( brain, isMasalife );
    DEC_ObjectFunctions::Register( brain );
    DEC_OrdersFunctions::Register( brain );
    DEC_PathFunctions::Register( brain );
    DEC_PerceptionFunctions::Register( brain );
    DEC_PopulationFunctions::Register( brain );
    DEC_TelepathyFunctions::Register( brain );
    DEC_TerrainFunctions::Register( brain );
    DEC_ResourceNetworkFunctions::Register( brain );
    DEC_UrbanObjectFunctions::Register( brain );

    RegisterMethods( brain );
}

typedef bool (*T_Function)( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element );
typedef bool (*T_FunctionBM)( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element );

bool BoolFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    bool value;
    if( element.ToBool( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool BoolFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    BoolFunction( refMission, name, element );
    return true;
}
bool EnumerationFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    int value;
    if( element.ToId( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool EnumerationFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    EnumerationFunction( refMission, name, element );
    return true;
}
bool PointFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< MT_Vector2D > value;
    if( element.ToPoint( value ) && value.get() )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool PointListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPointList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool PointFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< MT_Vector2D > value;
    if( element.ToPoint( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.point" ), name, value, false );
        return true;
    }
    return false;
}

bool PointListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPointList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.point" ), name, value, true );
        return true;
    }
    return false;
}
bool PolygonFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToPolygon( value ) && value.get() )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool PolygonListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToPolygonList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool AreaFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToPolygon( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.area" ), name, value, false );
        return true;
    }
    return false;
}
bool AreaListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToPolygonList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.area" ), name, value, true );
        return true;
    }
    return false;
}
bool LocationFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToLocation( value ) && value.get() )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool LocationListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToLocationList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool PathFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPath( value ) && !value.empty() )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool PathListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > > value;
    if( element.ToPathList( value ) && !value.empty() )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool PathFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPath( value ) && !value.empty() )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.point" ), name, value, true );
        return true;
    }
    return false;
}
bool PathListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > list;
    if( element.ToList( list ) )
    {
        knowledgeCreateFunction( refMission, "beginlist", name );
        for( auto it = list.begin(); it != list.end(); ++it )
            PathFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it );
        knowledgeCreateFunction( refMission, "endlist", name );
        return true;
    }
    return false;
}
bool DirectionFunction( const directia::tools::binders::ScriptRef& /*refMission*/, const std::string& /*name*/, MIL_MissionParameter_ABC& /*element*/ )
{
    // $$$$ LDC: FIXME The only existing Direction argument is dangerDirection_ which is never used by the brains.
    return false;
}
bool DirectionFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< MT_Vector2D > value;
    if( element.ToDirection( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission,  brain.GetScriptRef( "integration.ontology.types.direction" ), name, value, false );
        return true;
    }
    return false;
}
bool NatureAtlasFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    int value = 0;
    if( element.ToNatureAtlas( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool NatureAtlasFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    return NatureAtlasFunction( refMission, name, element );
}
bool AutomatFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Decision_ABC* value = 0;
    if( element.ToAutomat( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool AutomatFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Decision_ABC* value = 0;
    if( element.ToAutomat( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.automat" ), name, value, false );
        return true;
    }
    return false;
}
bool AutomatListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAutomatList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool AutomatListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAutomatList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.automat" ), name, value, true );
        return true;
    }
    return false;
}
bool AgentFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const DEC_Decision_ABC* value = 0; // $$$$ LDC: Parfois on se sert de champs dessus comme eniEnCours_...
    if( element.ToAgent( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool AgentFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const DEC_Decision_ABC* value = 0; // $$$$ LDC: Parfois on se sert de champs dessus comme eniEnCours_...
    if( element.ToAgent( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.agent" ), name, value, false );
        return true;
    }
    return false;
}
bool AgentListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const DEC_Decision_ABC* > value;
    if( element.ToAgentList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool AgentListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const DEC_Decision_ABC* > value;
    if( element.ToAgentList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.agent" ), name, value, true );
        return true;
    }
    return false;
}
bool AgentKnowledgeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Agent > value;
    if( element.ToAgentKnowledge( value ) && value.get() )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool AgentKnowledgeFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Agent > value;//@TODO SEE how to bind agent and knowledge agent with the same BM knowledge
    if( element.ToAgentKnowledge( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.agentKnowledge" ), name, value, false );
        return true;
    }
    return false;
}
bool AgentKnowledgeListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > value;
    if( element.ToAgentKnowledgeList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool AgentKnowledgeListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > value;
    if( element.ToAgentKnowledgeList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.agentKnowledge" ), name, value, true );
        return true;
    }
    return false;
}
bool ObjectKnowledgeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Object > value;
    if( element.ToObjectKnowledge( value ) && value && value->IsValid() )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool ObjectKnowledgeFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Object > value;
    if( element.ToObjectKnowledge( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.object" ), name, value, false );
        return true;
    }
    return false;
}
bool ObjectKnowledgeListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > value;
    if( element.ToObjectKnowledgeList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool ObjectKnowledgeListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > value;
    if( element.ToObjectKnowledgeList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.object" ), name, value, true );
        return true;
    }
    return false;
}
bool PopulationKnowledgeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Population > value;
    if( element.ToPopulationKnowledge( value ) && value )
    {
        refMission[ name ] = value->GetID();
        return true;
    }
    return false;
}
bool PopulationKnowledgeFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Population > value;
    if( element.ToPopulationKnowledge( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.population" ), name, value->GetID(), false );//@TODO MGD Add CompositeReachable for Population?
         return true;
    }
    return false;
}

bool UrbanBlockFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    MIL_UrbanObject_ABC* value = 0;
    if( element.ToUrbanBlock( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.urbanBlock" ), name, value, false );
        return true;
    }
    return false;
}

bool UrbanBlockListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< MIL_UrbanObject_ABC* > value;
    if( element.ToUrbanBlockList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.urbanBlock" ), name, value, true );
        return true;
    }
    return false;
}

bool DotationTypeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const PHY_DotationCategory* value = 0;
    if( element.ToDotationType( value ) && value )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}

bool DotationTypeFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DotationTypeFunction( refMission, name, element );
    return true;
}

bool DotationTypeListFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_DotationCategory* > value;
    if( element.ToDotationTypeList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}

bool ResourceNetworkNodeFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_ResourceNetwork > value;
    if( element.ToResourceNetworkNode( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.resourceNetwork" ), name, value, false );
        return true;
    }
    return false;
}

bool ResourceNetworkNodeListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_ResourceNetwork > > value;
    if( element.ToResourceNetworkNodeList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.resourceNetwork" ), name, value, true );
        return true;
    }
    return false;
}

bool ResourceNetworkTypeFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const PHY_ResourceNetworkType* value = 0;
    if( element.ToResourceNetworkType( value ) && value )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}

bool ResourceNetworkTypeListFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_ResourceNetworkType* > value;
    if( element.ToResourceNetworkTypeList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}

bool EquipmentTypeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const PHY_ComposanteTypePion* value = 0;
    if( element.ToEquipmentType( value ) && value )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}

bool EquipmentTypeFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const PHY_ComposanteTypePion* value = 0;
    if( element.ToEquipmentType( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.equipmentType" ), name, value, false );
        return true;
    }
    return false;
}

bool EquipmentTypeListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_ComposanteTypePion* > value;
    if( element.ToEquipmentTypeList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.equipmentType" ), name, value, true );
        return true;
    }
    return false;
}

bool GDHFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    float value = 0;
    if( element.ToGDH( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool GDHFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    GDHFunction( refMission, name, element );
    return true;
}
bool NumericFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    float value = 0;
    if( element.ToNumeric( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool NumericFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    NumericFunction( refMission, name, element );
    return true;
}
bool NumericListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
     std::vector< float > value;
     if( element.ToNumericList( value ) )
     {
         refMission[ name ] = value;
         return true;
     }
     return false;
}
bool NumericListFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    NumericListFunction( refMission, name, element );
    return true;
}
bool GenObjectFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Gen_Object > value;
    if( element.ToGenObject( value ) && value.get() )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool GenObjectFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Gen_Object > value;
    if( element.ToGenObject( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.genObject" ), name, value, false );
        return true;
    }
    return false;
}
bool GenObjectListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Gen_Object > > value;
    if( element.ToGenObjectList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool GenObjectListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Gen_Object > > value;
    if( element.ToGenObjectList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.genObject" ), name, value, true );
        return true;
    }
    return false;
}
bool MaintenancePrioritiesFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_ComposanteTypePion* > value;
    if( element.ToMaintenancePriorities( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.equipmentType" ), name, value, true );
        return true;
    }
    return false;
}
bool MaintenancePrioritiesFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_ComposanteTypePion* > value;
    if( element.ToMaintenancePriorities( value ) )
    {
        refMission[ name ] = value ;
        return true;
    }
    return false;
}
bool MedicalPrioritiesFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_HumanWound* > value;
    if( element.ToMedicalPriorities( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.humanWound" ), name, value, true );
        return true;
    }
    return false;
}
bool MedicalPrioritiesFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_HumanWound* > value;
    if( element.ToMedicalPriorities( value ) )
    {
        refMission[ name ] = value ;
        return true;
    }
    return false;
}
bool IndirectFireFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    int idIndirectFire;
    if( element.ToId( idIndirectFire ) )
    {
        refMission[ name ] = idIndirectFire;
        return true;
    }
    return false;
}
bool StringFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::string value;
    if( element.ToString( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool StringFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    StringFunction( refMission, name, element );
    return true;
}
bool ObjectiveListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Objective > > value;
    if( element.ToObjectiveList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool LocationCompositeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    return PointFunction( refMission, name, element )
        || PathFunction( refMission, name, element )
        || PolygonFunction( refMission, name, element )
        || AutomatFunction( refMission, name, element )
        || AgentFunction( refMission, name, element )
        || AgentKnowledgeFunction( refMission, name, element )
        || ObjectKnowledgeFunction( refMission, name, element )
        || PopulationKnowledgeFunction( refMission, name, element );
}

bool LocationCompositeListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > list;
    if( element.ToList( list ) )
    {
        std::vector< const DEC_Decision_ABC* > result;
        for( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >::const_iterator it = list.begin(); it != list.end(); ++it )
        {
            const DEC_Decision_ABC* value = 0;
            if( (*it)->ToAgent( value ) )
                result.push_back( value );
            else
            {
                boost::shared_ptr< DEC_Knowledge_Agent > knowledge;
                if( (*it)->ToAgentKnowledge( knowledge ) && knowledge.get() )
                    result.push_back( &knowledge->GetAgentKnown().GetDecision() );
            }
        }
        refMission[ name ] = result;
        return true;
    }
    return false;
}

bool LocationCompositeFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
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

bool LocationCompositeListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > list;
    if( element.ToList( list ) )
    {
        knowledgeCreateFunction( refMission, "beginlist", name );
        for( auto it = list.begin(); it != list.end(); ++it )
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

bool PhaseLineFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToLima( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.limaStatique" ), name, value, false );
        return true;
    }
    return false;
}

bool PhaseLineListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToLimaList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.limaStatique" ), name, value, true );
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
        functors[ "Agent" ] = AgentFunction;
        functors[ "AgentKnowledge" ] = AgentKnowledgeFunction;
        functors[ "AgentKnowledgeList" ] = AgentKnowledgeListFunction;
        functors[ "AgentList" ] = AgentListFunction;
        functors[ "Automat" ] = AutomatFunction;
        functors[ "AutomatList" ] = AutomatListFunction;
        functors[ "Boolean" ] = BoolFunction;
        functors[ "CrowdKnowledge" ] = PopulationKnowledgeFunction;
        functors[ "DateTime" ] = GDHFunction;
        functors[ "datetime" ] = GDHFunction;
        functors[ "Enumeration" ] = EnumerationFunction;
        functors[ "enumeration" ] = EnumerationFunction;
        functors[ "EquipmentType" ] = EquipmentTypeFunction;
        functors[ "GDH" ] = GDHFunction;
        functors[ "Heading" ] = DirectionFunction;
        functors[ "IndirectFire" ] = IndirectFireFunction;
        functors[ "Integer" ] = NumericFunction;
        functors[ "Itinerary" ] = PathFunction;
        functors[ "Location" ] = LocationFunction;
        functors[ "LocationComposite" ] = LocationCompositeFunction;
        functors[ "LocationCompositeList" ] = LocationCompositeListFunction;
        functors[ "LocationList" ] = LocationListFunction;
        functors[ "MaintenancePriorities" ] = MaintenancePrioritiesFunction;
        functors[ "MedicalPriorities" ] = MedicalPrioritiesFunction;
        functors[ "NatureAtlas" ] = NatureAtlasFunction;
        functors[ "Numeric" ] = NumericFunction;
        functors[ "NumericList" ] = NumericListFunction;
        functors[ "ObjectiveList" ] = ObjectiveListFunction;
        functors[ "ObjectKnowledge" ] = ObjectKnowledgeFunction;
        functors[ "ObjectKnowledgeList" ] = ObjectKnowledgeListFunction;
        functors[ "Path" ] = PathFunction;
        functors[ "PathList" ] = PathListFunction;
        functors[ "PlannedWork" ] = GenObjectFunction;
        functors[ "PlannedWorkList" ] = GenObjectListFunction;
        functors[ "Point" ] = PointFunction;
        functors[ "PointList" ] = PointListFunction;
        functors[ "Polygon" ] = PolygonFunction;
        functors[ "PolygonList" ] = PolygonListFunction;
        functors[ "PopulationKnowledge" ] = PopulationKnowledgeFunction;
        functors[ "ResourceType" ] = DotationTypeFunction;
        functors[ "String" ] = StringFunction;
        functors[ "string" ] = StringFunction;

        functorsBM[ "Agent" ] = AgentFunctionBM;
        functorsBM[ "AgentKnowledge" ] = AgentKnowledgeFunctionBM;
        functorsBM[ "AgentKnowledgeList" ] = AgentKnowledgeListFunctionBM;
        functorsBM[ "AgentList" ] = AgentListFunctionBM;
        functorsBM[ "Automat" ] = AutomatFunctionBM;
        functorsBM[ "AutomatList" ] = AutomatListFunctionBM;
        functorsBM[ "Boolean" ] = BoolFunctionBM;
        functorsBM[ "CrowdKnowledge" ] = PopulationKnowledgeFunctionBM;
        functorsBM[ "DateTime" ] = GDHFunctionBM;
        functorsBM[ "datetime" ] = GDHFunctionBM;
        functorsBM[ "Enumeration" ] = EnumerationFunctionBM;
        functorsBM[ "enumeration" ] = EnumerationFunctionBM;
        functorsBM[ "EquipmentType" ] = EquipmentTypeFunctionBM;
        functorsBM[ "EquipmentTypeList" ] = EquipmentTypeListFunctionBM;
        functorsBM[ "GDH" ] = GDHFunctionBM;
        functorsBM[ "Heading" ] = DirectionFunctionBM;
        functorsBM[ "Integer" ] = NumericFunctionBM;
        functorsBM[ "IntegerList" ] = NumericListFunctionBM;
        functorsBM[ "Itinerary" ] = PathFunctionBM;
        functorsBM[ "LocationComposite" ] = LocationCompositeFunctionBM;
        functorsBM[ "LocationCompositeList" ] = LocationCompositeListFunctionBM;
        functorsBM[ "MaintenancePriorities" ] = MaintenancePrioritiesFunctionBM;
        functorsBM[ "MedicalPriorities" ] = MedicalPrioritiesFunctionBM;
        functorsBM[ "NatureAtlas" ] = NatureAtlasFunctionBM;
        functorsBM[ "Numeric" ] = NumericFunctionBM;
        functorsBM[ "NumericList" ] = NumericListFunctionBM;
        functorsBM[ "ObjectKnowledge" ] = ObjectKnowledgeFunctionBM;
        functorsBM[ "ObjectKnowledgeList" ] = ObjectKnowledgeListFunctionBM;
        functorsBM[ "Path" ] = PathFunctionBM;
        functorsBM[ "PathList" ] = PathListFunctionBM;
        functorsBM[ "PhaseLine" ] = PhaseLineFunctionBM;
        functorsBM[ "PhaseLineList" ] = PhaseLineListFunctionBM;
        functorsBM[ "PlannedWork" ] = GenObjectFunctionBM;
        functorsBM[ "PlannedWorkList" ] = GenObjectListFunctionBM;
        functorsBM[ "Point" ] = PointFunctionBM;
        functorsBM[ "PointList" ] = PointListFunctionBM;
        functorsBM[ "Polygon" ] = AreaFunctionBM;
        functorsBM[ "PolygonList" ] = AreaListFunctionBM;
        functorsBM[ "PopulationKnowledge" ] = PopulationKnowledgeFunctionBM;
        functorsBM[ "ResourceNetworkNode" ] = ResourceNetworkNodeFunctionBM;
        functorsBM[ "ResourceNetworkNodeList" ] = ResourceNetworkNodeListFunctionBM;
        functorsBM[ "ResourceNetworkType" ] = ResourceNetworkTypeFunctionBM;
        functorsBM[ "ResourceNetworkTypeList" ] = ResourceNetworkTypeListFunctionBM;
        functorsBM[ "ResourceType" ] = DotationTypeFunctionBM;
        functorsBM[ "ResourceTypeList" ] = DotationTypeListFunctionBM;
        functorsBM[ "String" ] = StringFunctionBM;
        functorsBM[ "string" ] = StringFunctionBM;
        functorsBM[ "UrbanKnowledge" ] = UrbanBlockFunctionBM;
        functorsBM[ "UrbanKnowledgeList" ] = UrbanBlockListFunctionBM;
    }
}

class RegisterMissionParameterVisitor : public MIL_MissionParameterVisitor_ABC
{
public:
    RegisterMissionParameterVisitor( sword::Brain& brain, const directia::tools::binders::ScriptRef& refMission, directia::tools::binders::ScriptRef& knowledgeCreateFunction )
        : brain_( brain )
        , refMission_( refMission )
        , knowledgeCreateFunction_( knowledgeCreateFunction )
    {}
    virtual ~RegisterMissionParameterVisitor()
    {}
    virtual void Accept( const std::string& dianame, const MIL_OrderTypeParameter& type, MIL_MissionParameter_ABC& element )
    {
        std::string typeName = type.GetType().GetName();
        if( type.IsList() )
            typeName += "List";
        std::map< std::string, T_Function >::iterator itFind = functors.find( typeName );
        if( itFind != functors.end() )
            functors[ typeName ]( refMission_, dianame, element );
    }

private:
     sword::Brain& brain_;
     const directia::tools::binders::ScriptRef& refMission_;
     directia::tools::binders::ScriptRef& knowledgeCreateFunction_;
};

class RegisterMissionParameterBMVisitor : public MIL_MissionParameterVisitor_ABC
{
public:
    RegisterMissionParameterBMVisitor( sword::Brain& brain, const directia::tools::binders::ScriptRef& refMission, directia::tools::binders::ScriptRef& knowledgeCreateFunction )
        : brain_( brain )
        , refMission_( refMission )
        , knowledgeCreateFunction_( knowledgeCreateFunction )
    {}
    virtual ~RegisterMissionParameterBMVisitor()
    {}
    virtual void Accept( const std::string& dianame, const MIL_OrderTypeParameter& type, MIL_MissionParameter_ABC& element )
    {
        std::string typeName = type.GetType().GetName();
        if( type.IsList() )
            typeName += "List";
        std::map< std::string, T_FunctionBM >::iterator itFindBM = functorsBM.find( typeName );
        if( !!knowledgeCreateFunction_ && itFindBM != functorsBM.end() )
        {
            bool isInitialized = functorsBM[ typeName ]( brain_, knowledgeCreateFunction_, refMission_, dianame, element );
            if ( !isInitialized )
                refMission_[ dianame ] = brain_.GetScriptRef( "NIL" );
        }
    }

private:
     sword::Brain& brain_;
     const directia::tools::binders::ScriptRef& refMission_;
     directia::tools::binders::ScriptRef& knowledgeCreateFunction_;
};

// -----------------------------------------------------------------------------
// Name: RegisterMissionParameters
// Created: LDC 2009-05-04
// -----------------------------------------------------------------------------
void RegisterMissionParameters( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const boost::shared_ptr< MIL_Mission_ABC > mission, bool isMasalife )
{
    InitFunctions();
    if( isMasalife )
    {
        RegisterMissionParameterBMVisitor visitor( brain, refMission, knowledgeCreateFunction );
        mission->Visit( visitor );
    }
    else
    {
        RegisterMissionParameterVisitor visitor( brain, refMission, knowledgeCreateFunction );
        mission->Visit( visitor );
    }
}

#ifndef PLATFORM
#error PLATFORM must be defined (for instance vc80 or vc100_x64) for masalife and directia plugins
#endif

#ifndef PLUGIN
#ifdef NDEBUG
#define PLUGIN46( plugin ) std::string( "'plugin_" plugin "_" BOOST_PP_STRINGIZE( PLATFORM ) "-mt-4_6.plugin'," )
#define PLUGIN( plugin ) std::string( "'plugin_" plugin "_" BOOST_PP_STRINGIZE( PLATFORM ) "-mt.plugin'," )
#else
#define PLUGIN46( plugin ) std::string( "'plugin_" plugin "_" BOOST_PP_STRINGIZE( PLATFORM ) "-mt-gd-4_6.plugin'," )
#define PLUGIN( plugin ) std::string( "'plugin_" plugin "_" BOOST_PP_STRINGIZE( PLATFORM ) "-mt-gd.plugin'," )
#endif
#endif

namespace
{
    std::map< std::string, boost::shared_ptr< sword::Brain > > brainTable;

    void LoadResourcesFile( const std::string& file, const tools::Path& dir, sword::Brain& brain )
    {
        brain.GetScriptRef( "include" )( ( dir / tools::Path( file.c_str() ) ).ToUTF8() );
    }
}

bool CreateBrain( boost::shared_ptr< sword::Brain >& pArchetypeBrain, boost::shared_ptr< sword::Brain >& pBrain,
                  const tools::Path& includePath, const tools::Path& brainFile, bool isMasalife,
                  const std::string& type, bool reload, const tools::Path& integrationDir, sword::DEC_Logger* logger )
{
    // If this is not a reloading (i.e., 'reload' is false), we create a brain that shares an archetype brain.
    // The archetypes are saved in 'brainTable' for later reuse. We create an archetype brain if there is not
    // already an archetype in the table.
    // If this is a reloading, we do not reuse an existing archetype, but create a new archetype brain and return
    // this archetype as the brain to use.
    // Each time we create a new archetype, we return true, to tell the caller that the archetype must be initialized.
    const std::string& idx = brainFile.ToUTF8();
    if( !reload )
    {
        pArchetypeBrain = brainTable[idx];
        if( pArchetypeBrain )
        {
            pBrain.reset( new sword::Brain( *pArchetypeBrain, logger ) );
            if( isMasalife )
                pBrain->GetScriptRef( "masalife.brain.core.setRole" )( type );
            return false;
        }
    }
    if( isMasalife )
        pArchetypeBrain.reset( new sword::Brain(
            "plugins={"
            + PLUGIN( "masalife_brain" )
            + PLUGIN( "communication" )
            + PLUGIN46( "errorhandler" )
            + PLUGIN46( "devtools" )
            + "} cwd='" + includePath.ToUTF8() + "'", logger ) );
    else
        pArchetypeBrain.reset( new sword::Brain(
            "plugins={"
            + PLUGIN46( "eventmanager" )
            + PLUGIN46( "motivation" )
            + PLUGIN46( "errorhandler" )
            + PLUGIN46( "devtools" )
            + "} cwd='" + includePath.ToUTF8() + "'", logger ) );
    pArchetypeBrain->RegisterFunction( "LoadResourcesFile", std::function< void( const std::string& ) >(
        boost::bind( &LoadResourcesFile, _1, integrationDir, boost::ref( *pArchetypeBrain ) ) ) );
    pArchetypeBrain->GetScriptRef( "include" )( brainFile, includePath, type );
    if( !reload )
    {
        brainTable[idx] = pArchetypeBrain;
        pBrain.reset( new sword::Brain( *pArchetypeBrain, logger ) );
    }
    else
        pBrain = pArchetypeBrain;
    if( isMasalife )
        pBrain->GetScriptRef( "masalife.brain.core.setRole" )( type );
    return true;
}

}
