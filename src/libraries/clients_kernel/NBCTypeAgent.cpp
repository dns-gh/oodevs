// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "NBCTypeAgent.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: NBCTypeAgent constructor
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
NBCTypeAgent::NBCTypeAgent( xistream& input )
{
    input >> attribute( "name", name_ )
          >> attribute( "id", id_ );
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAgent destructor
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
NBCTypeAgent::~NBCTypeAgent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAgent::GetId
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
unsigned long NBCTypeAgent::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAgent::GetName
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
std::string NBCTypeAgent::GetName() const
{
    return name_;
}
