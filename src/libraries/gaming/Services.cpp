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
#include "game_asn/Dispatcher.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Logger_ABC.h"

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

// -----------------------------------------------------------------------------
// Name: Services::Update
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void Services::Update( const ASN1T_MsgServicesDescription& message )
{
    services_.clear();
    std::copy( message.services.elem, message.services.elem + message.services.n,
                std::inserter( services_, services_.begin() ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Services::HasService
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
bool Services::HasService( const std::string& name ) const
{
    return services_.find( name ) != services_.end();
}

// -----------------------------------------------------------------------------
// Name: Services::RequireService
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
bool Services::RequireService( const std::string& name ) const
{
    if( ! HasService( name ) )
    {
        logger_.Error() << "Host does not implement service '" << name << "'";
        return false;
    }
    return true;
}

