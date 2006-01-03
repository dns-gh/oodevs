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
    , nMin_( 0 )
    , nMax_( std::numeric_limits< unsigned int >::max() )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_IntegerType constructor
// Created: AGE 2005-08-23
// -----------------------------------------------------------------------------
AGR_IntegerType::AGR_IntegerType( const std::string& strAsnType, unsigned int nMin /*= 0*/, int nMax /*= std::numeric_limits< unsigned int >::max()*/ )
   : AGR_Type_ABC( strAsnType, "Numeric", "", "float", "int", false )
   , nMin_( nMin )
   , nMax_( nMax )
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
// Name: AGR_IntegerType::TesterSerializationCode
// Created: AGE 2005-08-23
// -----------------------------------------------------------------------------
std::string AGR_IntegerType::TesterSerializationCode( const AGR_Member& member ) const
{
    std::stringstream result;
    result << "    ASN_Tools::CopyNumeric( "
           << "pTarget_->GetTestParam_Numeric( " << nMin_ << ", " << nMax_ << " ), "
           << "asnMission." + member.ASNName() + " );\n";
    return result.str();
}

