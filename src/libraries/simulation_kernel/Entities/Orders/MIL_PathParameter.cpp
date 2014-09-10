// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PathParameter.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_Tools.h"
#include "MIL.h"
#include "CheckPoints/SerializationTools.h"
#include "protocol/Protocol.h"
#include <boost/make_shared.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_PathParameter )

// -----------------------------------------------------------------------------
// Name: MIL_PathParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_PathParameter::MIL_PathParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PathParameter constructor
// Created: LDC 2009-07-10
// -----------------------------------------------------------------------------
MIL_PathParameter::MIL_PathParameter( boost::shared_ptr< MT_Vector2D > point )
{
    path_.push_back( point );
}

// -----------------------------------------------------------------------------
// Name: MIL_PathParameter constructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PathParameter::MIL_PathParameter( const sword::Path& asn )
{
    if( asn.location().type() != sword::Location_Geometry_line )
        throw MASA_EXCEPTION( "Unexpected type passed for path" );
    path_.reserve( asn.location().coordinates().elem_size() );
    for( int i = 0; i < asn.location().coordinates().elem_size(); ++i )
    {
        path_.push_back( boost::make_shared< MT_Vector2D >() );
        MIL_Tools::ConvertCoordMosToSim( asn.location().coordinates().elem( i ), *path_[i] );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PathParameter constructor
// Created: LDC 2009-07-08
// -----------------------------------------------------------------------------
MIL_PathParameter::MIL_PathParameter( const std::vector< boost::shared_ptr< MT_Vector2D > >& pointList )
    : path_( pointList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PathParameter destructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PathParameter::~MIL_PathParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PathParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PathParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::ePath
        || type == MIL_ParameterType_ABC::eLocationComposite;
}

// -----------------------------------------------------------------------------
// Name: MIL_PathParameter::ToPath
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PathParameter::ToPath( std::vector< boost::shared_ptr< MT_Vector2D > >& value ) const
{
    value = path_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PathParameter::ToElement
// Created: MGD 2010-11-05
// -----------------------------------------------------------------------------
bool MIL_PathParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    elem.mutable_path()->mutable_location()->set_type( sword::Location_Geometry_line );
    for( std::size_t i = 0; i < path_.size(); ++i )
        MIL_Tools::ConvertCoordSimToMos( *path_[i], *elem.mutable_path()->mutable_location()->mutable_coordinates()->add_elem() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PathParameter::load
// Created: LGY 2011-07-25
// -----------------------------------------------------------------------------
void MIL_PathParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> path_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PathParameter::save
// Created: LGY 2011-07-25
// -----------------------------------------------------------------------------
void MIL_PathParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
    file << path_;
}
