// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "ObjectMessageService.h"

using namespace tools;
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: ObjectMessageService constructor
// Created: AGE 2007-03-07
// -----------------------------------------------------------------------------
ObjectMessageService::ObjectMessageService( DIN_Engine& engine, const DIN::DIN_Connector_ABC& connector )
    : DIN_MessageService( engine, connector )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: ObjectMessageService destructor
// Created: AGE 2007-03-07
// -----------------------------------------------------------------------------
ObjectMessageService::~ObjectMessageService()
{
    Terminate();
    for( CIT_Callbacks it = callbacks_.begin(); it != callbacks_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: ObjectMessageService::Register
// Created: AGE 2007-05-28
// -----------------------------------------------------------------------------
void ObjectMessageService::Register( unsigned long id, std::auto_ptr< ObjectMessageCallback_ABC > callback )
{
    DIN_MessageService::RegisterReceivedMessage( id, *callback, &ObjectMessageCallback_ABC::OnMessage );
    callbacks_.push_back( callback.release() );
}

// -----------------------------------------------------------------------------
// Name: ObjectMessageService::OnError
// Created: AGE 2007-08-23
// -----------------------------------------------------------------------------
bool ObjectMessageService::OnError( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    return error_ ? error_( link, reason ) : true;
}

// -----------------------------------------------------------------------------
// Name: ObjectMessageService::SetCallbackOnError
// Created: AGE 2007-08-23
// -----------------------------------------------------------------------------
void ObjectMessageService::SetCallbackOnError( const boost::function2< bool, DIN_Link&, const DIN_ErrorDescription& >& callback )
{
    error_ = callback;
}
