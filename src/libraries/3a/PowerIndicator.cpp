// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "PowerIndicator.h"
#include <xeumeuleu/xml.hpp>

using namespace aar;

// -----------------------------------------------------------------------------
// Name: PowerIndicator constructor
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
PowerIndicator::PowerIndicator( xml::xistream& xis )
    : id_           ( 0 )
    , nDirectFire_  ( 0 )
    , nIndirectFire_( 0 )
    , nCloseCombat_ ( 0 )
    , nEngineering_ ( 0 )
{
    xis >> xml::attribute( "name", name_ )
        >> xml::attribute( "id", id_ )
        >> xml::optional
        >> xml::start( "power-indicators" )
            >> xml::attribute( "direct-fire",   nDirectFire_ )
            >> xml::attribute( "indirect-fire", nIndirectFire_ )
            >> xml::attribute( "close-combat",  nCloseCombat_ )
            >> xml::attribute( "engineering",   nEngineering_ )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PowerIndicator destructor
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
PowerIndicator::~PowerIndicator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PowerIndicator::GetId
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
unsigned long PowerIndicator::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: PowerIndicator::GetName
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
const std::string& PowerIndicator::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: PowerIndicator::GetDirectFire
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
unsigned int PowerIndicator::GetDirectFire() const
{
    return nDirectFire_;
}

// -----------------------------------------------------------------------------
// Name: PowerIndicator::GetIndirectFire
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
unsigned int PowerIndicator::GetIndirectFire() const
{
    return nIndirectFire_;
}

// -----------------------------------------------------------------------------
// Name: PowerIndicator::GetCloseCombat
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
unsigned int PowerIndicator::GetCloseCombat() const
{
    return nCloseCombat_;
}

// -----------------------------------------------------------------------------
// Name: PowerIndicator::GetEngineering
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
unsigned int PowerIndicator::GetEngineering() const
{
    return nEngineering_;
}
