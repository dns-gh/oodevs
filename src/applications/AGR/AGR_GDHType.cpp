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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_GDHType.cpp $
// $Author: Age $
// $Modtime: 24/09/04 15:21 $
// $Revision: 3 $
// $Workfile: AGR_GDHType.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_GDHType.h"
#include "AGR_Member.h"

// -----------------------------------------------------------------------------
// Name: AGR_GDHType constructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_GDHType::AGR_GDHType()
    : AGR_Type_ABC( "ScipioBase:GDH", "GDH", "float", "double", false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_GDHType destructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_GDHType::~AGR_GDHType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_GDHType::ASNInitialisationCode
// Created: AGE 2004-09-14
// -----------------------------------------------------------------------------
std::string AGR_GDHType::ASNInitialisationCode( const AGR_Member& member ) const
{
    std::string strResult( "    if( !" );
    strResult += "NET_ASN_Tools::CopyGDH( ";
    strResult += member.ASNPrefixedName() + ", GetVariable( ";
    strResult += member.DIAIndexName();
    strResult += " ) )";
    strResult += " )\n        return EnumOrderErrorCode::error_invalid_mission_parameters;\n";
    return strResult;
}
