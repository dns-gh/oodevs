// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-17 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Class.cpp $
// $Author: Age $
// $Modtime: 21/09/04 12:01 $
// $Revision: 4 $
// $Workfile: AGR_Class.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_Class.h"
#include "AGR_Member.h"
#include "AGR_Workspace.h"

// -----------------------------------------------------------------------------
// Name: AGR_Class constructor
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
AGR_Class::AGR_Class( const std::string& strAsnName, const std::string& strModuleName )
    : strAsnName_( strAsnName )
    , strModuleName_( strModuleName )
    , strLowName_( "$$$$NotDefined$$$$" )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_Class destructor
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
AGR_Class::~AGR_Class()
{
    MT_DELETEOWNED( memberList_ );
}

// -----------------------------------------------------------------------------
// Name: AGR_Class::ASNPrefixedName
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
std::string AGR_Class::ASNPrefixedName( const AGR_Member& member ) const
{
    return "asnMission." + member.ASNName();
}

// -----------------------------------------------------------------------------
// Name: AGR_Class::Mos2ASNPrefixedName
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
std::string AGR_Class::Mos2ASNPrefixedName( const AGR_Member& member ) const
{
    return "asnMission." + member.ASNName();
}

// -----------------------------------------------------------------------------
// Name: AGR_Class::MemberList
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
const AGR_Class::T_MemberVector& AGR_Class::MemberList() const
{
    return memberList_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Class::RegisterMember
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
void AGR_Class::RegisterMember( AGR_Member& member )
{
    memberList_.push_back( &member );
}

// -----------------------------------------------------------------------------
// Name: AGR_Class::Name
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
const std::string& AGR_Class::Name() const
{
    return strAsnName_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Class::CompleteName
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_Class::CompleteName() const
{
    return strModuleName_ + strAsnName_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Class::GetLowName
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
const std::string& AGR_Class::LowName() const
{
    return strLowName_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Class::SetLowName
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
void AGR_Class::SetLowName( const std::string& strName )
{
    strLowName_ = strName;
}

// -----------------------------------------------------------------------------
// Name: AGR_Class::MilFileName
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_Class::MilFileName() const
{
    return "MIL_" + strAsnName_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Class::MilFilePathName
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_Class::MilFilePathName() const
{
    return ".\\" + MilFileName();
}

// -----------------------------------------------------------------------------
// Name: AGR_Class::HumanName
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_Class::HumanName() const
{
    std::string strResult( strAsnName_ );
    for( unsigned int i = 0; i < strResult.size(); ++i )
        if( strResult.at(i) == '_' )
            strResult.at(i) = ' ';
    return strResult;
}

// -----------------------------------------------------------------------------
// Name: AGR_Class::EnumName
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_Class::EnumName() const
{
    return "e" + strAsnName_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Class::ASNTypeName
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_Class::ASNTypeName() const
{
    return "T_" + strLowName_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Class::BaseName
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_Class::BaseName() const
{
    return strAsnName_;
}
