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
// $Archive: /MVW_v10/Build/SDK/Light2/src/TacticalLine_ABC.inl $
// $Author: Ape $
// $Modtime: 28/07/04 14:48 $
// $Revision: 4 $
// $Workfile: TacticalLine_ABC.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::GetID
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
inline
MIL_LineID TacticalLine_ABC::GetID() const
{
    return nID_;
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::GetPointList
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
inline
CT_PointVector& TacticalLine_ABC::GetPointList() const
{
    return pointList_;
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::GetName
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
inline
const std::string TacticalLine_ABC::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::SetName
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
inline
void TacticalLine_ABC::SetName( const std::string strName )
{
    strName_ = strName;
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::SetState
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
inline
void TacticalLine_ABC::SetState( TacticalLine_ABC::E_State nState )
{
    nState_ = nState;
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::SetNetworkState
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
inline
void TacticalLine_ABC::SetNetworkState( TacticalLine_ABC::E_NetworkState nNetworkState )
{
    nNetworkState_ = nNetworkState;
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::IsUpdatingToSim
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
inline
bool TacticalLine_ABC::IsUpdatingToSim() const
{
    return nNetworkState_ != eNetworkStateRegistered;
}


// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::IsCreatedByMos
/** @return 
*/
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
inline
bool TacticalLine_ABC::IsCreatedByMos() const
{
    return bCreatedBy;
}

// -----------------------------------------------------------------------------
// Name: TacticalLine_ABC::GetLevel
// Created: HME 2005-01-25
// -----------------------------------------------------------------------------
inline
E_NatureLevel TacticalLine_ABC::GetLevel() const
{
    return nLevel_;
}