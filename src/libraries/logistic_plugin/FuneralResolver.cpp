// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "FuneralResolver.h"
#include "clients_kernel/Tools.h"

using namespace plugins::logistic;

// -----------------------------------------------------------------------------
// Name: FuneralConsignData::operator>>
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void FuneralConsignData::operator>>( std::stringstream& output ) const
{
    static const std::string separator = ConsignData_ABC::GetSeparator();
    output  << requestId_            << separator
            << tick_                 << separator
            << simTime_              << separator   // << creationTick_         << separator    << unitId_  << separator
            << unit_                 << separator   // << handlingUnitId_       << separator
            << handlingUnit_         << separator   // << conveyingUnitId_      << separator
            << conveyingUnit_        << separator   
            << rank_                 << separator   // << packagingResourceId_  << separator
            << packagingResource_    << separator   // << stateId_              << separator
            << state_                << separator
            << stateEndTick_         << std::endl;
}

// -----------------------------------------------------------------------------
// Name: FuneralConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
const ConsignData_ABC& FuneralConsignData::ManageMessage( const ::sword::LogFuneralHandlingCreation& msg, ConsignResolver_ABC& resolver )
{
    resolver.GetSimTime( simTime_, tick_ );
    if( msg.has_tick() )
        creationTick_ = boost::lexical_cast< std::string >( msg.tick() );
    if( msg.has_unit() )
    {
        int unitId = msg.unit().id();
        unitId_ = boost::lexical_cast< std::string >( unitId );
        resolver.GetAgentName( unitId, unit_ );
    }
    if( msg.has_rank() )
        resolver.GetRankName( msg.rank(), rank_ );
    resolver.AddToLineIndex( 1 );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: FuneralConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
const ConsignData_ABC& FuneralConsignData::ManageMessage( const ::sword::LogFuneralHandlingUpdate& msg, ConsignResolver_ABC& resolver )
{
    resolver.GetSimTime( simTime_, tick_ );
    if( msg.has_current_state_end_tick() )
    {
        int entTick = msg.current_state_end_tick();
        if( entTick > 0 )
            stateEndTick_ = boost::lexical_cast< std::string >( entTick );
    }
    if( msg.has_handling_unit() )
    {
        if( msg.handling_unit().has_automat() )
        {
            int automatId = static_cast< int >( msg.handling_unit().automat().id() );
            handlingUnitId_ = boost::lexical_cast< std::string >( automatId );
            resolver.GetAutomatName( automatId, handlingUnit_ );
        }
        else if( msg.handling_unit().has_formation() )
        {
            int formationId = static_cast< int >( msg.handling_unit().formation().id() );
            handlingUnitId_ = boost::lexical_cast< std::string >( formationId );
            resolver.GetFormationName( formationId, handlingUnit_ );
        }
    }
    if( msg.has_convoying_unit() )
    {
        int unitId = msg.convoying_unit().id();
        if( unitId > 0 )
        {
            conveyingUnitId_ = boost::lexical_cast< std::string >( unitId );
            resolver.GetAgentName( unitId, conveyingUnit_ );
        }
    }
    if( msg.has_packaging_resource() )
    {
        packagingResourceId_ = boost::lexical_cast< std::string >( msg.packaging_resource().id() );
        resolver.GetResourceName( msg.packaging_resource(), packagingResource_ );
    }
    if( msg.has_state() )
    {
        sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus eState = msg.state();
        resolver.GetFuneralName( eState, state_ );
        stateId_ = boost::lexical_cast< std::string >( static_cast< int >( eState ) );
    }
    resolver.AddToLineIndex( 1 );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: FuneralResolver constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
FuneralResolver::FuneralResolver( const std::string& name, const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel ) 
    : ConsignResolver_ABC( name, model, staticModel )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FuneralResolver destructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
FuneralResolver::~FuneralResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FuneralResolver::IsManageable
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
bool FuneralResolver::IsManageable( const sword::SimToClient& message )
{
    return     message.message().has_log_funeral_handling_creation()
            || message.message().has_log_funeral_handling_update();
}

// -----------------------------------------------------------------------------
// Name: FuneralResolver::ManageMessage
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void FuneralResolver::ManageMessage( const sword::SimToClient& message )
{
    if( message.message().has_log_funeral_handling_creation() )
        TraceConsign< ::sword::LogFuneralHandlingCreation, FuneralConsignData >( message.message().log_funeral_handling_creation(), output_ );
    if( message.message().has_log_funeral_handling_update() )
        TraceConsign< ::sword::LogFuneralHandlingUpdate, FuneralConsignData >( message.message().log_funeral_handling_update(), output_ );
}

// -----------------------------------------------------------------------------
// Name: FuneralResolver::InitHeader
// Created: MMC 2012-08-24
// -----------------------------------------------------------------------------
void FuneralResolver::InitHeader()
{
    FuneralConsignData consign( tools::translate( "logistic", "request id" ).toAscii().constData() );
    consign.tick_                   = tools::translate( "logistic", "tick" ).toAscii().constData();
    consign.creationTick_           = tools::translate( "logistic", "creation tick" ).toAscii().constData();
    consign.stateEndTick_           = tools::translate( "logistic", "state end tick" ).toAscii().constData();
    consign.simTime_                = tools::translate( "logistic", "GDH" ).toAscii().constData();
    consign.unitId_                 = tools::translate( "logistic", "unit id" ).toAscii().constData();
    consign.handlingUnitId_         = tools::translate( "logistic", "handling unit id" ).toAscii().constData();
    consign.conveyingUnitId_        = tools::translate( "logistic", "conveying unit id" ).toAscii().constData();
    consign.packagingResourceId_    = tools::translate( "logistic", "packaging resource id" ).toAscii().constData();
    consign.stateId_                = tools::translate( "logistic", "state id" ).toAscii().constData();
    consign.unit_                   = tools::translate( "logistic", "unit" ).toAscii().constData();
    consign.handlingUnit_           = tools::translate( "logistic", "handling unit" ).toAscii().constData();
    consign.conveyingUnit_          = tools::translate( "logistic", "conveying unit" ).toAscii().constData();
    consign.rank_                   = tools::translate( "logistic", "rank" ).toAscii().constData();
    consign.packagingResource_      = tools::translate( "logistic", "packaging resource" ).toAscii().constData();
    consign.state_                  = tools::translate( "logistic", "state" ).toAscii().constData();
    SetHeader( consign );
}
