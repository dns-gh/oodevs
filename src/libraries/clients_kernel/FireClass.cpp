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
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: FireClass constructor
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
FireClass::FireClass( xistream& input )
{
    input >> attribute( "name", name_ )
          >> attribute( "id", id_ );
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
