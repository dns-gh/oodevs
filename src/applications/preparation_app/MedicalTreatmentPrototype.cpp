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
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Team_ABC.h"
#include "preparation/TeamsModel.h"
#include "clients_gui/LoadableLineEdit.h"
#include "preparation/MedicalTreatmentAttribute.h"
// #include "MedicalShapeLoader.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
MedicalTreatmentPrototype::MedicalTreatmentPrototype( QWidget* parent, const tools::Resolver_ABC< MedicalTreatmentType, std::string >& resolver, Object_ABC*& creation )
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
void MedicalTreatmentPrototype::Commit( const kernel::Team_ABC& )
{
    if( creation_ )
    {
        PropertiesDictionary& dictionary = creation_->Get< PropertiesDictionary >();
        MedicalTreatmentAttribute* attribute = new MedicalTreatmentAttribute( resolver_, dictionary );
        attribute->SetDoctors( doctors_->value() );
        attribute->SetReferenceID( std::string( referenceID_->text().toStdString() ) );
        for( auto it = capacities_.begin(); it != capacities_.end(); ++it )
        {
            int value = it->baseline_->value();
            attribute->UpdateTreatmentCapacity( std::string( it->name_ ), value );
        }
        creation_->Attach( *attribute );
    }
}
