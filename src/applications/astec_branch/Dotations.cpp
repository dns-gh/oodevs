// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Dotations.h"
#include "App.h"
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: Dotations constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Dotations::Dotations( Controller& controller )
    : controller_( controller )
    , bEmptyGasTank_( false )
{

}

// -----------------------------------------------------------------------------
// Name: Dotations destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Dotations::~Dotations()
{

}

// -----------------------------------------------------------------------------
// Name: Dotations::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Dotations::DoUpdate( const ASN1T_MsgUnitDotations& message )
{
    if( ! message.m.dotation_eff_ressourcePresent  )
        return;

    uint nSize = message.dotation_eff_ressource.n;
    while( nSize > 0 )
    {
        const ASN1T_DotationRessource& value = message.dotation_eff_ressource.elem[ --nSize ];
        unsigned long equipId = value.ressource_id;
        unsigned qty = value.quantite_disponible;
        dotations_[ equipId ] = qty;

        // $$$$ AGE 2006-02-13: 
//        if( App::GetApp().GetResource( equipId ).GetDotationName() == "carburant" )
//            bEmptyGasTank_ = (qty == 0);
    }

    controller_.Update( *this );
}
