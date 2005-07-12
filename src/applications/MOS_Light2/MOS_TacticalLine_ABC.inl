// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-14 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_TacticalLine_ABC.inl $
// $Author: Ape $
// $Modtime: 28/07/04 14:48 $
// $Revision: 4 $
// $Workfile: MOS_TacticalLine_ABC.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_TacticalLine_ABC::GetID
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
MOS_INLINE
MIL_LineID MOS_TacticalLine_ABC::GetID() const
{
    return nID_;
}


// -----------------------------------------------------------------------------
// Name: MOS_TacticalLine_ABC::GetPointList
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
MOS_INLINE
CT_PointVector& MOS_TacticalLine_ABC::GetPointList() const
{
    return pointList_;
}


// -----------------------------------------------------------------------------
// Name: MOS_TacticalLine_ABC::GetName
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
MOS_INLINE
const std::string MOS_TacticalLine_ABC::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: MOS_TacticalLine_ABC::SetName
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_TacticalLine_ABC::SetName( const std::string strName )
{
    strName_ = strName;
}


// -----------------------------------------------------------------------------
// Name: MOS_TacticalLine_ABC::SetState
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_TacticalLine_ABC::SetState( MOS_TacticalLine_ABC::E_State nState )
{
    nState_ = nState;
}


// -----------------------------------------------------------------------------
// Name: MOS_TacticalLine_ABC::SetNetworkState
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_TacticalLine_ABC::SetNetworkState( MOS_TacticalLine_ABC::E_NetworkState nNetworkState )
{
    nNetworkState_ = nNetworkState;
}


// -----------------------------------------------------------------------------
// Name: MOS_TacticalLine_ABC::IsUpdatingToSim
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_TacticalLine_ABC::IsUpdatingToSim() const
{
    return nNetworkState_ != eNetworkStateRegistered;
}


// -----------------------------------------------------------------------------
// Name: MOS_TacticalLine_ABC::IsCreatedByMos
/** @return 
*/
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_TacticalLine_ABC::IsCreatedByMos() const
{
    return bCreatedByMOS_;
}
