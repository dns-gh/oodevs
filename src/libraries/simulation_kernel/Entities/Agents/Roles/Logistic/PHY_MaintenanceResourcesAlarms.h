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
    static bool IsRepairerResourcesLevelReached( double rPreviousRatio, double rCurrentRatio );
    static bool IsHaulerResourcesLevelReached  ( double rPreviousRatio, double rCurrentRatio );
    //@}
};

#endif // __PHY_MaintenanceResourcesAlarms_h_
