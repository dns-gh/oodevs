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
#include "actions/MagicAction.h"
#include "actions/Numeric.h"
#include "actions/Direction.h"
#include "actions/Enumeration.h"

// -----------------------------------------------------------------------------
// Name: WeatherWidget constructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
WeatherWidget::WeatherWidget( QWidget* parent, const QString& title )
    : gui::WeatherWidget( parent, title )
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
void WeatherWidget::CreateParameters( actions::MagicAction& action, tools::Iterator< const kernel::OrderParameter& >& it )
{
    action.AddParameter( *new actions::parameters::Numeric( it.NextElement(), static_cast< float >( temperature_->value() ) ) );
    action.AddParameter( *new actions::parameters::Numeric( it.NextElement(), static_cast< float >( windSpeed_->value() ) ) );
    action.AddParameter( *new actions::parameters::Direction( it.NextElement(), windDirection_->value() ) );
    action.AddParameter( *new actions::parameters::Numeric( it.NextElement(), static_cast< float >( cloudFloor_->value() ) ) );
    action.AddParameter( *new actions::parameters::Numeric( it.NextElement(), static_cast< float >( cloudCeiling_->value() ) ) );
    action.AddParameter( *new actions::parameters::Numeric( it.NextElement(), static_cast< float >( cloudDensity_->value() ) ) );
    action.AddParameter( *new actions::parameters::Enumeration( it.NextElement(), type_->GetValue() ) );
}
