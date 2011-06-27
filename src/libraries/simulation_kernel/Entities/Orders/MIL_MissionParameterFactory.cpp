// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Orders/MIL_OrderTypeParameter.h"
#include "MIL_MissionParameterFactory.h"
#include "MIL_AgentServer.h"  
#include "MIL_Mission_ABC.h"
#include "MIL_AgentKnowledgeParameter.h"
#include "MIL_AgentParameter.h"
#include "MIL_AutomatParameter.h"
#include "MIL_AtlasNatureParameter.h"
#include "MIL_BoolParameter.h"
#include "MIL_DateTimeParameter.h"
#include "MIL_DirectionParameter.h"
#include "MIL_DotationTypeParameter.h"
#include "MIL_EnumerationParameter.h"
#include "MIL_EquipmentTypeParameter.h"
#include "MIL_IntegerParameter.h"
#include "MIL_LimaListParameter.h"
#include "MIL_ListParameter.h"
#include "MIL_LocationParameter.h"
#include "MIL_LogMaintenancePrioritiesParameter.h"
#include "MIL_LogMedicalPrioritiesParameter.h"
#include "MIL_OrderType_ABC.h"
#include "MIL_NullParameter.h"
#include "MIL_ObjectKnowledgeParameter.h"
#include "MIL_PathParameter.h"
#include "MIL_PlannedWorkParameter.h"
#include "MIL_PointParameter.h"
#include "MIL_PolygonParameter.h"
#include "MIL_PopulationKnowledgeParameter.h"
#include "MIL_RealParameter.h"
#include "MIL_ResourceNetworkParameter.h"
#include "MIL_StringParameter.h"
#include "MIL_TirIndirectParameter.h"
#include "MIL_UrbanBlockParameter.h"
#include "MIL_ExternalIdentifierWrapper.h"
#include "Network/NET_AsnException.h"
#include "protocol/Protocol.h"


// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create
// Created: LMT 2010-04-19
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( const MIL_OrderTypeParameter& /*type*/ )
{
    return boost::shared_ptr<MIL_MissionParameter_ABC>( new MIL_NullParameter() );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::Create
// Created: LDC 2009-04-29
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( const MIL_OrderTypeParameter& type, const sword::MissionParameter& message, const DEC_KnowledgeResolver_ABC& resolver )
{
    if( message.null_value() )
    {
        MIL_MissionParameter_ABC* ptr = 0;
        ptr = new MIL_NullParameter();
        boost::shared_ptr<MIL_MissionParameter_ABC> result( ptr );
        return result;
    }
    else if( !type.IsList()  && message.value_size() == 1 )
    {
        return Create( message.value().Get( 0 ), resolver );
    }
    else
    {
        MIL_MissionParameter_ABC* ptr = new MIL_ListParameter( resolver, message.value() );
        return boost::shared_ptr<MIL_MissionParameter_ABC>( ptr );
    }
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create
// Created: LDC 2010-09-22
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( const sword::MissionParameter_Value& message, const DEC_KnowledgeResolver_ABC& resolver )
{
    MIL_MissionParameter_ABC* ptr = 0;
    MIL_EntityManager_ABC& entityManager = MIL_AgentServer::GetWorkspace().GetEntityManager();
    if( message.has_booleanvalue() )
        ptr = new MIL_BoolParameter( message.booleanvalue() );
    else if( message.has_areal() )
        ptr = new MIL_RealParameter( message.areal() );
    else if( message.has_enumeration() )
        ptr = new MIL_EnumerationParameter( message.enumeration() );
    else if( message.has_path() )
        ptr = new MIL_PathParameter( message.path() );
    else if( message.has_point() )
        ptr = new MIL_PointParameter( message.point() );
    else if( message.has_area() )
    {
        ptr = new MIL_PolygonParameter( message.area() );
        if ( !static_cast< MIL_PolygonParameter* > ( ptr )->IsValid() )
            throw NET_AsnException< sword::OrderAck::ErrorCode >( sword::OrderAck::error_invalid_parameter );
    }
    else if( message.has_location() )
        ptr = new MIL_LocationParameter( message.location() );
    else if( message.has_heading() )
        ptr = new MIL_DirectionParameter( message.heading() );
    else if( message.has_nature() )
        ptr = new MIL_AtlasNatureParameter( message.nature() );
    else if( message.has_agent() )
        ptr = new MIL_AgentParameter( message.agent(), entityManager );
    else if( message.has_automat() )
        ptr = new MIL_AutomatParameter( message.automat(), entityManager );
    else if( message.has_agentknowledge() )
        ptr = new MIL_AgentKnowledgeParameter( message.agentknowledge(), resolver );
    else if( message.has_objectknowledge() )
        ptr = new MIL_ObjectKnowledgeParameter( message.objectknowledge(), resolver );
    else if( message.has_crowdknowledge() )
        ptr = new MIL_PopulationKnowledgeParameter( message.crowdknowledge(), resolver );
    else if( message.has_plannedwork() )
        ptr = new MIL_PlannedWorkParameter( message.plannedwork(), entityManager );
    else if ( message.has_resourcetype() )
        ptr = new MIL_DotationTypeParameter( message.resourcetype() );
    else if( message.has_equipmenttype() )
        ptr = new MIL_EquipmentTypeParameter( message.equipmenttype() );
    else if( message.has_indirectfire() )
    {
        sword::FireId msg;
        msg.set_id( message.indirectfire().id() );
        ptr = new MIL_TirIndirectParameter( msg );
    }
    else if( message.has_urbanknowledge() ) // $$$$ _RC_ LGY 2011-02-24: urban block identifier
        ptr = new MIL_UrbanBlockParameter( message.urbanknowledge(), entityManager );
    else if( message.has_datetime() )
        ptr = new MIL_DateTimeParameter( message.datetime() );
    else if( message.has_logmaintenancepriorities() )
        ptr = new MIL_LogMaintenancePrioritiesParameter( message.logmaintenancepriorities() );
    else if( message.has_logmedicalpriorities() )
        ptr = new MIL_LogMedicalPrioritiesParameter( message.logmedicalpriorities() );
    else if( message.has_acharstr() )
        ptr = new MIL_StringParameter( message.acharstr() );
    else if( message.has_missionobjective() )              // $$$$ LDC : This type doesn't seem to ever be converted to/from. Only lists of objectives exist.
        ptr = new MIL_NullParameter();
    else if( message.has_phaseline() )
        ptr = new MIL_LimaListParameter( message.phaseline() ); // group LimaList and Lima type  before updating protobuff
//        else if( message.has_value_line() ||
//            message.has_value_intelligenceList() )
        // $$$$ LDC : These types are exclusively managed by the OrderContext.
    else if( message.has_resourcenetwork() )
        ptr = new MIL_ResourceNetworkParameter( message.resourcenetwork() );
    else
        ptr = new MIL_ListParameter( resolver, message.list() );

    boost::shared_ptr<MIL_MissionParameter_ABC> result( ptr );
    if( message.has_external_identifier() )
        return boost::shared_ptr< MIL_MissionParameter_ABC >( new MIL_ExternalIdentifierWrapper( result, message.external_identifier() ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateAgentKnowledge
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateAgentKnowledge( boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_AgentKnowledgeParameter( agentKnowledge) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateDotationType
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateDotationType( const PHY_DotationCategory* dotationType )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_DotationTypeParameter( dotationType ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateEquipmentType
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateEquipmentType( const PHY_ComposanteTypePion* equipmentType )
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
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_RealParameter( param ) );
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
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateInteger
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateInteger( int id )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_IntegerParameter( id ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateObjectKnowledge
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateObjectKnowledge( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_ObjectKnowledgeParameter( pKnowledge ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePion
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePion( DEC_Decision_ABC* pion )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_AgentParameter( dynamic_cast<DEC_RolePion_Decision*>( pion ) ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePopulationKnowledge
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePopulationKnowledge( DEC_Knowledge_Population* populationKnowledge )
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
    sword::FireId msg;
    msg.set_id( id );
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_TirIndirectParameter( msg ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePoint
// Created: LDC 2009-07-24
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePoint( const MT_Vector2D& point )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_PointParameter( point ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateLocation
// Created: LDC 2009-07-24
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateLocation( boost::shared_ptr< TER_Localisation > pLocation )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_LocationParameter( pLocation ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateUrbanBlock
// Created: MGD 2010-01-15
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateUrbanBlock( UrbanObjectWrapper* pUrbanblock )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_UrbanBlockParameter( pUrbanblock ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateResourceNetwork
// Created: LMT 2011-05-12
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateResourceNetwork( boost::shared_ptr<class DEC_ResourceNetwork> resourceNetwork )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_ResourceNetworkParameter( resourceNetwork ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::Copy
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::Copy( const MIL_OrderType_ABC& orderType, const sword::MissionParameters& asn, std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& parameters, const DEC_KnowledgeResolver_ABC& resolver )
{
    parameters.clear();
    int size = asn.elem_size();
    for ( int i = 0; i < size; ++i )
        parameters.push_back( Create( orderType.GetParameterType(i), asn.elem(i), resolver ) );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePathConst
// Created: LDC 2009-07-08
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePathConst( const std::vector< boost::shared_ptr< MT_Vector2D > >& pointList )
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
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateDirection
// Created: PSN
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateDirection( boost::shared_ptr< MT_Vector2D > direction )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_DirectionParameter( direction ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateGenObject
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateGenObject( boost::shared_ptr< DEC_Gen_Object > param )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_PlannedWorkParameter( param ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetPawnParameter
// Created: LDC 2009-07-10
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetPawnParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, DEC_Decision_ABC* pion )
{
    pMission->SetParameter( parameter, CreatePion (pion ) );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateAutomat
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateAutomat( DEC_Decision_ABC* automat )
{    
    boost::shared_ptr<MIL_MissionParameter_ABC> result( new MIL_AutomatParameter( dynamic_cast<DEC_AutomateDecision*>( automat ) ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetAutomatParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetAutomatParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, DEC_Decision_ABC* automat )
{
    pMission->SetParameter( parameter, CreateAutomat( automat ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetDotationTypeParameter
// Created: LDC 2010-08-04
// -----------------------------------------------------------------------------
void  MIL_MissionParameterFactory::SetDotationTypeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const PHY_DotationCategory* type )
{
    pMission->SetParameter( parameter, CreateDotationType( type ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetNumericTypeParameter
// Created: LDC 2010-08-04
// -----------------------------------------------------------------------------
void  MIL_MissionParameterFactory::SetNumericTypeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, float value )
{
    pMission->SetParameter( parameter, Create( value ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetBoolParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetBoolParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, bool value )
{
    boost::shared_ptr< MIL_BoolParameter > boolParam( new MIL_BoolParameter( value ) );
    pMission->SetParameter( parameter, boolParam );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetObjectKnowledgeParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetObjectKnowledgeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    pMission->SetParameter( parameter, CreateObjectKnowledge( pKnowledge ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetNatureAtlasTypeParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetNatureAtlasTypeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, int natureAtlas )
{
    boost::shared_ptr< MIL_AtlasNatureParameter > natureParam( new MIL_AtlasNatureParameter( natureAtlas ) );
    pMission->SetParameter( parameter, natureParam );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetAgentKnowledgeParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetAgentKnowledgeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge )
{
    pMission->SetParameter( parameter, CreateAgentKnowledge( agentKnowledge ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetEnumereParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetEnumereParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, int value )
{
    pMission->SetParameter( parameter, Create( value ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetLocationParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetLocationParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< TER_Localisation > location )
{
    pMission->SetParameter( parameter, CreateLocation( location ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetPointParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetPointParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, MT_Vector2D& point )
{
    pMission->SetParameter( parameter, CreatePoint( point ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetGenObjectParameter
// Created: LMT 2011-01-19
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetGenObjectParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< DEC_Gen_Object > object )
{
    pMission->SetParameter( parameter, CreateGenObject( object ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetUrbanBlockParameter
// Created: MGD 2010-01-15
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetUrbanBlockParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, UrbanObjectWrapper* pUrbanblock )
{
    pMission->SetParameter( parameter, CreateUrbanBlock( pUrbanblock ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetDirectionParameter
// Created: PSN 2010-06-15
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetDirectionParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< MT_Vector2D > direction )
{
    pMission->SetParameter( parameter, CreateDirection( direction ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetResourceNetworkParameter
// Created: LMT 2011-05-12
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetResourceNetworkParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr<class DEC_ResourceNetwork> resourceNetwork )
{
    pMission->SetParameter( parameter, CreateResourceNetwork( resourceNetwork ) );
}

void MIL_MissionParameterFactory::AssignMissionListParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& params )
{
    boost::shared_ptr< MIL_ListParameter > listParam( new MIL_ListParameter( params ) );
    pMission->SetParameter( parameter, listParam );
}
