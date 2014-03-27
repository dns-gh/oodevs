// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ObjectAttributes.h"
#include "protocol/MessageParameters.h"

void CheckCount( const std::string& name, const sword::MissionParameter_Value& value, int count )
{
    protocol::Check( value.list_size() == count, STR( name << " attribute must have " << count  << " parameter(s)" ) );
}

bool GetBool( const std::string& name, const sword::MissionParameter_Value& value, unsigned int i )
{
    const auto& parameter = value.list( i );
    protocol::Check( parameter.has_booleanvalue(), STR( name << " attribute[" << i << "] must have a boolean value" ) );
    return parameter.booleanvalue();
}

int GetQuantity( const std::string& name, const sword::MissionParameter_Value& value, unsigned int i )
{
    const auto& parameter = value.list( i );
    protocol::Check( parameter.has_quantity(), STR( name << " attribute[" << i << "] must have a quantity" ) );
    return parameter.quantity();
}

float GetReal( const std::string& name, const sword::MissionParameter_Value& value, unsigned int i )
{
    const auto& parameter = value.list( i );
    protocol::Check( parameter.has_areal(), STR( name << " attribute[" << i << "] must have a real" ) );
    return parameter.areal();
}
