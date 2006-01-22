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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_KnowledgeObjectListType.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 14:54 $
// $Revision: 7 $
// $Workfile: AGR_KnowledgeObjectListType.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_KnowledgeObjectListType.h"
#include "AGR_Member.h"
#include "AGR_Class.h"

// -----------------------------------------------------------------------------
// Name: AGR_KnowledgeObjectListType constructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_KnowledgeObjectListType::AGR_KnowledgeObjectListType()
    : AGR_Type_ABC( "ScipioBase:ListKnowledgeObject", "ObjectKnowledgeList", "T_ListeConnaissancesObjet", "T_IdVector", true )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_KnowledgeObjectListType destructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_KnowledgeObjectListType::~AGR_KnowledgeObjectListType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_KnowledgeObjectListType::SerializationCode
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
std::string AGR_KnowledgeObjectListType::SerializationCode( const AGR_Member& member ) const
{
    return member.OwnerClass().KnowledgeAccessorCheck()
         + "    NET_ASN_Tools::CopyObjectKnowledgeList( GetVariable( " + member.DIAIndexName() + " )"
         + ", asnMission." + member.ASNName() + 
         + ", " + member.OwnerClass().KnowledgeAccessor() + " );\n";
}


// -----------------------------------------------------------------------------
// Name: AGR_KnowledgeObjectListType::ASNInitialisationCode
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
std::string AGR_KnowledgeObjectListType::ASNInitialisationCode( const AGR_Member& member ) const
{
    std::string strResult;
    strResult += member.OwnerClass().KnowledgeAccessorCheck();
    strResult += "    if( !";
    strResult += "NET_ASN_Tools::CopyObjectKnowledgeList( " + member.ASNPrefixedName();
    strResult += ", GetVariable( ";
    strResult += member.DIAIndexName() + " )";
    strResult += ", " + member.OwnerClass().KnowledgeAccessor() + " )";
    strResult += " )\n        return EnumOrderErrorCode::error_invalid_mission_parameters;\n";
    return strResult;
}

