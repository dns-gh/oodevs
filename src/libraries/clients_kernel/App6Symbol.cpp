// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "App6Symbol.h"
#include "Karma.h"

using namespace kernel;

App6Symbol::T_KarmaChars App6Symbol::karmaChars_;

// -----------------------------------------------------------------------------
// Name: App6Symbol constructor
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
App6Symbol::App6Symbol()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: App6Symbol destructor
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
App6Symbol::~App6Symbol()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: App6Symbol::Initialize
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
void App6Symbol::Initialize()
{
    karmaChars_[Karma::friend_]  = 'f';
    karmaChars_[Karma::enemy_]   = 'h';
    karmaChars_[Karma::neutral_] = 'n';
    karmaChars_[Karma::unknown_] = 'u';
}

// -----------------------------------------------------------------------------
// Name: App6Symbol::SetKarma
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
void App6Symbol::SetKarma( std::string& symbol, const kernel::Karma& karma )
{
    if( karmaChars_.empty() ) // $$$$ SBO 2007-02-26:
        Initialize();
    std::string::size_type pos = symbol.find_last_of( '/' );
    if( pos != std::string::npos )
    {
        if( symbol.size() > pos+2 )
            symbol[pos + 2] = karmaChars_[karma];
    }
    else if( symbol.size() > 2 )
        symbol[1] = karmaChars_[karma];
}

namespace
{
    unsigned int ElementsToKeep( E_PerceptionResult perception )
    {
        switch( perception )
        {
        default:
        case eNotSeen:
        case eDetection:      return 5;  // nothing                  sugpu
        case eRecognition:    return 7;  // side + category + weapon shgpuca
        case eIdentification: return 10; // all                      shgpucaaaw
        }
    }
}

// -----------------------------------------------------------------------------
// Name: App6Symbol::FilterPerceptionLevel
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
void App6Symbol::FilterPerceptionLevel( std::string& symbol, E_PerceptionResult perception )
{
    symbol = symbol.substr( 0, symbol.find_last_of( '/' ) + ElementsToKeep( perception ) + 1 );
}

// -----------------------------------------------------------------------------
// Name: App6Symbol::Merge
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
void App6Symbol::Merge( const std::string& from, std::string& to )
{
    if( to.empty() )
       to = from;
    else
    {
        unsigned i = 0;
        const std::size_t max = std::min( to.length(), from.length() );
        while( i < max && to[ i ] == from[ i ] )
            ++i;
        to = to.substr( 0, i );
    }
}
