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
    return overriden_ ? symbol_: computedSymbol_;
}

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy::MergeSymbol
// Created: JSR 2011-08-02
// -----------------------------------------------------------------------------
void SymbolHierarchy::MergeSymbol( const std::string& symbol, bool original /* = false */ )
{
    App6Symbol::Merge( symbol, computedSymbol_ );
    if( original )
        originalSymbol_ = computedSymbol_;
}

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy::ResetSymbol
// Created: LDC 2012-04-12
// -----------------------------------------------------------------------------
void SymbolHierarchy::ResetSymbol( const std::string& symbol )
{
    if( !symbol.empty() )
        computedSymbol_ = symbol;
    else if( !originalSymbol_.empty() )
        computedSymbol_ = originalSymbol_;
}

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy::PrepareForMerge
// Created: JSR 2011-08-03
// -----------------------------------------------------------------------------
void SymbolHierarchy::PrepareForMerge()
{
}

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy::OverrideValue
// Created: JSR 2011-08-02
// -----------------------------------------------------------------------------
void SymbolHierarchy::OverrideValue( const std::string& value )
{
    overriden_ = true;
    symbol_ = value;
}

// -----------------------------------------------------------------------------
// Name: SymbolHierarchy::Reset
// Created: JSR 2011-08-03
// -----------------------------------------------------------------------------
void SymbolHierarchy::Reset()
{
    overriden_ = false;
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
    kernel::App6Symbol::SetKarma( computedSymbol_, karma );
    kernel::App6Symbol::SetKarma( originalSymbol_, karma );
}
