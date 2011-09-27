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

#include "ADN_Types.h"
#include "ADN_Enums.h"

namespace helpers
{

class LogisticSupplyClass : public ADN_Type_String
{

public:
    LogisticSupplyClass();
    LogisticSupplyClass( const std::string name, int id );
    std::string GetItemName();
    void SetId( int id );
    int GetId();

public:
    ADN_Type_String strName_; // $$$ Dummy for  ADN_ChangeValueCommand_Pointer usage
    ADN_Type_Int id_;
};

typedef ADN_Type_Vector_ABC< LogisticSupplyClass > T_LogisticSupplyClass_Vector;
typedef T_LogisticSupplyClass_Vector::iterator    IT_LogisticSupplyClass_Vector;

// -----------------------------------------------------------------------------
// Name: ArmorInfos::GetItemName
// Created: APE 2004-11-10
// -----------------------------------------------------------------------------
inline
std::string LogisticSupplyClass::GetItemName()
{
    return std::string();
}

}

inline
std::ostream& operator<<( std::ostream& os, helpers::LogisticSupplyClass& value )
{
    os << value.GetData();
    return os;
}

#endif // __ADN_LogisticSupplyClass_h_
