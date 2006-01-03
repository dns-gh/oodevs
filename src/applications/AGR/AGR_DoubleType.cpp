// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-13 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_DoubleType.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 14:53 $
// $Revision: 4 $
// $Workfile: AGR_DoubleType.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_DoubleType.h"
#include "AGR_Member.h"

// -----------------------------------------------------------------------------
// Name: AGR_DoubleType constructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_DoubleType::AGR_DoubleType()
    : AGR_Type_ABC( "xsd:double", "Numeric", "", "float", "double", false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_DoubleType destructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_DoubleType::~AGR_DoubleType()
{
    //NOTHING
}

