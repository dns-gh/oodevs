// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Types_h_
#define __ADN_Types_h_

#include "ADN_Type_ABC.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_TypePtr_InVector_ABC.h"
#include "ADN_Type_Enum.h"

class QWidget;

typedef ADN_Type_ABC<bool>          ADN_Type_Bool;
typedef ADN_Type_ABC<int>           ADN_Type_Int;
typedef ADN_Type_ABC<double>        ADN_Type_Double;
typedef ADN_Type_ABC<std::string>   ADN_Type_String;
typedef ADN_Type_ABC<std::string>   ADN_Type_Time;

#define TYPEDEF_DECLARATION( BaseType, NewName )                        \
    typedef BaseType T_ ## NewName;

#define TYPEDEF_WITH_ITERATOR_DECLARATION( BaseType, NewName )          \
    TYPEDEF_DECLARATION( BaseType, NewName )                            \
    typedef BaseType::iterator IT_ ## NewName;

#define TYPEDEF_WITH_CONST_ITERATOR_DECLARATION( BaseType, NewName )    \
    TYPEDEF_DECLARATION( BaseType, NewName )                            \
    typedef BaseType::const_iterator CIT_ ## NewName;

#define TYPEDEF_FULL_DECLARATION( BaseType, NewName )                   \
    TYPEDEF_DECLARATION( BaseType, NewName )                            \
    typedef BaseType::iterator IT_ ## NewName;                          \
    typedef BaseType::const_iterator CIT_ ## NewName;

TYPEDEF_FULL_DECLARATION( std::vector<QWidget*>, PtrWidgetVector )
TYPEDEF_FULL_DECLARATION( std::list<std::string>, StringList )

#endif // __ADN_Types_h_
