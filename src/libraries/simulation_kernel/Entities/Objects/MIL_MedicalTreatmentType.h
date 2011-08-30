// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_MedicalTreatmentType_h_
#define __MIL_MedicalTreatmentType_h_

#include "MT_Tools/MT_Stl.h"
#include <map>
#include <set>

namespace xml
{
    class xistream;
}

class MIL_Time_ABC;

// =============================================================================
// Created: RFT 22/04/2008
// MIL_MedicalTreatmentType.h
// Modified: RFT 19/05/2008
// =============================================================================

class MIL_MedicalTreatmentType
{
public:
    enum E_InjuryCategories
    {
        eNone = 0,
        eUA,
        eUR,
        eDead
    };

public:
    //! @name Factory
    //@{
    static void Initialize( xml::xistream& xis, const MIL_Time_ABC& time );
    static void Terminate();

    static const MIL_MedicalTreatmentType* Find( const std::string& strName );
    static const MIL_MedicalTreatmentType* Find( unsigned int nID );
    static unsigned                        RegisteredCount();
    //@}

private:
    //!@ Constructor and destructor
    //@{
     MIL_MedicalTreatmentType( const std::string& strName, xml::xistream& xis, const MIL_Time_ABC& time );
    ~MIL_MedicalTreatmentType();
    //@}

public:
    //! @name
    //@{
    unsigned int       GetID() const;
    const std::string& GetName() const;
    unsigned int       GetDeathThreshold() const;
    float              GetTreatmentTime( int injuryCategory ) const;
    float              GetHospitalisationTime( int injuryCategory ) const;
    float              GetLifeExpectancy( E_InjuryCategories injuryCategory ) const;
    unsigned int       GetInjuryThreshold( E_InjuryCategories injuryCategory ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_MedicalTreatmentType* > T_MedicalTreatmentTypeMap;
    typedef T_MedicalTreatmentTypeMap::const_iterator                CIT_MedicalTreatmentTypeMap;

    struct InjuryTreatment
    {
        InjuryTreatment() : treatmentTime_( 0 ), hospitalisationTime_( 0 ), lifeExpectancy_( -1 ), injuryThreshold_ ( eNone ) {}

        float             treatmentTime_;
        float             hospitalisationTime_;
        float             lifeExpectancy_;
        unsigned int      injuryThreshold_;
    };

    typedef std::vector< InjuryTreatment >              T_InjuryTreatments;
    //@}

private:
    //! @name Assignement
    //@{
    MIL_MedicalTreatmentType& operator=( const MIL_MedicalTreatmentType& );
    //@}

    //! @name Helpers
    //@{
    static void ReadMedicalTreatment( xml::xistream& xis, const MIL_Time_ABC& time, std::set< unsigned int >& ids );
           void ReadMedicalTreatmentEffect( xml::xistream& xis );
           void ReadInjury( xml::xistream& xis, InjuryTreatment& injury );
    //@}

private:
    //! @name Data
    //@{
    const std::string   name_;
    unsigned int        nID_;
    int                 deathThreshold_;
    const MIL_Time_ABC& time_;
    T_InjuryTreatments  treatments_;
    //@}

    //! @name Static data
    //@{
    static T_MedicalTreatmentTypeMap    types_;
    //@}
};

#endif // __MIL_MedicalTreatmentType_h_