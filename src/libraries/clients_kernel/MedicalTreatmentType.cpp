// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "MedicalTreatmentType.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentType constructor
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
MedicalTreatmentType::MedicalTreatmentType( xistream& input )
{
    input >> attribute( "name", name_ )
          >> attribute( "id", id_ );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentType destructor
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
MedicalTreatmentType::~MedicalTreatmentType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentType::GetId
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
unsigned long MedicalTreatmentType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentType::GetName
// Created: RFT 2006-04-04
// -----------------------------------------------------------------------------
std::string MedicalTreatmentType::GetName() const
{
    return name_;
}
