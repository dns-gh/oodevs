// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Usages.h"
#include "Tools.h"
#include "clients_kernel/AccommodationType.h"
#include "clients_kernel/AccommodationTypes.h"
#include "clients_kernel/PropertiesDictionary.h"

using namespace gui;

const std::string Usages::defaultStr_ = "default";

// -----------------------------------------------------------------------------
// Name: Usages constructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
Usages::Usages( kernel::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes, float livingSpace )
    : dictionary_        ( dictionary )
    , accommodationTypes_( accommodationTypes)
    , livingSpace_       ( livingSpace )
{
    usages_[ defaultStr_ ] = 100;
    CIT_Usages it = usages_.find( defaultStr_ );
    static const QString defaultString = tools::translate( "Block", "PhysicalFeatures/Motivations/" ) + tools::translate( "Block", "Default" );
    dictionary_.Register( *static_cast< const Usages* >( this ), defaultString + tools::translate( "Block", "/Percentage" ), static_cast< const unsigned int &>( it->second ) );
    kernel::AccommodationType* accommodation = accommodationTypes_.Find( defaultStr_ );
    occupations_[ defaultStr_ ].first = static_cast< unsigned int >( livingSpace_ * ( accommodation ? accommodation->GetNominalCapacity() : 1 ) );
    occupations_[ defaultStr_ ].second = static_cast< unsigned int >( livingSpace_ * ( accommodation ? accommodation->GetMaxCapacity() : 1 ) );
    dictionary_.Register( *static_cast< const Usages* >( this ), defaultString + tools::translate( "Block", "/Nominal capacity" ), static_cast< const unsigned int &>( occupations_[ defaultStr_ ].first ) );
    dictionary_.Register( *static_cast< const Usages* >( this ), defaultString + tools::translate( "Block", "/Maximal capacity" ), static_cast< const unsigned int &>( occupations_[ defaultStr_ ].second ) );
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
// Name: Usages::Add
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
void Usages::Add( const std::string& usage, unsigned int proportion )
{
    usages_[ usage ] = proportion;
    const QString motivationString = tools::translate( "Block", "PhysicalFeatures/Motivations/" ) + usage.c_str();
    dictionary_.Register( *static_cast< const Usages* >( this ), motivationString + tools::translate( "Block", "/Percentage" ), static_cast< const unsigned int &>( usages_[ usage ] ) );

    float occupation = livingSpace_ * proportion * 0.01f;
    kernel::AccommodationType* accommodation = accommodationTypes_.Find( usage );
    occupations_[ usage ].first = static_cast< unsigned int >( occupation * ( accommodation ? accommodation->GetNominalCapacity() : 1 ) );
    occupations_[ usage ].second = static_cast< unsigned int >( occupation * ( accommodation ? accommodation->GetMaxCapacity() : 1 ) );
    dictionary_.Register( *static_cast< const Usages* >( this ), motivationString + tools::translate( "Block", "/Nominal capacity" ), static_cast< const unsigned int &>( occupations_[ usage ].first ) );
    dictionary_.Register( *static_cast< const Usages* >( this ), motivationString + tools::translate( "Block", "/Maximal capacity" ), static_cast< const unsigned int &>( occupations_[ usage ].second ) );

    usages_[ defaultStr_ ] -= proportion;
    if( usages_[ defaultStr_ ] )
    {
        occupation = livingSpace_ * usages_[ defaultStr_ ] * 0.01f;
        accommodation = accommodationTypes_.Find( defaultStr_ );
        occupations_[ defaultStr_ ].first = static_cast< unsigned int >( occupation * ( accommodation ? accommodation->GetNominalCapacity() : 1 ) );
        occupations_[ defaultStr_ ].second = static_cast< unsigned int >( occupation * ( accommodation ? accommodation->GetMaxCapacity() : 1 ) );
    }
    else
    {
        static const QString defaultString = tools::translate( "Block", "PhysicalFeatures/Motivations/" ) + tools::translate( "Block", "Default" );
        dictionary_.Remove( defaultString + tools::translate( "Block", "/Percentage" ) );
        dictionary_.Remove( defaultString + tools::translate( "Block", "/Nominal capacity" ) );
        dictionary_.Remove( defaultString + tools::translate( "Block", "/Maximal capacity" ) );
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
