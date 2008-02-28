// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ComponentType.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ComponentType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ComponentType::ComponentType( xml::xistream& xis )
    : hasMaintenance_( false )
    , hasMedical_( false )
    , hasSupply_( false )
{
    xis >> attribute( "name", name_ )
        >> attribute( "id", id_ );

    xis >> optional() 
        >> start( "logistic-functions" )
                >> list( "maintenance-functions", *this, &ComponentType::ReadPresence, hasMaintenance_ )
                >> list( "health-functions",      *this, &ComponentType::ReadPresence, hasMedical_ )
                >> list( "supply-functions",      *this, &ComponentType::ReadPresence, hasSupply_ );
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
void ComponentType::ReadPresence( xml::xistream& , bool& flag ) const
{
    flag = true;
}

// -----------------------------------------------------------------------------
// Name: ComponentType::GetName
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
std::string ComponentType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: ComponentType::GetId
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
unsigned long ComponentType::GetId() const
{
    return id_;
}
