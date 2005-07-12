//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Lima.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 6 $
// $Workfile: MOS_Lima.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_Lima::GetLimaID
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
inline
MIL_LimaID MOS_Lima::GetID() const
{
    return nID_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima::GetListPoints
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
inline
CT_PointPtrVector& MOS_Lima::GetListPoints() const
{
    return pointVector_;
}

//-----------------------------------------------------------------------------
// Name: MOS_Lima::GetListPoints
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
inline
T_PointPtrVector& MOS_Lima::GetListPoints()
{
    return pointVector_;
}


//-----------------------------------------------------------------------------
// Name: std::string 
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
inline
const std::string MOS_Lima::GetName() const
{
    return sName_;
}
//-----------------------------------------------------------------------------
// Name: MOS_Lima::GetStateLima
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
inline
E_StateLima MOS_Lima::GetState() const
{
    return nStateLima_;
}

//-----------------------------------------------------------------------------
// Name: MOS_Lima::GetNetworkState
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
inline
E_StateNetworkLima MOS_Lima::GetNetworkState() const
{
    return nStateNetworkLima_;
}

//-----------------------------------------------------------------------------
// Name: MOS_Lima::SetNetworkState
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
inline
void MOS_Lima::SetNetworkState( E_StateNetworkLima nStateNetworkLima )
{
    nStateNetworkLima_ = nStateNetworkLima;
}

//-----------------------------------------------------------------------------
// Name: MOS_Lima::RegisterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
inline
void MOS_Lima::RegisterPoint( MT_Vector2D& vPoint )
{
    pointVector_.push_back( &vPoint );
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima::UnregisterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
inline
void MOS_Lima::UnregisterPoint( const MT_Vector2D& vPoint )
{
    IT_PointPtrVector itPoint = std::find( pointVector_.begin() , pointVector_.end() , &vPoint );
    assert( itPoint != pointVector_.end() );
    pointVector_.erase( itPoint );

}
