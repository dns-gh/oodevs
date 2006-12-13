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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Member.cpp $
// $Author: Age $
// $Modtime: 21/09/04 12:01 $
// $Revision: 3 $
// $Workfile: AGR_Member.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_Member.h"
#include "AGR_Type_ABC.h"
#include "AGR_Workspace.h"
#include "AGR_Class.h"

// -----------------------------------------------------------------------------
// Name: AGR_Member constructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_Member::AGR_Member( const std::string& strName, const AGR_Type_ABC& type, const AGR_Class& ownerClass, bool bOptional /*= false*/ )
    : strASNName_( strName )
    , pType_( &type )
    , pOwnerClass_( &ownerClass )
    , bOptional_( bOptional )
{
    if( bOptional && !type.AllowOptionalMembers() )
        throw std::exception( std::string( "Optional members not allowed for type " + type.GetHumanName() ).c_str() );

    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_Member destructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_Member::~AGR_Member()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: agr_member::ChangeName
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
void AGR_Member::ChangeName( const std::string& strNewName )
{
    strASNName_ = strNewName;
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::ASNName
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Member::ASNName() const
{
    return strASNName_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::ASNPrefixedName
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
std::string AGR_Member::ASNPrefixedName() const
{
    return OwnerClass().ASNPrefixedName( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::Mos2ASNPrefixedName
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
std::string AGR_Member::Mos2ASNPrefixedName() const
{
    return OwnerClass().Mos2ASNPrefixedName( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::CPPName
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Member::CPPName() const
{
    return Normalize( strASNName_ ) + "_";
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::HumanName
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
std::string AGR_Member::HumanName() const
{
    if( strASNName_.length() == 0 )
        return strASNName_;

    // Replace "_" with " ".
    std::string strResult( strASNName_ );
    for( unsigned int i = 0; i < strResult.size(); ++i )
        if( strResult.at(i) == '_' )
            strResult.at(i) = ' ';

    // Put the first char in upper case.
    char firstChar = strResult.at(0);
    if( firstChar >= 'a' && firstChar <= 'z')
    {
        firstChar -= 32;
        strResult.at(0) = firstChar;
    }
    return strResult;
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::DIAIndexName
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Member::DIAIndexName() const
{
    std::string strNormalized = Normalize( strASNName_ );
    if( strNormalized.size() )
        strNormalized[0] = (char)toupper( strNormalized[0] );
    return "nDIA" + strNormalized + "Idx_";
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::Type
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
const AGR_Type_ABC& AGR_Member::Type() const
{
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::OwnerClass
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
const AGR_Class& AGR_Member::OwnerClass() const
{
    return *pOwnerClass_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::IsOptional
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
bool AGR_Member::IsOptional() const
{
    return bOptional_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::Normalize
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Member::Normalize( const std::string& strASN ) const
{
    std::string strResult = strASN;
    std::string::size_type nUnderscore = strResult.find( '_', 0 );
    while( nUnderscore != std::string::npos && nUnderscore + 1 < strResult.size() )
    {
        const char cLetterToChange = strResult[ nUnderscore + 1 ];
        strResult.replace( nUnderscore, 2, 1, (char)toupper( cLetterToChange ) );
        nUnderscore = strResult.find( '_', 0 );
    }
    return strResult;
}
    
// -----------------------------------------------------------------------------
// Name: AGR_Member::Declaration
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
//std::string AGR_Member::Declaration() const
//{
//    return pType_->Declaration( *this );
//}

// -----------------------------------------------------------------------------
// Name: AGR_Member::DIAIndexDeclaration
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Member::DIAIndexDeclaration() const
{
    return pType_->DIAIndexDeclaration( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::DIAInitialisationCode
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
std::string AGR_Member::DIAInitialisationCode() const
{
    return pType_->DIAInitialisationCode( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::ASNInitialisationCode
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Member::ASNInitialisationCode() const
{
    return pType_->ASNInitialisationCode( *this );
}

// -----------------------------------------------------------------------------
// Name: agr_member::DIAParametersInitialisationCode
// Created: AGE 2004-09-16
// -----------------------------------------------------------------------------
std::string AGR_Member::DIAParametersInitialisationCode() const
{
    return pType_->DIAParametersInitialisationCode( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::MissionInitialisationCode
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Member::MissionInitialisationCode() const
{
    return pType_->MissionInitialisationCode( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::Mos2InitialisationCode
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Member::Mos2InitialisationCode() const
{
    return pType_->Mos2InitialisationCode( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::Mos2OptionalParamCode
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
std::string AGR_Member::Mos2OptionalParamCode() const
{
    return pType_->Mos2OptionalParamCode( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::SerializationCode
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Member::SerializationCode() const
{
    return pType_->SerializationCode( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::SerializationCleaningCode
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Member::SerializationCleaningCode() const
{
    return pType_->SerializationCleaningCode( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::TesterSerializationCode
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
std::string AGR_Member::TesterSerializationCode() const
{
    return pType_->TesterSerializationCode( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::TesterSerializationCleaningCode
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
std::string AGR_Member::TesterSerializationCleaningCode() const
{
    return pType_->TesterSerializationCleaningCode( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::TesterParamAllocationCode
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
std::string AGR_Member::TesterParamAllocationCode() const
{
    return pType_->TesterParamAllocationCode( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::TesterParamCleaningCode
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
std::string AGR_Member::TesterParamCleaningCode() const
{
    return pType_->TesterParamCleaningCode( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::DIAType
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Member::DIAType() const
{
    return pType_->DIAType( *this );
}

// -----------------------------------------------------------------------------
// Name: AGR_Member::MissionXMLCode
// Created: NLD 2006-11-26
// -----------------------------------------------------------------------------
std::string AGR_Member::MissionXMLCode() const
{
    return pType_->MissionXMLCode( *this );
}
