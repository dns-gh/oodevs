// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-13 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_IntegerType.cpp $
// $Author: Nld $
// $Modtime: 14/03/05 17:38 $
// $Revision: 5 $
// $Workfile: AGR_IntegerType.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_IntegerType.h"
#include "AGR_Member.h"

// -----------------------------------------------------------------------------
// Name: AGR_IntegerType constructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_IntegerType::AGR_IntegerType()
    : AGR_Type_ABC( "xsd:integer", "Numeric", "", "float", "int", false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_IntegerType constructor
// Created: NLD 2005-03-14
// -----------------------------------------------------------------------------
AGR_IntegerType::AGR_IntegerType( const std::string& strAsnType )
    : AGR_Type_ABC( strAsnType, "Numeric", "", "float", "int", false )
{

}

// -----------------------------------------------------------------------------
// Name: AGR_IntegerType destructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_IntegerType::~AGR_IntegerType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_IntegerType::MosInitialisationCode
// Created: AGE 2004-09-14
// -----------------------------------------------------------------------------
std::string AGR_IntegerType::MosInitialisationCode( const AGR_Member& member ) const
{
    return "    MOS_ValueList* pParameters_" +  member.ASNName() + "= &CreateValueList( \"\", 1, 2 );\n"
         + "    pParameters_" + member.ASNName() + "->CreateValue( asnMission." + member.ASNName() + ", \"" + member.ASNName() + "\", MOSToolValueTitle | MOSToolValueText | MOSToolValueSlide | MOSToolValueModif );\n";
}


