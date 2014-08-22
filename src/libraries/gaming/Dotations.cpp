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
#include "Dotation.h"
#include "clients_gui/DictionaryUpdated.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Dotations constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Dotations::Dotations( kernel::Entity_ABC& entity, Controller& controller, const tools::Resolver_ABC< DotationType >& resolver, gui::PropertiesDictionary& dico
                    , const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver )
    : HierarchicExtension_ABC( automatResolver, formationResolver, teamResolver )
    , entity_       ( entity )
    , controller_   ( controller )
    , resolver_     ( resolver )
    , dictionary_   ( dico )
    , bEmptyGasTank_( false )
    , property_     ( tools::translate( "Dotations", "Resources" ) )
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
void Dotations::CreateDictionary( gui::PropertiesDictionary& dico ) const
{
    dico.RegisterExtension( entity_, this, tools::translate( "Dotations", "Resources/Out of gas" ), bEmptyGasTank_ );
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
        current.lowThresholdPercentage_ = value.low_threshold();
        current.highThresholdPercentage_ = value.high_threshold();
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
    for( auto it = differences.begin(); it != differences.end(); ++it )
    {
        Dotation* dotation = Find( it->type_->GetId() );
        if( !dotation )
        {
            dotation = new Dotation( *it );
            Register( it->type_->GetId(), *dotation );
            dictionary_.RegisterExtension( entity_, this, property_ + "/" + it->type_->GetName().c_str(), dotation->quantity_ );
        }
        else
            *dotation = *dotation + *it;
        if( it->type_->IsGas() )
            bEmptyGasTank_ = ( dotation->quantity_ == 0 );
    }
    if( kernel::Entity_ABC* superior = const_cast< kernel::Entity_ABC* >( GetSuperior() ) )
        if( Dotations* dotations = dynamic_cast< Dotations* >( superior->Retrieve< Dotations_ABC >() ) )
            dotations->Update( differences );
    controller_.Update( gui::DictionaryUpdated( entity_, property_ ) );

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
        for( auto it = elements_.begin(); it != elements_.end(); ++it )
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
// Name: Dotations::Accept
// Created: LDC 2010-06-07
// -----------------------------------------------------------------------------
bool Dotations::Accept( const kernel::DotationType& type ) const
{
    Dotation* dotation = Find( type.GetId() );
    return dotation != 0;
}

// -----------------------------------------------------------------------------
// Name: Dotations::IsEmptyGasTank
// Created: SLI 2014-08-22
// -----------------------------------------------------------------------------
bool Dotations::IsEmptyGasTank() const
{
    return bEmptyGasTank_;
}
