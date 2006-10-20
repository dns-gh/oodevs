// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Dotations.h"
#include "clients_kernel/Controller.h"
#include "Dotation.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "statusicons.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Dotations constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Dotations::Dotations( Controller& controller, const Resolver_ABC< DotationType >& resolver, PropertiesDictionary& dico )
    : controller_( controller )
    , resolver_( resolver )
    , dictionary_( dico )
    , bEmptyGasTank_( false )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: Dotations destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Dotations::~Dotations()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Dotations::CreateDictionary
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void Dotations::CreateDictionary( kernel::PropertiesDictionary& dico ) const
{
    dico.Register( *this, tools::translate( "Dotations", "Dotations/Plus de carburant" ), bEmptyGasTank_ );
}

// -----------------------------------------------------------------------------
// Name: Dotations::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Dotations::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( ! message.m.dotation_eff_ressourcePresent  )
        return;

    uint nSize = message.dotation_eff_ressource.n;
    while( nSize > 0 )
    {
        const ASN1T_DotationRessource& value = message.dotation_eff_ressource.elem[ --nSize ];
        DotationType& type = resolver_.Get( value.ressource_id );
        Dotation* dotation = Find( value.ressource_id );
        if( dotation )
            dotation->quantity_ = value.quantite_disponible;
        else
        {
            Dotation& newDotation = *new Dotation( type, value.quantite_disponible );
            Register( value.ressource_id, newDotation );
            dictionary_.Register( *this, tools::translate( "Dotations", "Dotations" ) + "/" + type.GetCategory(), ((const Dotation&)newDotation).quantity_ ); // $$$$ AGE 2006-06-22: 
        }
        if( type.IsGas() )
            bEmptyGasTank_ = ( value.quantite_disponible == 0 );
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Dotations::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Dotations::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( bEmptyGasTank_ && viewport.IsInside( where ) )
        tools.DrawIcon( xpm_gas, where, 150.f );
}
