// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PointParameter.h"
#include "MIL_ParameterType_Point.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_PointParameter constructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PointParameter::MIL_PointParameter( const ASN1T_Point& asn )
: pPoint_( new MT_Vector2D() )
{
    if( asn.type != EnumLocationType::point )
        throw std::runtime_error( "Unexpected type passed for point" );
    if( asn.coordinates.n != 1 )
        throw std::runtime_error( "Too many points" );
    MIL_Tools::ConvertCoordMosToSim( asn.coordinates.elem[0], *pPoint_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PointParameter constructor
// Created: LDC 2009-07-24
// -----------------------------------------------------------------------------
MIL_PointParameter::MIL_PointParameter( const MT_Vector2D& point )
: pPoint_( new MT_Vector2D( point ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PointParameter destructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PointParameter::~MIL_PointParameter()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MIL_PointParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PointParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_Point*>( &type ) != 0 );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_PointParameter::ToPoint
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PointParameter::ToPoint( ASN1T_Point& asn ) const
{
    NET_ASN_Tools::WritePoint( *pPoint_, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PointParameter::ToPoint
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PointParameter::ToPoint( boost::shared_ptr< MT_Vector2D >& result ) const
{
    result = pPoint_;
    return true;
}
