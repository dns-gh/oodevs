// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "PositionsPlugin.h"
#include "protocol/Protocol.h"
#include <boost/filesystem.hpp>

using namespace plugins::positions;

const std::string PositionsPlugin::separator_ = ";";

// -----------------------------------------------------------------------------
// Name: PositionsPlugin constructor
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
PositionsPlugin::PositionsPlugin( const std::string& filename, unsigned int exportFrequency )
    : filepath_       ( filename )
    , exportFrequency_( exportFrequency )
    , firstTick_      ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PositionsPlugin destructor
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
PositionsPlugin::~PositionsPlugin()
{
    try
    {
        Export(); // $$$$ MCO : try catch to prevent exceptions from leaving the destructor !
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: PositionsPlugin::NotifyClientAuthenticated
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
void PositionsPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PositionsPlugin::NotifyClientLeft
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
void PositionsPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PositionsPlugin::Receive
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
void PositionsPlugin::Receive( const sword::SimToClient& message )
{
    if( message.message().has_control_begin_tick() )
    {
        currentTime_ = boost::posix_time::from_iso_string( message.message().control_begin_tick().date_time().data() );
        if( firstTick_ || currentTime_ - lastExportTime_ >= boost::posix_time::seconds( exportFrequency_ ) )
            Export();
        firstTick_ = false;
    }
    else if( message.message().has_party_creation() )
    {
        teams_.AddTeam( message.message().party_creation().party().id(), message.message().party_creation().name() );
    }
    else if( message.message().has_automat_creation() )
    {
        teams_.AddAutomat( message.message().automat_creation().party().id(), message.message().automat_creation().automat().id() );
    }
    else if( message.message().has_unit_creation() )
    {
        teams_.AddUnit( message.message().unit_creation().automat().id(), message.message().unit_creation().unit().id(), message.message().unit_creation().name(), static_cast< unsigned >( times_.size() ) );
    }
    else if( message.message().has_unit_attributes() && message.message().unit_attributes().has_position() )
    {
        teams_.UpdatePosition( message.message().unit_attributes().unit().id(), message.message().unit_attributes().position() );
    }
}

// -----------------------------------------------------------------------------
// Name: PositionsPlugin::Export
// Created: ABR 2011-04-01
// -----------------------------------------------------------------------------
void PositionsPlugin::Export()
{
    SaveTime();
    ExportData();
}

// -----------------------------------------------------------------------------
// Name: PositionsPlugin::SaveTime
// Created: MCO 2011-04-05
// -----------------------------------------------------------------------------
void PositionsPlugin::SaveTime()
{
    if( currentTime_.is_not_a_date_time() )
        return;
    lastExportTime_ = currentTime_;
    times_.push_back( boost::posix_time::to_simple_string( lastExportTime_ ) );
}

// -----------------------------------------------------------------------------
// Name: PositionsPlugin::ExportData
// Created: MCO 2011-04-05
// -----------------------------------------------------------------------------
void PositionsPlugin::ExportData() const
{
    boost::filesystem::ofstream file( filepath_ );
    file << "Team (id)" << separator_ << "Unit (id)";
    for( CIT_Times it = times_.begin(); it != times_.end(); ++it )
        file << separator_ << *it;
    file << std::endl;
    teams_.Export( file );
}
