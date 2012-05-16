// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UrbanDisplayOptions.h"
#include "clients_gui/ChangePopulationDisplay.h"
#include "clients_kernel/AccommodationTypes.h"
#include "clients_kernel/AccommodationType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Usages_ABC.h"
#include "clients_kernel/UrbanColor_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UrbanDisplayOptions constructor
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
UrbanDisplayOptions::UrbanDisplayOptions( kernel::Controllers& controllers, const kernel::AccommodationTypes& accommodationTypes )
    : controllers_            ( controllers )
    , accommodationTypes_     ( accommodationTypes )
    , densityColor_           ( false )
    , accommodationColor_     ( false )
    , accommodationDisplayed_ ( "" )
    , unoccupiedDensity_      ( 0, 0, 0 )
    , minDensity_             ( 0.f ) // $$$$ LDC RC FIXME All these are common to all urban blocks and must therefore get out of this class.
    , maxDensity_             ( 1.f )
    , pGradient_              ( new Gradient() )
    , unoccupiedAccommodation_( 0, 0, 0 )
    , minAccommodationDensity_( 0.f )
    , maxAccommodationDensity_( 1.f )
    , pAccommodationGradient_ ( new Gradient() )
{
    pGradient_->AddColor( 0, Qt::green );
    pGradient_->AddColor( 1, Qt::red );
    pAccommodationGradient_->AddColor( 0, Qt::green );
    pAccommodationGradient_->AddColor( 1, Qt::yellow );
    pAccommodationGradient_->AddColor( 2, Qt::red );
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

void UrbanDisplayOptions::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "UrbanDensityColor" )
        densityColor_ = value.To< bool >();
    else if( name == "Density/urbanBlock" )
    {
        pGradient_.reset( new Gradient() );
        pGradient_->LoadValues( value.To< QString >() );
    }
    else if( name == "Density/min" )
        minDensity_ = value.To< float >();
    else if( name == "Density/max" )
        maxDensity_ = value.To< float >();
    else if( name == "Accommodation/urbanBlock" )
    {
        pAccommodationGradient_.reset( new Gradient() );
        pAccommodationGradient_->LoadValues( value.To< QString >() );
    }
    else if( name == "Accommodation/min" )
        minAccommodationDensity_ = value.To< float >();
    else if( name == "Accommodation/max" )
        maxAccommodationDensity_ = value.To< float >();
    else if( name == "UrbanAccommodationColor" )
        accommodationColor_ = value.To< bool >();
    else if( name == "UrbanAccommodationDisplayed" )
        accommodationDisplayed_ = value.To< QString >();
    else if( name == "Accommodation/unoccupied" )
        unoccupiedAccommodation_ = QColor( value.To< QString >() );
    else if( name == "Density/unoccupied" )
        unoccupiedDensity_ = QColor( value.To< QString >() );
    else
        return;
    controllers_.controller_.Update( *this );
}

namespace
{
    void SetUrbanColor( QColor& color, kernel::UrbanColor_ABC& urbanColor )
    {
        urbanColor.SetColor( static_cast< unsigned short >( color.red() ),
                             static_cast< unsigned short >( color.green() ),
                             static_cast< unsigned short >( color.blue() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanDisplayOptions::SetColor
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
bool UrbanDisplayOptions::SetColor( kernel::UrbanColor_ABC& color, float livingSpace, const kernel::T_HumansStrMap& humans, const kernel::Usages_ABC& usages )
{
    if( densityColor_ )
    {
        unsigned int nbrHumans = 0;
        for( kernel::T_HumansStrMap::const_iterator human = humans.begin(); human != humans.end(); ++human )
            for( kernel::CIT_BlockOccupation it = human->second.persons_.begin(); it != human->second.persons_.end(); ++ it )
                nbrHumans += it->second.first;
        if( nbrHumans == 0 )
            SetUrbanColor( unoccupiedDensity_, color );
        else
        {
            float density = nbrHumans / livingSpace;
            QColor result = pGradient_->Compute( density, color.Alpha(), minDensity_, maxDensity_ );
            SetUrbanColor( result, color );
        }
        return true;
    }
    else if( accommodationColor_ )
    {
        unsigned int nbrHumans = 0;
        for( kernel::T_HumansStrMap::const_iterator human = humans.begin(); human != humans.end(); ++human )
        {
            if( populationsDisplayed_.find( human->first ) != populationsDisplayed_.end() )
            {
                kernel::CIT_BlockOccupation occupation = human->second.persons_.find( accommodationDisplayed_ );
                if( occupation != human->second.persons_.end() )
                    nbrHumans += occupation->second.first;
            }
        }
        if( nbrHumans == 0.f )
            SetUrbanColor( unoccupiedAccommodation_, color );
        else
        {
            float proportion = static_cast< float >( usages.Find( accommodationDisplayed_.ascii() ) ) / 100.f;
            const kernel::AccommodationType* accommodationType = accommodationTypes_.Find( accommodationDisplayed_.ascii() );
            float nominalCapacity = accommodationType ? accommodationType->GetNominalCapacity() : 0.f;
            float density;
            if( livingSpace == 0.f || proportion == 0.f || nominalCapacity == 0.f )
                density = std::numeric_limits< float >::infinity();
            else
                density = nbrHumans / ( livingSpace * proportion * nominalCapacity );
            QColor result = pAccommodationGradient_->Compute( density, color.Alpha(), minAccommodationDensity_, maxAccommodationDensity_ );
            SetUrbanColor( result, color );
        }
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: UrbanDisplayOptions::NotifyUpdated
// Created: LDC 2011-03-28
// -----------------------------------------------------------------------------
void UrbanDisplayOptions::NotifyUpdated( const gui::ChangePopulationDisplay& population )
{
    if( population.visible_ )
        populationsDisplayed_.insert( population.populationName_ );
    else
        populationsDisplayed_.erase( population.populationName_ );
    controllers_.controller_.Update( *this );
}
