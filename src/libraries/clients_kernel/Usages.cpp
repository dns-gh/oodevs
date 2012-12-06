// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Usages.h"
#include "AccommodationType.h"
#include "AccommodationTypes.h"
#include "DictionaryUpdated.h"
#include "PropertiesDictionary.h"
#include "Tools.h"
#include "clients_kernel/Controller.h"

using namespace kernel;

const std::string Usages::defaultStr_ = "default";

// -----------------------------------------------------------------------------
// Name: Usages constructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
Usages::Usages( PropertiesDictionary& dictionary, const AccommodationTypes& accommodationTypes, float livingSpace, kernel::Entity_ABC& owner, kernel::Controller& controller  )
    : controller_        ( controller )
    , dictionary_        ( dictionary )
    , accommodationTypes_( accommodationTypes)
    , livingSpace_       ( livingSpace )
    , owner_             ( owner )
{
    UpdateDefault();
}

// -----------------------------------------------------------------------------
// Name: Usages destructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
Usages::~Usages()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Usages::UpdateDefault
// Created: ABR 2012-05-29
// -----------------------------------------------------------------------------
void Usages::UpdateDefault()
{
    unsigned int total = 0;
    for( auto it = usages_.begin(); it != usages_.end(); ++it )
        if( it->first != defaultStr_ )
            total += it->second;
    usages_[ defaultStr_ ] = 100 - total;

    static const QString defaultString = tools::translate( "Block", "PhysicalFeatures/Motivations/" ) + tools::translate( "Block", "Default" );
    if( usages_[ defaultStr_ ] )
    {
        float occupation = livingSpace_ * usages_[ defaultStr_ ] * 0.01f;
        AccommodationType* accommodation = accommodationTypes_.Find( defaultStr_ );
        occupations_[ defaultStr_ ].first = static_cast< unsigned int >( occupation * ( accommodation ? accommodation->GetNominalCapacity() : 1 ) );
        occupations_[ defaultStr_ ].second = static_cast< unsigned int >( occupation * ( accommodation ? accommodation->GetMaxCapacity() : 1 ) );

        dictionary_.Register( owner_, defaultString + tools::translate( "Block", "/Percentage" ), static_cast< const unsigned int &>( usages_[ defaultStr_ ] ) );
        dictionary_.Register( owner_, defaultString + tools::translate( "Block", "/Nominal capacity" ), static_cast< const unsigned int &>( occupations_[ defaultStr_ ].first ) );
        dictionary_.Register( owner_, defaultString + tools::translate( "Block", "/Maximal capacity" ), static_cast< const unsigned int &>( occupations_[ defaultStr_ ].second ) );
        controller_.Update( DictionaryUpdated( owner_, defaultString ) );
    }
    else
    {
        if( dictionary_.HasKey( defaultString + tools::translate( "Block", "/Percentage" ) ) )
        {
            dictionary_.Remove( defaultString + tools::translate( "Block", "/Percentage" ) );
            dictionary_.Remove( defaultString + tools::translate( "Block", "/Nominal capacity" ) );
            dictionary_.Remove( defaultString + tools::translate( "Block", "/Maximal capacity" ) );
            controller_.Update( DictionaryUpdated( owner_, defaultString ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Usages::Add
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
void Usages::Add( const std::string& usage, unsigned int proportion )
{
    usages_[ usage ] = proportion;
    const QString motivationString = tools::translate( "Block", "PhysicalFeatures/Motivations/" ) + usage.c_str();
    float occupation = livingSpace_ * proportion * 0.01f;
    AccommodationType* accommodation = accommodationTypes_.Find( usage );
    occupations_[ usage ].first = static_cast< unsigned int >( occupation * ( accommodation ? accommodation->GetNominalCapacity() : 1 ) );
    occupations_[ usage ].second = static_cast< unsigned int >( occupation * ( accommodation ? accommodation->GetMaxCapacity() : 1 ) );

    dictionary_.Register( owner_, motivationString + tools::translate( "Block", "/Percentage" ), static_cast< const unsigned int &>( usages_[ usage ] ) );
    dictionary_.Register( owner_, motivationString + tools::translate( "Block", "/Nominal capacity" ), static_cast< const unsigned int &>( occupations_[ usage ].first ) );
    dictionary_.Register( owner_, motivationString + tools::translate( "Block", "/Maximal capacity" ), static_cast< const unsigned int &>( occupations_[ usage ].second ) );

    controller_.Update( kernel::DictionaryUpdated( owner_, motivationString ) );

    UpdateDefault();
}

// -----------------------------------------------------------------------------
// Name: Usages::Remove
// Created: ABR 2012-05-25
// -----------------------------------------------------------------------------
void Usages::Remove( const std::string& usage )
{
    const QString motivationString = tools::translate( "Block", "PhysicalFeatures/Motivations/" ) + usage.c_str();
    dictionary_.Remove( motivationString );
    usages_.erase( usage );
    occupations_.erase( usage );

    dictionary_.Remove( motivationString + tools::translate( "Block", "/Percentage" ) );
    dictionary_.Remove( motivationString + tools::translate( "Block", "/Nominal capacity" ) );
    dictionary_.Remove( motivationString + tools::translate( "Block", "/Maximal capacity" ) );

    controller_.Delete( kernel::DictionaryUpdated( owner_, motivationString ) );

    UpdateDefault();
}

// -----------------------------------------------------------------------------
// Name: Usages::ClearAll
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void Usages::ClearAll()
{
    while( usages_.size() > 1 )
    {
        CIT_Usages it = usages_.begin();
        const std::string usage = it->first;
        if( usage == defaultStr_ )
            ++it;
        Remove( usage );
    }
}

// -----------------------------------------------------------------------------
// Name: Usages::Find
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
unsigned int Usages::Find( const std::string& usage ) const
{
    CIT_Usages it = usages_.find( usage );
    if( it != usages_.end() )
        return it->second;
    return 0u;
}
