// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PolygonParameter.h"
#include "simulation_terrain/TER_Localisation.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/Protocol.h"
#include "CheckPoints/SerializationTools.h"
#include "MIL.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_PolygonParameter )

// -----------------------------------------------------------------------------
// Name: MIL_PolygonParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_PolygonParameter::MIL_PolygonParameter()
    : valid_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonParameter constructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PolygonParameter::MIL_PolygonParameter( const sword::Polygon& asn )
    : pPolygon_( new TER_Localisation() )
    , valid_   ( false )
{
    valid_ = NET_ASN_Tools::ReadPolygon( asn, *pPolygon_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonParameter destructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PolygonParameter::~MIL_PolygonParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PolygonParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::ePolygon
        || type == MIL_ParameterType_ABC::eLocationComposite;
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonParameter::ToPolygon
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PolygonParameter::ToPolygon( boost::shared_ptr< TER_Localisation >& value ) const
{
    value = pPolygon_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_PolygonParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    NET_ASN_Tools::WritePolygon( *pPolygon_, *elem.mutable_area() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonParameter::IsValid
// Created: MMC 2011-05-30
// -----------------------------------------------------------------------------
bool MIL_PolygonParameter::IsValid() const
{
    return valid_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonParameter::load
// Created: LGY 2011-06-10
// -----------------------------------------------------------------------------
void MIL_PolygonParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> pPolygon_;
    file >> valid_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonParameter::save
// Created: LGY 2011-06-10
// -----------------------------------------------------------------------------
void MIL_PolygonParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
    file << pPolygon_;
    file << valid_;
}
