// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_NullParameter.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/Protocol.h"
#include "MIL.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_NullParameter )

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter constructor
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
MIL_NullParameter::MIL_NullParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter destructor
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
MIL_NullParameter::~MIL_NullParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::IsOfType
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
bool MIL_NullParameter::IsOfType( MIL_ParameterType_ABC::E_Type /*type*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::ToElement
// Created: LDC 2009-11-03
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToElement( sword::MissionParameter_Value& ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::MIL_ListParameter::ToList
// Created: LDC 2010-09-21
// -----------------------------------------------------------------------------
bool MIL_NullParameter::ToList( std::vector< sword::MissionParameter_Value >& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::load
// Created: LGY 2011-06-07
// -----------------------------------------------------------------------------
void MIL_NullParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_NullParameter::save
// Created: LGY 2011-06-07
// -----------------------------------------------------------------------------
void MIL_NullParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
}
