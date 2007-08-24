// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "App6Symbol.h"

namespace tools
{
    namespace app6
    {

// -----------------------------------------------------------------------------
// Name: Align
// Created: SBO 2007-08-23
// -----------------------------------------------------------------------------
void Align( std::string& symbol )
{
    if( symbol.length() != 15 )
        symbol.resize( 15, '*' );
}

// -----------------------------------------------------------------------------
// Name: FilterSymbol
// Created: SBO 2007-08-23
// -----------------------------------------------------------------------------
std::string FilterSymbol( const std::string& source, const std::string& filter )
{
    if( filter.empty() )
       return source;
    unsigned i = 0;
    const unsigned max = std::min( source.length(), filter.length() );
    while( i < max && source[ i ] == filter[ i ] )
        ++i;
    return source.substr( 0, i );
}

// -----------------------------------------------------------------------------
// Name: MergeSymbol
// Created: SBO 2007-08-22
// -----------------------------------------------------------------------------
std::string MergeSymbol( const std::string& to, const std::string& from )
{
    std::string result( to );
    for( unsigned int i = 0; i < std::min( result.length(), from.length() ); ++i )
        if( result[i] == '*' )
            result[i] = from[i];
    return result;
}

namespace
{
    unsigned int FieldToKeep( unsigned int level )
    {
        switch( level )
        {
            case 0: return 10; // identification => all                      shgpucaaaw
            case 1: return 7;  // recognition    => side + category + weapon shgpuca
            case 2: return 5;  // detection      => nothing                  sugpu
        }
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: FilterPerceptionLevel
// Created: SBO 2007-08-24
// -----------------------------------------------------------------------------
void FilterPerceptionLevel( std::string& symbol, unsigned int level )
{
    // $$$$ SBO 2007-08-24: 0 => identifie, 1 => reconnu, 2 => detecte, 3 => signale
    symbol = symbol.substr( 0, FieldToKeep( level ) );
    if( level > 1 )
    {
        tools::app6::SetLevel( symbol, "-" );
        tools::app6::SetAffiliation( symbol, 0 ); // $$$$ SBO 2007-08-24: Unknown
    }
}

// -----------------------------------------------------------------------------
// Name: SetAffiliation
// Created: SBO 2007-08-23
// -----------------------------------------------------------------------------
void SetAffiliation( std::string& symbol, unsigned int affiliation )
{
    static const char affiliations[4] = { 'U', 'F', 'H', 'N' }; // Unkown, Friend, Hostile, Neutral
    Align( symbol );
    if( affiliation < 4 )
        symbol[1] = affiliations[affiliation];
}


// -----------------------------------------------------------------------------
// Name: GetAffiliation
// Created: SBO 2007-08-24
// -----------------------------------------------------------------------------
std::string GetAffiliation( const std::string& symbol )
{
    if( symbol.length() < 2 )
        return "U";
    return symbol.substr( 1, 1 );;
}

// -----------------------------------------------------------------------------
// Name: SetBattleDimension
// Created: SBO 2007-08-24
// -----------------------------------------------------------------------------
void SetBattleDimension( std::string& symbol, char dimension )
{
    Align( symbol );
    symbol[2] = dimension;
}

// -----------------------------------------------------------------------------
// Name: SetFunction
// Created: SBO 2007-08-23
// -----------------------------------------------------------------------------
void SetFunction( std::string& symbol, const std::string& function )
{
    Align( symbol );
    for( unsigned int i = 0; i < function.length(); ++i )
        if( function[i] != '*' )
            symbol[i + 4] = function[i];
}

// -----------------------------------------------------------------------------
// Name: SetCommandPost
// Created: SBO 2007-08-23
// -----------------------------------------------------------------------------
void SetCommandPost( std::string& symbol, bool commandPost )
{
    Align( symbol );
    symbol[10] = commandPost ? 'A' : '-';
}
/*
namespace
{
    char ToChar( const ASN1T_EnumNatureLevel& level )
    {
        switch( level )
        {
        case EnumNatureLevel::o:      return 'A';
        case EnumNatureLevel::oo:     return 'C';
        case EnumNatureLevel::ooo:    return 'D';
        case EnumNatureLevel::i:      return 'E';
        case EnumNatureLevel::ii:     return 'F';
        case EnumNatureLevel::iii:    return 'G';
        case EnumNatureLevel::x:      return 'H';
        case EnumNatureLevel::xx:     return 'I';
        case EnumNatureLevel::xxx:    return 'J';
        case EnumNatureLevel::xxxx:   return 'K';
        case EnumNatureLevel::none:
        }
        return '-';
    }
}
*/
// -----------------------------------------------------------------------------
// Name: SetLevel
// Created: SBO 2007-08-23
// -----------------------------------------------------------------------------
void SetLevel( std::string& symbol, unsigned int level )
{
    static const char levels[12] = { '-', 'A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'M' };
    Align( symbol );
    if( level < 12 )
        symbol[11] = levels[level];
}

// -----------------------------------------------------------------------------
// Name: SetLevel
// Created: SBO 2007-08-23
// -----------------------------------------------------------------------------
void SetLevel( std::string& symbol, const std::string& level )
{
    static const std::string levels[11] = { "o", "oo", "ooo", "i", "ii", "iii", "x", "xx", "xxx", "xxxx", "xxxxx" };
    const std::string stripped = level.substr( level.find_last_of( "/" ) + 1 );
    for( unsigned int i = 0; i < 11; ++i )
        if( levels[i] == stripped )
        {
            SetLevel( symbol, i + 1 );
            return;
        }
    SetLevel( symbol, 0 );
}

// -----------------------------------------------------------------------------
// Name: GetLevel
// Created: SBO 2007-08-23
// -----------------------------------------------------------------------------
unsigned int GetLevel( const std::string& symbol )
{
    static const char levels[12] = { '-', 'A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'M' };
    if( symbol.length() < 12 )
        return 0;
    for( unsigned int i = 0; i < 12; ++i )
        if( symbol[11] == levels[i] )
            return i;
    return 0;
}

    }
}
