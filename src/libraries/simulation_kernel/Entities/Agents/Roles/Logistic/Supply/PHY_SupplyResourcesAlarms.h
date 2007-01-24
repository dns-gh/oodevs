// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_SupplyResourcesAlarms_h_
#define __PHY_SupplyResourcesAlarms_h_

#include "MIL.h"

// =============================================================================
// Created: NLD 2006-08-02
// =============================================================================
class PHY_SupplyResourcesAlarms
{
public:
    //! @name Initialization
    //@{
    static void Initialize( MIL_InputArchive& archive );
    static void Terminate ();
    //@}

    //! @name Operations
    //@{
    static bool IsConvoyTransporterResourcesLevelReached( MT_Float rPreviousRatio, MT_Float rCurrentRatio );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MT_Float >       T_LevelSet;
    typedef T_LevelSet::const_iterator CIT_LevelSet;
    //@}

private:
     PHY_SupplyResourcesAlarms();
    ~PHY_SupplyResourcesAlarms();

    //! @name Tools
    //@{
    static bool IsLevelReached( const T_LevelSet& levels, MT_Float rPreviousRatio, MT_Float rCurrentRatio );
    //@}

private:
    static T_LevelSet convoyTransporterResourcesLevels_;
};

#include "PHY_SupplyResourcesAlarms.inl"

#endif // __PHY_SupplyResourcesAlarms_h_
