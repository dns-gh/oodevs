// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "OrderParameter.h"
#include "OrderParameterValue.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: OrderParameter constructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
OrderParameter::OrderParameter( xml::xistream& xis )
    : optional_( false )
{
    std::string name, type;
    xis >> attribute( "name", name )
        >> attribute( "type", type )
        >> optional() >> attribute( "optional", optional_ )
        >> list( "value", *this, &OrderParameter::ReadValue );
    name_ = name.c_str();
    type_ = type.c_str();
}

// -----------------------------------------------------------------------------
// Name: OrderParameter destructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
OrderParameter::~OrderParameter()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::GetName
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
QString OrderParameter::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::GetType
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
QString OrderParameter::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::IsOptional
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
bool OrderParameter::IsOptional() const
{
    return optional_;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::ReadValue
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
void OrderParameter::ReadValue( xml::xistream& xis )
{
    OrderParameterValue* value = new OrderParameterValue( xis );
    Register( value->GetId(), *value );
}
