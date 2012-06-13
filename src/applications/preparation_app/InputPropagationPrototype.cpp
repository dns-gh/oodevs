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
        PropertiesDictionary& dico = creation_->Get< PropertiesDictionary >();
        InputToxicCloudAttribute* attribute = new InputToxicCloudAttribute( dico );
        attribute->SetSource( propagationFiles_->currentText().ascii(), dataField_->GetValue() );
        attribute->SetExportData( exportData_->isChecked() );
        creation_->Attach( *attribute );
    }
}
