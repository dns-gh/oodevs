// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "OrderType.h"
#include "OrderParameter.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: OrderType constructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderType::OrderType( xml::xistream& xis )
{
    xis >> attribute( "name", name_ )
        >> attribute( "id", id_ )
        >> list( "parameter", *this, &OrderType::ReadParameter );
}

// -----------------------------------------------------------------------------
// Name: OrderType destructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderType::~OrderType()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: OrderType::GetId
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
unsigned long OrderType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: OrderType::GetName
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
std::string OrderType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: OrderType::ReadParameter
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderType::ReadParameter( xml::xistream& xis )
{
    OrderParameter* parameter = new OrderParameter( xis );
    Register( parameter->GetName(), *parameter );
}
