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
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/DispatcherSenders.h"
#include "protocol/Helpers.h"

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
void Services::Declare( sword::EnumService id )
{
    services_.Declare( id );
}

// -----------------------------------------------------------------------------
// Name: Services::HasService
// Created: LDC 2010-03-19
// -----------------------------------------------------------------------------
bool Services::HasService( sword::EnumService id ) const
{
    return services_.Has( id );
}

// -----------------------------------------------------------------------------
// Name: Services::Send
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void Services::Send( ClientPublisher_ABC& publisher ) const
{
    dispatcher::ServicesDescription services;
    for( size_t i = 0; i < protocol::mapping::Service::size_; ++i )
        if( services_.Has( protocol::mapping::Service::data_[i].type ) )
            services().add_services( protocol::mapping::Service::data_[i].name );
    services.Send( publisher );
}
