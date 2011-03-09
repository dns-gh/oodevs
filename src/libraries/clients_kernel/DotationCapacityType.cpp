// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "DotationCapacityType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DotationCapacityType constructor
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
DotationCapacityType::DotationCapacityType( xml::xistream& xis )
{
    xis >> xml::attribute( "name", name_ )
        >> xml::attribute( "capacity", capacity_ )
        >> xml::attribute( "logistic-threshold", logisticThreshold_ )
        >> xml::attribute( "normalized-consumption", normalizedConsumption_ );
}

// -----------------------------------------------------------------------------
// Name: DotationCapacityType destructor
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
DotationCapacityType::~DotationCapacityType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationCapacityType::GetName
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
const std::string& DotationCapacityType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: DotationCapacityType::GetCapacity
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
unsigned int DotationCapacityType::GetCapacity() const
{
    return capacity_;
}

// -----------------------------------------------------------------------------
// Name: DotationCapacityType::GetLogisticThreshold
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
double DotationCapacityType::GetLogisticThreshold() const
{
    return logisticThreshold_;
}

// -----------------------------------------------------------------------------
// Name: DotationCapacityType::GetNormalizedConsumption
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
double DotationCapacityType::GetNormalizedConsumption() const
{
    return normalizedConsumption_;
}
