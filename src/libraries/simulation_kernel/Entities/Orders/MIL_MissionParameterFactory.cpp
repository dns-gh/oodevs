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
#include "MIL_ItineraryParameter.h"
#include "MIL_LimaFunctionParameter.h"
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
#include "MIL_ResourceNetworkNodeParameter.h"
#include "MIL_ResourceNetworkTypeParameter.h"
#include "MIL_StageParameter.h"
#include "MIL_StringParameter.h"
#include "MIL_TirIndirectParameter.h"
#include "MIL_UrbanBlockParameter.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Orders/MIL_OrderTypeParameter.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Tools/MIL_MessageParameters.h"
#include "protocol/Protocol.h"
#include <boost/smart_ptr/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::Create
// Created: LMT 2010-04-19
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( const MIL_OrderTypeParameter& /*type*/ )
{
    return boost::make_shared< MIL_NullParameter >();
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::Create
// Created: LDC 2009-04-29
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( const MIL_OrderTypeParameter& type, const sword::MissionParameter& message,
                                                                                 const DEC_KnowledgeResolver_ABC& resolver, const boost::optional< MT_Vector2D >& position )
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
        return Create( message.value( 0 ), resolver, position );
    }
    else
    {
        MIL_MissionParameter_ABC* ptr = new MIL_ListParameter( resolver, message.value() );
        return boost::shared_ptr<MIL_MissionParameter_ABC>( ptr );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::Create
// Created: LDC 2010-09-22
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_MissionParameter_ABC > MIL_MissionParameterFactory::Create( const sword::MissionParameter_Value& message, const DEC_KnowledgeResolver_ABC& resolver,
                                                                                   const boost::optional< MT_Vector2D >& position )
{
    MIL_MissionParameter_ABC* ptr = 0;
    MIL_EntityManager_ABC& entityManager = MIL_AgentServer::GetWorkspace().GetEntityManager();
    const ObjectTypeResolver_ABC& objectTypeResolver = MIL_AgentServer::GetWorkspace().GetObjectFactory();
    if( message.has_booleanvalue() )
        ptr = new MIL_BoolParameter( message.booleanvalue() );
    else if( message.has_areal() )
        ptr = new MIL_RealParameter( message.areal() );
    else if( message.has_intvalue() )
        ptr = new MIL_RealParameter( static_cast< float >( message.intvalue() ) );
    else if( message.has_enumeration() )
        ptr = new MIL_EnumerationParameter( message.enumeration() );
    else if( message.has_path() )
        ptr = new MIL_PathParameter( message.path() );
    else if( message.has_point() )
        ptr = new MIL_PointParameter( message.point() );
    else if( message.has_area() )
    {
        ptr = new MIL_PolygonParameter( message.area() );
        if( !static_cast< MIL_PolygonParameter* > ( ptr )->IsValid() )
            throw MASA_BADPARAM_ORDER( "area polygon is invalid" );
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
        ptr = new MIL_AgentKnowledgeParameter( message.agentknowledge(), resolver, entityManager );
    else if( message.has_object() )
        ptr = new MIL_ObjectKnowledgeParameter( message.object(), resolver, entityManager );
    else if( message.has_objectknowledge() )
        ptr = new MIL_ObjectKnowledgeParameter( message.objectknowledge(), resolver, entityManager );
    else if( message.has_crowdknowledge() )
        ptr = new MIL_PopulationKnowledgeParameter( message.crowdknowledge(), resolver, entityManager );
    else if( message.has_plannedwork() )
    {
        if( message.has_external_identifier() )
            ptr = new MIL_PlannedWorkParameter( message.plannedwork(), entityManager, message.external_identifier(), objectTypeResolver );
        else
            ptr = new MIL_PlannedWorkParameter( message.plannedwork(), entityManager, objectTypeResolver );
    }
    else if( message.has_resourcetype() )
        ptr = new MIL_DotationTypeParameter( message.resourcetype() );
    else if( message.has_equipmenttype() )
        ptr = new MIL_EquipmentTypeParameter( message.equipmenttype() );
    else if( message.has_indirectfire() )
    {
        sword::Id msg;
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
        ptr = new MIL_LimaListParameter( message.phaseline(), resolver ); // group LimaList and Lima type  before updating protobuff
//        else if( message.has_value_line() ||
//            message.has_value_intelligenceList() )
        // $$$$ LDC : These types are exclusively managed by the OrderContext.
    else if( message.has_resourcenetworknode() )
        ptr = new MIL_ResourceNetworkNodeParameter( message.resourcenetworknode() );
    else if( message.has_resourcenetworktype() )
        ptr = new MIL_ResourceNetworkTypeParameter( message.resourcenetworktype() );
    else if( message.has_limit() )
        throw MASA_BADPARAM_ORDER( "limit parameters are only allowed in mission context" );
    else if( message.has_pathfind() )
        ptr = new MIL_ItineraryParameter( message.pathfind(), position );
    else
        ptr = new MIL_ListParameter( resolver, message.list() );

    boost::shared_ptr<MIL_MissionParameter_ABC> result( ptr );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreateAgentKnowledge
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateAgentKnowledge( boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_AgentKnowledgeParameter >( agentKnowledge);
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreateDotationType
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateDotationType( const PHY_DotationCategory* dotationType )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_DotationTypeParameter >( dotationType );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreateEquipmentType
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateEquipmentType( const PHY_ComposanteTypePion* equipmentType )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_EquipmentTypeParameter >( equipmentType );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::Create
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( float param )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_RealParameter >( param );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::Create
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( int id )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_EnumerationParameter >( id );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreateInteger
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateInteger( int id )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_IntegerParameter >( id );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreateObjectKnowledge
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateObjectKnowledge( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_ObjectKnowledgeParameter >( pKnowledge );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreatePion
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePion( const DEC_Decision_ABC* pion )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_AgentParameter >( pion );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreateAgent
// Created: MCO 2012-12-03
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateAgent( const MIL_AgentPion& pion )
{
    return boost::make_shared< MIL_AgentParameter >( &pion.GetDecision() );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreatePopulationKnowledge
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePopulationKnowledge( boost::shared_ptr< DEC_Knowledge_Population > populationKnowledge )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_PopulationKnowledgeParameter >( populationKnowledge );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreatePopulationKnowledge
// Created: ABR 2012-11-29
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePopulationKnowledge( DEC_Decision_ABC* caller, int knowledgeId )
{
    boost::shared_ptr< DEC_Knowledge_Population > pKnowledge;
    auto bbKg = caller->GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
    return CreatePopulationKnowledge( pKnowledge );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::Create
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::Create( const std::string& message )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_StringParameter >( message );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreateStage
// Created: ABR 2012-12-18
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateStage( const std::string& message )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_StageParameter >( message );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreateTir
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateTir( int id )
{
    sword::Id msg;
    msg.set_id( id );
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_TirIndirectParameter >( msg );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreateLimaFunction
// Created: MMC 2013-10-14
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateLimaFunction( unsigned int id )
{
    return boost::make_shared< MIL_LimaFunctionParameter >( sword::PhaseLineOrder::Function( id ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreatePoint
// Created: LDC 2009-07-24
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePoint( const MT_Vector2D& point )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_PointParameter >( point );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreateLocation
// Created: LDC 2009-07-24
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateLocation( boost::shared_ptr< TER_Localisation > pLocation )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_LocationParameter >( pLocation );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreateUrbanBlock
// Created: MGD 2010-01-15
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateUrbanBlock( MIL_UrbanObject_ABC* pUrbanblock )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_UrbanBlockParameter >( pUrbanblock );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreateResourceNetwork
// Created: LMT 2011-05-12
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateResourceNetworkNode( boost::shared_ptr<class DEC_ResourceNetwork> resourceNetwork )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_ResourceNetworkNodeParameter >( resourceNetwork );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreateResourceNetworkType
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateResourceNetworkType( const PHY_ResourceNetworkType* resourceNetworkType )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_ResourceNetworkTypeParameter >( resourceNetworkType );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::Copy
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::Copy( const MIL_OrderType_ABC& orderType, const sword::MissionParameters& asn, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters, const DEC_KnowledgeResolver_ABC& resolver )
{
    parameters.clear();
    int size = asn.elem_size();
    for( int i = 0; i < size; ++i )
        parameters.push_back( Create( orderType.GetParameterType(i), asn.elem(i), resolver, boost::none ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreatePathConst
// Created: LDC 2009-07-08
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePathConst( const std::vector< boost::shared_ptr< MT_Vector2D > >& pointList )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_PathParameter >( pointList );
    return result;
}

// -----------------------------------------------------------------------------
// Name: void MIL_MissionParameterFactory::AssignPathConst
// Created: LMT 2011-12-16
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::AssignPathConst( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const std::vector< boost::shared_ptr< MT_Vector2D > >& pointList )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_PathParameter >( pointList );
    pMission->SetParameter( parameter, result );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreatePath
// Created: LDC 2009-07-10
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreatePath( boost::shared_ptr< MT_Vector2D > point )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_PathParameter >( point );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreateDirection
// Created: PSN
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateDirection( boost::shared_ptr< MT_Vector2D > direction )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_DirectionParameter >( direction );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreateGenObject
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateGenObject( boost::shared_ptr< DEC_Gen_Object > param )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_PlannedWorkParameter >( param );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetPawnParameter
// Created: LDC 2009-07-10
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetPawnParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, DEC_Decision_ABC* pion )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    pMission->SetParameter( parameter, CreatePion (pion ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::CreateAutomat
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
boost::shared_ptr<MIL_MissionParameter_ABC> MIL_MissionParameterFactory::CreateAutomat( DEC_Decision_ABC* automat )
{
    boost::shared_ptr<MIL_MissionParameter_ABC> result = boost::make_shared< MIL_AutomatParameter >( automat );
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetAutomatParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetAutomatParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, DEC_Decision_ABC* automat )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    pMission->SetParameter( parameter, CreateAutomat( automat ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetDotationTypeParameter
// Created: LDC 2010-08-04
// -----------------------------------------------------------------------------
void  MIL_MissionParameterFactory::SetDotationTypeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const PHY_DotationCategory* type )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    pMission->SetParameter( parameter, CreateDotationType( type ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetNumericTypeParameter
// Created: LDC 2010-08-04
// -----------------------------------------------------------------------------
void  MIL_MissionParameterFactory::SetNumericTypeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, float value )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    pMission->SetParameter( parameter, Create( value ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetBoolParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetBoolParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, bool value )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    boost::shared_ptr< MIL_BoolParameter > boolParam = boost::make_shared< MIL_BoolParameter >( value );
    pMission->SetParameter( parameter, boolParam );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetObjectKnowledgeParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetObjectKnowledgeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    pMission->SetParameter( parameter, CreateObjectKnowledge( pKnowledge ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetNatureAtlasTypeParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetNatureAtlasTypeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, int natureAtlas )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    boost::shared_ptr< MIL_AtlasNatureParameter > natureParam = boost::make_shared< MIL_AtlasNatureParameter >( natureAtlas );
    pMission->SetParameter( parameter, natureParam );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetAgentKnowledgeParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetAgentKnowledgeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    pMission->SetParameter( parameter, CreateAgentKnowledge( agentKnowledge ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetEnumereParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetEnumereParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, int value )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    pMission->SetParameter( parameter, Create( value ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetLocationParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetLocationParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< TER_Localisation > location )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    pMission->SetParameter( parameter, CreateLocation( location ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetPointParameter
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetPointParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, MT_Vector2D& point )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    pMission->SetParameter( parameter, CreatePoint( point ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetGenObjectParameter
// Created: LMT 2011-01-19
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetGenObjectParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< DEC_Gen_Object > object )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    pMission->SetParameter( parameter, CreateGenObject( object ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetUrbanBlockParameter
// Created: MGD 2010-01-15
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetUrbanBlockParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, MIL_UrbanObject_ABC* pUrbanblock )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    pMission->SetParameter( parameter, CreateUrbanBlock( pUrbanblock ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetDirectionParameter
// Created: PSN 2010-06-15
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetDirectionParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr< MT_Vector2D > direction )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    pMission->SetParameter( parameter, CreateDirection( direction ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetResourceNetworkParameter
// Created: LMT 2011-05-12
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetResourceNetworkNodeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, boost::shared_ptr<class DEC_ResourceNetwork> resourceNetwork )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    pMission->SetParameter( parameter, CreateResourceNetworkNode( resourceNetwork ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetResourceNetworkTypeParameter
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetResourceNetworkTypeParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const PHY_ResourceNetworkType* resourceNetworkType )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    pMission->SetParameter( parameter, CreateResourceNetworkType( resourceNetworkType ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::SetCrowdKnowledgeParameter
// Created: DDA 2011-08-29
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::SetCrowdKnowledgeParameter( DEC_Decision_ABC* caller, boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, int knowledgeId )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    pMission->SetParameter( parameter, CreatePopulationKnowledge( caller, knowledgeId ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionParameterFactory::AssignMissionListParameter
// Created: GGE 2011-06-27
// -----------------------------------------------------------------------------
void MIL_MissionParameterFactory::AssignMissionListParameter( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter, const std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& params )
{
    if( !pMission.get() )
        throw MASA_EXCEPTION( "Invalid mission" );
    boost::shared_ptr< MIL_ListParameter > listParam = boost::make_shared< MIL_ListParameter >( params );
    pMission->SetParameter( parameter, listParam );
}
