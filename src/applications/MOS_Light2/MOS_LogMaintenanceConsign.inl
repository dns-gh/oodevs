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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogMaintenanceConsign.inl $
// $Author: Age $
// $Modtime: 1/04/05 11:06 $
// $Revision: 1 $
// $Workfile: MOS_LogMaintenanceConsign.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign::GetID
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
uint MOS_LogMaintenanceConsign::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign::GetPion
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
MOS_Agent& MOS_LogMaintenanceConsign::GetPion() const
{
    return pion_;
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign::GetPionLogHandling
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
MOS_Agent* MOS_LogMaintenanceConsign::GetPionLogHandling() const
{
    return pPionLogHandling_;
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign::GetEquipmentTypeID
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
uint MOS_LogMaintenanceConsign::GetEquipmentTypeID() const
{
    return nEquipmentTypeID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign::GetBreakdownTypeID
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
uint MOS_LogMaintenanceConsign::GetBreakdownTypeID() const
{
    return nBreakdownTypeID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign::GetState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
MOS_LogMaintenanceConsign::E_State MOS_LogMaintenanceConsign::GetState() const
{
    return nState_;
}

