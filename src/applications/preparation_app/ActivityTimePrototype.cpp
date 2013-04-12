// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ActivityTimePrototype.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_gui/LoadableTimeEdit.h"
#include "preparation/TimeLimitedAttribute.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
ActivityTimePrototype::ActivityTimePrototype( QWidget* parent, Object_ABC*& creation )
    : ActivityTimePrototype_ABC( parent )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
ActivityTimePrototype::~ActivityTimePrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ActivityTimePrototype::Commit( const kernel::Team_ABC& )
{
    if( creation_ )
    {
        gui::PropertiesDictionary& dictionary = creation_->Get< gui::PropertiesDictionary >();
        TimeLimitedAttribute* attribute = new TimeLimitedAttribute( dictionary, *creation_ );
        QTime t = activityTime_->time();
        attribute->SetActivityTime( 3600 * t.hour() +
                                      60 * t.minute() +
                                           t.second() );
        creation_->Attach( *attribute );
    }
}
