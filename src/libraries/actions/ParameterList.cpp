// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ParameterList.h"
#include "Bool.h"
#include "Identifier.h"
#include "Numeric.h"
#include "Quantity.h"
#include "clients_kernel/OrderParameter.h"

#include "protocol/Protocol.h"

using namespace kernel;

namespace actions {
    namespace parameters {

// -----------------------------------------------------------------------------
// Name: ParameterList constructor
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
ParameterList::ParameterList( const kernel::OrderParameter& parameter )
    : Parameter_ABC( parameter.GetName().c_str() )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParameterList destructor
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
ParameterList::~ParameterList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParameterList::GetType
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
std::string ParameterList::GetType() const
{
    return parameter_.GetType();
}

// -----------------------------------------------------------------------------
// Name: ParameterList::Display
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void ParameterList::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Item( tools::translate( "ParameterList", "Parameter" ) ).Display( GetName() );
}

// -----------------------------------------------------------------------------
// Name: ParameterList::Serialize
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void ParameterList::Serialize( xml::xostream& xos ) const
{
    Parameter_ABC::Serialize( xos );
    xos << xml::attribute( "type", parameter_.GetType() );
}

// -----------------------------------------------------------------------------
// Name: ParameterList::CommitTo
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void ParameterList::CommitTo( Common::MsgMissionParameter& message ) const
{
    message.mutable_value()->mutable_list();

    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        Common::MsgMissionParameter_Value* parameter = message.mutable_value()->add_list();
        Common::MsgMissionParameter msg;
        it->second->CommitTo( msg );
        *parameter = msg.value();
    }
}

// -----------------------------------------------------------------------------
// Name: ParameterList::Clean
// Created: JSR 2010-04-15
// -----------------------------------------------------------------------------
void ParameterList::Clean( Common::MsgMissionParameter& message ) const
{
    if( message.value().list_size() > 0 )
        message.mutable_value()->clear_list();
}

// -----------------------------------------------------------------------------
// Name: ParameterList::AddList
// Created: JSR 2010-04-22
// -----------------------------------------------------------------------------
ParameterList& ParameterList::AddList( const std::string& name )
{
    ParameterList& ret = *new ParameterList( OrderParameter( name, "list", false ) );
    AddParameter( ret );
    return ret;
}

// -----------------------------------------------------------------------------
// Name: ParameterList::AddBool
// Created: JSR 2010-04-22
// -----------------------------------------------------------------------------
void ParameterList::AddBool( const std::string& name, bool value )
{
    AddParameter( *new Bool( OrderParameter( name, "bool", false ), value ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterList::AddIdentifier
// Created: JSR 2010-04-22
// -----------------------------------------------------------------------------
void ParameterList::AddIdentifier( const std::string& name, unsigned int id )
{
    AddParameter( *new Identifier( OrderParameter( name, "identifier", false ), id ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterList::AddNumeric
// Created: JSR 2010-04-22
// -----------------------------------------------------------------------------
void ParameterList::AddNumeric( const std::string& name, float value )
{
    AddParameter( *new Numeric( OrderParameter( name, "numeric", false ), value ) );
}

// -----------------------------------------------------------------------------
// Name: ParameterList::AddQuantity
// Created: JSR 2010-04-22
// -----------------------------------------------------------------------------
void ParameterList::AddQuantity( const std::string& name, int value )
{
    AddParameter( *new Quantity( OrderParameter( name, "quantity", false ), value ) );
}

    }
}
