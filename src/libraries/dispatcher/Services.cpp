// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Services.h"
#include "ClientPublisher_ABC.h"

#include "protocol/dispatchersenders.h"

//using namespace Common;
//using namespace MsgsDispatcherToClient;

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
    std::vector< const char* > names;
    std::transform( services_.begin(), services_.end(), std::back_inserter( names ), std::mem_fun_ref( &std::string::c_str ) );
    dispatcher::ServicesDescription services;   
    for( std::vector< const char* >::const_iterator iter(names.begin()); iter != names.end(); ++iter)
        *services().add_services( ) = *iter;   // $$$$ _RC_ FDS 2010-01-22: To validate
    services.Send( publisher );
}
