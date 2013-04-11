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
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ComponentType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ComponentType::ComponentType( xml::xistream& xis )
    : hasMaintenance_( false )
    , hasMedical_( false )
    , hasSupply_( false )
    , weight_( 0 )
    , width_( 0 )
    , spacing_( 0 )
{
    xis >> xml::attribute( "name", name_ )
        >> xml::attribute( "id", id_ )
        >> xml::attribute( "weight", weight_ )
        >> xml::optional >> xml::attribute( "width", width_ )
        >> xml::optional >> xml::attribute( "front-separation-distance", spacing_ );

    xis >> xml::optional
        >> xml::start( "logistic-functions" )
                >> xml::list( "maintenance-functions", *this, &ComponentType::ReadPresence, hasMaintenance_ )
                >> xml::list( "health-functions",      *this, &ComponentType::ReadPresence, hasMedical_ )
                >> xml::list( "supply-functions",      *this, &ComponentType::ReadPresence, hasSupply_ );
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

// -----------------------------------------------------------------------------
// Name: ComponentType::GetWeight
// Created: NPT 2012-11-16
// -----------------------------------------------------------------------------
float ComponentType::GetWeight() const
{
    return weight_;
}

// -----------------------------------------------------------------------------
// Name: ComponentType::GetWidth
// Created: LDC 2013-04-11
// -----------------------------------------------------------------------------
float ComponentType::GetWidth() const
{
    return width_;
}

// -----------------------------------------------------------------------------
// Name: ComponentType::GetSpacing
// Created: LDC 2013-04-11
// -----------------------------------------------------------------------------
float ComponentType::GetSpacing() const
{
    return spacing_;
}
