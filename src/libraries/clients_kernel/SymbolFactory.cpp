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
#include "SymbolCase.h"
#include "tools/GeneralConfig.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SymbolFactory constructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolFactory::SymbolFactory()
{
    xml::xifstream xis( tools::GeneralConfig::BuildResourceChildFile( "symbols.xml" ) );

    xis >> xml::start( "app6" );
        symbolRule_.reset( ReadRule( xis, "symbols", symbolBase_ ) );
        levelRule_ .reset( ReadRule( xis, "levels", levelBase_ ) );
                           ReadRule( xis, "automats", automatSymbol_ );
    xis >> xml::end;
    //ListSymbols();
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory destructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolFactory::~SymbolFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::ListSymbols
// Created: RPD 2011-01-26
// -----------------------------------------------------------------------------
void SymbolFactory::ListSymbols()
{
    xml::xofstream out("UnitSymbols.xml");
    out << xml::start("unit-symbols");
    TraverseTree(out, *symbolRule_.get());
    out << xml::end;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::TraverseTree
// Created: RPD 2011-01-26
// -----------------------------------------------------------------------------
void SymbolFactory::TraverseTree( xml::xofstream& out, const SymbolRule& rule )
{
    currentChain_.push_back( new std::string() );
    currentSymbol_.push_back( new std::string() );
    for( SymbolRule::CIT_Cases it = rule.GetCases().begin(); it != rule.GetCases().end(); ++it )
    {
        out << xml::start("unit-symbol");
        *currentChain_.back() = it->first;
        *currentSymbol_.back() = it->second->GetValue();
        std::string current;
        std::string symbol ("s*gpu");

        for( std::vector< std::string* >::iterator iter = currentChain_.begin(); iter != currentChain_.end(); ++iter )
        {
            if ( iter != currentChain_.begin() )
                current +="/";
            current += **(iter);
        }
        for( std::vector< std::string* >::iterator iter = currentSymbol_.begin(); iter != currentSymbol_.end(); ++iter )
        {
            symbol += **(iter);
        }

        std::string tail ("----------*****");
        tail = tail.substr( symbol.size(), 15-symbol.size() );
        symbol += tail;
        out << xml::attribute( "value", current );
        out << xml::attribute( "symbol", symbol );
        out << xml::end;
        if ( it->second && (it->second)->GetRule() )
        {
            TraverseTree( out, *(it->second)->GetRule() );
        }
    }
    delete ( currentChain_.back() );
    delete ( currentSymbol_.back() );
    currentChain_.pop_back();
    currentSymbol_.pop_back();
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::ReadRule
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
SymbolRule* SymbolFactory::ReadRule( xml::xistream& xis, const std::string& ruleName, std::string& base ) const
{
    SymbolRule* result = 0;
    xis >> xml::start( ruleName )
            >> xml::attribute( "base", base )
            >> xml::list( "choice", *this, &SymbolFactory::ReadRule, result )
        >> xml::end;
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
    std::string result;
    levelRule_->Evaluate( level, result );
    return ( ! result.empty() ) ? levelBase_ + result : result;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::CreateAutomatSymbol
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
std::string SymbolFactory::CreateAutomatSymbol() const
{
    return automatSymbol_;
}
