// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationType.h"
#include "xeumeuleu/xml.h"
using namespace xml;

// -----------------------------------------------------------------------------
// Name: PopulationType constructor
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
PopulationType::PopulationType( xml::xistream& xis, const Resolver_ABC< DecisionalModel, std::string >& modelResolver )
{
    std::string model;
    int id;
    xis >> attribute( "nom", name_ )
            >> content( "MosID", id )
            >> content( "ModeleDecisionnel", model );
    id_ = id; model_ = & modelResolver.Get( model );
    
    xis >> content( "DensiteConcentration", density_ );
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
MT_Float PopulationType::GetDensity() const
{
    return density_;
}
