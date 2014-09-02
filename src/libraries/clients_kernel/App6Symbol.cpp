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
#include <boost/assign.hpp>
#pragma warning( push, 0 )
#include <boost/bimap.hpp>
#pragma warning( pop )

using namespace kernel;

namespace
{
    typedef boost::bimap< char, Karma > T_Karmas;
    const T_Karmas karmas_= boost::assign::list_of< T_Karmas::relation >
        ( 'f', Karma::friend_ )( 'h', Karma::enemy_ )( 'n', Karma::neutral_ )( 'u', Karma::unknown_ );
}

// -----------------------------------------------------------------------------
// Name: App6Symbol::SetKarma
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
void App6Symbol::SetKarma( std::string& symbol, const kernel::Karma& karma )
{
    const auto pos = symbol.find_last_of( '/' );
    if( pos != std::string::npos )
    {
        if( symbol.size() > pos+2 )
            symbol[pos + 2] = karmas_.right.at( karma );
    }
    else if( symbol.size() > 2 )
        symbol[1] = karmas_.right.at( karma );
}

// -----------------------------------------------------------------------------
// Name: App6Symbol::GetBase
// Created: LDC 2013-04-22
// -----------------------------------------------------------------------------
std::string App6Symbol::GetBase( const std::string& symbol, Karma& karma )
{
    std::string result = symbol;
    karma = Karma::unknown_;
    const auto pos = symbol.find_last_of( '/' );
    if( pos != std::string::npos )
        result = symbol.substr( pos + 1 );
    if( result.size() > 2 )
    {
        auto it = karmas_.left.find( result[1] );
        if( it != karmas_.left.end() )
            karma = it->second;
        result[1] = '*';
    }
    return result;
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
    if( eIdentification != perception ) // keep all if identified
        symbol = symbol.substr( 0, symbol.find_last_of( '/' ) + ElementsToKeep( perception ) + 1 );
}

namespace
{
    int ToKeep( E_PerceptionResult perception )
    {
        switch( perception )
        {
        default:
        case eNotSeen:
        case eDetection:      return 3; // nothing                  sugpu
        case eRecognition:    return 5; // side + category + weapon shgpuca
        case eIdentification: return 9; // all                      shgpucaaaw
        }
    }
}

std::string App6Symbol::FilterNature( const std::string& nature, E_PerceptionResult perception )
{
    const int keep = ToKeep( perception ) - 3;
    QStringList list = QStringList::split( '/',  nature.c_str() );
    while( list.size() > keep )
        list.pop_back();
    const QString result = list.join( "/" );
    return result.isNull() ? "" : result.toStdString();
}
