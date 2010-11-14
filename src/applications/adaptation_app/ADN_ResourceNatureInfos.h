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

#ifndef __ADN_ResourceNatureInfos_h_
#define __ADN_ResourceNatureInfos_h_

#include "ADN_Types.h"
#include "ADN_Enums.h"

namespace helpers
{

class ResourceNatureInfos
    : public ADN_Type_String
{
    MT_COPYNOTALLOWED( ResourceNatureInfos )

public:
    ResourceNatureInfos();
    ResourceNatureInfos( const std::string name, int id );
    std::string GetItemName();
    void SetId( int id );
    int GetId();

public:
    ADN_Type_String strName_; // $$$ Dummy for  ADN_ChangeValueCommand_Pointer usage
    ADN_Type_Int id_;
};

typedef ADN_Type_Vector_ABC< ResourceNatureInfos > T_ResourceNatureInfos_Vector;
typedef T_ResourceNatureInfos_Vector::iterator    IT_ResourceNatureInfos_Vector;

// -----------------------------------------------------------------------------
// Name: ArmorInfos::GetItemName
// Created: APE 2004-11-10
// -----------------------------------------------------------------------------
inline
std::string ResourceNatureInfos::GetItemName()
{
    return std::string();
}

}

inline
std::ostream& operator<<( std::ostream& os, helpers::ResourceNatureInfos& value )
{
    os << value.GetData();
    return os;
}

#endif // __ADN_ResourceNatureInfos_h_
