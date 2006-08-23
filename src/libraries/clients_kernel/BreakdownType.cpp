// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "BreakdownType.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: BreakdownType constructor
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
BreakdownType::BreakdownType( xistream& xis )
{
    int id;
    std::string name;
    xis >> attribute( "nom", name )
        >> content( "MosID", id );
    id_ = id;
    name_ = name.c_str();
}

// -----------------------------------------------------------------------------
// Name: BreakdownType destructor
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
BreakdownType::~BreakdownType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BreakdownType::GetId
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
unsigned long BreakdownType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: BreakdownType::GetName
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
QString BreakdownType::GetName() const
{
    return name_;
}
