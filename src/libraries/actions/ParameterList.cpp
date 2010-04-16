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

#include "protocol/Protocol.h"

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
    displayer.Item( tools::translate( "ParameterList", "Action" ) ).Display( GetName() );
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

    }
}
