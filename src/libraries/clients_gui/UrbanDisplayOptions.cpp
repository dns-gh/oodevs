// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UrbanDisplayOptions.h"

#include "clients_kernel/AccommodationTypes.h"
#include "clients_kernel/AccommodationType.h"
#include "clients_kernel/ChangePopulationDisplay.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/PhysicalAttribute_ABC.h"
#include "clients_kernel/StructuralStateAttribute_ABC.h"
#include "clients_kernel/Usages_ABC.h"
#include "clients_kernel/UrbanColor_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UrbanDisplayOptions constructor
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
UrbanDisplayOptions::UrbanDisplayOptions( kernel::Controllers& controllers,
                                          const kernel::AccommodationTypes& accommodationTypes )
    : controllers_            ( controllers )
    , accommodationTypes_     ( accommodationTypes )
    , densityEnabled_         ( false )
    , accommodationEnabled_   ( false )
    , accommodationDisplayed_ ( "" )
    , unoccupiedDensity_      ( 0, 0, 0, 180 )
    , minDensity_             ( 0.f ) // $$$$ LDC RC FIXME All these are common to all urban blocks and must therefore get out of this class.
    , maxDensity_             ( 1.f )
    , densityGradient_        ( std::make_shared< Gradient >( "Gradient" ) )
    , unoccupiedAccommodation_( 0, 0, 0, 180 )
    , minAccommodation_       ( 0.f )
    , maxAccommodation_       ( 1.f )
    , accommodationGradient_  ( std::make_shared< Gradient >( "Graient" ) )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanDisplayOptions destructor
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
UrbanDisplayOptions::~UrbanDisplayOptions()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanDisplayOptions::Load
// Created: ABR 2014-08-05
// -----------------------------------------------------------------------------
void UrbanDisplayOptions::Load( const kernel::Options& options )
{
    densityEnabled_ = options.Get( "Density/Enabled" ).To< bool >();
    maxDensity_ = options.Get( "Density/Max" ).To< float >();
    minDensity_ = options.Get( "Density/Min" ).To< float >();
    unoccupiedDensity_ = QColor( options.Get( "Density/Unoccupied" ).To< QString >() );
    densityGradient_->LoadValues( options.Get( "Density/Gradient" ).To< QString >() );

    accommodationEnabled_ = options.Get( "Accommodation/Enabled" ).To< bool >();
    accommodationDisplayed_ = options.Get( "Accommodation/Displayed" ).To< QString >();
    maxAccommodation_ = options.Get( "Accommodation/Max" ).To< float >();
    minAccommodation_ = options.Get( "Accommodation/Min" ).To< float >();
    unoccupiedAccommodation_ = QColor( options.Get( "Accommodation/Unoccupied" ).To< QString >() );
    accommodationGradient_->LoadValues( options.Get( "Accommodation/Gradient" ).To< QString >() );
}

// -----------------------------------------------------------------------------
// Name: UrbanDisplayOptions::NotifyUpdated
// Created: LDC 2011-03-28
// -----------------------------------------------------------------------------
void UrbanDisplayOptions::NotifyUpdated( const kernel::ChangePopulationDisplay& population )
{
    if( population.visible_ )
        populationsDisplayed_.insert( population.populationName_ );
    else
        populationsDisplayed_.erase( population.populationName_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanDisplayOptions::ComputeUrbanColor
// Created: ABR 2014-07-08
// -----------------------------------------------------------------------------
QColor UrbanDisplayOptions::ComputeUrbanColor( const kernel::UrbanObject_ABC& object ) const
{
    const auto& color = object.Get< kernel::UrbanColor_ABC >();
    const float livingSpace = object.GetLivingSpace();
    const auto& humans = object.GetHumansMap();
    const auto& usages = object.Get< kernel::PhysicalAttribute_ABC >().GetUsages();
    const auto structuralStateAttribute = object.Retrieve< kernel::StructuralStateAttribute_ABC >();
    const auto structuralState = structuralStateAttribute ? structuralStateAttribute->GetValue() : 100u;
    if( densityEnabled_ )
    {
        unsigned int nbrHumans = 0;
        for( auto human = humans.begin(); human != humans.end(); ++human )
            for( auto it = human->second.persons_.begin(); it != human->second.persons_.end(); ++ it )
                nbrHumans += it->second.first;
        if( nbrHumans == 0 || structuralState == 0 )
            return unoccupiedDensity_;
        return densityGradient_->Compute( nbrHumans / ( livingSpace * ( structuralState / 100u ) ),
                                    color.Alpha(),
                                    minDensity_,
                                    maxDensity_ );
    }
    else if( accommodationEnabled_ )
    {
        unsigned int nbrHumans = 0;
        for( auto human = humans.begin(); human != humans.end(); ++human )
            if( populationsDisplayed_.find( human->first ) != populationsDisplayed_.end() )
            {
                auto occupation = human->second.persons_.find( accommodationDisplayed_ );
                if( occupation != human->second.persons_.end() )
                    nbrHumans += occupation->second.first;
            }
        if( nbrHumans == 0.f  || structuralState == 0 )
            return unoccupiedAccommodation_;
        const float proportion = static_cast< float >( usages.Find( accommodationDisplayed_.toAscii().constData() ) ) / 100.f;
        const auto* accommodationType = accommodationTypes_.Find( accommodationDisplayed_.toAscii().constData() );
        const float nominalCapacity = accommodationType ? accommodationType->GetNominalCapacity() : 0.f;
        const float density = livingSpace == 0.f || proportion == 0.f || nominalCapacity == 0.f
            ? std::numeric_limits< float >::infinity()
            : nbrHumans / ( livingSpace * proportion * nominalCapacity * ( structuralState / 100u ) );
        return accommodationGradient_->Compute( density,
                                                 color.Alpha(),
                                                 minAccommodation_,
                                                 maxAccommodation_ );
    }
    return QColor( color.Red(), color.Green(), color.Blue(), static_cast< int >( color.Alpha() * 255.f ) );
}

// -----------------------------------------------------------------------------
// Name: std::shared_ptr< Gradient >& UrbanDisplayOptions::GetDensityGradient
// Created: ABR 2014-10-20
// -----------------------------------------------------------------------------
const std::shared_ptr< Gradient >& UrbanDisplayOptions::GetDensityGradient() const
{
    return densityGradient_;
}

// -----------------------------------------------------------------------------
// Name: std::shared_ptr< Gradient >& UrbanDisplayOptions::GetAccommodationGradient
// Created: ABR 2014-10-20
// -----------------------------------------------------------------------------
const std::shared_ptr< Gradient >& UrbanDisplayOptions::GetAccommodationGradient() const
{
    return accommodationGradient_;
}
