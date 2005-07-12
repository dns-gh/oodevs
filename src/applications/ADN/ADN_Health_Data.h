// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Health_Data.h $
// $Author: Ape $
// $Modtime: 7/04/05 11:21 $
// $Revision: 2 $
// $Workfile: ADN_Health_Data.h $
//
// *****************************************************************************

#ifndef __ADN_Health_Data_h_
#define __ADN_Health_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"

class ADN_XmlInput_Helper;


// =============================================================================
/** @class  ADN_Health_Data
    @brief  ADN_Health_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_Health_Data
    : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_Health_Data )

public:
// *****************************************************************************
    class WoundInfo
    {
        MT_COPYNOTALLOWED( WoundInfo )
    public:
        WoundInfo();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        E_DoctorSkills  nType_;
        ADN_Type_Double rLifeExpectancy_;
        ADN_Type_Double rTreatTime_;
        ADN_Type_Double rPercentage_;
    };


// *****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
     ADN_Health_Data();
    ~ADN_Health_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( T_StringList& vFiles ) const;
    void Reset();
    //@}

private:
    void ReadArchive( ADN_XmlInput_Helper& input );
    void WriteArchive( MT_OutputArchive_ABC& output );

public:
    ADN_Type_Double rDiagnosticTime_;
    ADN_Type_Double rSortingTime_;

    WoundInfo wounds[eNbrDoctorSkills];

    ADN_Type_Double rShockTreatTime_;
    ADN_Type_Double rShockPercentage_;

    ADN_Type_Double rContaminationTreatTime_;
};


#endif // __ADN_Health_Data_h_
