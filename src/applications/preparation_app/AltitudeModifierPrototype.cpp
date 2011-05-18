// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "AltitudeModifierPrototype.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/LoadableSpinBox.h"
#include "preparation/AltitudeModifierAttribute.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AltitudeModifierPrototype constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierPrototype::AltitudeModifierPrototype( QWidget* parent, kernel::Object_ABC*& creation, kernel::Controllers& controllers, kernel::DetectionMap& detection )
    : AltitudeModifierPrototype_ABC( parent )
    , creation_   ( creation )
    , controllers_( controllers )
    , detection_  ( detection )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierPrototype destructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierPrototype::~AltitudeModifierPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierPrototype::Commit
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierPrototype::Commit()
{
    if( creation_ )
    {
        PropertiesDictionary& dico = creation_->Get< PropertiesDictionary >();
        AltitudeModifierAttribute* attribute = new AltitudeModifierAttribute( dico, detection_, *creation_, controllers_ );
        attribute->SetHeight( height_->value() );
        creation_->Attach( *attribute );
    }
}
