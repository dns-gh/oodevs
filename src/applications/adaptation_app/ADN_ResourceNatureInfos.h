// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ResourceNatureInfos_h_
#define __ADN_ResourceNatureInfos_h_

#include "ADN_Types.h"
#include "ADN_Enums.h"

namespace helpers
{

class ResourceNatureInfos : public ADN_Ref_ABC
                          , public ADN_DataTreeNode_ABC
{

public:
    ResourceNatureInfos();
    ResourceNatureInfos( const std::string name, int id );
    std::string GetItemName();
    void SetId( int id );
    int GetId();
    ResourceNatureInfos* CreateCopy();

public:
    ADN_Type_String strName_;
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
    os << value.strName_.GetData();
    return os;
}

#endif // __ADN_ResourceNatureInfos_h_
