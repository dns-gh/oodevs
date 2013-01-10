// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_HumanRepartition.h"

// -----------------------------------------------------------------------------
// Name: MIL_HumanRepartition constructor
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
MIL_HumanRepartition::MIL_HumanRepartition()
    : male_( 0.0f )
    , female_( 0.0f )
    , children_( 0.0f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_HumanRepartition constructor
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
MIL_HumanRepartition::MIL_HumanRepartition( xml::xistream& xis )
    : male_( 0.0f )
    , female_( 0.0f )
    , children_( 0.0f )
{
    xis >> xml::optional >> xml::start( "repartition" )
        >> xml::attribute<float>( "male", male_ )
        >> xml::attribute<float>( "female", female_ )
        >> xml::attribute<float>( "children", children_ )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_HumanRepartition destructor
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
MIL_HumanRepartition::~MIL_HumanRepartition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_HumanRepartition::GetMale
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
float MIL_HumanRepartition::GetMale() const
{
    return male_;
}

// -----------------------------------------------------------------------------
// Name: MIL_HumanRepartition::GetFemale
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
float MIL_HumanRepartition::GetFemale() const
{
    return female_;
}

// -----------------------------------------------------------------------------
// Name: MIL_HumanRepartition::GetChildren
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
float MIL_HumanRepartition::GetChildren() const
{
    return children_;
}

// -----------------------------------------------------------------------------
// Name: MIL_HumanRepartition::IsCivilian
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
bool MIL_HumanRepartition::IsCivilian() const
{
    return ( male_ > 0.0f || female_ > 0.0f || children_ > 0.0f );
}
