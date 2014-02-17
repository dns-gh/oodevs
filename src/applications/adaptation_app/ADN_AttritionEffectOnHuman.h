// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_AttritionEffectOnHuman_h_
#define __ADN_AttritionEffectOnHuman_h_

#include "ADN_RefWithName.h"
#include "ADN_Type_Enum.h"
#include "ADN_Enums.h"
#include "ADN_Types.h"

namespace helpers
{

class AttritionEffectOnHuman : public ADN_RefWithName
{

public:
    AttritionEffectOnHuman();

    std::string GetItemName();
    AttritionEffectOnHuman* CreateCopy();

    void ReadArchive( xml::xistream& );
    void WriteArchive( xml::xostream& ) const;

public:
    ADN_Type_Enum< E_EquipmentState_ADN, eNbrEquipmentState_ADN > nEquipmentState_;
    ADN_Type_Int nInjuredPercentage_;
    ADN_Type_Int nDeadPercentage_;
};

typedef ADN_Type_Vector_ABC< AttritionEffectOnHuman > T_AttritionEffectOnHuman_Vector;

}

#endif // __ADN_AttritionEffectOnHuman_h_
