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
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogMaintenanceConsign.inl $
// $Author: Age $
// $Modtime: 1/04/05 11:06 $
// $Revision: 1 $
// $Workfile: LogMaintenanceConsign.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetID
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
uint LogMaintenanceConsign::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetPion
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
Agent& LogMaintenanceConsign::GetPion() const
{
    return pion_;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetPionLogHandling
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
Agent* LogMaintenanceConsign::GetPionLogHandling() const
{
    return pPionLogHandling_;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetEquipmentTypeID
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
uint LogMaintenanceConsign::GetEquipmentTypeID() const
{
    return nEquipmentTypeID_;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetBreakdownTypeID
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
inline
uint LogMaintenanceConsign::GetBreakdownTypeID() const
{
    return nBreakdownTypeID_;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
LogMaintenanceConsign::E_State LogMaintenanceConsign::GetState() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::IsDiagnosed
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
inline
bool LogMaintenanceConsign::IsDiagnosed() const
{
    return bDiagnosed_;
}
