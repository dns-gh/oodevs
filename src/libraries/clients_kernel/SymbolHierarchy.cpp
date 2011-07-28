// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "SymbolHierarchy.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy constructor
// Created: LGY 2011-07-28
// -----------------------------------------------------------------------------
SymbolHierarchy::SymbolHierarchy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy destructor
// Created: LGY 2011-07-28
// -----------------------------------------------------------------------------
SymbolHierarchy::~SymbolHierarchy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy::GetValue
// Created: LGY 2011-07-28
// -----------------------------------------------------------------------------
const std::string& SymbolHierarchy::GetValue() const
{
    return symbol_;
}

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy::SetValue
// Created: LGY 2011-07-28
// -----------------------------------------------------------------------------
void SymbolHierarchy::SetValue( const std::string& value )
{
    symbol_ = value;
}
