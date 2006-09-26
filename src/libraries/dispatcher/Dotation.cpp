// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2006-09-25 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Dotation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Dotation constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Dotation::Dotation( const Model& /*model*/, const ASN1T_DotationRessource& asnMsg )
: nDotationType_( asnMsg.ressource_id )
   , nNbr_         ( asnMsg.quantite_disponible )
{
}

// -----------------------------------------------------------------------------
// Name: Dotation destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Dotation::~Dotation()
{

}
    
   
// -----------------------------------------------------------------------------
// Name: Dotation::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Dotation::Update( const ASN1T_DotationRessource& asnMsg )
{
    nNbr_ = asnMsg.quantite_disponible;
}
