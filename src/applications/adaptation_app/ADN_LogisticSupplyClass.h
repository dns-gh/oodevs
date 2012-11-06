// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_LogisticSupplyClass_h_
#define __ADN_LogisticSupplyClass_h_

#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"

namespace helpers
{

class LogisticSupplyClass : public ADN_RefWithName
{
public:
    LogisticSupplyClass();
    LogisticSupplyClass( const std::string& name, int id );
    LogisticSupplyClass* CreateCopy();
    std::string GetItemName() { return strName_.GetData(); }

public:
    ADN_Type_Int nId_;
};

typedef ADN_Type_Vector_ABC< LogisticSupplyClass > T_LogisticSupplyClass_Vector;
typedef T_LogisticSupplyClass_Vector::iterator    IT_LogisticSupplyClass_Vector;

}

inline
std::ostream& operator<<( std::ostream& os, helpers::LogisticSupplyClass& value )
{
    os << value.strName_.GetData();
    return os;
}

#endif // __ADN_LogisticSupplyClass_h_
