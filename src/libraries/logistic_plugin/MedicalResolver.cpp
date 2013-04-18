// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "MedicalResolver.h"
#include "NameResolver_ABC.h"
#include "clients_kernel/Tools.h"
#pragma warning( push, 0 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

using namespace plugins::logistic;

// -----------------------------------------------------------------------------
// Name: MedicalConsignData::operator>>
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void MedicalConsignData::operator>>( std::stringstream& output ) const
{
    output  << requestId_    << separator_
            << tick_         << separator_
            << simTime_      << separator_   // << creationTick_ << separator_ 
            << unit_         << separator_
            << unitId_       << separator_
            << provider_     << separator_
            << providerId_   << separator_
            << rank_         << separator_
            << wound_        << separator_
            << nbc_          << separator_
            << mental_       << separator_   // << stateId_      << separator_
            << state_        << separator_
            << stateEndTick_ << std::endl;

}

// -----------------------------------------------------------------------------
// Name: MedicalConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
const ConsignData_ABC& MedicalConsignData::ManageMessage( const ::sword::LogMedicalHandlingCreation& msg, ConsignResolver_ABC& resolver )
{
    const NameResolver_ABC& nameResolver = resolver.GetNameResolver();
    if( msg.has_tick() )
        creationTick_ = boost::lexical_cast< std::string >( msg.tick() );
    if( msg.has_unit() )
    {
        unitId_ = boost::lexical_cast< std::string >( msg.unit().id() );
        nameResolver.GetAgentName( msg.unit().id(), unit_ );
    }
    if( msg.has_rank() )
        nameResolver.GetRankName( msg.rank(), rank_ );
    if( msg.has_wound() )
        nameResolver.GetWoundName( msg.wound(), wound_ );
    std::string strYes = tools::translate( "logistic", "yes" ).toAscii().constData();
    std::string strNo = tools::translate( "logistic", "no" ).toAscii().constData();
    if( msg.has_mental_wound() )
        mental_ = msg.mental_wound() ? strYes : strNo;
    if( msg.has_nbc_contaminated() )
        nbc_ = msg.nbc_contaminated() ? strYes : strNo;
    resolver.AddToLineIndex( 1 );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MedicalConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
const ConsignData_ABC& MedicalConsignData::ManageMessage( const ::sword::LogMedicalHandlingUpdate& msg, ConsignResolver_ABC& resolver )
{
    const NameResolver_ABC& nameResolver = resolver.GetNameResolver();
    if( msg.has_current_state_end_tick() )
    {
        int entTick = msg.current_state_end_tick();
        if( entTick > 0 )
            stateEndTick_ = boost::lexical_cast< std::string >( entTick );
    }
    if( msg.has_unit() )
    {
        unitId_ = boost::lexical_cast< std::string >( msg.unit().id() );
        nameResolver.GetAgentName( msg.unit().id(), unit_ );
    }
    if( msg.has_provider() )
    {
        providerId_ = boost::lexical_cast< std::string >( msg.provider().id() );
        nameResolver.GetAgentName( msg.provider().id(), provider_ );
    }
    if( msg.has_state() )
    {
        sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus eState = msg.state();
        nameResolver.GetMedicalName( eState, state_ );
        stateId_ = boost::lexical_cast< std::string >( static_cast< int >( eState ) );
    }
    std::string strYes = tools::translate( "logistic", "yes" ).toAscii().constData();
    std::string strNo = tools::translate( "logistic", "no" ).toAscii().constData();
    if( msg.has_wound() )
        nameResolver.GetWoundName( msg.wound(), wound_ );
    if( msg.has_mental_wound() )
        mental_ = msg.mental_wound() ? strYes : strNo;
    if( msg.has_nbc_contaminated() )
        nbc_ = msg.nbc_contaminated() ? strYes : strNo;
    resolver.AddToLineIndex( 1 );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MedicalConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
const ConsignData_ABC& MedicalConsignData::ManageMessage( const ::sword::LogMedicalHandlingDestruction& msg, ConsignResolver_ABC& resolver )
{
    const NameResolver_ABC& nameResolver = resolver.GetNameResolver();
    resolver.GetSimTime( simTime_, tick_ );
    if( msg.has_unit() )
    {
        unitId_ = boost::lexical_cast< std::string >( msg.unit().id() );
        nameResolver.GetAgentName( msg.unit().id(), unit_ );
    }
    state_ = tools::translate( "logistic", "consign finished" ).toAscii().constData();
    resolver.AddToLineIndex( 1 );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MedicalResolver constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
MedicalResolver::MedicalResolver( const std::string& name, const NameResolver_ABC& nameResolver, int maxHistoricFiles, int maxFileLines ) 
    : ConsignResolver_ABC( name, nameResolver, maxHistoricFiles, maxFileLines )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalResolver destructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
MedicalResolver::~MedicalResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalResolver::IsManageable
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
bool MedicalResolver::IsManageable( const sword::SimToClient& message )
{
    return  message.message().has_log_medical_handling_creation()
        ||  message.message().has_log_medical_handling_update()
        ||  message.message().has_log_medical_handling_destruction();
}

// -----------------------------------------------------------------------------
// Name: MedicalResolver::IsEmptyLineMessage
// Created: MMC 2012-09-11
// -----------------------------------------------------------------------------
bool MedicalResolver::IsEmptyLineMessage( const sword::SimToClient& message )
{
    return message.message().has_log_medical_handling_destruction();
}

// -----------------------------------------------------------------------------
// Name: MedicalResolver::ManageMessage
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void MedicalResolver::ManageMessage( const sword::SimToClient& message )
{
    if( message.message().has_log_medical_handling_creation() )
        TraceConsign< ::sword::LogMedicalHandlingCreation, MedicalConsignData >( message.message().log_medical_handling_creation(), output_ );
    if( message.message().has_log_medical_handling_update() )
        TraceConsign< ::sword::LogMedicalHandlingUpdate, MedicalConsignData >( message.message().log_medical_handling_update(), output_ );
    if( message.message().has_log_medical_handling_destruction() && message.message().log_medical_handling_destruction().has_request() )
    {
        TraceConsign< ::sword::LogMedicalHandlingDestruction, MedicalConsignData >( message.message().log_medical_handling_destruction(), output_ );
        DestroyConsignData( message.message().log_medical_handling_destruction().request().id() );
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalResolver::InitHeader
// Created: MMC 2012-08-24
// -----------------------------------------------------------------------------
void MedicalResolver::InitHeader()
{
    MedicalConsignData consign( tools::translate( "logistic", "request id" ).toAscii().constData() );
    consign.tick_           = tools::translate( "logistic", "tick" ).toAscii().constData();
    consign.creationTick_   = tools::translate( "logistic", "creation tick" ).toAscii().constData();
    consign.stateEndTick_   = tools::translate( "logistic", "state end tick" ).toAscii().constData();
    consign.simTime_        = tools::translate( "logistic", "GDH" ).toAscii().constData();
    consign.unitId_         = tools::translate( "logistic", "unit id" ).toAscii().constData();
    consign.providerId_     = tools::translate( "logistic", "provider id" ).toAscii().constData();
    consign.stateId_        = tools::translate( "logistic", "state id" ).toAscii().constData();
    consign.rank_           = tools::translate( "logistic", "rank" ).toAscii().constData();
    consign.wound_          = tools::translate( "logistic", "wound" ).toAscii().constData();
    consign.nbc_            = tools::translate( "logistic", "nbc" ).toAscii().constData();
    consign.mental_         = tools::translate( "logistic", "mental" ).toAscii().constData();
    consign.unit_           = tools::translate( "logistic", "unit" ).toAscii().constData();
    consign.provider_       = tools::translate( "logistic", "provider" ).toAscii().constData();
    consign.state_          = tools::translate( "logistic", "state" ).toAscii().constData();
    SetHeader( consign );
}

// -----------------------------------------------------------------------------
// Name: MedicalResolver::MaintenanceResolver
// Created: MMC 2012-09-03
// -----------------------------------------------------------------------------
ConsignData_ABC* MedicalResolver::CreateConsignData( int requestId )
{
    return static_cast< ConsignData_ABC* >( new MedicalConsignData( boost::lexical_cast< std::string >( requestId ) ) ); 
}
