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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_BooleanType.cpp $
// $Author: Nld $
// $Modtime: 18/10/04 11:53 $
// $Revision: 3 $
// $Workfile: AGR_BooleanType.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_BooleanType.h"
#include "AGR_Member.h"

// -----------------------------------------------------------------------------
// Name: AGR_BooleanType constructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_BooleanType::AGR_BooleanType()
    : AGR_Type_ABC( "xsd:boolean", "Bool", "", "bool", "bool", false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_BooleanType destructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_BooleanType::~AGR_BooleanType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_BooleanType::MosInitialisationCode
// Created: AGE 2004-09-14
// -----------------------------------------------------------------------------
std::string AGR_BooleanType::MosInitialisationCode( const AGR_Member& member ) const
{
    return "    MOS_ValueList* pParameters_" +  member.ASNName() + "= &CreateValueList( \"\", 1, 2 );\n"
         + "    pParameters_" + member.ASNName() + "->CreateValue( asnMission." + member.ASNName() + ", \"" + member.ASNName() + "\", MOSToolValueCheck | MOSToolValueModif );\n";
}
