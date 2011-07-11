// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UndergroundPrototype.h"
#include "actions/ParameterList.h"
#include "clients_gui/LoadableComboBox.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: UndergroundPrototype constructor
// Created: JSR 2011-07-08
// -----------------------------------------------------------------------------
UndergroundPrototype::UndergroundPrototype( QWidget* parent, kernel::Controller& controller, actions::parameters::ParameterList*& attributesList )
    : UndergroundPrototype_ABC( parent, controller )
    , attributesList_( attributesList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundPrototype destructor
// Created: JSR 2011-07-08
// -----------------------------------------------------------------------------
UndergroundPrototype::~UndergroundPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundPrototype::Commit
// Created: JSR 2011-07-08
// -----------------------------------------------------------------------------
void UndergroundPrototype::Commit()
{
    if( CheckValidity() )
    {
        actions::parameters::ParameterList& list = attributesList_->AddList( "Underground" );
        list.AddIdentifier( "AttributeId", sword::ObjectMagicAction::underground );
        list.AddString( "Network", network_->currentText().ascii() );
    }
}
