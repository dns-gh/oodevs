// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AltitudeModifierPrototype.h"
#include "actions/ParameterList.h"
#include "protocol/Protocol.h"
#include "clients_gui/LoadableSpinBox.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: AltitudeModifierPrototype constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierPrototype::AltitudeModifierPrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList )
    : AltitudeModifierPrototype_ABC( parent )
    , attributesList_( attributesList )
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
    actions::parameters::ParameterList& list = attributesList_->AddList( "AltitudeModifier" );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction::altitude_modifier );
    list.AddQuantity( "Height", height_->value() );
}
