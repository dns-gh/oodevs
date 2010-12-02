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
#include "protocol/protocol.h"

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
MIL_PathParameter::MIL_PathParameter( const Common::MsgPath& asn )
{
    if( asn.location().type() != Common::MsgLocation_Geometry_line )
        throw std::runtime_error( "Unexpected type passed for path" );
    path_.reserve( asn.location().coordinates().elem_size());
    for( int i = 0; i < asn.location().coordinates().elem_size(); ++i )
    {
        path_.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D() ) );
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
bool MIL_PathParameter::ToElement( Common::MsgMissionParameter_Value& elem ) const
{
    elem.mutable_path()->mutable_location()->set_type( Common::MsgLocation_Geometry_line );
    const unsigned int size = path_.size();
    for( unsigned int i = 0; i < size; ++i )
        MIL_Tools::ConvertCoordSimToMos( *path_[i], *elem.mutable_path()->mutable_location()->mutable_coordinates()->add_elem() );
    return true;
}
