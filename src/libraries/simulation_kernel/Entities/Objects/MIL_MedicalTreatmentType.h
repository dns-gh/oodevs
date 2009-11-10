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

#include "MIL.h"
#include <map>

namespace xml
{
    class xistream;
}

// =============================================================================
// Created: RFT 22/04/2008
// MIL_MedicalTreatmentType.h
// Modified: RFT 19/05/2008
// =============================================================================

class MIL_MedicalTreatmentType
{
public:
    enum E_InjuryCategories{
        eNone = 0,
        eUA,
        eUR,
        eDead,
    };

public:	
    //! @name Factory
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate();

    static const MIL_MedicalTreatmentType* Find( const std::string& strName );
    static const MIL_MedicalTreatmentType* Find( uint nID );
    //@}

private:
	//!@ Constructor and destructor
	//@{   
    MIL_MedicalTreatmentType( const std::string& strName, xml::xistream& xis );
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
    typedef std::map< std::string, const MIL_MedicalTreatmentType*, sCaseInsensitiveLess > T_MedicalTreatmentTypeMap;
    typedef T_MedicalTreatmentTypeMap::const_iterator                                      CIT_MedicalTreatmentTypeMap;
    
    struct T_InjuryDescription {
        T_InjuryDescription() : treatmentTime_( 0 ), hospitalisationTime_( 0 ), lifeExpectancy_( -1 ), injuryThreshold_( eNone ) {}
        float             treatmentTime_;
        float             hospitalisationTime_;
        float             lifeExpectancy_;
        unsigned int      injuryThreshold_;
    };

    typedef std::map< E_InjuryCategories, T_InjuryDescription > T_MedicalTreatmentEffectMap;
    typedef T_MedicalTreatmentEffectMap::const_iterator         CIT_MedicalTreatmentEffectMap;
    //@}

private:
    //! @name Helpers
    //@{
    static void ReadMedicalTreatment( xml::xistream& xis );    
           void ReadMedicalTreatmentEffect( xml::xistream& xis );
    //@}

private:
    const std::string           strName_;
    unsigned int                nID_;
    int                         deathThreshold_;
    T_MedicalTreatmentEffectMap medicalTreatmentEffect_;
    
    static T_MedicalTreatmentTypeMap    types_;
};

#endif // __MIL_MedicalTreatmentType_h_