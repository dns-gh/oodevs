// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "SymbolFactory.h"
#include "App6Symbol.h"
#include "HierarchyLevel_ABC.h"
#include "Karma.h"
#include "SymbolCase.h"
#include "SymbolRule.h"
#include "tools/ExerciseConfig.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SymbolFactory constructor
// Created: ABR 2011-05-25
// -----------------------------------------------------------------------------
SymbolFactory::SymbolFactory()
    : initialized_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory constructor
// Created: ABR 2011-05-25
// -----------------------------------------------------------------------------
SymbolFactory::SymbolFactory( xml::xistream& xis )
    : initialized_( true )
{
    ReadSymbols( xis );
    ListSymbols();
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
// Name: SymbolFactory::ReadSymbols
// Created: ABR 2011-05-25
// -----------------------------------------------------------------------------
void SymbolFactory::ReadSymbols( xml::xistream& xis )
{
    xis >> xml::start( "app6" );
    symbolRule_.reset( ReadRule( xis, "symbols", symbolBase_ ) );
    levelRule_ .reset( ReadRule( xis, "levels", levelBase_ ) );
    ReadRule( xis, "automats", automatSymbol_ );
    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::Load
// Created: ABR 2011-05-25
// -----------------------------------------------------------------------------
void SymbolFactory::Load( const tools::ExerciseConfig& config )
{
    currentChain_.clear();
    currentSymbol_.clear();
    availableSymbols_.clear();
    config.GetLoader().LoadPhysicalFile( "symbols", boost::bind( &SymbolFactory::ReadSymbols, this, _1 ) );
    ListSymbols();
    initialized_ = true;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::Unload
// Created: JSR 2011-08-02
// -----------------------------------------------------------------------------
void SymbolFactory::Unload()
{
    currentChain_.clear();
    currentSymbol_.clear();
    availableSymbols_.clear();
    initialized_ = false;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::ListSymbols
// Created: RPD 2011-01-26
// -----------------------------------------------------------------------------
void SymbolFactory::ListSymbols()
{
    if( !symbolRule_.get() )
        return;
    xml::xobufferstream out;
    out << xml::start( "unit-symbols" );
    TraverseTree( out, *symbolRule_ );
    out << xml::end;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::TraverseTree
// Created: RPD 2011-01-26
// -----------------------------------------------------------------------------
void SymbolFactory::TraverseTree( xml::xostream& out, const SymbolRule& rule )
{
    currentChain_.push_back( new std::string() );
    currentSymbol_.push_back( new std::string() );
    for( SymbolRule::CIT_Cases it = rule.GetCases().begin(); it != rule.GetCases().end(); ++it )
    {
        out << xml::start( "unit-symbol" );
        *currentChain_.back() = it->first;
        *currentSymbol_.back() = it->second->GetValue();
        std::string current;
        std::string symbol ( "s*gpuc" );

        for( std::vector< std::string* >::iterator iter = currentChain_.begin(); iter != currentChain_.end(); ++iter )
        {
            if( iter != currentChain_.begin() )
                current +="/";
            current += **(iter);
        }
        for( std::vector< std::string* >::iterator iter = currentSymbol_.begin(); iter != currentSymbol_.end(); ++iter )
        {
            symbol += **( iter );
        }

        std::string tail ( "----------*****" );
        tail = tail.substr( symbol.size(), 15-symbol.size() );
        symbol += tail;
        availableSymbols_.push_back( current );
        out << xml::attribute( "value", current );
        out << xml::attribute( "symbol", symbol );
        out << xml::end;
        if( it->second && ( it->second)->GetRule() )
        {
            TraverseTree( out, *( it->second)->GetRule() );
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
    if( !initialized_ || !symbolRule_.get() )
        return "";
    std::string result( symbolBase_ );
    symbolRule_->Evaluate( hierarchy, result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::GetNatureFromSymbol
// Created: ABR 2012-08-08
// -----------------------------------------------------------------------------
std::string SymbolFactory::GetNatureFromSymbol( const std::string& symbol ) const
{
    return symbolRule_->ConvertToNature( symbol );
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::CreateLevelSymbol
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
std::string SymbolFactory::CreateLevelSymbol( const std::string& level ) const
{
    if( !initialized_ || !levelRule_.get() )
        return "";
    std::string result;
    levelRule_->Evaluate( level, result );
    return ( ! result.empty() ) ? levelBase_ + result : result;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::CreateLevelSymbol
// Created: ABR 2011-05-24
// -----------------------------------------------------------------------------
std::string SymbolFactory::CreateLevelSymbol( const HierarchyLevel_ABC& level ) const
{
    if( !initialized_ )
        return "";
    return CreateLevelSymbol( level.GetName().ascii() );
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::CreateAutomatSymbol
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
std::string SymbolFactory::CreateAutomatSymbol() const
{
    if( !initialized_ )
        return "";
    return automatSymbol_;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::GetSymbolBase
// Created: LDC 2012-04-11
// -----------------------------------------------------------------------------
std::string SymbolFactory::GetSymbolBase( const Karma& karma ) const
{
    std::string result( symbolBase_ );
    kernel::App6Symbol::SetKarma( result, karma );
    return result;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::IsThisChainAvailable
// Created: RPD 2011-04-08
// -----------------------------------------------------------------------------
bool SymbolFactory::IsThisChainAvailable( const std::string& chain ) const
{
    if( !initialized_ )
        return false;
    std::vector< std::string >::const_iterator  it;
    for( it = availableSymbols_.begin() ; it != availableSymbols_.end() ; ++it )
        if( *it == chain )
            break;
    return it != availableSymbols_.end();
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::GetSymbolRule
// Created: ABR 2011-05-26
// -----------------------------------------------------------------------------
SymbolRule* SymbolFactory::GetSymbolRule() const
{
    return symbolRule_.get();
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::GetAvailableSymbols
// Created: ABR 2011-05-26
// -----------------------------------------------------------------------------
const std::vector< std::string >& SymbolFactory::GetAvailableSymbols() const
{
    return availableSymbols_;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::FillSymbols
// Created: LGY 2011-07-22
// -----------------------------------------------------------------------------
void SymbolFactory::FillSymbols( const std::string& symbol, const std::string& karma, std::set< std::string >& result ) const
{
    std::string compare = symbol;
    std::string base = symbolBase_;
    std::string::size_type position = 0;
    while( ( position = base.find( '*', position ) ) != std::string::npos )
        base.replace( position, 1, karma );
    result.insert( base );
    if( !symbol.empty() )
    {
        while( compare != base && compare.size() >= base.size() )
        {
            result.insert( compare );
            compare = compare.substr( 0, compare.length() - 1 );
        }
    }
}