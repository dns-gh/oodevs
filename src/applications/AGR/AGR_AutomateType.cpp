// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-10-05 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_AutomateType.cpp $
// $Author: Age $
// $Modtime: 5/10/04 9:49 $
// $Revision: 1 $
// $Workfile: AGR_AutomateType.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_AutomateType.h"
#include "AGR_Member.h"

// -----------------------------------------------------------------------------
// Name: AGR_AutomateType constructor
// Created: AGE 2004-10-05
// -----------------------------------------------------------------------------
AGR_AutomateType::AGR_AutomateType()
    : AGR_Type_ABC( "ScipioBase:Automate", "Automate", "", "T_Automate", "uint", false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_AutomateType destructor
// Created: AGE 2004-10-05
// -----------------------------------------------------------------------------
AGR_AutomateType::~AGR_AutomateType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_AutomateType::ASNInitialisationCode
// Created: AGE 2004-10-05
// -----------------------------------------------------------------------------
std::string AGR_AutomateType::ASNInitialisationCode( const AGR_Member& member ) const
{
    std::string strResult( "    if( !" );
    strResult += "NET_ASN_Tools::CopyAutomate( ";
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
// Name: AGR_AutomateType::MosInitialisationCode
// Created: AGE 2004-10-05
// -----------------------------------------------------------------------------
std::string AGR_AutomateType::MosInitialisationCode( const AGR_Member& member ) const
{
    return "    CreateAgent( asnMission." + member.ASNName()+ ", \"" + member.ASNName() + "\" );\n";
}
