// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "PopulationType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationType constructor
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
PopulationType::PopulationType( xml::xistream& xis, const tools::Resolver_ABC< DecisionalModel, std::string >& modelResolver )
{
    std::string model;
    xis >> xml::attribute( "name", name_ )
        >> xml::attribute( "id", id_ )
        >> xml::attribute( "decisional-model", model )
        >> xml::attribute( "concentration-density", density_ );
    model_ = & modelResolver.Get( model );
}

// -----------------------------------------------------------------------------
// Name: PopulationType destructor
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
PopulationType::~PopulationType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationType::GetName
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
std::string PopulationType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: PopulationType::GetId
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
unsigned long PopulationType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: PopulationType::GetDensity
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
float PopulationType::GetDensity() const
{
    return density_;
}

// -----------------------------------------------------------------------------
// Name: PopulationType::GetDecisionalModel
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
const DecisionalModel& PopulationType::GetDecisionalModel() const
{
    return *model_;
}
