// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PathListParameter.h"
#include "simulation_orders/MIL_ParameterType_PathList.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/protocol.h"
#include "tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_PathListParameter constructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PathListParameter::MIL_PathListParameter( const Common::MsgPathList& asn )
{
    pathList_.resize( asn.elem_size() );
    for( int i = 0; i < asn.elem_size(); ++i)
    {
        if( asn.elem( i ).location().type() != Common::MsgLocation_Geometry_line )
            throw std::runtime_error( "Unexpected type passed for path" );
        pathList_[i].resize( asn.elem( i ).location().coordinates().elem_size() );
        for( int j = 0; j < asn.elem( i ).location().coordinates().elem_size(); ++j )
        {
            pathList_[i][j].reset( new MT_Vector2D() );
            MIL_Tools::ConvertCoordMosToSim( asn.elem( i ).location().coordinates().elem(j), *pathList_[i][j] );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PathListParameter constructor
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
MIL_PathListParameter::MIL_PathListParameter( const std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& pathList )
    : pathList_( pathList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PathListParameter destructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PathListParameter::~MIL_PathListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PathListParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PathListParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return dynamic_cast< const MIL_ParameterType_PathList* >( &type ) != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PathListParameter::ToPathList
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PathListParameter::ToPathList( Common::MsgPathList& asn ) const
{
    for( unsigned int i = 0; i < pathList_.size(); ++i )
    {
        Common::MsgLocation& location = *asn.add_elem()->mutable_location();
        location.set_type( Common::MsgLocation::line );
        const unsigned int size = pathList_[i].size();
        if( !pathList_[i].empty() )
            for( unsigned int j = 0; j < size; ++j )
                MIL_Tools::ConvertCoordSimToMos( *pathList_[i][j], *location.mutable_coordinates()->add_elem() );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PathListParameter::ToPathList
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PathListParameter::ToPathList( std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& value ) const
{
    value = pathList_;
    return true;
}
