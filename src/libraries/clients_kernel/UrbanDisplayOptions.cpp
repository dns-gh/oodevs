// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "UrbanDisplayOptions.h"
#include "AccommodationTypes.h"
#include "AccommodationType.h"
#include "ChangePopulationDisplay.h"
#include "Controller.h"
#include "Controllers.h"
#include "OptionVariant.h"
#include "Usages_ABC.h"
#include "UrbanColor_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanDisplayOptions constructor
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
UrbanDisplayOptions::UrbanDisplayOptions( Controllers& controllers, const AccommodationTypes& accommodationTypes )
    : controllers_            ( controllers )
    , accommodationTypes_     ( accommodationTypes )
    , densityColor_           ( false )
    , accommodationColor_     ( false )
    , accommodationDisplayed_ ( "" )
    , unoccupiedDensity_      ( 0, 0, 0, 180 )
    , minDensity_             ( 0.f ) // $$$$ LDC RC FIXME All these are common to all urban blocks and must therefore get out of this class.
    , maxDensity_             ( 1.f )
    , pGradient_              ( new Gradient() )
    , unoccupiedAccommodation_( 0, 0, 0, 180 )
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

void UrbanDisplayOptions::OptionChanged( const std::string& name, const OptionVariant& value )
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
    void SetUrbanColor( QColor& color, UrbanColor_ABC& urbanColor )
    {
        urbanColor.SetColor( color.red(), color.green(), color.blue(), color.alpha() );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanDisplayOptions::SetColor
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
bool UrbanDisplayOptions::SetColor( UrbanColor_ABC& color, float livingSpace, const T_HumansStrMap& humans, const Usages_ABC& usages, unsigned int structuralState )
{
    if( densityColor_ )
    {
        unsigned int nbrHumans = 0;
        for( T_HumansStrMap::const_iterator human = humans.begin(); human != humans.end(); ++human )
            for( CIT_BlockOccupation it = human->second.persons_.begin(); it != human->second.persons_.end(); ++ it )
                nbrHumans += it->second.first;
        if( nbrHumans == 0 || structuralState == 0 )
            SetUrbanColor( unoccupiedDensity_, color );
        else
        {
            float density = nbrHumans / ( livingSpace * ( structuralState / 100u ) ) ;
            QColor result = pGradient_->Compute( density, color.Alpha(), minDensity_, maxDensity_ );
            SetUrbanColor( result, color );
        }
        return true;
    }
    else if( accommodationColor_ )
    {
        unsigned int nbrHumans = 0;
        for( T_HumansStrMap::const_iterator human = humans.begin(); human != humans.end(); ++human )
        {
            if( populationsDisplayed_.find( human->first ) != populationsDisplayed_.end() )
            {
                CIT_BlockOccupation occupation = human->second.persons_.find( accommodationDisplayed_ );
                if( occupation != human->second.persons_.end() )
                    nbrHumans += occupation->second.first;
            }
        }
        if( nbrHumans == 0.f || structuralState == 0 )
            SetUrbanColor( unoccupiedAccommodation_, color );
        else
        {
            float proportion = static_cast< float >( usages.Find( accommodationDisplayed_.ascii() ) ) / 100.f;
            const AccommodationType* accommodationType = accommodationTypes_.Find( accommodationDisplayed_.ascii() );
            float nominalCapacity = accommodationType ? accommodationType->GetNominalCapacity() : 0.f;
            float density;
            if( livingSpace == 0.f || proportion == 0.f || nominalCapacity == 0.f )
                density = std::numeric_limits< float >::infinity();
            else
                density = nbrHumans / ( livingSpace * proportion * nominalCapacity * ( structuralState / 100u ) );
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
void UrbanDisplayOptions::NotifyUpdated( const kernel::ChangePopulationDisplay& population )
{
    if( population.visible_ )
        populationsDisplayed_.insert( population.populationName_ );
    else
        populationsDisplayed_.erase( population.populationName_ );
    controllers_.controller_.Update( *this );
}
