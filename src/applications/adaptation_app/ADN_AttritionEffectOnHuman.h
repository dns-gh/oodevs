// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_AttritionEffectOnHuman_h_
#define __ADN_AttritionEffectOnHuman_h_

#include "ADN_Type_Enum.h"
#include "ADN_Enums.h"
#include "ADN_Types.h"

namespace helpers
{

class AttritionEffectOnHuman
    : public ADN_Ref_ABC
    , public ADN_DataTreeNode_ABC
{

public:
    AttritionEffectOnHuman();

    std::string GetItemName();

    void ReadArchive( xml::xistream& );
    void WriteArchive( xml::xostream& );

public:
    ADN_Type_Enum< E_EquipmentState, eNbrEquipmentState > nEquipmentState_;
    ADN_Type_Int nInjuredPercentage_;
    ADN_Type_Int nDeadPercentage_;
    ADN_Type_String strName_;
};

typedef ADN_Type_Vector_ABC<AttritionEffectOnHuman>   T_AttritionEffectOnHuman_Vector;
typedef T_AttritionEffectOnHuman_Vector::iterator    IT_AttritionEffectOnHuman_Vector;

}

#endif // __ADN_AttritionEffectOnHuman_h_
