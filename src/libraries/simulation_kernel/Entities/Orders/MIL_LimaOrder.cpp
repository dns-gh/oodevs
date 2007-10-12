// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_LimaOrder.h"

#include "MIL_LimaFunction.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_AsnException.h"

uint MIL_LimaOrder::nNextID_ = 0;

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder constructor
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
MIL_LimaOrder::MIL_LimaOrder( const ASN1T_LimaOrder& asn )
    : nID_          ( ++nNextID_ )
    , points_       ()
    , functions_    ()
    , bFlag_        ( false )
    , bScheduleFlag_( false )
{
    NET_ASN_Tools::ReadTick( asn.horaire, nSchedule_);

    if( !NET_ASN_Tools::ReadLine( asn.lima, points_ ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_lima );

    for( uint i = 0; i < asn.fonctions.n; ++i )
    {
        const MIL_LimaFunction* pFunction = MIL_LimaFunction::Find( asn.fonctions.elem[ i ] );
        if( !pFunction )
            throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_lima_function );
        functions_.insert( pFunction );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder destructor
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
MIL_LimaOrder::~MIL_LimaOrder()
{
}


// =============================================================================
// GEOMETRY
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_LimaOrder::Intersect2D
// Created: JVT 02-12-05
//-----------------------------------------------------------------------------
bool MIL_LimaOrder::Intersect2D( const MT_Line& line, MT_Vector2D& vPos ) const      //$$$ devrait prendre un set en paramètre (plusieurs intersections possibles)
{
    CIT_PointVector itPoint = points_.begin();
    const MT_Vector2D* pLastPoint = &*itPoint;
    for( ++itPoint; itPoint != points_.end(); ++itPoint )
    {
        const MT_Vector2D* pCurPoint = &*itPoint;
        MT_Line lineLima( *pLastPoint, *pCurPoint );

        if( lineLima.Intersect2D( line, vPos ) == eDoIntersect )
            return true;
        pLastPoint = pCurPoint;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::Intersect2D
// Created: NLD 2003-08-22
// -----------------------------------------------------------------------------
bool MIL_LimaOrder::Intersect2D( const T_PointVector& polyline, T_PointSet& intersectionSet ) const
{
    intersectionSet.clear();
    
    MT_Vector2D vIntersect;

    CIT_PointVector itPoint = polyline.begin();
    const MT_Vector2D* pLastPoint = &*itPoint;
    for( ++itPoint; itPoint != polyline.end(); ++itPoint )
    {
        const MT_Vector2D* pCurPoint = &*itPoint;

        MT_Line lineTmp( *pLastPoint, *pCurPoint );

        if( Intersect2D( lineTmp, vIntersect ) )
            intersectionSet.insert( vIntersect );
        pLastPoint = pCurPoint;
    }
    return !intersectionSet.empty();
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::Serialize
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
void MIL_LimaOrder::Serialize( ASN1T_LimaOrder& asn ) const
{
    NET_ASN_Tools::WriteLine( points_, asn.lima );
    NET_ASN_Tools::WriteTick( nSchedule_, asn.horaire );

    asn.fonctions.n = functions_.size();
    if( !functions_.empty() )
    {
        asn.fonctions.elem = new ASN1T_EnumLimaType[ functions_.size() ];
        uint i = 0;
        for( CIT_LimaFunctions it = functions_.begin(); it != functions_.end(); ++it )
            asn.fonctions.elem[ i++ ] = (**it).GetAsnID();
    }
}
