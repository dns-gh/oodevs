// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "SymbolFactory.h"
#include "SymbolRule.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: SymbolFactory constructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolFactory::SymbolFactory( xml::xistream& xis )
{
    xis >> start( "app6" );
        symbolRule_    = ReadRule( xis, "symbols", symbolBase_ );
        levelRule_     = ReadRule( xis, "levels", levelBase_ );
                         ReadRule( xis, "automats", automatSymbol_ );
    xis >> end();
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory destructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolFactory::~SymbolFactory()
{
    delete symbolRule_;
    delete levelRule_;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::ReadRule
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
SymbolRule* SymbolFactory::ReadRule( xml::xistream& xis, const std::string& ruleName, std::string& base ) const
{
    SymbolRule* result = 0;
    xis >> start( ruleName )
            >> attribute( "base", base )
            >> list( "choice", *this, &SymbolFactory::ReadRule, result )
        >> end();
    return result;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::ReadRule
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
void SymbolFactory::ReadRule( xml::xistream& xis, SymbolRule*& rule ) const
{
    if( rule )
        throw std::runtime_error( __FUNCTION__ );
    rule = new SymbolRule( xis );
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::CreateSymbol
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
std::string SymbolFactory::CreateSymbol( const std::string& hierarchy ) const
{
    std::string result( symbolBase_ );
    symbolRule_->Evaluate( hierarchy, result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::CreateLevelSymbol
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
std::string SymbolFactory::CreateLevelSymbol( const std::string& level ) const
{
    std::string result( levelBase_ );
    levelRule_->Evaluate( level, result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::CreateAutomatSymbol
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
std::string SymbolFactory::CreateAutomatSymbol() const
{
    return automatSymbol_;
}
