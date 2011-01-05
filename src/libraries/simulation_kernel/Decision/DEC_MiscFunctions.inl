// *****************************************************************************
//
// $Created: NLD 2004-07-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_MiscFunctions.inl $
// $Author: Nld $
// $Modtime: 9/03/05 16:17 $
// $Revision: 2 $
// $Workfile: DEC_MiscFunctions.inl $
//
// *****************************************************************************

#include "Entities/Orders/MIL_Report.h"
#include "Entities/Orders/MIL_MissionParameter_ABC.h"
#include "Entities/Orders/MIL_MissionParameterFactory.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "MT_Tools/MT_FormatString.h"

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_MiscFunctions::Report
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
template< typename T >
void DEC_MiscFunctions::Report( T& caller, int type, int reportId )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > params;
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_MiscFunctions::ReportAgentKnowledge
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
template< typename T > 
void DEC_MiscFunctions::ReportAgentKnowledge( T& caller, int type, int reportId, boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam( MIL_MissionParameterFactory::Create( agentKnowledge ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_MiscFunctions::ReportDotationType
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
template< typename T >
void DEC_MiscFunctions::ReportDotationType( T& caller, int type, int reportId, const PHY_DotationCategory* dotationType )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam( MIL_MissionParameterFactory::Create( dotationType ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_MiscFunctions::ReportEquipmentType
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
template< typename T >
void DEC_MiscFunctions::ReportEquipmentType( T& caller, int type, int reportId, const PHY_ComposanteTypePion* equipmentType )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam( MIL_MissionParameterFactory::Create( equipmentType ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_MiscFunctions::ReportFloat
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
template< typename T >
void DEC_MiscFunctions::ReportFloat( T& caller, int type, int reportId, float param )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam( MIL_MissionParameterFactory::Create( param ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_MiscFunctions::ReportFloatFloat
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
template< typename T >
void DEC_MiscFunctions::ReportFloatFloat( T& caller, int type, int reportId, float param1, float param2 )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam1( MIL_MissionParameterFactory::Create( param1 ) );
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam2( MIL_MissionParameterFactory::Create( param2 ) );
        params.push_back( missionParam1 );
        params.push_back( missionParam2 );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_MiscFunctions::ReportId
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
template< typename T >
void DEC_MiscFunctions::ReportId( T& caller, int type, int reportId, int id )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam( MIL_MissionParameterFactory::Create( id ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_MiscFunctions::ReportObjectKnoweldge
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
template< typename T >
void DEC_MiscFunctions::ReportObjectKnoweldge( T& caller, int type, int reportId, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam( MIL_MissionParameterFactory::Create( pKnowledge ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_MiscFunctions::ReportPion
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
template< typename T >
void DEC_MiscFunctions::ReportPion( T& caller, int type, int reportId, DEC_Decision_ABC* pion )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        DEC_RolePion_Decision* pionDec = static_cast< DEC_RolePion_Decision* >( pion ); // $$$$ LDC: FIXME Is an ABC or a concrete type passed from dia?
        std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam( MIL_MissionParameterFactory::Create( pionDec ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_MiscFunctions::ReportPionAutomate
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
template< typename T >
void DEC_MiscFunctions::ReportPionAutomate( T& caller, int type, int reportId, DEC_Decision_ABC* pion, DEC_Decision_ABC* automate )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > params;
        DEC_RolePion_Decision* pionDec = static_cast< DEC_RolePion_Decision* >( pion ); // $$$$ LDC: FIXME Is an ABC or a concrete type passed from dia?
        boost::shared_ptr<MIL_MissionParameter_ABC> missionParam1( MIL_MissionParameterFactory::Create( pionDec ) );
        DEC_AutomateDecision* automateDec = static_cast< DEC_AutomateDecision* >( automate ); // $$$$ LDC: FIXME Is an ABC or a concrete type passed from dia?
        boost::shared_ptr<MIL_MissionParameter_ABC> missionParam2( MIL_MissionParameterFactory::Create( automateDec ) );
        params.push_back( missionParam1 );
        params.push_back( missionParam2 );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_MiscFunctions::ReportPionPion
// Created: MGD 2010-06-16
// -----------------------------------------------------------------------------
template< typename T >
void DEC_MiscFunctions::ReportPionPion( T& caller, int type, int reportId, DEC_Decision_ABC* pion1, DEC_Decision_ABC* pion2 )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > params;
        DEC_RolePion_Decision* pionDec = static_cast< DEC_RolePion_Decision* >( pion1 ); // $$$$ LDC: FIXME Is an ABC or a concrete type passed from dia?
        boost::shared_ptr<MIL_MissionParameter_ABC> missionParam1( MIL_MissionParameterFactory::Create( pionDec ) );
        DEC_RolePion_Decision* pionDec2 = static_cast< DEC_RolePion_Decision* >( pion2 ); // $$$$ LDC: FIXME Is an ABC or a concrete type passed from dia?
        boost::shared_ptr<MIL_MissionParameter_ABC> missionParam2( MIL_MissionParameterFactory::Create( pionDec2 ) );
        params.push_back( missionParam1 );
        params.push_back( missionParam2 );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}


// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_MiscFunctions::ReportPopulationKnowledge
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
template< typename T >
void DEC_MiscFunctions::ReportPopulationKnowledge( T& caller, int type, int reportId, int populationKnowledge )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > params;
        DEC_Knowledge_Population* pKnowledge = caller.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID(populationKnowledge);
        boost::shared_ptr<MIL_MissionParameter_ABC> missionParam( MIL_MissionParameterFactory::Create( pKnowledge ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_MiscFunctions::ReportTirPion
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
template< typename T >
void DEC_MiscFunctions::ReportTirPion( T& caller, int type, int reportId, int id )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > params;
        boost::shared_ptr<MIL_MissionParameter_ABC> missionParam( MIL_MissionParameterFactory::CreateTir( id ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_MiscFunctions::ReportString
// Created: MGD 2010-03-24
// -----------------------------------------------------------------------------
template< typename T >
void DEC_MiscFunctions::ReportString( T& caller, int type, int reportId, const std::string& message )
{
    if( const MIL_Report* pReport = MIL_Report::Find( reportId ) )
    {
        std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > params;
        boost::shared_ptr< MIL_MissionParameter_ABC > missionParam( MIL_MissionParameterFactory::Create( message ) );
        params.push_back( missionParam );
        pReport->Send( caller, MIL_Report::E_Type( type ), params );
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::Trace
// Created: AHC 2009-07-30
//-----------------------------------------------------------------------------
template <typename T>
void DEC_MiscFunctions::Trace( const T& caller, const std::string& message )
{
    client::Trace msg;
    MIL_AgentServer::GetWorkspace().GetEntityManager().SetToTasker( *msg().mutable_source(), caller.GetID() );
    *msg().mutable_message() = message.c_str();
    msg.Send( NET_Publisher_ABC::Publisher() );
}

//-----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::Debug
// Created: AHC 2009-07-30
//-----------------------------------------------------------------------------
template <typename T>
void DEC_MiscFunctions::Debug( const T& caller, const std::string& callerType, const std::string& message )
{
    if( !MIL_AgentServer::GetWorkspace().GetConfig().UseDecDebug() )
        return;
    MT_LOG_INFO_MSG( MT_FormatString( "%s %d says : [%s]", callerType.c_str(), caller.GetID(), message.c_str() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::DebugDrawPoints
// Created: AHC 2009-07-30
// -----------------------------------------------------------------------------
template <typename T>
void DEC_MiscFunctions::DebugDrawPoints(const T& caller, std::vector< boost::shared_ptr< MT_Vector2D > > points )
{
    client::DebugPoints message;
    MIL_AgentServer::GetWorkspace().GetEntityManager().SetToTasker( *message().mutable_source(), caller.GetID() );
    NET_ASN_Tools::WriteCoordinates( points, *message().mutable_coordinates() );
    message.Send( NET_Publisher_ABC::Publisher() );
    delete message().mutable_coordinates();
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::DebugDrawPoint
// Created: NLD 2005-03-22
// Created: AHC 2009-07-30-----------------------------------------------
template <typename T>
void DEC_MiscFunctions::DebugDrawPoint( const T& caller, const MT_Vector2D* pPoint )
{
    assert( pPoint );
    client::DebugPoints message;
    MIL_AgentServer::GetWorkspace().GetEntityManager().SetToTasker( *message().mutable_source(), caller.GetID() );
    NET_ASN_Tools::WritePoint( *pPoint, *message().mutable_coordinates()->add_elem() );
    message.Send( NET_Publisher_ABC::Publisher() );
}
