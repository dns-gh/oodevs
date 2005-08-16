//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Limit/MIL_Limit.cpp $
// $Author: Age $
// $Modtime: 13/05/05 18:14 $
// $Revision: 8 $
// $Workfile: MIL_Limit.cpp $
//
//*****************************************************************************

#include "MIL_Pch.h"

#include "MIL_Limit.h"

#include "Tools/MIL_Tools.h"
#include "MIL_AgentServer.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"

#include "TER/TER_PathFindManager.h"
#include "TER/TER_DynamicData.h"

using namespace DIN;

MIL_MOSIDManager MIL_Limit::idManager_;

//-----------------------------------------------------------------------------
// Name: MIL_Limit constructor
// Created: NLD 2002-08-08
// Last Modified : JVT 02-10-10
//-----------------------------------------------------------------------------
MIL_Limit::MIL_Limit()
    : nID_          ( (uint)-1 )
    , bCreatedBySim_( false )
    , nLevel_       ( EnumNatureLevel::ooo )
    , bDestroyed_( false )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_Limit destructor
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
MIL_Limit::~MIL_Limit()
{
    // $$$$ AGE 2005-03-24: 
    assert( fuseauDatas_.empty() );
}

//=============================================================================
// INIT / CLEANUP
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_Limit::Initialize
// Created: NLD 2002-08-08
// Last modified: JVT 03-02-27
//-----------------------------------------------------------------------------
bool MIL_Limit::Initialize( const ASN1T_MsgLimitCreation& asnMsg, MIL_MOSContextID nCtx )
{
    bCreatedBySim_ = false;
    nID_           = asnMsg.oid;
    nLevel_        = asnMsg.level;

    NET_ASN_MsgLimitCreationAck asnAckMsg;
    asnAckMsg.GetAsnMsg().oid = nID_;
    
    if( !idManager_.IsMosIDValid( nID_ ) || !idManager_.LockMosID( nID_ ) )
    {
        asnAckMsg.GetAsnMsg().error_code = EnumInfoContextErrorCode::error_invalid_id;
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

//-----------------------------------------------------------------------------
// Name: MIL_Limit::Initialize
// Created: NLD 2003-04-22
//-----------------------------------------------------------------------------
void MIL_Limit::Initialize()
{
    bCreatedBySim_ = true;
    pointVector_.clear();
    distanceDatas_.clear();
    nID_           = idManager_.GetFreeSimID();
    nLevel_        = EnumNatureLevel::ooo;
}

//-----------------------------------------------------------------------------
// Name: MIL_Limit::Cleanup
// Created: NLD 2002-08-08
// Last Modified : JVT 02-10-10
//-----------------------------------------------------------------------------
void MIL_Limit::Cleanup()
{
    NET_ASN_MsgLimitDestruction asnMsg;
    asnMsg.GetAsnMsg() = nID_;
    asnMsg.Send();

    if( !bCreatedBySim_ )
    {
        assert( idManager_.IsMosIDValid( nID_ ) );
        bool bOut = idManager_.ReleaseMosID( nID_ );
        assert( bOut );
    }
    else
    {
        assert( idManager_.IsSimIDValid( nID_ ) );
        idManager_.ReleaseSimID( nID_ );
    }

    pointVector_.clear();
    distanceDatas_.clear();
    nID_ = (uint)-1;
    Destroy();
}


//-----------------------------------------------------------------------------
// Name: MIL_Limit::Cleanup
// Created: NLD 2002-09-10
// Last Modified : JVT 02-10-10
//-----------------------------------------------------------------------------
void MIL_Limit::Cleanup( MIL_MOSContextID nCtx )
{
    if( !bCreatedBySim_ )
    {
        assert( idManager_.IsMosIDValid( nID_ ) );
        bool bOut = idManager_.ReleaseMosID( nID_ );
        assert( bOut );
    }
    else
    {
        assert( idManager_.IsSimIDValid( nID_ ) );
        idManager_.ReleaseSimID( nID_ );
    }


    NET_ASN_MsgLimitDestructionAck asnAckMsg;
    asnAckMsg.GetAsnMsg().oid        = nID_;
    asnAckMsg.GetAsnMsg().error_code = EnumInfoContextErrorCode::no_error;
    asnAckMsg.Send( nCtx );

    pointVector_.clear();
    distanceDatas_.clear();
    nID_ = (uint)-1;
    Destroy();
}

//=============================================================================
// MAIN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_Limit::Update
// Created: NLD 2002-09-13
// Last Modified : JVT 02-10-10
//-----------------------------------------------------------------------------
bool MIL_Limit::Update( const ASN1T_MsgLimitUpdate& asnMsg, MIL_MOSContextID nCtx )
{
    NET_ASN_MsgLimitUpdateAck asnAckMsg;
    asnAckMsg.GetAsnMsg().oid = nID_;

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

// -----------------------------------------------------------------------------
// Name: MIL_Limit::RegisterFuseau
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
void MIL_Limit::RegisterFuseau( const MIL_Fuseau& fuseau ) const
{
    if( fuseau.GetLeftLimit() )
        RegisterLimit( *fuseau.GetLeftLimit(), fuseau );
    if( fuseau.GetRightLimit() )
        RegisterLimit( *fuseau.GetRightLimit(), fuseau );
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit::RegisterLimit
// Created: AGE 2005-03-24
// -----------------------------------------------------------------------------
void MIL_Limit::RegisterLimit( const MIL_Limit& other, const MIL_Fuseau& fuseau ) const
{
    if( IncreaseReference( other ) )
        return;
    if( other.IncreaseReference( *this ) )
        return;
    CreateDynamicData( other, fuseau );
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit::RemoveFuseau
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
void MIL_Limit::RemoveFuseau( const MIL_Fuseau& fuseau ) const
{
    if( fuseau.GetLeftLimit() )
        RemoveLimit( *fuseau.GetLeftLimit() );
    if( fuseau.GetRightLimit() )
        RemoveLimit( *fuseau.GetRightLimit() );

    if( fuseauDatas_.empty() && bDestroyed_ )
        delete this;
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit::RemoveLimit
// Created: AGE 2005-03-24
// -----------------------------------------------------------------------------
void MIL_Limit::RemoveLimit( const MIL_Limit& other ) const
{
    if( DecreaseReference( other ) )
        return;
    other.DecreaseReference( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit::IncreaseReference
// Created: AGE 2005-03-24
// -----------------------------------------------------------------------------
bool MIL_Limit::IncreaseReference( const MIL_Limit& other ) const
{
    IT_DynamicDatas itOther = fuseauDatas_.find( &other );
    if( itOther != fuseauDatas_.end() )
    {
        ++itOther->second.second;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit::DecreaseReference
// Created: AGE 2005-03-24
// -----------------------------------------------------------------------------
bool MIL_Limit::DecreaseReference( const MIL_Limit& other ) const
{
    IT_DynamicDatas itOther = fuseauDatas_.find( &other );
    if( itOther != fuseauDatas_.end() )
    {
        T_ReferencedData& data = itOther->second;
        if( --data.second == 0 )
        {
            delete data.first;
            fuseauDatas_.erase( itOther );
        }
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit::CreateDynamicData
// Created: AGE 2005-03-24
// -----------------------------------------------------------------------------
void MIL_Limit::CreateDynamicData( const MIL_Limit& other, const MIL_Fuseau& fuseau ) const
{
    T_ReferencedData& data = fuseauDatas_[ &other ];
    if( &other == this )
        // The limit itself
        data.first = CreateDynamicData( pointVector_ );
    else
        // Middle of the fuseau
        data.first = fuseau.CreateDynamicData();
    data.second = 1;
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit::CreateDynamicData
// Created: AGE 2005-03-24
// -----------------------------------------------------------------------------
TER_DynamicData* MIL_Limit::CreateDynamicData( const T_PointVector& points ) const
{
    // $$$$ AGE 2005-03-25: Maybe also add around the data
    TER_DynamicData* pData = & TER_PathFindManager::GetPathFindManager().CreateDynamicData();
    CIT_PointVector itPoint = points.begin();
    const MT_Vector2D* pPrevPoint = &*itPoint;
    for( ++itPoint ; itPoint != points.end(); ++itPoint )
    {
        const MT_Vector2D* pCurPoint = &*itPoint;
        TER_PathFindManager::GetPathFindManager().CreateLineTree( *pPrevPoint, *pCurPoint, *pData );
        pPrevPoint = pCurPoint;
    }
    return pData;
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit::Distance
// Created: AGE 2005-05-13
// -----------------------------------------------------------------------------
MT_Float MIL_Limit::Distance( const MT_Vector2D& p ) const
{
    return std::sqrt( SquareDistance( p ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit::DistanceData constructor
// Created: AGE 2005-05-12
// -----------------------------------------------------------------------------
MIL_Limit::DistanceData::DistanceData( const MT_Vector2D& from, const MT_Vector2D& to )
    : origin_( from )
    , direction_( to - from )
    , rSquareLength_( direction_.SquareMagnitude() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit::DistanceData::SquareDistance
// Created: AGE 2005-05-12
// -----------------------------------------------------------------------------
MT_Float MIL_Limit::DistanceData::SquareDistance( const MT_Vector2D& p ) const
{
    const MT_Vector2D v( p - origin_ );
    const MT_Float rDotProduct = DotProduct( direction_, v );
    if( rDotProduct <= 0 )
        return p.SquareDistance( origin_ );
    if( rDotProduct >= rSquareLength_ )
        return std::numeric_limits< MT_Float >::max();
    const MT_Float rCrossProduct = direction_.rX_ * v.rY_ - direction_.rY_ * v.rX_;
    return std::fabs( rCrossProduct );
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit::Distance
// Created: AGE 2005-03-25
// -----------------------------------------------------------------------------
MT_Float MIL_Limit::SquareDistance( const MT_Vector2D& p ) const
{
    MT_Float rResult = std::numeric_limits< MT_Float >::max();
    for( CIT_DistanceDatas it = distanceDatas_.begin(); it != distanceDatas_.end(); ++it )
    {
        const DistanceData& data = *it;
        const MT_Float rSquareDistance = data.SquareDistance( p );
        if( rResult > rSquareDistance )
            rResult = rSquareDistance;
    }
    return rResult;
}

//=============================================================================
// TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_Limit::ReadPoints
// Created: NLD 2002-09-13
//-----------------------------------------------------------------------------
void MIL_Limit::ReadPoints( const ASN1T__SeqOfCoordUTM& listPoint )
{
    pointVector_.clear();   pointVector_.reserve( listPoint.n );
    distanceDatas_.clear(); distanceDatas_.reserve( listPoint.n );
    for( uint i = 0; i < listPoint.n; ++i )
    {
        MT_Vector2D vPosTmp;
        MIL_Tools::ConvertCoordMosToSim( listPoint.elem[i], vPosTmp );
        if( i > 0 )
            distanceDatas_.push_back( DistanceData( pointVector_.back(), vPosTmp ) );
        pointVector_.push_back( vPosTmp );
    }
    assert( !pointVector_.empty() );
}

//-----------------------------------------------------------------------------
// Name: MIL_Limit::AddPoint
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
void MIL_Limit::AddPoint( const MT_Vector2D& vPos )
{
    if( ! pointVector_.empty() )
        distanceDatas_.push_back( DistanceData( pointVector_.back(), vPos ) );
    pointVector_.push_back( vPos );
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit::SendFullState
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
void MIL_Limit::SendFullState() const
{
    assert( !pointVector_.empty() );
    ASN1T_CoordUTM* pCoord = new ASN1T_CoordUTM[ pointVector_.size() ];//$$$ RAM
    uint i = 0; 
    for( CIT_PointVector itPoint = pointVector_.begin(); itPoint != pointVector_.end(); ++itPoint )
        NET_ASN_Tools::WritePoint( *itPoint, pCoord[i++] );

    NET_ASN_MsgLimitCreation asnMsg;
    asnMsg.GetAsnMsg().oid                          = nID_;
    asnMsg.GetAsnMsg().level                        = (ASN1T_EnumNatureLevel)nLevel_;
    asnMsg.GetAsnMsg().geometrie.type               = EnumTypeLocalisation::line;
    asnMsg.GetAsnMsg().geometrie.vecteur_point.n    = pointVector_.size();
    asnMsg.GetAsnMsg().geometrie.vecteur_point.elem = pCoord;
    asnMsg.Send();

    delete [] pCoord;
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit::Destroy
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
void MIL_Limit::Destroy()
{
    bDestroyed_ = true;
    if( fuseauDatas_.empty() )
        delete this;
}
