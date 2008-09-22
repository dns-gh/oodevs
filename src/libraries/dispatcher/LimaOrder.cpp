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
#include "ClientPublisher_ABC.h"
#include "game_asn/Simulation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LimaOrder constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
LimaOrder::LimaOrder( const ASN1T_LimaOrder& asn )
    : location_ ( asn.lima )
    , schedule_ ( (const char*)asn.horaire.data, 15 )
{
    std::copy( asn.fonctions.elem, asn.fonctions.elem + asn.fonctions.n, std::back_inserter( functions_ ) );
}

// -----------------------------------------------------------------------------
// Name: LimaOrder destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
LimaOrder::~LimaOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LimaOrder::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void LimaOrder::Send( ASN1T_LimaOrder& asn ) const
{
    location_.Send( asn.lima );
    asn.horaire = schedule_.c_str();
    asn.fonctions.n = functions_.size();
    asn.fonctions.elem = (ASN1T_EnumLimaType*)( functions_.empty() ? 0 : &functions_.front() );
}
