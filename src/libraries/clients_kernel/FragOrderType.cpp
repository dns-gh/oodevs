// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "FragOrderType.h"
#include "OrderParameter.h"
#include <xeumeuleu/xml.h>

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FragOrderType constructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
FragOrderType::FragOrderType( xml::xistream& xis )
    : OrderType( xis )
    , isDefaultOrder_  ( false )
    , isWithoutMission_( false )
{
    xis >> optional() >> attribute( "available-for-all-mission", isDefaultOrder_ )
        >> optional() >> attribute( "available-without-mission", isWithoutMission_ )
        >> list( "parameter", *this, &FragOrderType::ReadParameter );
}

// -----------------------------------------------------------------------------
// Name: FragOrderType destructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
FragOrderType::~FragOrderType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FragOrderType::IsDefaultOrder
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
bool FragOrderType::IsDefaultOrder() const
{
    return isDefaultOrder_;
}

// -----------------------------------------------------------------------------
// Name: FragOrderType::IsMissionRequired
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
bool FragOrderType::IsMissionRequired() const
{
    return !isWithoutMission_;
}

// -----------------------------------------------------------------------------
// Name: FragOrderType::ReadParameter
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
void FragOrderType::ReadParameter( xml::xistream& xis )
{
    OrderParameter* param = new OrderParameter( xis );
    Register( Count(), *param );
}
