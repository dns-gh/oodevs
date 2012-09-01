// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "MedicalResolver.h"
#include "clients_kernel/Tools.h"
#pragma warning( push, 0 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

using namespace plugins::logistic;

// -----------------------------------------------------------------------------
// Name: MedicalConsignData::Write
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void MedicalConsignData::operator>>( std::stringstream& output ) const
{
    static std::string separator = ConsignData_ABC::GetSeparator();
    output  << requestId_    << separator
            << tick_         << separator
            << simTime_      << separator   // << creationTick_ << separator << unitId_ << separator
            << unit_         << separator   // << providerId_   << separator
            << provider_     << separator
            << rank_         << separator
            << wound_        << separator
            << nbc_          << separator
            << mental_       << separator   // << stateId_      << separator
            << state_        << separator
            << stateEndTick_ << std::endl;
}

// -----------------------------------------------------------------------------
// Name: MedicalConsignData::Write
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
const ConsignData_ABC& MedicalConsignData::ManageMessage( const ::sword::LogMedicalHandlingCreation& msg, ConsignResolver_ABC& resolver )
{
    resolver.GetSimTime( simTime_, tick_ );
    if( msg.has_tick() )
        creationTick_ = boost::lexical_cast< std::string >( msg.tick() );
    if( msg.has_unit() )
    {
        unitId_ = boost::lexical_cast< std::string >( msg.unit().id() );
        resolver.GetAgentName( msg.unit().id(), unit_ );
    }
    if( msg.has_rank() )
        resolver.GetRankName( msg.rank(), rank_ );
    if( msg.has_wound() )
        resolver.GetWoundName( msg.wound(), wound_ );
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
// Name: MedicalConsignData::Write
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
const ConsignData_ABC& MedicalConsignData::ManageMessage( const ::sword::LogMedicalHandlingUpdate& msg, ConsignResolver_ABC& resolver )
{
    resolver.GetSimTime( simTime_, tick_ );
    if( msg.has_current_state_end_tick() )
    {
        int entTick = msg.current_state_end_tick();
        if( entTick > 0 )
            stateEndTick_ = boost::lexical_cast< std::string >( entTick );
    }
    if( msg.has_unit() )
    {
        unitId_ = boost::lexical_cast< std::string >( msg.unit().id() );
        resolver.GetAgentName( msg.unit().id(), unit_ );
    }
    if( msg.has_provider() )
    {
        providerId_ = boost::lexical_cast< std::string >( msg.provider().id() );
        resolver.GetAgentName( msg.provider().id(), provider_ );
    }
    if( msg.has_state() )
    {
        sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus eState = msg.state();
        resolver.GetMedicalName( eState, state_ );
        stateId_ = boost::lexical_cast< std::string >( static_cast< int >( eState ) );
    }
    std::string strYes = tools::translate( "logistic", "yes" ).toAscii().constData();
    std::string strNo = tools::translate( "logistic", "no" ).toAscii().constData();
    if( msg.has_wound() )
        resolver.GetWoundName( msg.wound(), wound_ );
    if( msg.has_mental_wound() )
        mental_ = msg.mental_wound() ? strYes : strNo;
    if( msg.has_nbc_contaminated() )
        nbc_ = msg.nbc_contaminated() ? strYes : strNo;
    resolver.AddToLineIndex( 1 );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MedicalResolver constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
MedicalResolver::MedicalResolver( const std::string& name, const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel ) 
    : ConsignResolver_ABC( name, model, staticModel )
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
    return      message.message().has_log_medical_handling_creation()
            ||  message.message().has_log_medical_handling_update();
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
