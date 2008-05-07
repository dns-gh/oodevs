// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "DisPlugin.h"
#include "DisExtensionFactory.h"
#include "DisTypeResolver.h"
#include "FireManager.h"
#include "clients_kernel/CoordinateConverter.h"
#include "dispatcher/Config.h"
#include "dispatcher/Model.h"
#include <xeumeuleu/xml.h>
#include "UdpNetwork.h"

using namespace dis;
using namespace dispatcher;

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
// Name: DisPlugin constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisPlugin::DisPlugin( dispatcher::Model& model, const dispatcher::Config& config, xml::xistream& xis )
    : model_( model )
    , network_( new UdpNetwork( xml::attribute< std::string >( xis, "server" ), xml::attribute< unsigned short >( xis, "port" ) ) )
    , converter_( new kernel::CoordinateConverter( config ) )
    , resolver_( new DisTypeResolver( config.BuildPhysicalChildFile( "dis.xml" ) ) )
    , factory_( new DisExtensionFactory( *network_, *this, *converter_, *resolver_, xis ) )
    , fires_( new FireManager( *network_, *this, (unsigned char)xml::attribute< unsigned short >( xis, "exercise"  ) ) )
    , timeStep_( ReadTimeStep( config.GetSessionFile() ) )
    , time_( 0 )
{
    model_.RegisterFactory( *factory_ );
}

// -----------------------------------------------------------------------------
// Name: DisPlugin destructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisPlugin::~DisPlugin()
{
    model_.UnregisterFactory( *factory_ );
}

// -----------------------------------------------------------------------------
// Name: DisPlugin::Receive
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
void DisPlugin::Receive( const ASN1T_MsgsSimToClient& message )
{
    if( message.msg.t == T_MsgsSimToClient_msg_msg_control_begin_tick )
        time_ += timeStep_;
    fires_->Update( message );
}

// -----------------------------------------------------------------------------
// Name: DisPlugin::NotifyClientAuthenticated
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
void DisPlugin::NotifyClientAuthenticated( ClientPublisher_ABC& , Profile_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisPlugin::NotifyClientLeft
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
void DisPlugin::NotifyClientLeft( ClientPublisher_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisPlugin::GetTime
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
unsigned long DisPlugin::GetTime() const
{
    // 31MSBs are for the 3600s in the hour
    // LSB is to flag absolute time
//    const float seconds = float( time_ % 3600 );
//    unsigned long timestamp = (unsigned long)( (seconds/3600.0) * 0x7fffffff );
//    timestamp <<= 1;
//    return timestamp;
    return time_ % 3600;
}

