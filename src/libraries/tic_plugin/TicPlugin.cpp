// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tic_plugin_pch.h"
#include "TicPlugin.h"
#include "ExtensionFactory.h"
#include "dispatcher/Config.h"
#include "dispatcher/Model.h"
#include "clients_kernel/CoordinateConverter.h"
#include <xeumeuleu/xml.h>

using namespace tic;

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
// Name: TicPlugin constructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
TicPlugin::TicPlugin( dispatcher::Model& model, const dispatcher::Config& config )
    : model_    ( model )
    , converter_( new kernel::CoordinateConverter( config ) )
    , factory_  ( new ExtensionFactory( *converter_, (float)ReadTimeStep( config.GetSessionFile() ) ) )
{
    model_.RegisterFactory( *factory_ );
}

// -----------------------------------------------------------------------------
// Name: TicPlugin destructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
TicPlugin::~TicPlugin()
{
    model_.UnregisterFactory( *factory_ );
}

// -----------------------------------------------------------------------------
// Name: TicPlugin::Receive
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void TicPlugin::Receive( const ASN1T_MsgsSimToClient&  )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TicPlugin::NotifyClientAuthenticated
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void TicPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& , dispatcher::Profile_ABC&  )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TicPlugin::NotifyClientLeft
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void TicPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC&  )
{
    // NOTHING
}
