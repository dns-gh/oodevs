// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "edxlhave_app_pch.h"
#include "MedicalTreatmentType.h"
#include <xeumeuleu/xml.hpp>

using namespace edxl;


// -----------------------------------------------------------------------------
// Name: MedicalTreatmentType constructor
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
MedicalTreatmentType::MedicalTreatmentType( xml::xistream& xis )
{
    xis >> xml::attribute( "id", id_ )
        >> xml::attribute( "name", name_ );
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentType destructor
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
MedicalTreatmentType::~MedicalTreatmentType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentType::GetId
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
unsigned int MedicalTreatmentType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: MedicalTreatmentType::GetName
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
const std::string& MedicalTreatmentType::GetName() const
{
    return name_;
}
