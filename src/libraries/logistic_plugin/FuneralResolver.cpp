// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "FuneralResolver.h"
#include "NameResolver_ABC.h"
#include "clients_kernel/Tools.h"

using namespace plugins::logistic;

// -----------------------------------------------------------------------------
// Name: FuneralConsignData::operator>>
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void FuneralConsignData::operator>>( std::stringstream& output ) const
{
    output  << requestId_            << separator_
            << tick_                 << separator_
            << simTime_              << separator_   // << creationTick_         << separator_    << unitId_  << separator_
            << unit_                 << separator_   // << handlingUnitId_       << separator_
            << handlingUnit_         << separator_   // << conveyingUnitId_      << separator_
            << conveyingUnit_        << separator_   
            << rank_                 << separator_   // << packagingResourceId_  << separator_
            << packagingResource_    << separator_   // << stateId_              << separator_
            << state_                << separator_
            << stateEndTick_         << std::endl;
}

// -----------------------------------------------------------------------------
// Name: FuneralConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
const ConsignData_ABC& FuneralConsignData::ManageMessage( const ::sword::LogFuneralHandlingCreation& msg, ConsignResolver_ABC& resolver )
{
    const NameResolver_ABC& nameResolver = resolver.GetNameResolver();
    resolver.GetSimTime( simTime_, tick_ );
    if( msg.has_tick() )
        creationTick_ = boost::lexical_cast< std::string >( msg.tick() );
    if( msg.has_unit() )
    {
        int unitId = msg.unit().id();
        unitId_ = boost::lexical_cast< std::string >( unitId );
        nameResolver.GetAgentName( unitId, unit_ );
    }
    if( msg.has_rank() )
        nameResolver.GetRankName( msg.rank(), rank_ );
    resolver.AddToLineIndex( 1 );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: FuneralConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
const ConsignData_ABC& FuneralConsignData::ManageMessage( const ::sword::LogFuneralHandlingUpdate& msg, ConsignResolver_ABC& resolver )
{
    const NameResolver_ABC& nameResolver = resolver.GetNameResolver();
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
            nameResolver.GetAutomatName( automatId, handlingUnit_ );
        }
        else if( msg.handling_unit().has_formation() )
        {
            int formationId = static_cast< int >( msg.handling_unit().formation().id() );
            handlingUnitId_ = boost::lexical_cast< std::string >( formationId );
            nameResolver.GetFormationName( formationId, handlingUnit_ );
        }
    }
    if( msg.has_convoying_unit() )
    {
        int unitId = msg.convoying_unit().id();
        if( unitId > 0 )
        {
            conveyingUnitId_ = boost::lexical_cast< std::string >( unitId );
            nameResolver.GetAgentName( unitId, conveyingUnit_ );
        }
    }
    if( msg.has_packaging_resource() )
    {
        packagingResourceId_ = boost::lexical_cast< std::string >( msg.packaging_resource().id() );
        nameResolver.GetResourceName( msg.packaging_resource(), packagingResource_ );
    }
    if( msg.has_state() )
    {
        sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus eState = msg.state();
        nameResolver.GetFuneralName( eState, state_ );
        stateId_ = boost::lexical_cast< std::string >( static_cast< int >( eState ) );
    }
    resolver.AddToLineIndex( 1 );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: FuneralResolver constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
FuneralResolver::FuneralResolver( const std::string& name, const NameResolver_ABC& nameResolver ) 
    : ConsignResolver_ABC( name, nameResolver )
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
            || message.message().has_log_funeral_handling_update()
            || message.message().has_log_funeral_handling_destruction();
}

// -----------------------------------------------------------------------------
// Name: FuneralResolver::IsEmptyLineMessage
// Created: MMC 2012-09-11
// -----------------------------------------------------------------------------
bool FuneralResolver::IsEmptyLineMessage( const sword::SimToClient& message )
{
    return message.message().has_log_funeral_handling_destruction();
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
    if( message.message().has_log_funeral_handling_destruction() && message.message().log_funeral_handling_destruction().has_request() )
        DestroyConsignData( message.message().log_funeral_handling_destruction().request().id() );
}

// -----------------------------------------------------------------------------
// Name: FuneralResolver::InitHeader
// Created: MMC 2012-08-24
// -----------------------------------------------------------------------------
void FuneralResolver::InitHeader()
{
    FuneralConsignData consign( tools::translate( "logistic", "request id" ).toAscii().constData()  );
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

// -----------------------------------------------------------------------------
// Name: FuneralResolver::CreateConsignData
// Created: MMC 2012-08-24
// -----------------------------------------------------------------------------
ConsignData_ABC* FuneralResolver::CreateConsignData( int requestId )
{ 
    return static_cast< ConsignData_ABC* >( new FuneralConsignData( boost::lexical_cast< std::string >( requestId ) ) ); 
}
