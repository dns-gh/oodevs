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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_PolygonType.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 14:54 $
// $Revision: 5 $
// $Workfile: AGR_PolygonType.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_PolygonType.h"
#include "AGR_Member.h"

// -----------------------------------------------------------------------------
// Name: AGR_PolygonType constructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_PolygonType::AGR_PolygonType()
    : AGR_Type_ABC( "ScipioBase:Polygon", "Polygon", "TER_Localisation", "T_Localisation", "Location", true )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_PolygonType destructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_PolygonType::~AGR_PolygonType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_PolygonType::Mos2InitialisationCode
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
std::string AGR_PolygonType::Mos2InitialisationCode( const AGR_Member& member ) const
{
    return "    CreateLocation( " + member.Mos2ASNPrefixedName() + ", \"" 
            + member.HumanName() + "\", " + ( member.IsOptional() ? "true" : "false" ) + " );\n";
}
