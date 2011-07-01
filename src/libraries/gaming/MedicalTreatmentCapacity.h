// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MedicalTreatmentCapacity_h_
#define __MedicalTreatmentCapacity_h_

namespace kernel
{
    class MedicalTreatmentType;
}

// =============================================================================
/** @class  MedicalTreatmentCapacity
    @brief  MedicalTreatmentCapacity
*/
// Created: JCR 2010-06-28
// =============================================================================
struct MedicalTreatmentCapacity
{
    MedicalTreatmentCapacity() : baseline_ ( 0 ), available_ ( 0 ), emergency_ ( 0 ), type_ ( 0 ) {}

    unsigned baseline_;
    unsigned available_; // TODO UA /UR
    unsigned emergency_;
    std::string name_;
    const kernel::MedicalTreatmentType* type_;
};

#endif // __MedicalTreatmentCapacity_h_
