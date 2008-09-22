// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Dotation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Dotation constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Dotation::Dotation( const ASN1T_ResourceDotations& asnMsg )
   : nDotationType_( asnMsg.ressource_id )
   , nNbr_         ( asnMsg.quantite_disponible )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dotation constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
Dotation::Dotation( const ASN1T_DotationStock & asnMsg )
   : nDotationType_( asnMsg.ressource_id )
   , nNbr_         ( asnMsg.quantite_disponible )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dotation destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Dotation::~Dotation()
{
    // NOTHING
}
       
// -----------------------------------------------------------------------------
// Name: Dotation::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Dotation::Update( const ASN1T_ResourceDotations& asnMsg )
{
    nNbr_ = asnMsg.quantite_disponible;
}

// -----------------------------------------------------------------------------
// Name: Dotation::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Dotation::Update( const ASN1T_DotationStock& asnMsg )
{
    nNbr_ = asnMsg.quantite_disponible;
}

// -----------------------------------------------------------------------------
// Name: Dotation::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Dotation::Send( ASN1T_ResourceDotations& asnMsg ) const
{
    asnMsg.ressource_id        = nDotationType_;
    asnMsg.quantite_disponible = nNbr_;
}

// -----------------------------------------------------------------------------
// Name: Dotation::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Dotation::Send( ASN1T_DotationStock& asnMsg ) const
{
    asnMsg.ressource_id        = nDotationType_;
    asnMsg.quantite_disponible = nNbr_;
}
