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
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MissionType constructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
MissionType::MissionType( xml::xistream& xis, E_MissionType type )
    : OrderType( xis )
    , type_( type )
{
    Read( xis );
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
// Name: MissionType::Read
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void MissionType::Read( xml::xistream& xis )
{
    std::string mrt;
    xis >> xml::optional >> xml::attribute( "mrt-dia-behavior", mrt )
        >> xml::list( "parameter", *this, &MissionType::ReadParameter );
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

// -----------------------------------------------------------------------------
// Name: MissionType::GetType
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
E_MissionType MissionType::GetType() const
{
    return type_;
}
