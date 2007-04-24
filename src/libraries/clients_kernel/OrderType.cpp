// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "OrderType.h"
#include "OrderParameter.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: OrderType constructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
OrderType::OrderType( xml::xistream& xis )
{
    std::string name;
    xis >> attribute( "name", name )
        >> attribute( "id", id_ );
    name_ = name.c_str();
}

// -----------------------------------------------------------------------------
// Name: OrderType destructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
OrderType::~OrderType()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: OrderType::GetId
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
unsigned long OrderType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: OrderType::GetName
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
QString OrderType::GetName() const
{
    return name_;
}
