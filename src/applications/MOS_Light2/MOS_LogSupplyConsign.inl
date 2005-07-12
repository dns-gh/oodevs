// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogSupplyConsign.inl $
// $Author: Age $
// $Modtime: 1/04/05 11:07 $
// $Revision: 1 $
// $Workfile: MOS_LogSupplyConsign.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign::GetID
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
uint MOS_LogSupplyConsign::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign::GetPion
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
MOS_Agent& MOS_LogSupplyConsign::GetPion() const
{
    return pion_;
}

// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign::GetPionLogHandling
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
MOS_Agent* MOS_LogSupplyConsign::GetPionLogHandling() const
{
    return pPionLogHandling_;
}

// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign::GetPionLogConvoying
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
inline
MOS_Agent* MOS_LogSupplyConsign::GetPionLogConvoying() const
{
    return pPionLogConvoying_;
}

// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign::GetState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
MOS_LogSupplyConsign::E_State MOS_LogSupplyConsign::GetState() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign::GetDotations
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
inline
const MOS_LogSupplyConsign::T_DotationMap& MOS_LogSupplyConsign::GetDotations() const
{
    return dotations_;
}
