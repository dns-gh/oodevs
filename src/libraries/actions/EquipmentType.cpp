// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "EquipmentType.h"
#include "clients_kernel/EquipmentType.h"
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.hpp>

using namespace actions;
using namespace parameters;

EquipmentType::EquipmentType( const kernel::OrderParameter& parameter )
    : Parameter< std::string >( parameter )
    , type_( 0 )
{
    // NOTHING
}

EquipmentType::EquipmentType( const kernel::OrderParameter& parameter, unsigned int id, const tools::Resolver_ABC< kernel::EquipmentType >& resolver )
    : Parameter< std::string >( parameter )
    , type_( &resolver.Get( id ) )
{
    SetValue( type_->GetName() );
}

EquipmentType::~EquipmentType()
{
    // NOTHING
}

void EquipmentType::Serialize( xml::xostream& xos ) const
{
    Parameter< std::string >::Serialize( xos );
    if( IsSet() && type_ )
        xos << xml::attribute( "value", type_->GetId() );
}

void EquipmentType::CommitTo( std::string& content ) const
{
    if( IsSet() && type_ )
        content += boost::lexical_cast< std::string >( type_->GetId() );
}

void EquipmentType::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
    if( IsSet() && type_ )
        message.add_value()->mutable_equipmenttype()->set_id( type_->GetId() );
}

void EquipmentType::CommitTo( sword::MissionParameter_Value& message ) const
{
    if( IsSet() && type_ )
        message.mutable_equipmenttype()->set_id( type_->GetId() );
}

void EquipmentType::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

std::string EquipmentType::SerializeType() const
{
    return "equipmenttype";
}
