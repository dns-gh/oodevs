// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_TirIndirectParameter.h"
#include "protocol/Protocol.h"
#include "MIL.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_TirIndirectParameter )

// -----------------------------------------------------------------------------
// Name: MIL_TirIndirectParameter constructor
// Created: LGY 2011-06-15
// -----------------------------------------------------------------------------
MIL_TirIndirectParameter::MIL_TirIndirectParameter()
    : data_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_TirIndirectParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_TirIndirectParameter::MIL_TirIndirectParameter( const sword::Id& asn )
    : data_( asn.id() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_TirIndirectParameter destructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_TirIndirectParameter::~MIL_TirIndirectParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_TirIndirectParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_TirIndirectParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eIndirectFire;
}

// -----------------------------------------------------------------------------
// Name: MIL_TirIndirectParameter::ToId
// Created: MGD 2010-11-19
// -----------------------------------------------------------------------------
bool MIL_TirIndirectParameter::ToId( int& id ) const
{
    id = data_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_TirIndirectParameter::ToIndirectFire
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_TirIndirectParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    elem.mutable_indirectfire()->set_id( data_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_TirIndirectParameter::load
// Created: LGY 2011-06-15
// -----------------------------------------------------------------------------
void MIL_TirIndirectParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> data_;
}

// -----------------------------------------------------------------------------
// Name: MIL_TirIndirectParameter::save
// Created: LGY 2011-06-15
// -----------------------------------------------------------------------------
void MIL_TirIndirectParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
    file << data_;
}
