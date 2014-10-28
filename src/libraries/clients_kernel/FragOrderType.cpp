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
#include "Entity_ABC.h"
#include "OrderParameter.h"
#include <boost/algorithm/string.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FragOrderType constructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
FragOrderType::FragOrderType( xml::xistream& xis )
    : OrderType( xis )
    , isWithoutMission_( false )
{
    xis >> xml::optional >> xml::attribute( "available-without-mission", isWithoutMission_ )
        >> xml::optional >> xml::attribute( "dia-type", diaType_ )
        >> xml::list( "parameter", *this, &FragOrderType::ReadParameter );
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

// -----------------------------------------------------------------------------
// Name: FragOrderType::GetType
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
E_MissionType FragOrderType::GetType() const
{
    return eMissionType_FragOrder;
}
