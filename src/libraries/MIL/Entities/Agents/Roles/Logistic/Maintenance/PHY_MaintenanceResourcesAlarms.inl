// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceResourcesAlarms::IsRepairerResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
inline
bool PHY_MaintenanceResourcesAlarms::IsRepairerResourcesLevelReached( MT_Float rPreviousRatio, MT_Float rCurrentRatio )
{
    return IsLevelReached( repairerResourcesLevels_, rPreviousRatio, rCurrentRatio );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceResourcesAlarms::IsHaulerResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
inline
bool PHY_MaintenanceResourcesAlarms::IsHaulerResourcesLevelReached( MT_Float rPreviousRatio, MT_Float rCurrentRatio )
{
    return IsLevelReached( haulerResourcesLevels_, rPreviousRatio, rCurrentRatio );
}

