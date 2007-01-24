// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_MaintenanceResourcesAlarms_h_
#define __PHY_MaintenanceResourcesAlarms_h_

#include "MIL.h"

// =============================================================================
// Created: NLD 2006-08-02
// =============================================================================
class PHY_MaintenanceResourcesAlarms
{
public:
    //! @name Initialization
    //@{
    static void Initialize( MIL_InputArchive& archive );
    static void Terminate ();
    //@}

    //! @name Operations
    //@{
    static bool IsRepairerResourcesLevelReached( MT_Float rPreviousRatio, MT_Float rCurrentRatio );
    static bool IsHaulerResourcesLevelReached  ( MT_Float rPreviousRatio, MT_Float rCurrentRatio );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MT_Float >       T_LevelSet;
    typedef T_LevelSet::const_iterator CIT_LevelSet;
    //@}

private:
     PHY_MaintenanceResourcesAlarms();
    ~PHY_MaintenanceResourcesAlarms();

    //! @name Tools
    //@{
    static bool IsLevelReached( const T_LevelSet& levels, MT_Float rPreviousRatio, MT_Float rCurrentRatio );
    //@}

private:
    static T_LevelSet repairerResourcesLevels_;
    static T_LevelSet haulerResourcesLevels_;
};

#include "PHY_MaintenanceResourcesAlarms.inl"

#endif // __PHY_MaintenanceResourcesAlarms_h_
