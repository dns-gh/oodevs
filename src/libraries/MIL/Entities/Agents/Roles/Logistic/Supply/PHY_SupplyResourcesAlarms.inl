// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_SupplyResourcesAlarms::IsRepairerResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
inline
bool PHY_SupplyResourcesAlarms::IsConvoyTransporterResourcesLevelReached( MT_Float rPreviousRatio, MT_Float rCurrentRatio )
{
    return IsLevelReached( convoyTransporterResourcesLevels_, rPreviousRatio, rCurrentRatio );
}

