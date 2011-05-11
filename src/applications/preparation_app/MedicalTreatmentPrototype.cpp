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
#include "clients_gui/ValuedListItem.h"
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
void MedicalTreatmentPrototype::Commit()
{
    if( creation_ )
    {
        PropertiesDictionary& dico = creation_->Get< PropertiesDictionary >();
        MedicalTreatmentAttribute* attribute = new MedicalTreatmentAttribute( resolver_, dico );
        attribute->SetDoctors( doctors_->value() );
        attribute->SetReferenceID( std::string( referenceID_->text().ascii() ) );
        for( CIT_Capacities it = capacities_.begin(); it != capacities_.end(); ++it )
        {
            int value = it->baseline_->value();
            attribute->UpdateTreatmentCapacity( std::string( it->name_ ), value );
        }
        creation_->Attach( *attribute );
    }
}

/*

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype::CanImport
// Created: AME 2010-06-28
// -----------------------------------------------------------------------------
bool MedicalTreatmentPrototype::CanLoad()
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype::Import
// Created: AME 2010-06-29
// -----------------------------------------------------------------------------
void MedicalTreatmentPrototype::CommitShapeObject( const kernel::ShapeObject& importObject )
{
    if( creation_ )
    {
        PropertiesDictionary& dico = creation_->Get< PropertiesDictionary >();
        MedicalTreatmentAttribute* attribute = new MedicalTreatmentAttribute( resolver_ );
        attribute->SetDoctors( 0 );
        attribute->SetReferenceID( importObject.GetAttributeValue( "reference" ) ) ;
        for( CIT_Capacities itcap = capacities_.begin(); itcap != capacities_.end(); ++itcap )
          attribute->UpdateTreatmentCapacity( std::string( itcap->name_ ), 0 );

        attribute->CreateDictionary( dico );
        creation_->Attach( *attribute );
    }
}

// -----------------------------------------------------------------------------
// Name: kernel::ShapeObjectLoader_ABC* MedicalTreatmentPrototype::ObjectAttributePrototypeContainer::LoadObjects
// Created: AME 2010-06-29
// -----------------------------------------------------------------------------
kernel::ShapeObjectLoader_ABC* MedicalTreatmentPrototype::LoadObjects( const std::string& filename, const kernel::CoordinateConverter_ABC& coordinatesConverter )
{
    return new MedicalShapeLoader( filename, coordinatesConverter );
}
*/