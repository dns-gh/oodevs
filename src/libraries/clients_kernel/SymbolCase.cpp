// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "SymbolCase.h"
#include "SymbolRule.h"
#include "SymbolVisitor_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SymbolCase constructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolCase::SymbolCase( xml::xistream& xis )
{
    xis >> xml::attribute( "symbol", value_ )
        >> xml::attribute( "name", name_ )
        >> xml::list( "choice", *this, &SymbolCase::ReadRule );
}

// -----------------------------------------------------------------------------
// Name: SymbolCase destructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolCase::~SymbolCase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SymbolCase::ReadRule
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void SymbolCase::ReadRule( xml::xistream& xis )
{
    if( rule_.get() )
        throw std::runtime_error( __FUNCTION__ );
    rule_.reset( new SymbolRule( xis ) );
}

// -----------------------------------------------------------------------------
// Name: SymbolCase::GetValue
// Created: RPD 2010-01-26
// -----------------------------------------------------------------------------
const std::string& SymbolCase::GetValue() const
{
    return value_;
}

// -----------------------------------------------------------------------------
// Name: SymbolCase::GetRule
// Created: RPD 2010-01-26
// -----------------------------------------------------------------------------
const SymbolRule* SymbolCase::GetRule() const
{
    return rule_.get();
}

// -----------------------------------------------------------------------------
// Name: SymbolCase::Evaluate
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void SymbolCase::Evaluate( const std::string& request, std::string& result ) const
{
    result += value_;
    if( rule_.get() )
        rule_->Evaluate( request, result );
}

// -----------------------------------------------------------------------------
// Name: SymbolCase::Accept
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
void SymbolCase::Accept( SymbolVisitor_ABC& visitor ) const
{
    visitor.AddChoice( rule_.get(), name_, value_ );
}
