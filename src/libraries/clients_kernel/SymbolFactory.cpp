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
#include "App6Symbol.h"
#include "HierarchyLevel_ABC.h"
#include "Karma.h"
#include "SymbolCase.h"
#include "SymbolRule.h"
#include "tools/ExerciseConfig.h"
#include "tools/PhyLoader.h"
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
    karmaPosition_ = symbolBase_.find_first_of( '*' );
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
    availableSymbols_.clear();
    symbolNatureMap_.clear();
    config.GetPhyLoader().LoadPhysicalFile( "symbols", boost::bind( &SymbolFactory::ReadSymbols, this, _1 ) );
    ListSymbols();
    initialized_ = true;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::Unload
// Created: JSR 2011-08-02
// -----------------------------------------------------------------------------
void SymbolFactory::Unload()
{
    availableSymbols_.clear();
    symbolNatureMap_.clear();
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
    TraverseTree( *symbolRule_ );
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::TraverseTree
// Created: RPD 2011-01-26
// -----------------------------------------------------------------------------
void SymbolFactory::TraverseTree( const SymbolRule& rule )
{
    std::vector< std::string* > currentChain;
    std::vector< std::string* > currentSymbol;
    FillSymbols( rule, currentChain, currentSymbol );
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::FillSymbols
// Created: LDC 2013-04-23
// -----------------------------------------------------------------------------
void SymbolFactory::FillSymbols( const SymbolRule& rule, std::vector< std::string* >& currentChain, std::vector< std::string* >& currentSymbol )
{
    currentChain.push_back( new std::string() );
    currentSymbol.push_back( new std::string() );
    for( SymbolRule::CIT_Cases it = rule.GetCases().begin(); it != rule.GetCases().end(); ++it )
    {
        *currentChain.back() = it->first;
        *currentSymbol.back() = it->second->GetValue();
        std::string current;
        std::string symbol ( "s*gpu" );

        for( auto iter = currentChain.begin(); iter != currentChain.end(); ++iter )
        {
            if( iter != currentChain.begin() )
                current +="/";
            current += **(iter);
        }
        for( auto iter = currentSymbol.begin(); iter != currentSymbol.end(); ++iter )
            symbol += **( iter );

        symbolNatureMap_[ symbol ] = current;
        availableSymbols_.push_back( current );
        if( it->second && ( it->second)->GetRule() )
            FillSymbols( *( it->second)->GetRule(), currentChain, currentSymbol );
    }
    delete ( currentChain.back() );
    delete ( currentSymbol.back() );
    currentChain.pop_back();
    currentSymbol.pop_back();
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
        throw MASA_EXCEPTION( "Invalid rule." );
    rule = new SymbolRule( xis );
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::CreateSymbol
// Created: AGE 2006-10-23
// -----------------------------------------------------------------------------
std::string SymbolFactory::CreateSymbol( const std::string& hierarchy ) const
{
    if( !initialized_ || !symbolRule_.get() )
        return std::string();
    std::string result( symbolBase_ );
    symbolRule_->Evaluate( hierarchy, result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::CreateMoveSymbol
// Created: LDC 2013-04-09
// -----------------------------------------------------------------------------
std::string SymbolFactory::CreateMoveSymbol( const std::string& hierarchy ) const
{
    if( !initialized_ || !symbolRule_.get() )
        return std::string();
    std::string result( symbolBase_ );
    if( symbolRule_->EvaluateMove( hierarchy, result ) )
        return result;
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::CreateStaticSymbol
// Created: LDC 2013-04-09
// -----------------------------------------------------------------------------
std::string SymbolFactory::CreateStaticSymbol( const std::string& hierarchy ) const
{
    if( !initialized_ || !symbolRule_.get() )
        return std::string();
    std::string result( symbolBase_ );
    if( symbolRule_->EvaluateStatic( hierarchy, result ) )
        return result;
    return std::string();
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
    return CreateLevelSymbol( level.GetName().toStdString() );
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

// -----------------------------------------------------------------------------
// Name: SymbolFactory::Merge
// Created: LDC 2013-04-22
// -----------------------------------------------------------------------------
void SymbolFactory::Merge( const std::string& from, std::string& to ) const
{
    if( to.empty() )
       to = from;
    else
    {
        unsigned i = 0;
        const std::size_t max = std::min( to.length(), from.length() );
        while( i < max && ( to[ i ] == from[ i ] || i == karmaPosition_ ) )
            ++i;
        to = to.substr( 0, i );
    }
}

namespace
{
    std::string GetAssociatedSymbol( const std::string& symbol, const std::map< std::string, std::string >& symbolNatureMap, const SymbolFactory* factory, std::string( SymbolFactory::*func )( const std::string& ) const )
    {
        Karma karma = Karma::unknown_;
        std::string base = App6Symbol::GetBase( symbol, karma );
        auto it = symbolNatureMap.find( base );
        if( it != symbolNatureMap.end() )
        {
            std::string category = it->second;
            std::string staticSymbol = (factory->*func)( category );
            App6Symbol::SetKarma( staticSymbol, karma );
            return staticSymbol;
        }
        return std::string();
    }
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::GetAssociatedStatic
// Created: LDC 2013-04-22
// -----------------------------------------------------------------------------
std::string SymbolFactory::GetAssociatedStatic( const std::string& symbol ) const
{
    return GetAssociatedSymbol( symbol, symbolNatureMap_, this, &SymbolFactory::CreateStaticSymbol );
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::GetAssociatedMove
// Created: LDC 2013-04-22
// -----------------------------------------------------------------------------
std::string SymbolFactory::GetAssociatedMove( const std::string& symbol ) const
{
    return GetAssociatedSymbol( symbol, symbolNatureMap_, this, &SymbolFactory::CreateMoveSymbol );
}
