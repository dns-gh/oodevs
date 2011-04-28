// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "FireClass.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FireClass constructor
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
FireClass::FireClass( xml::xistream& input )
{
    input >> xml::attribute( "name", name_ );
    canPropagate_ = input.has_child( "surfaces" );
}

// -----------------------------------------------------------------------------
// Name: FireClass destructor
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
FireClass::~FireClass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireClass::GetName
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
std::string FireClass::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: FireClass::CanPropagate
// Created: BCI 2011-04-27
// -----------------------------------------------------------------------------
bool FireClass::CanPropagate() const
{
    return canPropagate_;
}
