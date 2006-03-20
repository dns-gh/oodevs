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
        >> start( "FonctionsLogistiques" )
            >> optional()
                >> list( "Maintenance", *this, ReadPresence, hasMaintenance_ )
            >> optional()
                >> list( "Sante", *this, ReadPresence, hasMedical_ )
            >> optional()
                >> list( "Ravitaillement", *this, ReadPresence, hasSupply_ );

//    hasMaintenance_ = xis.exists( "Maintenance" );
//    hasMedical_     = xis.exists( "Sante" );
//    hasSupply_      = xis.exists( "Ravitaillement" );
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
// Name: ComponentType::ReadPresence
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void ComponentType::ReadPresence( xml::xistream& , bool& flag )
{
    flag = true;
}

// -----------------------------------------------------------------------------
// Name: ComponentType::GetName
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
const std::string& ComponentType::GetName() const
{
    return name_;
}
