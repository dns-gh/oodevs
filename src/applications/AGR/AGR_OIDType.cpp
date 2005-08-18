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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_OIDType.cpp $
// $Author: Age $
// $Modtime: 24/09/04 15:20 $
// $Revision: 4 $
// $Workfile: AGR_OIDType.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_OIDType.h"
#include "AGR_Member.h"

// -----------------------------------------------------------------------------
// Name: AGR_OIDType constructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_OIDType::AGR_OIDType()
    : AGR_Type_ABC( "ScipioBase:OID", "ID", "", "T_ID", "uint", false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_OIDType destructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_OIDType::~AGR_OIDType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_OIDType::Mos2InitialisationCode
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_OIDType::Mos2InitialisationCode( const AGR_Member& member ) const
{
    return "    //NOTHING\n";
}

