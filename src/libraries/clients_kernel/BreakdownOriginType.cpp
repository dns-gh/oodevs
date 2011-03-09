// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "BreakdownOriginType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: BreakdownOriginType constructor
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
BreakdownOriginType::BreakdownOriginType( xml::xistream& xis )
{
    xis >> xml::attribute( "type", name_ )
        >> xml::attribute( "origin", origin_ )
        >> xml::attribute( "percentage", percentage_ );
}

// -----------------------------------------------------------------------------
// Name: BreakdownOriginType destructor
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
BreakdownOriginType::~BreakdownOriginType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BreakdownOriginType::GetName
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
std::string BreakdownOriginType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: BreakdownOriginType::GetOrigin
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
std::string BreakdownOriginType::GetOrigin() const
{
    return origin_;
}

// -----------------------------------------------------------------------------
// Name: BreakdownOriginType::GetPercentage
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
double BreakdownOriginType::GetPercentage() const
{
    return percentage_;
}
