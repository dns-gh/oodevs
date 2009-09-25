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
#include "MIL_ParameterType_PathList.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_PathListParameter constructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PathListParameter::MIL_PathListParameter( const ASN1T_PathList& asn )
{
    pathList_.resize( asn.n );
    for( unsigned int i = 0; i < asn.n; ++i)
    {
        ASN1T_Path& path = asn.elem[i];
        if( path.type != EnumLocationType::line )
            throw std::runtime_error( "Unexpected type passed for path" );
        pathList_[i].resize( path.coordinates.n );
        for( unsigned int j = 0; j < path.coordinates.n; ++j )
        {
            pathList_[i][j].reset( new MT_Vector2D() );
            MIL_Tools::ConvertCoordMosToSim( path.coordinates.elem[j], *pathList_[i][j] );
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
    return( dynamic_cast<const MIL_ParameterType_PathList*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_PathListParameter::ToPathList
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PathListParameter::ToPathList( ASN1T_PathList& asn ) const
{
    unsigned int size = pathList_.size();
    ASN1T_Location* pLcal = new ASN1T_Location[ size ];
    asn.n = size;
    asn.elem = pLcal;

    for( unsigned int i = 0; i < size ; i++ )
    {
        ASN1T_Location& location = pLcal[i];
        location.type = EnumLocationType::line;
        int size = pathList_[i].size();
        location.coordinates.n = size; 
        if( !pathList_[i].empty() )
        {
            location.coordinates.elem = new ASN1T_CoordLatLong[ size ];
            for( int j = 0; j < size; ++j )
                MIL_Tools::ConvertCoordSimToMos( *pathList_[i][j], location.coordinates.elem[j] );
        }
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
