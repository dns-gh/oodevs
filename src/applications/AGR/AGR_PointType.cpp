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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_PointType.cpp $
// $Author: Age $
// $Modtime: 24/09/04 15:20 $
// $Revision: 3 $
// $Workfile: AGR_PointType.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_PointType.h"
#include "AGR_Member.h"

// -----------------------------------------------------------------------------
// Name: AGR_PointType constructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_PointType::AGR_PointType()
    : AGR_Type_ABC( "ScipioBase:Point", "Point", "T_Point", "Position", true )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_PointType destructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_PointType::~AGR_PointType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_PointType::ASNInitialisationCode
// Created: AGE 2004-09-14
// -----------------------------------------------------------------------------
std::string AGR_PointType::ASNInitialisationCode( const AGR_Member& member ) const
{
    std::string strResult( "    if( !" );
    strResult += "NET_ASN_Tools::CopyPoint( ";
    strResult += member.ASNPrefixedName() + ", GetVariable( ";
    strResult += member.DIAIndexName();
    if( member.IsOptional() )
        strResult += " ), asnMission.m." + member.ASNName() + "Present )";
    else
        strResult += " ) )";
    strResult += " )\n        return EnumOrderErrorCode::error_invalid_mission_parameters;\n";
    return strResult;
}

// -----------------------------------------------------------------------------
// Name: AGR_PointType::AllowOptionalMembers
// Created: NLD 2006-08-04
// -----------------------------------------------------------------------------
bool AGR_PointType::AllowOptionalMembers() const
{
    return true;
}
