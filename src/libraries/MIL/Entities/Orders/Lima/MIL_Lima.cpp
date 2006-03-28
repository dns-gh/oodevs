//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Lima/MIL_Lima.cpp $
// $Author: Nld $
// $Modtime: 17/02/05 14:14 $
// $Revision: 4 $
// $Workfile: MIL_Lima.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_Lima.h"

#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_IDManager.h"
#include "Tools/MIL_Tools.h"

//-----------------------------------------------------------------------------
// Name: MIL_Lima constructor
// Created: NLD 2002-08-08
// Last Modified : JVT 02-10-10
//-----------------------------------------------------------------------------
MIL_Lima::MIL_Lima()
    : nID_( (uint)-1 )
{
    
}

//-----------------------------------------------------------------------------
// Name: MIL_Lima destructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
MIL_Lima::~MIL_Lima()
{
    
}

//=============================================================================
// INIT / CLEANUP
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_Lima::Initialize
// Created: NLD 2002-08-08
// Last modified: JVT 03-02-27
//-----------------------------------------------------------------------------
bool MIL_Lima::Initialize( const ASN1T_MsgLimaCreation& asnMsg, MIL_MOSContextID nCtx )
{
    nID_ = asnMsg.oid;

    NET_ASN_MsgLimaCreationAck asnAckMsg;
    asnAckMsg.GetAsnMsg().oid = nID_;

    if( !MIL_IDManager::limas_.IsMosIDValid( nID_ ) || !MIL_IDManager::limas_.LockMosID( nID_ ) )
    {
        asnAckMsg.GetAsnMsg().error_code = EnumInfoContextErrorCode::error_invalid_id;
        asnAckMsg.Send( nCtx );
        return false;
    }
    
    nLimaFunction_ = (E_LimaFunctions)asnMsg.fonction;
    if( nLimaFunction_ >= eLimaFuncNone )
    {
        asnAckMsg.GetAsnMsg().error_code = EnumInfoContextErrorCode::error_invalid_lima_function;
        asnAckMsg.Send( nCtx );
        return false;        
    }

    //$$$ Utiliser NET_ASN_Tools
    if( asnMsg.geometrie.type != EnumTypeLocalisation::line || asnMsg.geometrie.vecteur_point.n < 2 )
    {
        asnAckMsg.GetAsnMsg().error_code = EnumInfoContextErrorCode::error_invalid_geometry;
        asnAckMsg.Send( nCtx );
        return false;        
    }

    ReadPoints( asnMsg.geometrie.vecteur_point );

    asnAckMsg.GetAsnMsg().error_code = EnumInfoContextErrorCode::no_error;
    asnAckMsg.Send( nCtx );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_Lima::Cleanup
// Created: NLD 2002-08-08
// Last Modified : JVT 02-10-10
//-----------------------------------------------------------------------------
void MIL_Lima::Cleanup()
{
    /*
    NET_ASN_MsgLimaDestruction asnMsg;
    asnMsg.GetAsnMsg() = nID_;
    asnMsg.Send();
    */

    pointVector_.clear();
    nID_ = (uint)-1;
}


//-----------------------------------------------------------------------------
// Name: MIL_Lima::Cleanup
// Created: NLD 2002-09-10
// Last Modified : JVT 02-10-10
//-----------------------------------------------------------------------------
void MIL_Lima::Cleanup( MIL_MOSContextID nCtx )
{
    bool bOut = MIL_IDManager::limas_.ReleaseMosID( nID_ );
    assert( bOut );

    NET_ASN_MsgLimaDestructionAck asnAckMsg;
    asnAckMsg.GetAsnMsg().oid        = nID_;
    asnAckMsg.GetAsnMsg().error_code = EnumInfoContextErrorCode::no_error;
    asnAckMsg.Send( nCtx );

    pointVector_.clear();
    nID_ = (uint)-1;
}

//=============================================================================
// MAIN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_Lima::Update
// Created: NLD 2002-09-13
// Last Modified : JVT 02-10-10
//-----------------------------------------------------------------------------
bool MIL_Lima::Update( const ASN1T_MsgLimaUpdate& asnMsg, MIL_MOSContextID nCtx )
{
    NET_ASN_MsgLimaUpdateAck asnAckMsg;
    asnAckMsg.GetAsnMsg().oid = nID_;

    nLimaFunction_ = (E_LimaFunctions)asnMsg.fonction;
    if( nLimaFunction_ >= eLimaFuncNone )
    {
        asnAckMsg.GetAsnMsg().error_code = EnumInfoContextErrorCode::error_invalid_lima_function;
        asnAckMsg.Send( nCtx );
        return false;        
    }
    
    if( asnMsg.geometrie.type != EnumTypeLocalisation::line || asnMsg.geometrie.vecteur_point.n < 2 )
    {
        asnAckMsg.GetAsnMsg().error_code = EnumInfoContextErrorCode::error_invalid_geometry;
        asnAckMsg.Send( nCtx );
        return false;        
    }

    ReadPoints( asnMsg.geometrie.vecteur_point );

    asnAckMsg.GetAsnMsg().error_code = EnumInfoContextErrorCode::no_error;
    asnAckMsg.Send( nCtx );
    return true;
}

//=============================================================================
// TOOLS
//=============================================================================


//-----------------------------------------------------------------------------
// Name: MIL_Lima::ReadPoints
// Created: NLD 2002-09-13
//-----------------------------------------------------------------------------
void MIL_Lima::ReadPoints( const ASN1T__SeqOfCoordUTM& listPoint )
{
    pointVector_.clear(); pointVector_.reserve( listPoint.n );
    for( uint i = 0; i < listPoint.n; ++i )
    {
        MT_Vector2D vPosTmp;
        MIL_Tools::ConvertCoordMosToSim( listPoint.elem[i], vPosTmp );
        pointVector_.push_back( vPosTmp );
    }
}


//=============================================================================
// COLLISIONS
//=============================================================================


//-----------------------------------------------------------------------------
// Name: MIL_Lima::Intersect2D
// Created: JVT 02-12-05
//-----------------------------------------------------------------------------
bool MIL_Lima::Intersect2D( const MT_Line& line, MT_Vector2D& vPos ) const      //$$$ devrait prendre un set en paramètre (plusieurs intersections possibles)
{
    CIT_PointVector itPoint = pointVector_.begin();
    const MT_Vector2D* pLastPoint = &*itPoint;
    for( ++itPoint; itPoint != pointVector_.end(); ++itPoint )
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
// Name: MIL_Lima::Intersect2D
// Created: NLD 2003-08-22
// -----------------------------------------------------------------------------
bool MIL_Lima::Intersect2D( const T_PointVector& polyline, T_PointSet& intersectionSet ) const
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

// -----------------------------------------------------------------------------
// Name: MIL_Lima::SendFullState
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
void MIL_Lima::SendFullState() const
{
    assert( !pointVector_.empty() );
    ASN1T_CoordUTM* pCoord = new ASN1T_CoordUTM[ pointVector_.size() ];//$$$ RAM
    uint i = 0; 
    for( CIT_PointVector itPoint = pointVector_.begin(); itPoint != pointVector_.end(); ++itPoint )
        NET_ASN_Tools::WritePoint( *itPoint, pCoord[i++] );

    NET_ASN_MsgLimaCreation asnMsg;
    asnMsg.GetAsnMsg().oid                          = nID_;
    asnMsg.GetAsnMsg().fonction                     = (ASN1T_EnumTypeLima)nLimaFunction_;
    asnMsg.GetAsnMsg().geometrie.type               = EnumTypeLocalisation::line;
    asnMsg.GetAsnMsg().geometrie.vecteur_point.n    = pointVector_.size();
    asnMsg.GetAsnMsg().geometrie.vecteur_point.elem = pCoord;
    asnMsg.Send();

    delete [] pCoord;
}
