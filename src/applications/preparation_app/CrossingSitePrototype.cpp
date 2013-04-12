// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "CrossingSitePrototype.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_gui/LoadableSpinBox.h"
#include "clients_gui/LoadableCheckBox.h"
#include "preparation/CrossingSiteAttribute.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype constructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype::CrossingSitePrototype( QWidget* parent, Object_ABC*& creation )
    : CrossingSitePrototype_ABC( parent )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype destructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype::~CrossingSitePrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CrossingSitePrototype::Commit( const kernel::Team_ABC& )
{
    if( creation_ )
    {
        gui::PropertiesDictionary& dictionary = creation_->Get< gui::PropertiesDictionary >();
        CrossingSiteAttribute* attribute = new CrossingSiteAttribute( dictionary, *creation_ );
        attribute->SetWidth( width_->value() );
        attribute->SetDepth( depth_->value() );
        attribute->SetFlowSpeed( speed_->value() );
        attribute->SetConstruction( needsConstruction_->isChecked() );
        creation_->Attach( *attribute );
    }
}
