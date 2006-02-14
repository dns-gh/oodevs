// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ComponentType.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ComponentType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ComponentType::ComponentType( xml::xistream& xis )
{
    int id;
    xis >> attribute( "nom", name_ )
            >> content( "MosID", id );
    id_ = id;

    xis >> optional() 
        >> start( "FonctionsLogistiques" );
    hasMaintenance_ = xis.exists( "Maintenance" );
    hasMedical_     = xis.exists( "Sante" );
    hasSupply_      = xis.exists( "Ravitaillement" );
}

// -----------------------------------------------------------------------------
// Name: ComponentType destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ComponentType::~ComponentType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ComponentType::GetName
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
const std::string& ComponentType::GetName() const
{
    return name_;
}
