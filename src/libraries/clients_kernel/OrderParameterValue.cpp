// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "OrderParameterValue.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: OrderParameterValue constructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
OrderParameterValue::OrderParameterValue( xml::xistream& xis )
{
    xis >> xml::attribute( "id", id_ )
        >> xml::attribute( "name", name_ );
}

// -----------------------------------------------------------------------------
// Name: OrderParameterValue constructor
// Created: JSR 2010-04-09
// -----------------------------------------------------------------------------
OrderParameterValue::OrderParameterValue( int id, const std::string& name )
    : id_( id )
    , name_( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderParameterValue destructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
OrderParameterValue::~OrderParameterValue()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderParameterValue::GetId
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
int OrderParameterValue::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: OrderParameterValue::GetName
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
std::string OrderParameterValue::GetName() const
{
    return name_;
}
