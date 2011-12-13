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
#include "Message.h"
#pragma warning( push )
#pragma warning( disable : 4702 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

using namespace tools;

// -----------------------------------------------------------------------------
// Name: ObjectMessageService constructor
// Created: AGE 2007-03-07
// -----------------------------------------------------------------------------
ObjectMessageService::ObjectMessageService()
    : nbMessagesReceived_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectMessageService destructor
// Created: AGE 2007-03-07
// -----------------------------------------------------------------------------
ObjectMessageService::~ObjectMessageService()
{
    for( CIT_Callbacks it = callbacks_.begin(); it != callbacks_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: ObjectMessageService::Register
// Created: AGE 2007-05-28
// -----------------------------------------------------------------------------
void ObjectMessageService::Register( unsigned long id, std::auto_ptr< ObjectMessageCallback_ABC > callback )
{
    callbacks_[ id ] = callback.release();
}

// -----------------------------------------------------------------------------
// Name: ObjectMessageService::Retrieve
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
ObjectMessageCallback_ABC* ObjectMessageService::Retrieve( unsigned long id )
{
    return callbacks_[ id ];
}

// -----------------------------------------------------------------------------
// Name: ObjectMessageService::OnMessage
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void ObjectMessageService::OnMessage( const std::string& endpoint, Message& message )
{
    ++nbMessagesReceived_;
    unsigned long tag;
    message >> tag;
    CIT_Callbacks it = callbacks_.find( tag );
    if( it == callbacks_.end() )
        throw std::runtime_error( "Unknown message tag " + boost::lexical_cast< std::string >( tag ) );
    it->second->OnMessage( endpoint, message );
}
