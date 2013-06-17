// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "Resource.h"
#include "ParameterVisitor_ABC.h"

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: Resource constructor
// Created: ABR 2011-11-24
// -----------------------------------------------------------------------------
Resource::Resource( const kernel::OrderParameter& parameter, const std::string& value )
    : String( parameter, value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Resource destructor
// Created: ABR 2011-11-24
// -----------------------------------------------------------------------------
Resource::~Resource()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Resource::SerializeType
// Created: ABR 2011-11-24
// -----------------------------------------------------------------------------
std::string Resource::SerializeType() const
{
    return "resource";
}

// -----------------------------------------------------------------------------
// Name: Resource::Accept
// Created: ABR 2013-06-14
// -----------------------------------------------------------------------------
void Resource::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
