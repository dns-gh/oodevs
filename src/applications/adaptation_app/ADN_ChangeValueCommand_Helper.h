// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-13 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_ChangeValueCommand_Helper.h $
// $Author: Ape $
// $Modtime: 21/10/04 17:47 $
// $Revision: 1 $
// $Workfile: ADN_ChangeValueCommand_Helper.h $
//
// *****************************************************************************

#ifndef __ADN_ChangeValueCommand_Helper_h_
#define __ADN_ChangeValueCommand_Helper_h_

#include "qtundo.h"
#include "ADN_Type_ABC.h"

template< class ValueType >
class ADN_ChangeValueCommand_Chooser
{
public:
    static QtCommand* CreateCommand( ADN_Type_ABC< ValueType >&, ValueType, ValueType );
};

QtCommand* ADN_CreateCommand( ADN_Type_ABC< bool >&, bool, bool );
QtCommand* ADN_CreateCommand( ADN_Type_ABC< int >&, int, int );
QtCommand* ADN_CreateCommand( ADN_Type_ABC< double >&, double, double );
QtCommand* ADN_CreateCommand( ADN_Type_ABC< std::string >&, const std::string&, const std::string& );

#endif // __ADN_ChangeValueCommand_Helper_h_
