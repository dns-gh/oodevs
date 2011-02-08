// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "DisExtensionFactory.h"
#include "DisExtension.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Formation.h"
#include "dispatcher/Side.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::dis;

// -----------------------------------------------------------------------------
// Name: DisExtensionFactory constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisExtensionFactory::DisExtensionFactory( UdpNetwork& network, const Time_ABC& time, const kernel::CoordinateConverter_ABC& converter, const DisTypeResolver& resolver, xml::xistream& xis )
    : network_( network )
    , time_( time )
    , converter_( converter )
    , resolver_( resolver )
    , site_( xis.attribute< unsigned short >( "site" ) )
    , application_( xis.attribute< unsigned short >( "application" ) )
    , exercise_( (unsigned char)xis.attribute< unsigned short >( "exercise" ) )
    , lag_( xis.attribute< bool >( "lag", false ) )
    , id_( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisExtensionFactory destructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisExtensionFactory::~DisExtensionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisExtensionFactory::Create
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
void DisExtensionFactory::Create( dispatcher::Agent& entity )
{
    std::auto_ptr< DisExtension > extension( new DisExtension( time_, *this, converter_, network_, resolver_, entity, exercise_, lag_ ) );
    entity.Attach( *extension.release() );
}

// -----------------------------------------------------------------------------
// Name: DisExtensionFactory::CreateNewIdentifier
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
rpr::EntityIdentifier DisExtensionFactory::CreateNewIdentifier()
{
    rpr::EntityIdentifier id( site_, application_, id_ );
    ++id_;
    return id;
}
