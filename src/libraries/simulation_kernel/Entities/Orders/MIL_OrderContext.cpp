//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Pion/MIL_OrderContext.cpp $
// $Author: Nld $
// $Modtime: 21/03/05 12:17 $
// $Revision: 8 $
// $Workfile: MIL_OrderContext.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_OrderContext.h"

#include "MIL_LimaFunction.h"
#include "TER_LimitData.h"
#include "MIL_AgentServer.h"
#include "MIL_TacticalLineManager.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_OrderContext::MIL_OrderContext()
    : limas_    ()  
    , fuseau_   ()
    , dirDanger_( 0., 1. )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext constructor
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
MIL_OrderContext::MIL_OrderContext( const ASN1T_OrderContext& asn, const MT_Vector2D& vOrientationRefPos )
    : limas_    ()  
    , fuseau_   ()
    , dirDanger_()
{
    // Dir danger
    NET_ASN_Tools::ReadDirection( asn.direction_dangereuse, dirDanger_ );

    // Limas
    limas_.clear();
    for( uint i = 0; i < asn.limas.n; ++i )
        limas_.push_back( MIL_LimaOrder( asn.limas.elem[ i ] ) );

    // Limites
    T_PointVector leftLimitData;
    T_PointVector rightLimitData;

    if( asn.m.limite_gauchePresent && !NET_ASN_Tools::ReadLine( asn.limite_gauche, leftLimitData ) )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_limit );

    if( asn.m.limite_droitePresent && !NET_ASN_Tools::ReadLine( asn.limite_droite, rightLimitData ))
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_limit );

    if( !leftLimitData.empty() && !rightLimitData.empty() && leftLimitData == rightLimitData )
        throw NET_AsnException< ASN1T_EnumOrderErrorCode >( EnumOrderErrorCode::error_invalid_limit );

    if( leftLimitData.empty() || rightLimitData.empty() )
        fuseau_.Reset();
    else
        fuseau_.Reset( vOrientationRefPos, leftLimitData, rightLimitData, FindLima( MIL_LimaFunction::LDM_ ), FindLima( MIL_LimaFunction::LFM_ ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_OrderContext::MIL_OrderContext( const MIL_OrderContext& rhs )
    : limas_    ( rhs.limas_     )  
    , fuseau_   () //$$$
    , dirDanger_( rhs.dirDanger_ )
{
    fuseau_ = rhs.fuseau_;
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderContext destructor
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
MIL_OrderContext::~MIL_OrderContext()
{
    fuseau_.Reset();
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::Serialize
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
void MIL_OrderContext::Serialize( ASN1T_OrderContext& asn ) const
{
    NET_ASN_Tools::WriteDirection( dirDanger_, asn.direction_dangereuse );

    // Limites
    if( fuseau_.GetLeftLimit() )
    {
        asn.m.limite_gauchePresent = 1;
        NET_ASN_Tools::WriteLine( fuseau_.GetLeftLimit()->GetPoints(), asn.limite_gauche );
    }
    if( fuseau_.GetRightLimit() )
    {
        asn.m.limite_droitePresent = 1;
        NET_ASN_Tools::WriteLine( fuseau_.GetRightLimit()->GetPoints(), asn.limite_droite );
    }

    // Limas
    asn.limas.n = limas_.size();
    if( !limas_.empty() )
    {
        asn.limas.elem = new ASN1T_LimaOrder[ limas_.size() ];
        uint i = 0;
        for( CIT_LimaVector it = limas_.begin(); it != limas_.end(); ++it )
            it->Serialize( asn.limas.elem[i++] );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::CleanAfterSerialization
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
void MIL_OrderContext::CleanAfterSerialization( ASN1T_OrderContext& asn ) const
{
    if( asn.m.limite_gauchePresent )
        NET_ASN_Tools::Delete( asn.limite_gauche );
    if( asn.m.limite_droitePresent )
        NET_ASN_Tools::Delete( asn.limite_droite );

    if( asn.limas.n > 0 )
    {
        for( uint i = 0; i < asn.limas.n; ++i )
        {
            NET_ASN_Tools::Delete( asn.limas.elem[ i ].lima );
            if( asn.limas.elem[ i ].fonctions.n > 0 )
                delete [] asn.limas.elem[ i ].fonctions.elem;
        }
        delete [] asn.limas.elem;
    }
}
