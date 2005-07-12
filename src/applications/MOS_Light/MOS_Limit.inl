//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Limit.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 6 $
// $Workfile: MOS_Limit.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_Limit::IsLocal
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
inline
bool MOS_Limit::IsLocal() const
{
    return bLocal_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Limit::GetLimitID
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
inline
MIL_LimitID MOS_Limit::GetID() const
{
    return nID_;
}

//-----------------------------------------------------------------------------
// Name: MOS_Limit::GetListPoints
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
inline
CT_PointPtrVector& MOS_Limit::GetListPoints() const
{
    return pointVector_;
}

//-----------------------------------------------------------------------------
// Name: MOS_Limit::GetListPoints
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
inline
T_PointPtrVector& MOS_Limit::GetListPoints()
{
    return pointVector_;
}


//-----------------------------------------------------------------------------
// Name: std::string 
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
inline
const std::string MOS_Limit::GetName() const
{
    return sName_;
}
//-----------------------------------------------------------------------------
// Name: MOS_Limit::GetStateLimit
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
inline
E_StateLimit MOS_Limit::GetState() const
{
    return nStateLimit_;
}

//-----------------------------------------------------------------------------
// Name: MOS_Limit::GetNetworkState
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
inline
E_StateNetworkLimit MOS_Limit::GetNetworkState() const
{
    return nStateNetworkLimit_;
}

//-----------------------------------------------------------------------------
// Name: MOS_Limit::SetNetworkState
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
inline
void MOS_Limit::SetNetworkState( E_StateNetworkLimit nStateNetworkLimit )
{
    nStateNetworkLimit_ = nStateNetworkLimit;
}

//-----------------------------------------------------------------------------
// Name: MOS_Limit::RegisterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
inline
void MOS_Limit::RegisterPoint( MT_Vector2D& vPoint )
{
    pointVector_.push_back( &vPoint );
}


//-----------------------------------------------------------------------------
// Name: MOS_Limit::UnregisterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
inline
void MOS_Limit::UnregisterPoint( const MT_Vector2D& vPoint )
{
    IT_PointPtrVector itPoint = std::find( pointVector_.begin() , pointVector_.end() , &vPoint );
    assert( itPoint != pointVector_.end() );
    pointVector_.erase( itPoint );

}
