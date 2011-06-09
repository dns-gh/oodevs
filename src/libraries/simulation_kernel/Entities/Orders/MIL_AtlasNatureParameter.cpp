// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AtlasNatureParameter.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AtlasNatureParameter )

// -----------------------------------------------------------------------------
// Name: MIL_AtlasNatureParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_AtlasNatureParameter::MIL_AtlasNatureParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AtlasNatureParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AtlasNatureParameter::MIL_AtlasNatureParameter( const sword::Nature& asn )
{
    nature_ = asn.flags();
}

// -----------------------------------------------------------------------------
// Name: MIL_AtlasNatureParameter constructor
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
MIL_AtlasNatureParameter::MIL_AtlasNatureParameter( int nature )
    : nature_( nature )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AtlasNatureParameter destructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AtlasNatureParameter::~MIL_AtlasNatureParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AtlasNatureParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_AtlasNatureParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eNatureAtlas;
}

// -----------------------------------------------------------------------------
// Name: MIL_AtlasNatureParameter::ToNatureAtlas
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AtlasNatureParameter::ToNatureAtlas( int& atlas ) const
{
    atlas = nature_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AtlasNatureParameter::ToElement
// Created: MGD 2010-11-19
// -----------------------------------------------------------------------------
bool MIL_AtlasNatureParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    elem.mutable_nature()->set_flags( nature_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AtlasNatureParameter::serialize
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_AtlasNatureParameter::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< MIL_BaseParameter >( *this )
         & nature_;
}
