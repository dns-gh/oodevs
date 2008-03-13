// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "OrderContext.h"
#include "OrderType.h"
#include "OrderParameter.h"
#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: OrderContext constructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
OrderContext::OrderContext( xml::xistream& xis )
{
    xis >> xml::optional()
            >> xml::start( "context" )
                >> xml::list( "parameter", *this, &OrderContext::ReadParameter )
            >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: OrderContext destructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
OrderContext::~OrderContext()
{
    for( T_Parameters::const_iterator it = parameters_.begin(); it != parameters_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: OrderContext::ReadParameter
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void OrderContext::ReadParameter( xml::xistream& xis )
{
    parameters_.push_back( new OrderParameter( xis ) );
}

// -----------------------------------------------------------------------------
// Name: OrderContext::AddParameters
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void OrderContext::AddParameters( OrderType& type ) const
{
    for( T_Parameters::const_iterator it = parameters_.begin(); it != parameters_.end(); ++it )
        type.AddParameter( **it );
}
