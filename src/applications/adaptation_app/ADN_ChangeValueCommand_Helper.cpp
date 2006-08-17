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
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_ChangeValueCommand_Helper.cpp $
// $Author: Ape $
// $Modtime: 21/10/04 17:47 $
// $Revision: 1 $
// $Workfile: ADN_ChangeValueCommand_Helper.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_ChangeValueCommand_Helper.h"

#include "ADN_ChangeValueCommand_Bool.h"
#include "ADN_ChangeValueCommand_Int.h"
#include "ADN_ChangeValueCommand_Double.h"
#include "ADN_ChangeValueCommand_String.h"

QtCommand* ADN_CreateCommand( ADN_Type_ABC< bool >& data, bool oVal , bool nVal )
{
    return new ADN_ChangeValueCommand_Bool( data, oVal, nVal );
}

QtCommand* ADN_CreateCommand( ADN_Type_ABC< int >& data, int oVal , int nVal )
{
    return new ADN_ChangeValueCommand_Int( data, oVal, nVal );
}

QtCommand* ADN_CreateCommand( ADN_Type_ABC< double >& data, double oVal, double nVal )
{
    return new ADN_ChangeValueCommand_Double( data, oVal, nVal );
    
}

QtCommand* ADN_CreateCommand( ADN_Type_ABC< std::string >& data, const std::string& oVal, const std::string& nVal )
{
    return new ADN_ChangeValueCommand_String( data, oVal, nVal );
}
