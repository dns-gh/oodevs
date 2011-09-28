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

const std::string Usages::defaultStr = "default";

// -----------------------------------------------------------------------------
// Name: Usages constructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
Usages::Usages( kernel::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes, float livingSpace )
    : dictionary_        ( dictionary )
    , accommodationTypes_( accommodationTypes)
    , livingSpace_       ( livingSpace )
{
    usages_[ defaultStr ] = 100;
    CIT_Usages it = usages_.find( defaultStr );
    static const QString defaultString = tools::translate( "Block", "PhysicalFeatures/Motivations/" ) + tools::translate( "Block", "Default" );
    dictionary_.Register( *this, defaultString + tools::translate( "Block", "/Percentage" ), it->second );
    kernel::AccommodationType* accommodation = accommodationTypes_.Find( defaultStr );
    occupations[ defaultStr ].first = static_cast< unsigned int >( livingSpace_ * ( accommodation ? accommodation->GetNominalCapacity() : 1 ) );
    occupations[ defaultStr ].second = static_cast< unsigned int >( livingSpace_ * ( accommodation ? accommodation->GetMaxCapacity() : 1 ) );
    dictionary_.Register( *this, defaultString + tools::translate( "Block", "/Nominal capacity" ), occupations[ defaultStr ].first );
    dictionary_.Register( *this, defaultString + tools::translate( "Block", "/Maximal capacity" ), occupations[ defaultStr ].second );
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
    dictionary_.Register( *this, motivationString + tools::translate( "Block", "/Percentage" ), usages_[ usage ] );

    float occupation = livingSpace_ * proportion * 0.01f;
    kernel::AccommodationType* accommodation = accommodationTypes_.Find( usage );
    occupations[ usage ].first = static_cast< unsigned int >( occupation * ( accommodation ? accommodation->GetNominalCapacity() : 1 ) );
    occupations[ usage ].second = static_cast< unsigned int >( occupation * ( accommodation ? accommodation->GetMaxCapacity() : 1 ) );
    dictionary_.Register( *this, motivationString + tools::translate( "Block", "/Nominal capacity" ), occupations[ usage ].first );
    dictionary_.Register( *this, motivationString + tools::translate( "Block", "/Maximal capacity" ), occupations[ usage ].second );

    usages_[ defaultStr ] -= proportion;
    if( usages_[ defaultStr ] )
    {
        occupation = livingSpace_ * usages_[ defaultStr ] * 0.01f;
        accommodation = accommodationTypes_.Find( defaultStr );
        occupations[ defaultStr ].first = static_cast< unsigned int >( occupation * ( accommodation ? accommodation->GetNominalCapacity() : 1 ) );
        occupations[ defaultStr ].second = static_cast< unsigned int >( occupation * ( accommodation ? accommodation->GetMaxCapacity() : 1 ) );
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
