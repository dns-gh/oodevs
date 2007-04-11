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
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "statusicons.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Dotations constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Dotations::Dotations( Controller& controller, const Resolver_ABC< DotationType >& resolver, PropertiesDictionary& dico, kernel::Entity_ABC& holder )
    : controller_( controller )
    , resolver_( resolver )
    , holder_( holder )
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
    dico.Register( *this, tools::translate( "Dotations", "Dotations/Out of gas" ), bEmptyGasTank_ );
}

// -----------------------------------------------------------------------------
// Name: Dotations::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Dotations::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( ! message.m.dotation_eff_ressourcePresent  )
        return;

    std::vector< Dotation > differences;
    uint nSize = message.dotation_eff_ressource.n;
    while ( nSize > 0 )
    {
        const ASN1T_DotationRessource& value = message.dotation_eff_ressource.elem[ --nSize ];
        Dotation previous( resolver_.Get( value.ressource_id ) );
        if( Dotation* dotation = Find( value.ressource_id ) )
            previous = *dotation;
        Dotation current( previous );
        current.quantity_ = value.quantite_disponible;
        differences.push_back( current - previous );
    }
    Update( differences );
}

// -----------------------------------------------------------------------------
// Name: Dotations::Update
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
void Dotations::Update( const std::vector< Dotation >& differences )
{
    for( std::vector< Dotation >::const_iterator it = differences.begin(); it != differences.end(); ++it )
    {
        Dotation* dotation = Find( it->type_->GetId() );
        if( !dotation )
        {
            dotation = new Dotation( *it );
            Register( it->type_->GetId(), *dotation );
            dictionary_.Register( *this, tools::translate( "Dotations", "Dotations" ) + "/" + it->type_->GetCategory(), ((const Dotation&)dotation).quantity_ ); // $$$$ AGE 2006-06-22: 
        }
        else
            *dotation = *dotation + *it;
        if( it->type_->IsGas() )
            bEmptyGasTank_ = ( it->quantity_ == 0 );
    }
    if( const kernel::Entity_ABC* superior = holder_.Get< kernel::TacticalHierarchies >().GetSuperior() )
        if( Dotations* dotations = const_cast< Dotations* >( superior->Retrieve< Dotations >() ) )
            dotations->Update( differences );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Dotations::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Dotations::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( bEmptyGasTank_ && viewport.IsHotpointVisible() )
        tools.DrawIcon( xpm_gas, where, 150.f );
}
