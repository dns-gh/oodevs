// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-02-09 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_ENT_Tr_Skeleton.h $
// $Author: Ape $
// $Modtime: 3/03/05 16:59 $
// $Revision: 3 $
// $Workfile: AGR_ENT_Tr_Skeleton.h $
//
// *****************************************************************************

#ifndef __ENT_Tr_Gen_h_
#define __ENT_Tr_Gen_h_

#include "ENT_Tr_ABC.h"
#include "ENT_Enums_Gen.h"

// =============================================================================
/** @class  AGR_ENT_Tr_Skeleton
    @brief  AGR_ENT_Tr_Skeleton
*/
// Created: APE 2005-02-09
// =============================================================================
class ENT_Tr
: public ENT_Tr_ABC
{
public:
    static void InitTranslations();

public:
    // ConvertFrom functions 
$ConvertFromFunctions$

    // ConvertTo functions
$ConvertToFunctions$

private:
    // Typedefs
$Typdefs$

    // Converters
$Converters$
};


#endif // __ENT_Tr_Gen_h_