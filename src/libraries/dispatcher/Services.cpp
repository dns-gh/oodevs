// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Services.h"
#include "ClientPublisher_ABC.h"
#include "protocol/DispatcherSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Services constructor
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
Services::Services()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Services destructor
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
Services::~Services()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Services::Declare
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void Services::Declare( const std::string& name )
{
    services_.insert( name );
}

// -----------------------------------------------------------------------------
// Name: Services::Send
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void Services::Send( ClientPublisher_ABC& publisher ) const
{
    dispatcher::ServicesDescription services;
    for( std::set< std::string >::const_iterator it = services_.begin(); it != services_.end(); ++it )
        services().add_services( *it );
    services.Send( publisher );
}
