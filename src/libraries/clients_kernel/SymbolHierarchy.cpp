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
#include "App6Symbol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy constructor
// Created: LGY 2011-07-28
// -----------------------------------------------------------------------------
SymbolHierarchy::SymbolHierarchy()
    : overriden_( false )
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
// Name: SymbolHierarchy::MergeSymbol
// Created: JSR 2011-08-02
// -----------------------------------------------------------------------------
void SymbolHierarchy::MergeSymbol( const std::string& symbol )
{
    App6Symbol::Merge( symbol, originalSymbol_ );
    if( !overriden_ )
        symbol_ = originalSymbol_;
}

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy::ResetSymbol
// Created: LDC 2012-04-12
// -----------------------------------------------------------------------------
void SymbolHierarchy::ResetSymbol( const std::string& symbol )
{
    originalSymbol_ = symbol;
    if( !overriden_ )
        symbol_ = originalSymbol_;
}

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy::PrepareForMerge
// Created: JSR 2011-08-03
// -----------------------------------------------------------------------------
void SymbolHierarchy::PrepareForMerge()
{
    originalSymbol_.clear();
}

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy::OverrideValue
// Created: JSR 2011-08-02
// -----------------------------------------------------------------------------
void SymbolHierarchy::OverrideValue( const std::string& value )
{
    overriden_ = ( value != originalSymbol_ );
    symbol_ = value;
}

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy::Reset
// Created: JSR 2011-08-03
// -----------------------------------------------------------------------------
void SymbolHierarchy::Reset()
{
    overriden_ = false;
    symbol_ = originalSymbol_;
}

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy::SetOverriden
// Created: LDC 2012-05-30
// -----------------------------------------------------------------------------
void SymbolHierarchy::SetOverriden( bool value )
{
    overriden_ = value;
}

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy::IsOverriden
// Created: JSR 2011-08-02
// -----------------------------------------------------------------------------
bool SymbolHierarchy::IsOverriden() const
{
    return overriden_;
}

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy::UpdateKarma
// Created: LGY 2011-12-06
// -----------------------------------------------------------------------------
void SymbolHierarchy::UpdateKarma( const Karma& karma )
{
    kernel::App6Symbol::SetKarma( symbol_, karma );
}
