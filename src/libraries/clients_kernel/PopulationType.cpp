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
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: PopulationType constructor
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
PopulationType::PopulationType( xml::xistream& xis, const Resolver_ABC< DecisionalModel, QString >& modelResolver )
{
    std::string model, name;
    int id;
    xis >> attribute( "nom", name )
            >> content( "MosID", id )
            >> content( "ModeleDecisionnel", model );
    id_ = id; model_ = & modelResolver.Get( model.c_str() );
    name_ = name.c_str();
    
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
QString PopulationType::GetName() const
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
