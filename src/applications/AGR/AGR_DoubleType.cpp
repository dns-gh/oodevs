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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_DoubleType.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 14:53 $
// $Revision: 4 $
// $Workfile: AGR_DoubleType.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_DoubleType.h"
#include "AGR_Member.h"

// -----------------------------------------------------------------------------
// Name: AGR_DoubleType constructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_DoubleType::AGR_DoubleType()
    : AGR_Type_ABC( "xsd:double", "Numeric", "", "float", "double", false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_DoubleType destructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_DoubleType::~AGR_DoubleType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_DoubleType::MosInitialisationCode
// Created: AGE 2004-09-14
// -----------------------------------------------------------------------------
std::string AGR_DoubleType::MosInitialisationCode( const AGR_Member& member ) const
{
    return "    MOS_ValueList* pParameters_" +  member.ASNName() + "= &CreateValueList( \"\", 1, 2 );\n"
         + "    pParameters_" + member.ASNName() + "->CreateValue( asnMission." + member.ASNName() + ", \"" + member.ASNName() + "\", MOSToolValueTitle | MOSToolValueText | MOSToolValueSlide | MOSToolValueModif );\n";
}
