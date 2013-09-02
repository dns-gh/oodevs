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

namespace xml
{
    class xistream;
}

// =============================================================================
// Created: NLD 2006-08-02
// =============================================================================
class PHY_SupplyResourcesAlarms
{
public:
    //! @name Initialization
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();
    //@}

    //! @name Operations
    //@{
    static bool IsConvoyTransporterResourcesLevelReached( double rPreviousRatio, double rCurrentRatio );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< double >       T_LevelSet;
    typedef T_LevelSet::const_iterator CIT_LevelSet;
    //@}

private:
     PHY_SupplyResourcesAlarms();
    virtual ~PHY_SupplyResourcesAlarms();

    //! @name Tools
    //@{
    static bool IsLevelReached( const T_LevelSet& levels, double rPreviousRatio, double rCurrentRatio );
    //@}
    //! @name Helpers
    //@{
    static void ReadResourceAvailabilityAlert ( xml::xistream& xis );
    //@}

private:
    static T_LevelSet convoyTransporterResourcesLevels_;
};

#endif // __PHY_SupplyResourcesAlarms_h_
