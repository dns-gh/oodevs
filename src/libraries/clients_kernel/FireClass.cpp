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
    input >> xml::attribute( "name", name_ )
          >> xml::attribute( "id", id_ )
          >> xml::attribute( "defaultheat", defaultHeat_ );
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
// Name: FireClass::GetId
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
unsigned long FireClass::GetId() const
{
    return id_;
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
// Name: FireClass::GetDefaultHeat
// Created: JSR 2010-03-12
// -----------------------------------------------------------------------------
int FireClass::GetDefaultHeat() const
{
    return defaultHeat_;
}
