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
#include "simulation_orders/MIL_ParameterType_Path.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_Tools.h"

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
MIL_PathParameter::MIL_PathParameter( const ASN1T_Path& asn )
{
    if( asn.type != EnumLocationType::line )
        throw std::runtime_error( "Unexpected type passed for path" );
    path_.reserve( asn.coordinates.n );
    for( unsigned int i = 0; i < asn.coordinates.n; ++i )
    {
        path_.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D() ) );
        MIL_Tools::ConvertCoordMosToSim( asn.coordinates.elem[i], *path_[i] );
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
bool MIL_PathParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_Path*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_PathParameter::ToPath
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PathParameter::ToPath( ASN1T_Path& asn ) const
{
    asn.type = EnumLocationType::line;
    int size = path_.size();
    asn.coordinates.n = size; 
    if( !path_.empty() )
    {
        asn.coordinates.elem = new ASN1T_CoordLatLong[ size ];
        for( int i = 0; i < size; ++i )
            MIL_Tools::ConvertCoordSimToMos( *path_[i], asn.coordinates.elem[i] );
    }
    return true;
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
