// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "WeatherWidget.h"
#include "clients_gui/WeatherHelpers.h"

// -----------------------------------------------------------------------------
// Name: WeatherWidget constructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
WeatherWidget::WeatherWidget( const QString& objectName, QWidget* parent, const QString& title )
    : gui::WeatherWidget( objectName, parent, title )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget destructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
WeatherWidget::~WeatherWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget::CreateParameters
// Created: JSR 2010-04-23
// -----------------------------------------------------------------------------
gui::WeatherParameters WeatherWidget::CreateParameters()
{
    gui::WeatherParameters result = 
           { type_->GetValue(),
             static_cast< float >( temperature_->value() ),
             static_cast< float >( windSpeed_->value() ),
             windDirection_->value(),
             static_cast< float >( cloudFloor_->value() ),
             static_cast< float >( cloudCeiling_->value() ),
             static_cast< float >( cloudDensity_->value() ) };
    return result;
}
