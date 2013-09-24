// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ObstaclePrototype.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_gui/LoadableCheckBox.h"
#include "preparation/ObstacleAttribute.h"
#include "preparation/WeatherModel.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype::ObstaclePrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
ObstaclePrototype::ObstaclePrototype( QWidget* parent, Object_ABC*& creation, const WeatherModel& weather )
    : ObstaclePrototype_ABC( parent )
    , creation_( creation )
    , weather_( weather )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype::~ObstaclePrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
ObstaclePrototype::~ObstaclePrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype::GetCreationDate
// Created: JSR 2013-09-17
// -----------------------------------------------------------------------------
QDateTime ObstaclePrototype::GetCreationDate() const
{
    return weather_.GetDate();
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObstaclePrototype::Commit( const kernel::Team_ABC& )
{
    if( creation_ )
    {
        gui::PropertiesDictionary& dictionary = creation_->Get< gui::PropertiesDictionary >();
        ObstacleAttribute* attribute = new ObstacleAttribute( dictionary, *creation_ );
        attribute->SetActivationTime( GetActivationTime() );
        attribute->SetActivityTime( GetActivityTime() );
        attribute->Activate( IsActivated() );
        creation_->Attach( *attribute );
    }
}
