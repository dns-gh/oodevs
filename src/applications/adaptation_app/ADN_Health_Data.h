// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Health_Data_h_
#define __ADN_Health_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_AvailabilityWarning.h"

namespace xml { class xistream; }

// =============================================================================
/** @class  ADN_Health_Data
    @brief  ADN_Health_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_Health_Data : public ADN_Data_ABC
{

public:
// *****************************************************************************
    class WoundInfo
    {
    public:
        WoundInfo();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        E_DoctorSkills  nType_;
        ADN_Type_Time   lifeExpectancy_;
        ADN_Type_Time   treatTime_;
        ADN_Type_Time   restingTime_;
        ADN_Type_Double rPercentage_;
    };


// *****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Health_Data();
    virtual ~ADN_Health_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( T_StringList& vFiles ) const;
    void Reset();
    //@}

private:
    void ReadArchive( xml::xistream& input );
    void ReadInjury ( xml::xistream& input );
    void ReadResourceAvailability ( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

public:
    ADN_Type_Time   diagnosticTime_;
    ADN_Type_Time   sortingTime_;

    WoundInfo wounds[eNbrDoctorSkills];

    ADN_Type_Time   shockTreatTime_;
    ADN_Type_Time   shockRestingTime_;
    ADN_Type_Double rShockPercentage_;

    ADN_Type_Time   contaminationTreatTime_;
    ADN_Type_Time   contaminationRestingTime_;
    ADN_Type_Double diagnosisLifeExpectancyFactor_;

    T_AvailabilityWarning_Vector vChangeOverWarnings_;
    T_AvailabilityWarning_Vector vCollectingWarnings_;
    T_AvailabilityWarning_Vector vDoctorsWarnings_;
};


#endif // __ADN_Health_Data_h_
