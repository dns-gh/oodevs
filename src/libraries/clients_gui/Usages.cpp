// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Usages.h"
#include "PropertiesDictionary.h"
#include "clients_kernel/AccommodationType.h"
#include "clients_kernel/AccommodationTypes.h"
#include "clients_kernel/Tools.h"

using namespace kernel;
using namespace gui;

const std::string Usages::defaultStr_ = "default";

// -----------------------------------------------------------------------------
// Name: Usages constructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
Usages::Usages( PropertiesDictionary& dictionary, const AccommodationTypes& accommodationTypes, float livingSpace, kernel::Entity_ABC& owner )
    : dictionary_        ( dictionary )
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

namespace
{
    template< typename T >
    void CreateProperties( kernel::Entity_ABC& entity, PropertiesDictionary& dictionary, const QString& name, T& value )
    {
        dictionary.Register( entity, name, value, true, eUrbanTemplate );
    }
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

        CreateProperties( owner_, dictionary_, defaultString + tools::translate( "Block", "/Percentage" ), usages_[ defaultStr_ ] );
        CreateProperties( owner_, dictionary_, defaultString + tools::translate( "Block", "/Nominal capacity" ), occupations_[ defaultStr_ ].first );
        CreateProperties( owner_, dictionary_, defaultString + tools::translate( "Block", "/Maximal capacity" ), occupations_[ defaultStr_ ].second );
    }
    else
    {
        if( dictionary_.HasKey( defaultString + tools::translate( "Block", "/Percentage" ) ) )
        {
            dictionary_.Remove( defaultString + tools::translate( "Block", "/Percentage" ) );
            dictionary_.Remove( defaultString + tools::translate( "Block", "/Nominal capacity" ) );
            dictionary_.Remove( defaultString + tools::translate( "Block", "/Maximal capacity" ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Usages::UpdateMotivations
// Created: NPT 2012-11-14
// -----------------------------------------------------------------------------
void Usages::UpdateMotivations( float livingSpace )
{
    livingSpace_ = livingSpace;
    for( auto it = usages_.begin(); it != usages_.end(); ++it )
    {
        const std::string usageName = it->first.c_str() != defaultStr_ ? it->first : tools::translate( "Block", "Default" ).toStdString();
        const QString motivationString = tools::translate( "Block", "PhysicalFeatures/Motivations/" ) + usageName.c_str();
        float occupation = livingSpace_ * it->second * 0.01f;
        AccommodationType* accommodation = accommodationTypes_.Find( usageName );
        occupations_[ usageName ].first = static_cast< unsigned int >( occupation * ( accommodation ? accommodation->GetNominalCapacity() : 1 ) );
        occupations_[ usageName ].second = static_cast< unsigned int >( occupation * ( accommodation ? accommodation->GetMaxCapacity() : 1 ) );
        if( occupations_[ usageName ].first != 0 )
        {
            CreateProperties( owner_, dictionary_, motivationString + tools::translate( "Block", "/Percentage" ), usages_[ usageName ] );
            CreateProperties( owner_, dictionary_, motivationString + tools::translate( "Block", "/Nominal capacity" ), occupations_[ usageName ].first );
            CreateProperties( owner_, dictionary_, motivationString + tools::translate( "Block", "/Maximal capacity" ), occupations_[ usageName ].second );
        }
    }
    UpdateDefault();
}

// -----------------------------------------------------------------------------
// Name: Usages::Add
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
void Usages::Add( const std::string& usage, unsigned int proportion )
{
    AccommodationType* accommodation = accommodationTypes_.Find( usage );
    if( accommodation )
    {
        usages_[ usage ] = proportion;
        const QString motivationString = tools::translate( "Block", "PhysicalFeatures/Motivations/" ) + usage.c_str();
        float occupation = livingSpace_ * proportion * 0.01f;

        occupations_[ usage ].first = static_cast< unsigned int >( occupation * ( accommodation->GetNominalCapacity() ) );
        occupations_[ usage ].second = static_cast< unsigned int >( occupation * ( accommodation->GetMaxCapacity() ) );

        CreateProperties( owner_, dictionary_, motivationString + tools::translate( "Block", "/Percentage" ), usages_[ usage ] );
        CreateProperties( owner_, dictionary_, motivationString + tools::translate( "Block", "/Nominal capacity" ), occupations_[ usage ].first );
        CreateProperties( owner_, dictionary_, motivationString + tools::translate( "Block", "/Maximal capacity" ), occupations_[ usage ].second );
    }

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
        auto it = usages_.begin();
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
    auto it = usages_.find( usage );
    if( it != usages_.end() )
        return it->second;
    return 0u;
}
