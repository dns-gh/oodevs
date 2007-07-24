// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "SymbolCase.h"
#include "SymbolRule.h"
#include "SymbolVisitor_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: SymbolCase constructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolCase::SymbolCase( xml::xistream& xis )
    : rule_( 0 )
{
    xis >> attribute( "symbol", value_ )
        >> attribute( "name", name_ )
        >> list( "choice", *this, &SymbolCase::ReadRule );
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
    if( rule_ )
        throw std::runtime_error( __FUNCTION__ );
    rule_ = new SymbolRule( xis );
}

// -----------------------------------------------------------------------------
// Name: SymbolCase::Evaluate
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void SymbolCase::Evaluate( const std::string& request, std::string& result ) const
{
    result += value_;
    if( rule_ )
        rule_->Evaluate( request, result );
}

// -----------------------------------------------------------------------------
// Name: SymbolCase::Accept
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
void SymbolCase::Accept( SymbolVisitor_ABC& visitor ) const
{
    visitor.AddChoice( rule_, name_, value_ );
}
