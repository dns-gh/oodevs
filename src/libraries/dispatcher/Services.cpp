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
#include "game_asn/DispatcherSenders.h"
#include "ClientPublisher_ABC.h"

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
    services().services.n    = names.size();
    services().services.elem = names.empty() ? 0 : &names.front();
    services.Send( publisher );
}
