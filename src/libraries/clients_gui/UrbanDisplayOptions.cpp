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
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include <urban/ColorAttribute.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UrbanDisplayOptions constructor
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
UrbanDisplayOptions::UrbanDisplayOptions( kernel::Controllers& controllers ) 
    : controllers_( controllers )
    , densityColor_( false )
    , accommodationColor_( false )
    , accommodationDisplayed_( "" )
    , unoccupiedDensity_( 0, 0, 0 )
    , minDensity_  ( 0.f ) // $$$$ LDC RC FIXME All these are common to all urban blocks and must therefore get out of this class.
    , maxDensity_  ( 1.f )
    , pGradient_   ( new Gradient() )
    , unoccupiedAccommodation_( 0, 0, 0 )
    , minAccommodationDensity_  ( 0.f )
    , maxAccommodationDensity_  ( 1.f )
    , pAccommodationGradient_   ( new Gradient() )
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
    else return;
    controllers_.controller_.Update( *this );
}

namespace
{
    void SetUrbanColor( QColor& color, urban::ColorAttribute& colorAttribute )
    {
        colorAttribute.SetRed( static_cast< unsigned short >( color.red() ) );
        colorAttribute.SetGreen( static_cast< unsigned short >( color.green() ) );
        colorAttribute.SetBlue( static_cast< unsigned short >( color.blue() ) );
    }
}
// -----------------------------------------------------------------------------
// Name: UrbanDisplayOptions::SetColor
// Created: LDC 2011-03-25
// -----------------------------------------------------------------------------
bool UrbanDisplayOptions::SetColor( urban::ColorAttribute* attribute, float density, const std::map< std::string, float >& motivations )
{    
    if( attribute )
    {
        if( densityColor_ )
        {
            if( density == 0.f )
                SetUrbanColor( unoccupiedDensity_, *attribute );
            else
            {
                QColor result = pGradient_->Compute( density, attribute->Alpha(), minDensity_, maxDensity_ );
                SetUrbanColor( result, *attribute );
            }
            return true;
        }
        else if( accommodationColor_ )
        {
            // $$$$ FIXME Compute density according to motivations
            std::map< std::string, float >::const_iterator it = motivations.find( accommodationDisplayed_.ascii() );
            float proportion = ( it == motivations.end() ) ? 0.f : it->second;
            if( density == 0.f )
                SetUrbanColor( unoccupiedAccommodation_, *attribute );
            else
            {
                if( proportion == 0.f )
                    density = std::numeric_limits< float >::infinity();
                else
                    density /= proportion;
                QColor result = pAccommodationGradient_->Compute( density, attribute->Alpha(), minAccommodationDensity_, maxAccommodationDensity_ );
                SetUrbanColor( result, *attribute );
            }
            return true;
        }        
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: UrbanDisplayOptions::NotifyUpdated
// Created: LDC 2011-03-28
// -----------------------------------------------------------------------------
void UrbanDisplayOptions::NotifyUpdated( const gui::ChangePopulationDisplay& population )
{
    // $$$$ TODO population.populationName_ and visible_
}
