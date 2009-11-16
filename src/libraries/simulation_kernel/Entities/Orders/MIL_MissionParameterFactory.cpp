// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_MissionParameterFactory.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"

#include "MIL_Mission_ABC.h"

#include "MIL_AgentKnowledgeParameter.h"
#include "MIL_AgentKnowledgeListParameter.h"
#include "MIL_AgentListParameter.h"
#include "MIL_AgentParameter.h"
#include "MIL_AutomatParameter.h"
#include "MIL_AutomatListParameter.h"
#include "MIL_AtlasNatureParameter.h"
#include "MIL_BoolParameter.h"
#include "MIL_DateTimeParameter.h"
#include "MIL_DirectionParameter.h"
#include "MIL_DotationTypeParameter.h"
#include "MIL_EnumerationParameter.h"
#include "MIL_EquipmentTypeParameter.h"
#include "MIL_LocationParameter.h"
#include "MIL_LocationListParameter.h"
#include "MIL_LogMaintenancePrioritiesParameter.h"
#include "MIL_LogMedicalPrioritiesParameter.h"
#include "MIL_MissionObjectiveListParameter.h"
#include "MIL_NullParameter.h"
#include "MIL_ObjectKnowledgeParameter.h"
#include "MIL_ObjectKnowledgeListParameter.h"
#include "MIL_PathParameter.h"
#include "MIL_PathListParameter.h"
#include "MIL_PlannedWorkParameter.h"
#include "MIL_PlannedWorkListParameter.h"
#include "MIL_PointParameter.h"
#include "MIL_PointListParameter.h"
#include "MIL_PolygonParameter.h"
#include "MIL_PolygonListParameter.h"
#include "MIL_PopulationKnowledgeParameter.h"
#include "MIL_RealParameter.h"
#include "MIL_StringParameter.h"
#include "MIL_TirIndirectParameter.h"
#include "MIL_UrbanBlockParameter.h"

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::Create
// Created: LDC 2009-04-29
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( const ASN1T_MissionParameter& asn, const DEC_KnowledgeResolver_ABC& resolver )
{
    MIL_MissionParameter_ABC* ptr = 0;
    if( asn.null_value )
        ptr = new MIL_NullParameter();
    else
    {
        MIL_EntityManager_ABC& entityManager = MIL_AgentServer::GetWorkspace().GetEntityManager();
        switch( asn.value.t )
        {
            case T_MissionParameter_value_aBool:
            {
                ptr = new MIL_BoolParameter( asn.value.u.aBool ? true : false );
                break;
            }
            case T_MissionParameter_value_aReal:
            {
                ptr = new MIL_RealParameter( asn.value.u.aReal );
                break;
            }
            case T_MissionParameter_value_enumeration:
            {
                ptr = new MIL_EnumerationParameter( asn.value.u.enumeration );
                break;
            }
            case T_MissionParameter_value_path:
            {
                ptr = new MIL_PathParameter( *asn.value.u.path );
                break;
            }
            case T_MissionParameter_value_pathList:
            {
                ptr = new MIL_PathListParameter( *asn.value.u.pathList );
                break;
            }
            case T_MissionParameter_value_point:
            {
                ptr = new MIL_PointParameter( *asn.value.u.point );
                break;
            }
            case T_MissionParameter_value_pointList:
            {
                ptr = new MIL_PointListParameter( *asn.value.u.pointList );
                break;
            }
            case T_MissionParameter_value_polygon:
            {
                ptr = new MIL_PolygonParameter( *asn.value.u.polygon );
                break;
            }
            case T_MissionParameter_value_polygonList:
            {
                ptr = new MIL_PolygonListParameter( *asn.value.u.polygonList );
                break;
            }
            case T_MissionParameter_value_location:
            {
                ptr = new MIL_LocationParameter( *asn.value.u.location );
                break;
            }
            case T_MissionParameter_value_locationList:
            {
                ptr = new MIL_LocationListParameter( *asn.value.u.locationList );
                break;
            }
            case T_MissionParameter_value_heading:
            {
                ptr = new MIL_DirectionParameter( asn.value.u.heading );
                break;
            }
            case T_MissionParameter_value_atlasNature:
            {
                ptr = new MIL_AtlasNatureParameter( *asn.value.u.atlasNature );
                break;
            }
            case T_MissionParameter_value_unit:
            {
                ptr = new MIL_AgentParameter( asn.value.u.unit, entityManager );
                break;
            }
            case T_MissionParameter_value_unitList:
            {
                ptr = new MIL_AgentListParameter( *asn.value.u.unitList, entityManager );
                break;
            }
            case T_MissionParameter_value_automat:
            {
                ptr = new MIL_AutomatParameter( asn.value.u.automat, entityManager );
                break;
            }
            case T_MissionParameter_value_automatList:
            {
                ptr = new MIL_AutomatListParameter( *asn.value.u.automatList, entityManager );
                break;
            }
            case T_MissionParameter_value_unitKnowledge:
            {
                ptr = new MIL_AgentKnowledgeParameter( asn.value.u.unitKnowledge, resolver );
                break;
            }
            case T_MissionParameter_value_unitKnowledgeList:
            {
                ptr = new MIL_AgentKnowledgeListParameter( *asn.value.u.unitKnowledgeList, resolver );
                break;
            }
            case T_MissionParameter_value_objectKnowledge:
            {
                ptr = new MIL_ObjectKnowledgeParameter( asn.value.u.objectKnowledge, resolver );
                break;
            }
            case T_MissionParameter_value_objectKnowledgeList:
            {
                ptr = new MIL_ObjectKnowledgeListParameter( *asn.value.u.objectKnowledgeList, resolver );
                break;
            }
            case T_MissionParameter_value_populationKnowledge:
            {
                ptr = new MIL_PopulationKnowledgeParameter( asn.value.u.populationKnowledge, resolver );
                break;
            }
            case T_MissionParameter_value_plannedWork:
            {
                ptr = new MIL_PlannedWorkParameter( *asn.value.u.plannedWork, entityManager );
                break;
            }
            case T_MissionParameter_value_plannedWorkList:
            {
                ptr = new MIL_PlannedWorkListParameter( *asn.value.u.plannedWorkList, entityManager );
                break;
            }
            case T_MissionParameter_value_dotationType:
            {
                ptr = new MIL_DotationTypeParameter( asn.value.u.dotationType );
                break;
            }
            case T_MissionParameter_value_equipmentType:
            {
                ptr = new MIL_EquipmentTypeParameter( asn.value.u.equipmentType );
                break;
            }
            case T_MissionParameter_value_tirIndirect:
            {
                ptr = new MIL_TirIndirectParameter( asn.value.u.tirIndirect );
                break;
            }
            case T_MissionParameter_value_dateTime:
            {
                ptr = new MIL_DateTimeParameter( *asn.value.u.dateTime );
                break;
            }
            case T_MissionParameter_value_logMaintenancePriorities:
            {
                ptr = new MIL_LogMaintenancePrioritiesParameter( *asn.value.u.logMaintenancePriorities );
                break;
            }
            case T_MissionParameter_value_logMedicalPriorities:
            {
                ptr = new MIL_LogMedicalPrioritiesParameter( *asn.value.u.logMedicalPriorities );
                break;
            }
            case T_MissionParameter_value_aCharStr:
            {
                ptr = new MIL_StringParameter( asn.value.u.aCharStr );
                break;
            }
            case T_MissionParameter_value_missionObjective:
            {
                // $$$$ LDC : This type doesn't seem to ever be converted to/from. Only lists of objectives exist.
                ptr = new MIL_NullParameter();
                break;
            }
            case T_MissionParameter_value_missionObjectiveList:
            {
                ptr = new MIL_MissionObjectiveListParameter( *asn.value.u.missionObjectiveList );
                break;
            }
            case T_MissionParameter_value_urbanBlock:
            {
                ptr = new MIL_UrbanBlockParameter( asn.value.u.urbanBlock, MIL_AgentServer::GetWorkspace() );
                break;
            }
            case T_MissionParameter_value_line:
            case T_MissionParameter_value_limasOrder:
            case T_MissionParameter_value_intelligenceList:
                // $$$$ LDC : These types are exclusively managed by the OrderContext.
            default:
                ptr = new MIL_NullParameter();
        }
    }
    boost::shared_ptr<MIL_MissionParameter_ABC> result( ptr );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_AgentKnowledgeParameter( agentKnowledge) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( const PHY_DotationCategory* dotationType )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_DotationTypeParameter( dotationType ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( const PHY_ComposanteTypePion* equipmentType )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_EquipmentTypeParameter( equipmentType ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( float param )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_RealParameter( double( param ) ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( int id )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_EnumerationParameter( id ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_ObjectKnowledgeParameter( pKnowledge ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( DEC_RolePion_Decision* pion )
{ 
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_AgentParameter( pion ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePawn
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePawn( DEC_Decision_ABC* pion )
{
    return Create( dynamic_cast<DEC_RolePion_Decision*>( pion ) );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( DEC_AutomateDecision* automate )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_AutomatParameter( automate ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( DEC_Knowledge_Population* populationKnowledge )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_PopulationKnowledgeParameter( populationKnowledge ) );
    return result; 
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( const std::string& message )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_StringParameter( message ) );
    return result; 
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateTir
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateTir( int id )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_TirIndirectParameter( id ) );
    return result; 
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create
// Created: LDC 2009-07-24
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( const MT_Vector2D& point )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_PointParameter( point ) );
    return result; 
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create
// Created: LDC 2009-07-24
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( boost::shared_ptr< TER_Localisation > pLocation )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_LocationParameter( pLocation ) );
    return result; 
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::Copy
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::Copy( const ASN1T_MissionParameters& asn, std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& parameters, const DEC_KnowledgeResolver_ABC& resolver )
{
    parameters.clear();
    int size = asn.n;
    for ( int i = 0; i < size; ++i )
        parameters.push_back( Create( asn.elem[i], resolver ) );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePath
// Created: LDC 2009-07-08
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePath( const std::vector< boost::shared_ptr< MT_Vector2D > >& pointList )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_PathParameter( pointList ) );
    return result; 
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePath
// Created: LDC 2009-07-10
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePath( boost::shared_ptr< MT_Vector2D > point )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_PathParameter( point ) );
    return result; 
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetPawnParameter
// Created: LDC 2009-07-10
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetPawnParameter( MIL_Mission_ABC* pMission, const std::string& parameter, DEC_Decision_ABC* pion )
{
    pMission->SetParameter( parameter, CreatePawn (pion ) );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateAutomat
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateAutomat( DEC_Decision_ABC* automat )
{
    return Create( dynamic_cast<DEC_AutomateDecision*>( automat ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetAutomatParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetAutomatParameter( MIL_Mission_ABC* pMission, const std::string& parameter, DEC_Decision_ABC* automat )
{
    pMission->SetParameter( parameter, CreateAutomat( automat ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetBoolParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetBoolParameter( MIL_Mission_ABC* pMission, const std::string& parameter, bool value )
{
    boost::shared_ptr< MIL_BoolParameter > boolParam( new MIL_BoolParameter( value ) );
    pMission->SetParameter( parameter, boolParam );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetObjectKnowledgeParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetObjectKnowledgeParameter( MIL_Mission_ABC* pMission, const std::string& parameter, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    pMission->SetParameter( parameter, Create( pKnowledge ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetNatureAtlasTypeParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetNatureAtlasTypeParameter( MIL_Mission_ABC* pMission, const std::string& parameter, int natureAtlas )
{
    boost::shared_ptr< MIL_AtlasNatureParameter > natureParam( new MIL_AtlasNatureParameter( natureAtlas ) );
    pMission->SetParameter( parameter, natureParam );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetAutomatListParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetAutomatListParameter( MIL_Mission_ABC* pMission, const std::string& parameter, const std::vector< DEC_Decision_ABC* >& automatList )
{
    std::vector< DEC_AutomateDecision* > typedAutomatList;
    for( std::vector< DEC_Decision_ABC* >::const_iterator it = automatList.begin(); it != automatList.end(); ++it )
        typedAutomatList.push_back( dynamic_cast<DEC_AutomateDecision*>( *it ) );
    boost::shared_ptr< MIL_AutomatListParameter > listParam( new MIL_AutomatListParameter( typedAutomatList ) );
    pMission->SetParameter( parameter, listParam );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetAgentKnowledgeParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetAgentKnowledgeParameter( MIL_Mission_ABC* pMission, const std::string& parameter, boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge )
{
    pMission->SetParameter( parameter, Create( agentKnowledge ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetPathListParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetPathListParameter( MIL_Mission_ABC* pMission, const std::string& parameter, const std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& list )
{
    boost::shared_ptr< MIL_PathListParameter > listParam( new MIL_PathListParameter( list ) );
    pMission->SetParameter( parameter, listParam );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetEnumereParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetEnumereParameter( MIL_Mission_ABC* pMission, const std::string& parameter, int value )
{
    pMission->SetParameter( parameter, Create( value ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetLocationParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetLocationParameter( MIL_Mission_ABC* pMission, const std::string& parameter, boost::shared_ptr< TER_Localisation > location )
{
    pMission->SetParameter( parameter, Create( location ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetObjectKnowledgeListParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetObjectKnowledgeListParameter( MIL_Mission_ABC* pMission, const std::string& parameter, const std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& objectKnowledgeList )
{
    boost::shared_ptr< MIL_ObjectKnowledgeListParameter > listParam( new MIL_ObjectKnowledgeListParameter( objectKnowledgeList ) );
    pMission->SetParameter( parameter, listParam );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetPointParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetPointParameter( MIL_Mission_ABC* pMission, const std::string& parameter, MT_Vector2D& point )
{
    pMission->SetParameter( parameter, Create( point ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetAgentKnowledgeListParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetAgentKnowledgeListParameter( MIL_Mission_ABC* pMission, const std::string& parameter, const std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& agentKnowledgeList )
{
    boost::shared_ptr< MIL_AgentKnowledgeListParameter > listParam( new MIL_AgentKnowledgeListParameter( agentKnowledgeList ) );
    pMission->SetParameter( parameter, listParam );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetGenObjectListParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetGenObjectListParameter( MIL_Mission_ABC* pMission, const std::string& parameter, const std::vector< boost::shared_ptr< DEC_Gen_Object > >& list )
{
    boost::shared_ptr< MIL_PlannedWorkListParameter > listParam( new MIL_PlannedWorkListParameter( list ) );
    pMission->SetParameter( parameter, listParam );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetPionListParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetPionListParameter( MIL_Mission_ABC* pMission, const std::string& parameter, const std::vector< DEC_Decision_ABC* >& pionList )
{
    std::vector< DEC_RolePion_Decision* > agentList;
    for( std::vector< DEC_Decision_ABC* >::const_iterator it = pionList.begin(); it != pionList.end(); ++it )
        agentList.push_back( dynamic_cast<DEC_RolePion_Decision*>( *it ) );
    boost::shared_ptr< MIL_AgentListParameter > listParam( new MIL_AgentListParameter( agentList ) );
    pMission->SetParameter( parameter, listParam );    
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetLocationListParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetLocationListParameter( MIL_Mission_ABC* pMission, const std::string& parameter, const std::vector< boost::shared_ptr< TER_Localisation > >& locationList )
{
    boost::shared_ptr< MIL_LocationListParameter > listParam( new MIL_LocationListParameter( locationList ) );
    pMission->SetParameter( parameter, listParam );    
}
