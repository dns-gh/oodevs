// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ResourceNetworkType.h"
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"
#include "clients_kernel/ResourceNetworkType.h"

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: ResourceNetworkType constructor
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
ResourceNetworkType::ResourceNetworkType( const kernel::OrderParameter& parameter )
    : Parameter< std::string >( parameter )
    , type_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkType constructor
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
ResourceNetworkType::ResourceNetworkType( const kernel::OrderParameter& parameter, const std::string name, const tools::StringResolver< kernel::ResourceNetworkType >& resolver )
    : Parameter< std::string >( parameter )
    , type_( &resolver.Get( name ) )
{
    SetValue( type_->GetName() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkType destructor
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
ResourceNetworkType::~ResourceNetworkType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkType::Serialize
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
void ResourceNetworkType::Serialize( xml::xostream& xos ) const
{
    Parameter< std::string >::Serialize( xos );
    if( IsSet() && type_ )
        xos << xml::attribute( "value", type_->GetName() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkType::CommitTo
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
void ResourceNetworkType::CommitTo( std::string& content ) const
{
    if( IsSet() && type_ )
        content += type_->GetName();
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkType::CommitTo
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
void ResourceNetworkType::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() && type_ )
        message.add_value()->mutable_resourcenetworktype()->set_name( type_->GetName() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkType::CommitTo
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
void ResourceNetworkType::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() && type_ )
        message.mutable_resourcenetworktype()->set_name( type_->GetName() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkType::Accept
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
void ResourceNetworkType::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkType::SerializeType
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
std::string ResourceNetworkType::SerializeType() const
{
    return "resourcenetworktype";
}
