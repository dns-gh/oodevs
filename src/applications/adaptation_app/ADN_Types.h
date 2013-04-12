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

typedef ADN_Type_ABC< bool >        ADN_Type_Bool;
typedef ADN_Type_ABC< int >         ADN_Type_Int;
typedef ADN_Type_ABC< double >      ADN_Type_Double;
typedef ADN_Type_ABC< std::string > ADN_Type_String;
typedef ADN_Type_ABC< std::string > ADN_Type_Time;
typedef ADN_Type_ABC< tools::Path > ADN_Type_Path;

#endif // __ADN_Types_h_
