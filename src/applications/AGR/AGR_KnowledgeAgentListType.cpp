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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_KnowledgeAgentListType.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 14:54 $
// $Revision: 6 $
// $Workfile: AGR_KnowledgeAgentListType.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_KnowledgeAgentListType.h"
#include "AGR_Member.h"
#include "AGR_Class.h"

// -----------------------------------------------------------------------------
// Name: AGR_KnowledgeAgentListType constructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_KnowledgeAgentListType::AGR_KnowledgeAgentListType()
    : AGR_Type_ABC( "ScipioBase:ListKnowledgeAgent", "AgentKnowledgeList", "", "T_ListeConnaissancesAgent", "T_IdVector", true )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_KnowledgeAgentListType destructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_KnowledgeAgentListType::~AGR_KnowledgeAgentListType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_KnowledgeAgentListType::ASNInitialisationCode
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
std::string AGR_KnowledgeAgentListType::ASNInitialisationCode( const AGR_Member& member ) const
{
    std::string strResult;
    strResult += member.OwnerClass().KnowledgeAccessorCheck();
    strResult += "    if( !";
    strResult += "NET_ASN_Tools::CopyAgentKnowledgeList( " + member.ASNPrefixedName();
    strResult += ", GetVariable( ";
    strResult += member.DIAIndexName() + " )";
    strResult += ", " + member.OwnerClass().KnowledgeAccessor() + " )";
    strResult += " )\n        return EnumOrderErrorCode::error_invalid_mission_parameters;\n";
    return strResult;
}

// -----------------------------------------------------------------------------
// Name: AGR_KnowledgeAgentListType::SerializationCode
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
std::string AGR_KnowledgeAgentListType::SerializationCode( const AGR_Member& member ) const
{
    return member.OwnerClass().KnowledgeAccessorCheck()
         + "    NET_ASN_Tools::CopyAgentKnowledgeList( GetVariable( " + member.DIAIndexName() + " )"
         + ", asnMission." + member.ASNName() + 
         + ", " + member.OwnerClass().KnowledgeAccessor() + " );\n";
}

