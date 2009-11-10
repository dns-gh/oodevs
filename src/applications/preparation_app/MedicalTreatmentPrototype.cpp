// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "MedicalTreatmentPrototype.h"
#include "clients_kernel/MedicalTreatmentType.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/ValuedListItem.h"
#include "preparation/MedicalTreatmentAttribute.h"
#include "preparation/ObjectAttributesContainer.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
MedicalTreatmentPrototype::MedicalTreatmentPrototype( QWidget* parent, const tools::Resolver_ABC< MedicalTreatmentType >& resolver, Object_ABC*& creation )
    : MedicalTreatmentPrototype_ABC( parent, resolver )
    , creation_( creation )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
MedicalTreatmentPrototype::~MedicalTreatmentPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void MedicalTreatmentPrototype::Commit()
{
    if( creation_ )
    {
        PropertiesDictionary& dico = creation_->Get< PropertiesDictionary >();
        MedicalTreatmentAttribute* attribute = new MedicalTreatmentAttribute( dico );
        for( QListViewItem* item = treatmentTypes_->firstChild(); item != 0; item = item->nextSibling() )
            if( item->isSelected() )
                attribute->AddMedicalTreatment( *static_cast< gui::ValuedListItem* >( item )->GetValue< const MedicalTreatmentType >() );        
        creation_->Get< ObjectAttributesContainer >().Register( *attribute );
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void MedicalTreatmentPrototype::Clean()
{
    // NOTHING
}
