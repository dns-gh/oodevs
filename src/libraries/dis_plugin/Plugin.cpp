// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "Plugin.h"
#include "FireManager.h"
#include "Model.h"
#include "UdpNetwork.h"
#include "clients_kernel/CoordinateConverter.h"
#include "dispatcher/Config.h"
#include "dispatcher/Model_ABC.h"
#include "rpr/EntityTypeResolver.h"
#include "tic/PlatformDelegateFactory.h"
#include <xeumeuleu/xml.hpp>

#pragma warning( disable: 4355 )

using namespace plugins::dis;

namespace
{
    unsigned ReadTimeStep( const std::string& session )
    {
        xml::xifstream xis( session );
        unsigned step;
        xis >> xml::start( "session" ) >> xml::start( "config" )
                >> xml::start( "simulation" ) >> xml::start( "time" )
                    >> xml::attribute( "step", step );
        return step;
    }
}

// -----------------------------------------------------------------------------
// Name: Plugin constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
Plugin::Plugin( dispatcher::Model_ABC& model, const dispatcher::Config& config, dispatcher::Logger_ABC& logger, xml::xistream& xis )
    : model_    ( model )
    , network_  ( new UdpNetwork( xis.attribute< std::string >( "server" ), xis.attribute< unsigned short >( "port" ), logger ) )
    , converter_( new kernel::CoordinateConverter( config ) )
    , timeStep_ ( ReadTimeStep( config.GetSessionFile() ) )
    , platforms_( new tic::PlatformDelegateFactory( *converter_, static_cast< float >( timeStep_ ) ) )
    , resolver_ ( new rpr::EntityTypeResolver( xml::xifstream( config.BuildPhysicalChildFile( "dis.xml" ) ) ) )
    , factory_  ( new Model( *network_, *this, *converter_, *resolver_, xis, *platforms_ ) )
    , fires_    ( new FireManager( *network_, *this, static_cast< unsigned char >( xis.attribute< unsigned short >( "exercise"  ) ) ) )
    , time_     ( 0 )
{
    model_.RegisterFactory( *factory_ );
}

// -----------------------------------------------------------------------------
// Name: Plugin destructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
Plugin::~Plugin()
{
    model_.UnregisterFactory( *factory_ );
}

// -----------------------------------------------------------------------------
// Name: Plugin::Receive
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
void Plugin::Receive( const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_control_begin_tick() )
        time_ += timeStep_;
    fires_->Update( wrapper );
}

// -----------------------------------------------------------------------------
// Name: Plugin::GetTime
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
unsigned long Plugin::GetTime() const
{
    // 31MSBs are for the 3600s in the hour
    // LSB is to flag absolute time
    const float seconds = float( time_ % 3600 );
    unsigned long timestamp = (unsigned long)( (seconds / 3600.0) * 0x7fffffff );
    timestamp <<= 1;
    return timestamp;
}

