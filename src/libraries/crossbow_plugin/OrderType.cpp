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
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        delete it->second;
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
    const OrderParameter* parameter = new OrderParameter( xis );
    parameters_[parameter->GetName()] = parameter;
}

// -----------------------------------------------------------------------------
// Name: OrderType::GetParameterCount
// Created: SBO 2007-08-22
// -----------------------------------------------------------------------------
unsigned int OrderType::GetParameterCount() const
{
    return parameters_.size();
}

// -----------------------------------------------------------------------------
// Name: OrderType::FindParameter
// Created: SBO 2007-08-22
// -----------------------------------------------------------------------------
const OrderParameter* OrderType::FindParameter( const std::string& name ) const
{
    CIT_Parameters it = parameters_.find( name );
    if( it != parameters_.end() )
        return it->second;
    return 0;
}
