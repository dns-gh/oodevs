// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-04-26 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Enumeration.cpp $
// $Author: Ape $
// $Modtime: 22/02/05 10:22 $
// $Revision: 6 $
// $Workfile: AGR_Enumeration.cpp $
//
// *****************************************************************************

#include "AGR_pch.h"
#include "AGR_Enumeration.h"
#include "AGR_Member.h"

#include <assert.h>

// -----------------------------------------------------------------------------
// Name: AGR_Enumeration constructor
/** @param  strName 
*/
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
AGR_Enumeration::AGR_Enumeration( const std::string& strName )
    : AGR_Type_ABC( strName, "Enumeration", "", "id", "uint", false )
    , strName_( strName )
{
    
}

// -----------------------------------------------------------------------------
// Name: AGR_Enumeration destructor
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
AGR_Enumeration::~AGR_Enumeration()
{
    
}

// -----------------------------------------------------------------------------
// Name: AGR_Enumeration::Read
/** @param  input 
*/
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
void AGR_Enumeration::Read( MT_InputArchive_ABC& input )
{
    while( input.NextListElement() )
    {
        input.Section( "xsd:enumeration" );
        std::string strValue;
        input.ReadAttribute( "value", strValue );
        assert( std::find( valueList_.begin(), valueList_.end(), strValue ) == valueList_.end() );
        valueList_.push_back( strValue );
        input.EndSection(); // xsd:enumeration
    }
}

// -----------------------------------------------------------------------------
// Name: AGR_Enumeration::MosInitialisationCode
// Created: AGE 2004-09-14
// -----------------------------------------------------------------------------
std::string AGR_Enumeration::MosInitialisationCode( const AGR_Member& member ) const
{
    std::string strResult = "    MOS_ValueButtonList< ASN1T_" + strName_ + " >* pSelector_" + member.ASNName() + " = &CreateVarList( asnMission." + member.ASNName() + ",\"" + member.ASNName() + "\", 2, 3, false );\n";
    for( CIT_String_Vector itValues = valueList_.begin(); itValues != valueList_.end(); ++itValues )
    {
        strResult += "    pSelector_" + member.ASNName() + "->CreateValue( \"" + *itValues + "\", " + strName_ + "::" + *itValues;
        if( itValues == valueList_.begin() )
            strResult += ", true";
        strResult += " );\n";
    }
    return strResult;
}

// -----------------------------------------------------------------------------
// Name: AGR_Enumeration::Mos2InitialisationCode
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
std::string AGR_Enumeration::Mos2InitialisationCode( const AGR_Member& member ) const
{
    std::string strResult = "    MOS_ParamComboBox< ASN1T_" + strName_ + " >* pSelector_" + member.ASNName() + " = &CreateVarList( asnMission." + member.ASNName() + ",\"" + member.HumanName() + "\" );\n";
    for( CIT_String_Vector itValues = valueList_.begin(); itValues != valueList_.end(); ++itValues )
        strResult += "    pSelector_" + member.ASNName() + "->AddItem( \"" + *itValues + "\", " + strName_ + "::" + *itValues + " );\n";
    return strResult;
}

// -----------------------------------------------------------------------------
// Name: AGR_Enumeration::TesterSerializationCode
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
std::string AGR_Enumeration::TesterSerializationCode( const AGR_Member& member ) const
{
    std::stringstream ss;
    ss << "    ASN_Tools::CopyEnumeration( pTarget_->GetTestParam_Enumeration( 0, "
       << ( valueList_.size() - 1 ) 
       << " ), asnMission." << member.ASNName() 
       << " );\n";
    return ss.str();
}