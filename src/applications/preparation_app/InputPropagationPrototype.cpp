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
#include "preparation/ObjectAttributesContainer.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: InputPropagationPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
InputPropagationPrototype::InputPropagationPrototype( QWidget* parent, kernel::Object_ABC*& creation )
    : InputPropagationPrototype_ABC( parent )
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
void InputPropagationPrototype::Commit()
{
    if( creation_ )
    {
        PropertiesDictionary& dico = creation_->Get< PropertiesDictionary >();
        InputToxicCloudAttribute* attribute = new InputToxicCloudAttribute( dico );                                  
                                  attribute->SetSource( source_, dataField_->GetValue() );
                                  attribute->SetExportData( exportData_->isChecked() );
        creation_->Get< ObjectAttributesContainer >().Register( *attribute );
    }
}

// -----------------------------------------------------------------------------
// Name: InputPropagationPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void InputPropagationPrototype::Clean()
{
    // NOTHING
}
