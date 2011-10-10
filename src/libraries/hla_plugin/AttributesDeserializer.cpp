// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AttributesDeserializer.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AttributesDeserializer constructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
AttributesDeserializer::AttributesDeserializer( const std::string& identifier, RemoteAgentListener_ABC& listener )
    : identifier_( identifier )
    , listener_  ( listener )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttributesDeserializer destructor
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
AttributesDeserializer::~AttributesDeserializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttributesDeserializer::Register
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void AttributesDeserializer::Register( const std::string& attribute, T_Notification notification )
{
    notifications_[ attribute ] = notification;
}

// -----------------------------------------------------------------------------
// Name: AttributesDeserializer::Deserialize
// Created: SLI 2011-10-10
// -----------------------------------------------------------------------------
void AttributesDeserializer::Deserialize( const std::string& identifier, ::hla::Deserializer& deserializer )
{
    T_Notifications::const_iterator notification = notifications_.find( identifier );
    if( notification != notifications_.end() )
        notification->second( deserializer, identifier_, listener_ );
}
