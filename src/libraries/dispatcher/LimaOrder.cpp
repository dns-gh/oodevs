// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LimaOrder.h"
#include "Network_Def.h"
#include "game_asn/Asn.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LimaOrder constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
LimaOrder::LimaOrder( Model& /*model*/, const ASN1T_LimaOrder& asn )
    : location_ ( asn.lima )
    , functions_()
{
    for( unsigned i = 0; i < asn.fonctions.n; ++i )
        functions_.push_back( asn.fonctions.elem[i] );
}

// -----------------------------------------------------------------------------
// Name: LimaOrder destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
LimaOrder::~LimaOrder()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: LimaOrder::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void LimaOrder::Send( ASN1T_LimaOrder& asn ) const
{
    location_.Send( asn.lima );
    SendContainerValues< ASN1T__SeqOfEnumLimaType, ASN1T_EnumLimaType, T_Functions >( functions_, asn.fonctions );
}

// -----------------------------------------------------------------------------
// Name: LimaOrder::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void LimaOrder::AsnDelete( ASN1T_LimaOrder& asn )
{
    Localisation::AsnDelete( asn.lima );
    if( asn.fonctions.n )
        delete [] asn.fonctions.elem;
}
