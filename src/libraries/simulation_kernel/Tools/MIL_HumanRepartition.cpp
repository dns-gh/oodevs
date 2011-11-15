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
#include "protocol/ClientSenders.h"
#include "Network/NET_AsnException.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_HumanRepartition )

// -----------------------------------------------------------------------------
// Name: MIL_HumanRepartition constructor
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
MIL_HumanRepartition::MIL_HumanRepartition()
    : hasChanged_( false )
    , male_( 0.0f )
    , female_( 0.0f )
    , children_( 0.0f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_HumanRepartition constructor
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
MIL_HumanRepartition::MIL_HumanRepartition( const MIL_HumanRepartition& humanRepartition )
{
    male_   = humanRepartition.GetMale();
    female_ = humanRepartition.GetFemale();
    children_ = humanRepartition.GetChildren();
}

// -----------------------------------------------------------------------------
// Name: MIL_HumanRepartition constructor
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
MIL_HumanRepartition::MIL_HumanRepartition( xml::xistream& xis )
    : hasChanged_( false )
    , male_( 0.0f )
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
// Name: MIL_HumanRepartition::WriteODB
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
void MIL_HumanRepartition::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "repartition" )
        << xml::attribute( "male", male_ )
        << xml::attribute( "female", female_ )
        << xml::attribute( "children", children_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_HumanRepartition::load
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
void MIL_HumanRepartition::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> male_
         >> female_
         >> children_;
}

// -----------------------------------------------------------------------------
// Name: MIL_HumanRepartition::save
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
void MIL_HumanRepartition::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << male_
         << female_
         << children_;
}

// -----------------------------------------------------------------------------
// Name: MIL_HumanRepartition::HasChanged
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
bool MIL_HumanRepartition::HasChanged() const
{
    return hasChanged_;
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
// Name: MIL_HumanRepartition::IsExist
// Created: MMC 2011-10-07
// -----------------------------------------------------------------------------
bool MIL_HumanRepartition::IsExist() const
{
    return ( male_ > 0.0f || female_ > 0.0f || children_ > 0.0f );
}