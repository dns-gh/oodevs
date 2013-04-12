// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ArmorInfos_h_
#define __ADN_ArmorInfos_h_

#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_AttritionEffectOnHuman.h"

namespace helpers
{

class ArmorInfos : public ADN_RefWithName
{

public:
    ArmorInfos();
    std::string GetItemName();

    ArmorInfos* CreateCopy();
    void ReadArchive( xml::xistream& );
    void ReadAttrition( xml::xistream& );
    void WriteArchive( xml::xostream& );
    void CreateDefaultAttrition();

public:
    ADN_Type_Enum< E_ProtectionType, eNbrProtectionType > nType_;
    ADN_Type_Time   neutralizationAverageTime_;
    ADN_Type_Time   neutralizationVariance_;
    ADN_Type_Double rBreakdownEVA_;
    ADN_Type_Double rBreakdownNEVA_;
    T_AttritionEffectOnHuman_Vector vAttritionEffects_;
};

typedef ADN_Type_Vector_ABC< ArmorInfos > T_ArmorInfos_Vector;

// -----------------------------------------------------------------------------
// Name: ArmorInfos::GetItemName
// Created: APE 2004-11-10
// -----------------------------------------------------------------------------
inline
std::string ArmorInfos::GetItemName()
{
    return std::string();
}

}

#endif // __ADN_ArmorInfos_h_
