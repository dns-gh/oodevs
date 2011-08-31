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
Dotations::Dotations( Controller& controller, const tools::Resolver_ABC< DotationType >& resolver, PropertiesDictionary& dico
                    , const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver )
    : HierarchicExtension_ABC( automatResolver, formationResolver, teamResolver )
    , controller_( controller )
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
    dico.Register( *this, tools::translate( "Dotations", "Resources/Out of gas" ), bEmptyGasTank_ );
}

// -----------------------------------------------------------------------------
// Name: Dotations::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Dotations::DoUpdate( const sword::UnitAttributes& message )
{
    if( !message.has_resource_dotations() )
        return;

    uint nSize = message.resource_dotations().elem_size();
    std::vector< Dotation > differences; differences.reserve( nSize );
    while( nSize > 0 )
    {
        const sword::ResourceDotations_ResourceDotation& value = message.resource_dotations().elem( --nSize );
        Dotation previous( resolver_.Get( value.type().id() ) );
        if( Dotation* dotation = Find( value.type().id() ) )
            previous = *dotation;
        Dotation current( previous );
        current.quantity_ = value.quantity();
        current.thresholdPercentage_ = value.threshold();
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
            dictionary_.Register( *this, tools::translate( "Dotations", "Resources" ) + "/" + it->type_->GetName().c_str(), ( ( const Dotation& ) *dotation ).quantity_ ); // $$$$ AGE 2006-06-22:
        }
        else
            *dotation = *dotation + *it;
        if( it->type_->IsGas() )
            bEmptyGasTank_ = ( dotation->quantity_ == 0 );
    }
    if( kernel::Entity_ABC* superior = const_cast< kernel::Entity_ABC* >( GetSuperior() ) )
        if( Dotations* dotations = dynamic_cast< Dotations* >( superior->Retrieve< Dotations_ABC >() ) )
            dotations->Update( differences );
    controller_.Update( *(Dotations_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Dotations::SetSuperior
// Created: SBO 2007-04-12
// -----------------------------------------------------------------------------
void Dotations::SetSuperior( const kernel::Entity_ABC& superior )
{
    kernel::Entity_ABC* currentSuperior = const_cast< kernel::Entity_ABC* >( GetSuperior() );
    if( !currentSuperior || &superior != currentSuperior )
    {
        // create dotation differences
        std::vector< Dotation > differences;
        differences.reserve( elements_.size() );
        for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
            differences.push_back( *it->second );
        // add dotations to new superior
        if( Dotations* dotations = dynamic_cast< Dotations* >( const_cast< kernel::Entity_ABC& >( superior ).Retrieve< Dotations_ABC >() ) )
            dotations->Update( differences );

        // remove dotations from previous superior
        if( currentSuperior )
            if( Dotations* dotations = dynamic_cast< Dotations* >( currentSuperior->Retrieve< Dotations_ABC >() ) )
            {
                std::transform( differences.begin(), differences.end(), differences.begin(), std::negate< Dotation >() );
                dotations->Update( differences );
            }
    }
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

// -----------------------------------------------------------------------------
// Name: Dotations::Accept
// Created: LDC 2010-06-07
// -----------------------------------------------------------------------------
bool Dotations::Accept( const kernel::DotationType& type ) const
{
    Dotation* dotation = Find( type.GetId() );
    return dotation && dotation->quantity_ > 0;
}
