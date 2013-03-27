// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "InputPropagationPrototype.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "preparation/InputToxicCloudAttribute.h"
#include "clients_gui/RichCheckBox.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: InputPropagationPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
InputPropagationPrototype::InputPropagationPrototype( QWidget* parent, const tools::GeneralConfig& config, kernel::Object_ABC*& creation )
    : InputPropagationPrototype_ABC( parent, config )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InputPropagationPrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
InputPropagationPrototype::~InputPropagationPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InputPropagationPrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void InputPropagationPrototype::Commit( const kernel::Team_ABC& )
{
    if( creation_ )
    {
        PropertiesDictionary& dictionary = creation_->Get< PropertiesDictionary >();
        creation_->Attach( *new InputToxicCloudAttribute( dictionary, propagationFiles_->currentText(),
                            dataField_->GetValue().c_str(), exportData_->isChecked(), *creation_ ) );
    }
}
