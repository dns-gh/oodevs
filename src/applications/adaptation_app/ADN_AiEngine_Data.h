// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_AiEngine_Data_h_
#define __ADN_AiEngine_Data_h_

#include "ADN_Data_ABC.h"

#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_TypePtr_InVector_ABC.h"
#include "ADN_Enums.h"

class xml::xistream;

// =============================================================================
/** @class  ADN_AiEngine_Data
    @brief  ADN_AiEngine_Data
*/
// Created: AGN 2004-06-15
// =============================================================================
class ADN_AiEngine_Data : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_AiEngine_Data )

public:
    //! @name Constructors/Destructor
    //@{
             ADN_AiEngine_Data();
    virtual ~ADN_AiEngine_Data();
    //@}

    void Reset();
    void FilesNeeded(T_StringList& l) const;

private:
    void ReadArchive( xml::xistream& );
    void WriteArchive( xml::xostream& );

public:
    ADN_Type_Double rPertinenceMaxDecrease_;
    ADN_Type_Double rOperationalStateMaxDecrease_;
    ADN_Type_Double rNeutralizedStateMaxDecrease_;
    ADN_Type_Double rMinorEquipmentWeight_;
    ADN_Type_Double rMajorEquipmentWeight_;
    ADN_Type_Double rHumanWeight_;
    ADN_Type_Time rDefaultFeedbackTime_;
};


#endif // __ADN_AiEngine_Data_h_
