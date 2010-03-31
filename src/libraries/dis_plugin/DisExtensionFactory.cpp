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
#include "dispatcher/AgentSymbols.h"
#include "dispatcher/Automat.h"
#include "dispatcher/AutomatSymbols.h"
#include "dispatcher/Formation.h"
#include "dispatcher/FormationSymbols.h"
#include "dispatcher/Side.h"
#include "dispatcher/SideSymbols.h"
#include <xeumeuleu/xml.h>

using namespace plugins::dis;
using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: DisExtensionFactory constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisExtensionFactory::DisExtensionFactory( UdpNetwork& network, const Time_ABC& time, const kernel::CoordinateConverter_ABC& converter, const DisTypeResolver& resolver, xml::xistream& xis )
    : network_( network )
    , time_( time )
    , converter_( converter )
    , resolver_( resolver )
    , site_( xml::attribute< unsigned short >( xis, "site" ) )
    , application_( xml::attribute< unsigned short >( xis, "application" ) )
    , exercise_( (unsigned char)xml::attribute< unsigned short >( xis, "exercise" ) )
    , lag_( xml::attribute( xis, "lag", false ) )
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
    if( !entity.Retrieve< dispatcher::EntitySymbols_ABC >() )
        entity.Attach< dispatcher::EntitySymbols_ABC >( *new dispatcher::AgentSymbols( entity ) );
    std::auto_ptr< DisExtension > extension( new DisExtension( time_, *this, converter_, network_, resolver_, entity, exercise_, lag_ ) );
    entity.Attach( *extension.release() );
}

// -----------------------------------------------------------------------------
// Name: DisExtensionFactory::Create
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void DisExtensionFactory::Create( dispatcher::Automat& entity )
{
    if( !entity.Retrieve< dispatcher::EntitySymbols_ABC >() )
        entity.Attach< dispatcher::EntitySymbols_ABC >( *new dispatcher::AutomatSymbols( entity ) );
}

// -----------------------------------------------------------------------------
// Name: DisExtensionFactory::Create
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void DisExtensionFactory::Create( dispatcher::Formation& entity )
{
    if( !entity.Retrieve< dispatcher::EntitySymbols_ABC >() )
        entity.Attach< dispatcher::EntitySymbols_ABC >( *new dispatcher::FormationSymbols( entity ) );
}

// -----------------------------------------------------------------------------
// Name: DisExtensionFactory::Create
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void DisExtensionFactory::Create( dispatcher::Side& entity )
{
    if( !entity.Retrieve< dispatcher::EntitySymbols_ABC >() )
        entity.Attach< dispatcher::EntitySymbols_ABC >( *new dispatcher::SideSymbols( entity ) );
}

// -----------------------------------------------------------------------------
// Name: DisExtensionFactory::CreateNewIdentifier
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
EntityIdentifier DisExtensionFactory::CreateNewIdentifier()
{
    EntityIdentifier id( site_, application_, id_ ); // site, application, id
    ++id_;
    return id;
}
