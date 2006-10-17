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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_AgentType.cpp $
// $Author: Age $
// $Modtime: 24/09/04 15:21 $
// $Revision: 3 $
// $Workfile: AGR_AgentType.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_AgentType.h"
#include "AGR_Member.h"

// -----------------------------------------------------------------------------
// Name: AGR_AgentType constructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_AgentType::AGR_AgentType()
    : AGR_Type_ABC( "ScipioBase:Agent", "Agent", "T_Pion", "uint", false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_AgentType destructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_AgentType::~AGR_AgentType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_AgentType::ASNInitialisationCode
// Created: AGE 2004-09-14
// -----------------------------------------------------------------------------
std::string AGR_AgentType::ASNInitialisationCode( const AGR_Member& member ) const
{
    std::string strResult( "    if( !" );
    strResult += "NET_ASN_Tools::CopyAgent( ";
    strResult += member.ASNPrefixedName() + ", GetVariable( ";
    strResult += member.DIAIndexName();
    strResult += " ) )";
    strResult += " )\n        return EnumOrderErrorCode::error_invalid_mission_parameters;\n";
    return strResult;
}

// -----------------------------------------------------------------------------
// Name: AGR_AgentType::MOS2RCCode
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_AgentType::MOS2RCCode( const std::string& strAsnParam ) const
{
    std::stringstream strTmp;
    strTmp << "AgentLink( "
           << strAsnParam
           << ", rcResolver"
           << " )";
    return strTmp.str();
}
