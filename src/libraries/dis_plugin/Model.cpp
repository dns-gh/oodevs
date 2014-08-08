// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "Model.h"
#include "AgentProxy.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Formation.h"
#include "dispatcher/Side.h"
#include "tic/PlatformDelegate_ABC.h"
#include "tic/PlatformDelegateFactory_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/make_shared.hpp>

using namespace plugins::dis;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
Model::Model( UdpNetwork& network, const Time_ABC& time, const kernel::CoordinateConverter_ABC& converter, const rpr::EntityTypeResolver& resolver, xml::xistream& xis, tic::PlatformDelegateFactory_ABC& factory )
    : network_( network )
    , time_( time )
    , converter_( converter )
    , resolver_( resolver )
    , factory_( factory )
    , site_( xis.attribute< unsigned short >( "site" ) )
    , application_( xis.attribute< unsigned short >( "application" ) )
    , exercise_( (unsigned char)xis.attribute< unsigned short >( "exercise" ) )
    , lag_( xis.attribute< bool >( "lag", false ) )
    , id_( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
Model::~Model()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model::Create
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
void Model::Create( dispatcher::Agent& entity )
{
    IdentifierFactory_ABC& that = *this;
    agents_.push_back( boost::make_shared< AgentProxy >( time_, that, converter_, network_, resolver_, entity, exercise_, lag_, factory_.Create( entity ) ) );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateNewIdentifier
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
rpr::EntityIdentifier Model::CreateNewIdentifier()
{
    rpr::EntityIdentifier id( site_, application_, id_ );
    ++id_;
    return id;
}
