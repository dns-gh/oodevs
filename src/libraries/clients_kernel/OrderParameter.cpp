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
    std::string name;
    xis >> attribute( "name", name_ )
        >> attribute( "type", type_ )
        >> optional() >> attribute( "optional", optional_ )
        >> list( "value", *this, &OrderParameter::ReadValue );
}

// -----------------------------------------------------------------------------
// Name: OrderParameter constructor
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
OrderParameter::OrderParameter( const std::string& name, const std::string& type, bool optional )
    : name_    ( name )
    , type_    ( type )
    , optional_( optional )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderParameter destructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
OrderParameter::~OrderParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::GetName
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
std::string OrderParameter::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::GetType
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
std::string OrderParameter::GetType() const
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
// Name: OrderParameter::GetValue
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
const OrderParameterValue& OrderParameter::GetValue( unsigned int id ) const
{
    CIT_OrderParameterValues it = values_.find( id );
    if( it == values_.end() )
        throw std::runtime_error( "Undefined enumeration value." ); // $$$$ SBO 2007-05-25: 
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::ReadValue
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
void OrderParameter::ReadValue( xml::xistream& xis )
{
    OrderParameterValue value( xis );
    values_.insert( std::make_pair( value.GetId(), value ) );
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::Accept
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void OrderParameter::Accept( OrderParameterValueVisitor_ABC& visitor ) const
{
    for( CIT_OrderParameterValues it = values_.begin(); it != values_.end(); ++it )
        visitor.Visit( it->second );
}
