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
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: OrderType constructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
OrderType::OrderType( xml::xistream& xis )
{
    xis >> xml::attribute( "name", name_ )
        >> xml::attribute( "id", id_ );
}

// -----------------------------------------------------------------------------
// Name: OrderType constructor
// Created: JSR 2010-04-06
// -----------------------------------------------------------------------------
OrderType::OrderType( )
    : id_( 0 )
{
    // NOTHING
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
const std::string& OrderType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: OrderType::GetDoctrineInformation
// Created: SBO 2009-06-24
// -----------------------------------------------------------------------------
const std::string& OrderType::GetDoctrineInformation() const
{
    return doctrine_;
}

// -----------------------------------------------------------------------------
// Name: OrderType::GetUsageInformation
// Created: SBO 2009-06-24
// -----------------------------------------------------------------------------
const std::string& OrderType::GetUsageInformation() const
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
    xis >> xml::optional >> xml::start( "description" )
            >> xml::optional >> xml::start( "doctrine" ) >> doctrine_ >> xml::end
            >> xml::optional >> xml::start( "usage" ) >> usage_ >> xml::end
        >> xml::end;
}
