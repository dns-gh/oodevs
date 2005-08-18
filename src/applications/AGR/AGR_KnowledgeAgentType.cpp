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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_KnowledgeAgentType.cpp $
// $Author: Age $
// $Modtime: 24/09/04 15:21 $
// $Revision: 5 $
// $Workfile: AGR_KnowledgeAgentType.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_KnowledgeAgentType.h"
#include "AGR_Member.h"
#include "AGR_Class.h"

// -----------------------------------------------------------------------------
// Name: AGR_KnowledgeAgentType constructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_KnowledgeAgentType::AGR_KnowledgeAgentType()
    : AGR_Type_ABC( "ScipioBase:KnowledgeAgent", "AgentKnowledge", "", "T_ConnaissanceAgent", "uint", false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_KnowledgeAgentType destructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_KnowledgeAgentType::~AGR_KnowledgeAgentType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_KnowledgeAgentType::ASNInitialisationCode
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
std::string AGR_KnowledgeAgentType::ASNInitialisationCode( const AGR_Member& member ) const
{
    std::string strResult( "    if( !" );
    strResult += "NET_ASN_Tools::CopyAgentKnowledge( " + member.ASNPrefixedName();
    strResult += ", GetVariable( ";
    strResult += member.DIAIndexName() + " )";
    strResult += ", " + member.OwnerClass().KnowledgeAccessor() + " )";
    strResult += " )\n        return EnumOrderErrorCode::error_invalid_mission_parameters;\n";
    return strResult;    
}

// -----------------------------------------------------------------------------
// Name: AGR_KnowledgeAgentType::SerializationCode
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
std::string AGR_KnowledgeAgentType::SerializationCode( const AGR_Member& member ) const
{
    return "    NET_ASN_Tools::CopyAgentKnowledge( GetVariable( " + member.DIAIndexName() + " )"
         + ", asnMission." + member.ASNName() + 
         + ", " + member.OwnerClass().KnowledgeAccessor() + " );\n";
}
