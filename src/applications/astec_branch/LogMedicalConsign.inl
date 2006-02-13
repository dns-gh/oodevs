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
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogMedicalConsign.inl $
// $Author: Age $
// $Modtime: 1/04/05 11:07 $
// $Revision: 1 $
// $Workfile: LogMedicalConsign.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetID
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
uint LogMedicalConsign::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetPion
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
Agent& LogMedicalConsign::GetPion() const
{
    return pion_;
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetPionLogHandling
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
Agent* LogMedicalConsign::GetPionLogHandling() const
{
    return pPionLogHandling_;
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
LogMedicalConsign::E_State LogMedicalConsign::GetState() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::IsContaminated
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
bool LogMedicalConsign::IsContaminated() const
{
    return bContaminated_;
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::IsMentalDeceased
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
bool LogMedicalConsign::IsMentalDeceased() const
{
    return bMentalDeceased_;
}
