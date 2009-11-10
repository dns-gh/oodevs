// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "MedicalTreatmentPrototype_ABC.h"
#include "clients_kernel/MedicalTreatmentType.h"
#include "tools/Iterator.h"
#include "clients_gui/ValuedListItem.h"
#include "Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype_ABC constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
MedicalTreatmentPrototype_ABC::MedicalTreatmentPrototype_ABC( QWidget* parent, const tools::Resolver_ABC< MedicalTreatmentType >& resolver )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "MedicalTreatmentPrototype_ABC", "MedicalTreatment parameters" ) )
    , resolver_( resolver )
{
    new QLabel( tools::translate( "MedicalTreatmentPrototype_ABC", "MedicalTreatment Type:" ), this );
    treatmentTypes_ = new QListView( this );
    treatmentTypes_->setSelectionMode( QListView::Multi );    
    treatmentTypes_->setMinimumHeight( 5 * treatmentTypes_->height() ); // 5 visible lines
    treatmentTypes_->addColumn( tools::translate( "MedicalTreatmentPrototype_ABC", "Type" ) );
    FillTypes();

    new QLabel( tools::translate( "MedicalTreatmentPrototype_ABC", "Beds:" ), this );
    beds_ = new QSpinBox( 0, 1000, 1, this );
    
    new QLabel( tools::translate( "MedicalTreatmentPrototype_ABC", "Initial available beds:" ), this );
    availableBeds_ = new QSpinBox( 0, 1000, 1, this );

    new QLabel( tools::translate( "MedicalTreatmentPrototype_ABC", "Doctors:" ), this );
    doctors_ = new QSpinBox( 0, 1000, 1, this );

    new QLabel( tools::translate( "MedicalTreatmentPrototype_ABC", "Initial available doctors:" ), this );
    availableDoctors_ = new QSpinBox( 0, 1000, 1, this );
}
    
// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype_ABC destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
MedicalTreatmentPrototype_ABC::~MedicalTreatmentPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype_ABC::FillTypes
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void MedicalTreatmentPrototype_ABC::FillTypes()
{
    treatmentTypes_->clear();
    tools::Iterator< const MedicalTreatmentType& > it( resolver_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const MedicalTreatmentType& element = it.NextElement();
        ValuedListItem* item = new ValuedListItem( treatmentTypes_ );
        item->SetNamed( element );
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype_ABC::showEvent
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void MedicalTreatmentPrototype_ABC::showEvent( QShowEvent* e )
{
    FillTypes();
    QGroupBox::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype_ABC::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool MedicalTreatmentPrototype_ABC::CheckValidity() const
{
    return beds_->value() >= availableBeds_->value() &&
           doctors_->value() >= availableDoctors_->value();
}
