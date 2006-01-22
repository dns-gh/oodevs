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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_KnowledgePopulationType.cpp $
// $Author: Age $
// $Modtime: 24/09/04 15:21 $
// $Revision: 6 $
// $Workfile: AGR_KnowledgePopulationType.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_KnowledgePopulationType.h"
#include "AGR_Member.h"
#include "AGR_Class.h"

// -----------------------------------------------------------------------------
// Name: AGR_KnowledgePopulationType constructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_KnowledgePopulationType::AGR_KnowledgePopulationType()
    : AGR_Type_ABC( "ScipioBase:KnowledgePopulation", "PopulationKnowledge", "T_ConnaissancePopulation", "uint", false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_KnowledgePopulationType destructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_KnowledgePopulationType::~AGR_KnowledgePopulationType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_KnowledgePopulationType::ASNInitialisationCode
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
std::string AGR_KnowledgePopulationType::ASNInitialisationCode( const AGR_Member& member ) const
{
    std::string strResult;
    strResult += member.OwnerClass().KnowledgeAccessorCheck();
    strResult += "    if( !";
    strResult += "NET_ASN_Tools::CopyPopulationKnowledge( " + member.ASNPrefixedName();
    strResult += ", GetVariable( ";
    strResult += member.DIAIndexName() + " )";
    strResult += ", " + member.OwnerClass().KnowledgeAccessor() + " )";
    strResult += " )\n        return EnumOrderErrorCode::error_invalid_mission_parameters;\n";
    return strResult;
}

// -----------------------------------------------------------------------------
// Name: AGR_KnowledgePopulationType::SerializationCode
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
std::string AGR_KnowledgePopulationType::SerializationCode( const AGR_Member& member ) const
{
    return member.OwnerClass().KnowledgeAccessorCheck()
         + "    NET_ASN_Tools::CopyPopulationKnowledge( GetVariable( " + member.DIAIndexName() + " )"
         + ", asnMission." + member.ASNName() + 
         + ", " + member.OwnerClass().KnowledgeAccessor() + " );\n";
}


// -----------------------------------------------------------------------------
// Name: AGR_KnowledgePopulationType::MOS2RCCode
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_KnowledgePopulationType::MOS2RCCode( const std::string& strAsnParam ) const
{
    std::stringstream strTmp;
    strTmp << "PopulationKnowledgeLink( "
           << strAsnParam
           << " )";
    return strTmp.str();
}
