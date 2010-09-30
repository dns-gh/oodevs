// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_MaintenanceResourcesAlarms_h_
#define __PHY_MaintenanceResourcesAlarms_h_

#include "MT_Tools/MT_Tools_Types.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// Created: NLD 2006-08-02
// =============================================================================
class PHY_MaintenanceResourcesAlarms
{
public:
    //! @name Initialization
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();
    //@}

    //! @name Operations
    //@{
    static bool IsRepairerResourcesLevelReached( MT_Float rPreviousRatio, MT_Float rCurrentRatio );
    static bool IsHaulerResourcesLevelReached  ( MT_Float rPreviousRatio, MT_Float rCurrentRatio );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
     PHY_MaintenanceResourcesAlarms();
    virtual ~PHY_MaintenanceResourcesAlarms();
    //@}

    //! @name Types
    //@{
    typedef std::set< MT_Float >         T_LevelSet;
    typedef T_LevelSet::const_iterator CIT_LevelSet;
    //@}

    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadResource( xml::xistream& xis );
    static bool IsLevelReached( const T_LevelSet& levels, MT_Float rPreviousRatio, MT_Float rCurrentRatio );
    //@}

private:
    static T_LevelSet repairerResourcesLevels_;
    static T_LevelSet haulerResourcesLevels_;
};

#endif // __PHY_MaintenanceResourcesAlarms_h_
