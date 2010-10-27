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
MedicalTreatmentPrototype_ABC::MedicalTreatmentPrototype_ABC( QWidget* parent, const tools::Resolver_ABC< MedicalTreatmentType, std::string >& resolver )
    : ObjectAttributePrototype_ABC( parent, tools::translate( "MedicalTreatmentPrototype_ABC", "MedicalTreatment parameters" ) )
    , resolver_( resolver )
{
    QVBox* vbox = new QVBox( this, tools::translate( "MedicalTreatmentPrototype_ABC", "MedicalTreatment Type:" ) );
    {
        QHBox* canvas = new QHBox( vbox );
        new QLabel( tools::translate( "MedicalTreatmentPrototype_ABC", "Hospital ID:" ), canvas );
        referenceID_ = new QLineEdit( canvas );
    }
    {
        QHBox* canvas = new QHBox( vbox );
        new QLabel( tools::translate( "MedicalTreatmentPrototype_ABC", "Doctors:" ), canvas );
        doctors_ = new QSpinBox( 0, 1000, 1, canvas );
    }
    {
        QVBox* bedCapacities = new QVBox( vbox );
        new QLabel( tools::translate( "MedicalTreatmentPrototype_ABC", "Bed Capacities:" ), bedCapacities );
        FillCapacityTypes( bedCapacities );
    }
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
void MedicalTreatmentPrototype_ABC::FillCapacityTypes( QWidget* parent )
{
    capacities_.clear();
    tools::Iterator< const MedicalTreatmentType& > it( resolver_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const MedicalTreatmentType& element = it.NextElement();
        capacities_.push_back( new Capacity( parent, element.GetName() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype_ABC::showEvent
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void MedicalTreatmentPrototype_ABC::showEvent( QShowEvent* e )
{
    QGroupBox::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype_ABC::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool MedicalTreatmentPrototype_ABC::CheckValidity() const
{
    return referenceID_->text().length() > 0;
}
