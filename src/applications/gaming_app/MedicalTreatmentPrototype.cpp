// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MedicalTreatmentPrototype.h"
#include "clients_kernel/MedicalTreatmentType.h"
#include "clients_gui/LoadableLineEdit.h"
#include "protocol/Protocol.h"
#include "tools/Iterator.h"
#include "actions/ParameterList.h"

using namespace sword;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
MedicalTreatmentPrototype::MedicalTreatmentPrototype( QWidget* parent, const tools::Resolver_ABC< MedicalTreatmentType, std::string >& resolver, actions::parameters::ParameterList*& attributesList )
    : MedicalTreatmentPrototype_ABC( parent, resolver )
    , attributesList_ ( attributesList )
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
    actions::parameters::ParameterList& list = attributesList_->AddList( "MedicalTreatment" );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_medical_treatment );
    list.AddString( "ExternalReferenceId", referenceID_->text().ascii() );
    list.AddQuantity( "Doctors", doctors_->value() );
    list.AddIdentifier( "Status", 0 ); // JCR TODO : Check status available ?

    int i = 0;
    actions::parameters::ParameterList& capacities = list.AddList( "BedCapacities" );
    for( CIT_Capacities it = capacities_.begin(); it != capacities_.end(); ++it, ++i )
    {
        const int value = it->baseline_->value();
        actions::parameters::ParameterList& bed = capacities.AddList( "BedCapacity" );

        bed.AddIdentifier( "TypeId", i );
        bed.AddQuantity( "BaseLineCount", value );
        bed.AddQuantity( "AvailableCount", value );
        bed.AddQuantity( "EmergencyCount", 0 );
    }
}
