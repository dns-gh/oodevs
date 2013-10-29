// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Services.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Logger_ABC.h"
#include "clients_kernel/Services.h"
#include "protocol/Helpers.h"
#include "protocol/Protocol.h"
#include <boost/optional.hpp>

using namespace sword;

// -----------------------------------------------------------------------------
// Name: Services constructor
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
Services::Services( kernel::Controller& controller, kernel::Logger_ABC& logger )
    : controller_( controller )
    , logger_( logger )
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

namespace
{
    boost::optional< sword::EnumService > FindService( const std::string& name )
    {
        for( size_t i = 0; i < protocol::mapping::Service::size_; ++i )
            if( protocol::mapping::Service::data_[i].name == name )
                return protocol::mapping::Service::data_[i].type;
        return boost::none;
    }
}

// -----------------------------------------------------------------------------
// Name: Services::Update
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void Services::Update( const sword::ServicesDescription& message )
{
    services_.Clear();
    for( int i = 0; i < message.services_size(); ++i )
        if( auto service = FindService( message.services( i ) ) )
            services_.Declare( *service );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Services::HasService
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
bool Services::HasService( sword::EnumService id ) const
{
    return services_.Has( id );
}

// -----------------------------------------------------------------------------
// Name: Services::RequireService
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
bool Services::RequireService( sword::EnumService id ) const
{
    if( HasService( id ) )
        return true;
    logger_.Error( "Host does not implement service '" + sword::EnumService_Name( id ) + "'" );
    return false;
}

