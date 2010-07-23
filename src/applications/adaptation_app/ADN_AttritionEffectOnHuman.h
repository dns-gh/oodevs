// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-11-15 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_AttritionInfos.h $
// $Author: Nld $
// $Modtime: 20/07/05 14:11 $
// $Revision: 13 $
// $Workfile: ADN_AttritionInfos.h $
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
    MT_COPYNOTALLOWED( AttritionEffectOnHuman )

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
