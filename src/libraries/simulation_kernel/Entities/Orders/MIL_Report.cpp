//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Pion/MIL_Report.cpp $
// $Author: Nld $
// $Modtime: 21/03/05 12:17 $
// $Revision: 8 $
// $Workfile: MIL_Report.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Report.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Formation.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Populations/MIL_Population.h"
#include "MIL_MissionParameter_ABC.h"
#include "MIL_ParameterType_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <xeumeuleu/xml.hpp>

MIL_Report::T_ReportMap      MIL_Report::reports_;
MIL_IDManager                MIL_Report::ids_;

// -----------------------------------------------------------------------------
// Name: MIL_Report::Initialize
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
void MIL_Report::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing reports types" );
    xis >> xml::start( "reports" )
            >> xml::list( "report", &MIL_Report::ReadReport )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::ReadReport
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void MIL_Report::ReadReport( xml::xistream& xis )
{
    unsigned int id;
    xis >> xml::attribute( "id", id );
    const MIL_Report*& pReport = reports_[ id ];
    if( pReport )
        xis.error( "Report id already defined" );
    pReport = new MIL_Report( id, xis );
}

namespace
{
    MIL_Report::E_Type ConvertCategory( const std::string& category )
    {
        if( category == "information" )
            return MIL_Report::eRcTypeMessage;
        if( category == "exceptional" )
            return MIL_Report::eRcTypeEvent;
        if( category == "warning" )
            return MIL_Report::eRcTypeWarning;
        return MIL_Report::eRcTypeOperational; // default
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Report constructor
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
MIL_Report::MIL_Report( unsigned int id, xml::xistream& xis )
    : nID_( id )
    , strMessage_( xis.attribute< std::string >( "message" ) ) 
{
    category_ = ConvertCategory( xis.attribute< std::string >( "category" ) );
    xis >> xml::list( "parameter", *this, &MIL_Report::ReadParameter );
    ids_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report destructor
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
MIL_Report::~MIL_Report()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::ReadParameter
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void MIL_Report::ReadParameter( xml::xistream& xis )
{
    std::string strType;
    xis >> xml::attribute( "type", strType );
    const MIL_ParameterType_ABC* pParameter = MIL_ParameterType_ABC::Find( strType );
    if( !pParameter )
        xis.error( "Unknown parameter type" );
    parameters_.push_back( pParameter );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::DoSend
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
bool MIL_Report::DoSend( client::Report& message, E_Type nType, std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& params ) const
{
    if( params.size() != parameters_.size() )
    {
        MT_LOG_ERROR_MSG( "Report '" << strMessage_ << "' send failed (invalid DIA parameters)" );
        return false;
    }
    message().mutable_report()->set_id( std::numeric_limits< unsigned int >::max() - ids_.GetFreeId() ); // descending order
    message().mutable_type()->set_id( nID_ );
    message().set_category( sword::Report::EnumReportType( nType ) );
    NET_ASN_Tools::WriteGDH( MIL_AgentServer::GetWorkspace().GetRealTime(), *message().mutable_time() );
    for( unsigned int i = 0; i < parameters_.size(); ++i )
    {
        if( !params[ i ]->IsOfType( parameters_[i]->GetType() ) )
            return false;
        sword::MissionParameter& paramProtobuff = *message().mutable_parameters()->add_elem();
        if( !params[ i ]->ToElement( *paramProtobuff.mutable_value()->Add() ) )
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::Send
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------

template<>
void MIL_Report::Send< MIL_Automate >( const MIL_Automate& sender, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& diaParameters ) const
{
    client::Report message;
    if( DoSend( message, nType, diaParameters ) )
    {
        message().mutable_source()->mutable_automat()->set_id( sender.GetID() );
        message.Send( NET_Publisher_ABC::Publisher() );
    }
}

template<>
void MIL_Report::Send< MIL_Population >( const MIL_Population& sender, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& diaParameters ) const
{
    client::Report message;
    if( DoSend( message, nType, diaParameters ) )
    {
        message().mutable_source()->mutable_crowd()->set_id( sender.GetID() );
        message.Send( NET_Publisher_ABC::Publisher() );
    }
}

template<>
void MIL_Report::Send<  MIL_Agent_ABC >( const MIL_Agent_ABC& sender, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& diaParameters ) const
{
    client::Report message;
    if( DoSend( message, nType, diaParameters ) )
    {
        message().mutable_source()->mutable_unit()->set_id( sender.GetID() );
        message.Send( NET_Publisher_ABC::Publisher() );
    }
}

template<>
void MIL_Report::Send<  MIL_AgentPion >( const MIL_AgentPion& sender, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& diaParameters ) const
{
    MIL_Report::Send< MIL_Agent_ABC >( sender, nType, diaParameters );
}

template<>
void MIL_Report::Send< MIL_Formation >( const MIL_Formation& sender, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& diaParameters ) const
{
    client::Report message;
    if( DoSend( message, nType, diaParameters ) )
    {
        message().mutable_source()->mutable_formation()->set_id( sender.GetID() );
        message.Send( NET_Publisher_ABC::Publisher() );
    }
}

template<>
void MIL_Report::Send< DEC_Decision_ABC >( const DEC_Decision_ABC& sender, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& diaParameters ) const
{
    client::Report message;
    if( DoSend( message, nType, diaParameters ) )
    {
        MIL_AgentServer::GetWorkspace().GetEntityManager().SetToTasker( *message().mutable_source(), sender.GetID() );
        message.Send( NET_Publisher_ABC::Publisher() );
    }
}
