// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ObjectSymbols.h"
#include "Object.h"
#include "Side.h"
#include "tools/App6Symbol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ObjectSymbols constructor
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
ObjectSymbols::ObjectSymbols( const Object& holder )
    : holder_( holder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectSymbols destructor
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
ObjectSymbols::~ObjectSymbols()
{
    // NOTHING
}

/*
    // B14: pp 337
    // Abattis                      : UpdateFunction( symbol, "OS----" );
    // Antitank under construction  : UpdateFunction( symbol, "OADU--" );
    // Antitank complete            : UpdateFunction( symbol, "OADC--" );
    // Antitank mined complete      : UpdateFunction( symbol, "OAR---" );
    // Antitank wall                : UpdateFunction( symbol, "OAW---" );
    // mine                         : UpdateFunction( symbol, "OMU---" );
    // Antitank mine                : UpdateFunction( symbol, "OMT---" );
    // mine cluster                 : UpdateFunction( symbol, "OMC---" );
    // mine field                   : UpdateFunction( symbol, "OFS---" ); // with A instead of M in [3] for planned
    // bridge planned               : UpdateFunction( symbol, "ORP---" );
    // bridge                       : UpdateFunction( symbol, "ORS---" );
    // bridge destruction           : UpdateFunction( symbol, "ORA---" );
    // bridge destruction complete  : UpdateFunction( symbol, "ORC---" );

    // crossing site                : UpdateFunction( symbol, "BCA---" );
    // nuclear, bio, chemic         : UpdateFunction( symbol, "NR----" );
    // nuclear, bio, chemic         : UpdateFunction( symbol, "NB----" );
    // nuclear, bio, chemic         : UpdateFunction( symbol, "NC----" );

    // Indirect fire, impact point  : UpdateFunction( symbol, "GPWI--" ); // symbol( "G*GP********--X" );
    // Indirect fire, action point  : UpdateFunction( symbol, "GPP---" ); // symbol( "G*GP********--X" );
*/

namespace
{
    std::string MakeSymbol( const std::string& type )
    {
        // $$$$ LDC FIXME: Would be better with a map.
        std::string symbol( "G*MP********--X" );
        if( type == "abattis" )
            tools::app6::SetFunction( symbol, "OS----" );
        else if( type == "bouchon mines" )
            tools::app6::SetFunction( symbol, "OMC---" );
        else if( type == "zone minee lineaire" )
        {
            tools::app6::SetBattleDimension( symbol, 'A' );
            tools::app6::SetFunction( symbol, "OFS---" );
        }
        else if( type == "destruction pont" || type == "destruction route" )
            tools::app6::SetFunction( symbol, "ORA---" );
        else if( type == "plot ravitaillement"
              || type == "zone implantation canon"
              || type == "zone implantation cobra"
              || type == "zone implantation lrm"
              || type == "zone implantation mortier" )
        {
            tools::app6::SetBattleDimension( symbol, 'G' );
            tools::app6::SetFunction( symbol, "GPP---" );
        }
        else if( type == "poste controle" )
            tools::app6::SetFunction( symbol, "OESP--" ); // 'O'bject 'E'mergency 'S'afety 'P'atrol
        else if( type == "fire" )
            tools::app6::SetFunction( symbol, "OEIF--" ); // 'O'bject 'E'mergency 'I'ncident 'F'ire
        else if( type == "zone protegee" )
            tools::app6::SetFunction( symbol, "OESS--" ); // 'O'bject 'E'mergency 'S'afety 'S'helter
        return symbol;
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectSymbols::BuildSymbol
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
std::string ObjectSymbols::BuildSymbol( bool up /*= true*/ ) const
{
    std::string symbol( MakeSymbol( holder_.type_ ) );
    if( up )
        return tools::app6::MergeSymbol( holder_.side_.Get< EntitySymbols_ABC >().BuildSymbol(), symbol );
    return symbol;
}
