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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogMedicalConsign.inl $
// $Author: Age $
// $Modtime: 1/04/05 11:07 $
// $Revision: 1 $
// $Workfile: MOS_LogMedicalConsign.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign::GetID
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
uint MOS_LogMedicalConsign::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign::GetPion
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
MOS_Agent& MOS_LogMedicalConsign::GetPion() const
{
    return pion_;
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign::GetPionLogHandling
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
MOS_Agent* MOS_LogMedicalConsign::GetPionLogHandling() const
{
    return pPionLogHandling_;
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign::GetState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
MOS_LogMedicalConsign::E_State MOS_LogMedicalConsign::GetState() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign::IsContaminated
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
bool MOS_LogMedicalConsign::IsContaminated() const
{
    return bContaminated_;
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign::IsMentalDeceased
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
bool MOS_LogMedicalConsign::IsMentalDeceased() const
{
    return bMentalDeceased_;
}
