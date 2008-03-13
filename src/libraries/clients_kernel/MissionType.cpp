// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "MissionType.h"
#include "OrderParameter.h"
#include "OrderContext.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MissionType constructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
MissionType::MissionType( xml::xistream& xis )
    : OrderType( xis )
{
    xis >> list( "parameter", *this, &MissionType::ReadParameter );
}

// -----------------------------------------------------------------------------
// Name: MissionType constructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
MissionType::MissionType( xml::xistream& xis, const OrderContext& context )
    : OrderType( xis )
{
    context.AddParameters( *this );
    xis >> list( "parameter", *this, &MissionType::ReadParameter );
}

// -----------------------------------------------------------------------------
// Name: MissionType destructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
MissionType::~MissionType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionType::ReadParameter
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
void MissionType::ReadParameter( xml::xistream& xis )
{
    OrderParameter* param = new OrderParameter( xis );
    Register( Count(), *param );
}
