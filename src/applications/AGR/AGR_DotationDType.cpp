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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_DotationDType.cpp $
// $Author: Age $
// $Modtime: 24/09/04 15:20 $
// $Revision: 3 $
// $Workfile: AGR_DotationDType.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_DotationDType.h"
#include "AGR_Member.h"

// -----------------------------------------------------------------------------
// Name: AGR_DotationDType constructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_DotationDType::AGR_DotationDType()
    : AGR_Type_ABC( "ScipioBase:TypeDotationTrancheD", "Dotation", "T_Dotation", "Dotation", false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_DotationDType destructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_DotationDType::~AGR_DotationDType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_DotationDType::Mos2InitialisationCode
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
std::string AGR_DotationDType::Mos2InitialisationCode( const AGR_Member& member ) const
{
    std::string strTmp = "    CreateDotationDType( " + member.Mos2ASNPrefixedName() + ", \"" 
                            + member.HumanName() + "\"";

    strTmp += ");\n";
    return strTmp;
}

