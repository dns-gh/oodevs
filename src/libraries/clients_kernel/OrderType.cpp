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
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: OrderType constructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
OrderType::OrderType( xml::xistream& xis )
{
    xis >> attribute( "name", name_ )
        >> attribute( "id", id_ );
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
std::string OrderType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: OrderType::GetDoctrineInformation
// Created: SBO 2009-06-24
// -----------------------------------------------------------------------------
std::string OrderType::GetDoctrineInformation() const
{
    return doctrine_;
}

// -----------------------------------------------------------------------------
// Name: OrderType::GetUsageInformation
// Created: SBO 2009-06-24
// -----------------------------------------------------------------------------
std::string OrderType::GetUsageInformation() const
{
    return usage_;
}

// -----------------------------------------------------------------------------
// Name: OrderType::AddParameter
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void OrderType::AddParameter( const OrderParameter& parameter )
{
    OrderParameter* copy = new OrderParameter( parameter );
    Register( Count(), *copy );
}

// -----------------------------------------------------------------------------
// Name: OrderType::ReadDescriptions
// Created: SBO 2009-06-24
// -----------------------------------------------------------------------------
void OrderType::ReadDescriptions( xml::xistream& xis )
{
    xis >> optional() >> start( "descriptions" )
            >> start( "doctrine" ) >> doctrine_ >> end()
            >> start( "usage" ) >> usage_ >> end()
        >> end();
}
