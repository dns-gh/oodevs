// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_MedicalResourcesAlarms_h_
#define __PHY_MedicalResourcesAlarms_h_

namespace xml
{
    class xistream;
}

// =============================================================================
// Created: NLD 2006-08-02
// =============================================================================
class PHY_MedicalResourcesAlarms
{
public:
    //! @name Initialization
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();
    //@}

    //! @name Operations
    //@{
    static bool IsEvacuationResourcesLevelReached( double rPreviousRatio, double rCurrentRatio );
    static bool IsCollectionResourcesLevelReached( double rPreviousRatio, double rCurrentRatio );
    static bool IsDoctorResourcesLevelReached    ( double rPreviousRatio, double rCurrentRatio );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< double >       T_LevelSet;
    typedef T_LevelSet::const_iterator CIT_LevelSet;
    //@}
    //! @name Helpers
    //@{
    static void ReadResourceLevel( xml::xistream& xis );
    //@}

private:
     PHY_MedicalResourcesAlarms();
    virtual ~PHY_MedicalResourcesAlarms();

    //! @name Tools
    //@{
    static bool IsLevelReached( const T_LevelSet& levels, double rPreviousRatio, double rCurrentRatio );
    //@}

private:
    static T_LevelSet evacuationResourcesLevels_;
    static T_LevelSet collectionResourcesLevels_;
    static T_LevelSet doctorsResourcesLevels_;
};

#endif // __PHY_MedicalResourcesAlarms_h_
