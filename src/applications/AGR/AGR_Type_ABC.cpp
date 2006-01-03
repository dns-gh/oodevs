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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Type_ABC.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:40 $
// $Revision: 5 $
// $Workfile: AGR_Type_ABC.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_Type_ABC.h"
#include "AGR_Member.h"

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC constructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_Type_ABC::AGR_Type_ABC( const std::string& strAsnType, const std::string& strFunctionSuffix, const std::string& strCPPType, const std::string& strDIAType, const std::string& strTesterType, bool bRequiresCleaning )
    : strAsnType_         ( strAsnType )
    , strFunctionSuffix_  ( strFunctionSuffix )
    , strCPPType_         ( strCPPType )
    , strDIAType_         ( strDIAType )
    , strTesterType_      ( strTesterType )
    , strHumanName_       ( strDIAType )
    , bExplicitMember_    ( !strCPPType_.empty() )
    , bRequiresCleaning_  ( bRequiresCleaning )
{
    if( strHumanName_.substr( 0, 2 ) == "T_" )
        strHumanName_.erase( 0, 2 );
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC destructor
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_Type_ABC::~AGR_Type_ABC()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::IsOfType
// Created: AGE 2004-09-14
// -----------------------------------------------------------------------------
bool AGR_Type_ABC::IsOfType( const std::string& strASNType ) const
{
    return RemoveNamespace( strAsnType_ ) == RemoveNamespace( strASNType );
}

// -----------------------------------------------------------------------------
// Name: agr_type_abc::DIAIndexDeclaration
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::DIAIndexDeclaration( const AGR_Member& member ) const
{
    return "    static int " + member.DIAIndexName() + ";\n";
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::DIAInitialisationCode
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::DIAInitialisationCode( const AGR_Member& member ) const
{
    return "    " + member.DIAIndexName() + " = DEC_Tools::InitializeDIAField( \"" + member.CPPName() + "\", diaType );\n";
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::Declaration
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::Declaration( const AGR_Member& member ) const
{
    if( bExplicitMember_ )
        return "    " + strCPPType_ + " " + member.CPPName() + ";\n";
    else
        return "";
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::ASNInitialisationCode
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::ASNInitialisationCode( const AGR_Member& member ) const
{
    std::string strResult( "    if( !" );
    strResult += "NET_ASN_Tools::Copy" + strFunctionSuffix_ + "( "
               + member.ASNPrefixedName()
               + ( bExplicitMember_ ? ", " + member.CPPName() : "" )
               + ", GetVariable( " +  member.DIAIndexName() + " ) )"
               + " )\n        return EnumOrderErrorCode::error_invalid_mission_parameters;\n";
    return strResult;
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::DIAParametersInitialisationCode
// Created: AGE 2004-09-16
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::DIAParametersInitialisationCode( const AGR_Member& member ) const
{
    return "    NET_ASN_Tools::Copy" + strFunctionSuffix_
           + "( diaParams[nCurDIAParamIdx++]"
           + ( bExplicitMember_ ? ", " + member.CPPName() : "" )
           + ", GetVariable( " + member.DIAIndexName() + ")"
           + ");\n";
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::MemberInitialisationCode
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::MemberInitialisationCode( const AGR_Member& member ) const
{
    return ResetCode( member );
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::MissionInitialisationCode
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::MissionInitialisationCode( const AGR_Member& member ) const
{
    std::string strResult( "    " );
    strResult +="NET_ASN_Tools::Copy" + strFunctionSuffix_ 
              + "( mission.GetVariable( " + member.DIAIndexName() + " )"
              + ( bExplicitMember_ ? ", " + member.CPPName() : "" )
              + ", GetVariable( " + member.DIAIndexName() + " ) );\n";
    return strResult;    
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::Mos2InitialisationCode
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::Mos2InitialisationCode( const AGR_Member& member ) const
{
    return "    Create" + strFunctionSuffix_ + "( " + member.Mos2ASNPrefixedName() + ", \"" 
            + member.HumanName() + "\", " + ( member.IsOptional() ? "true" : "false" ) + " );\n";
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::ResetCode
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::ResetCode( const AGR_Member& member ) const
{
    std::string strResult( "    " );
    strResult += "NET_ASN_Tools::Reset" + strFunctionSuffix_
            + "( " + ( bExplicitMember_ ? member.CPPName() + ", " : "" )
            + "GetVariable( " + member.DIAIndexName() + " ) );\n";
    return strResult;
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::SerializationCode
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::SerializationCode( const AGR_Member& member ) const
{
    return "    NET_ASN_Tools::Copy" + strFunctionSuffix_
           + "( GetVariable( " + member.DIAIndexName() + " )"
           + ", asnMission." + member.ASNName()
           + " );\n";
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::SerializationCleaningCode
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::SerializationCleaningCode( const AGR_Member& member ) const
{
    if( bRequiresCleaning_ )
        return "    NET_ASN_Tools::Delete( asnMission." + member.ASNName()+ " );\n";
    else
        return "";
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::TesterParamAllocationCode
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::TesterParamAllocationCode( const AGR_Member& member ) const
{
    if( bRequiresCleaning_ )
        return "    const " + strTesterType_ + "& " + member.CPPName() 
               + " = pTarget_->GetTestParam_" + strFunctionSuffix_ + "();\n";
    else
        return "";
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::TesterParamCleaningCode
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::TesterParamCleaningCode( const AGR_Member& member ) const
{
    if( bRequiresCleaning_ )
        return "    delete &" + member.CPPName() + ";\n";
    else
        return "";
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::TesterSerializationCode
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::TesterSerializationCode( const AGR_Member& member ) const
{
    if( bRequiresCleaning_ )
        return "    ASN_Tools::Copy" + strFunctionSuffix_
               + "( " + member.CPPName() + ", asnMission." + member.ASNName() + " );\n";
    else
        return "    ASN_Tools::Copy" + strFunctionSuffix_
               + "( pTarget_->GetTestParam_" + strFunctionSuffix_ + "()"
               + ", asnMission." + member.ASNName() + " );\n";
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::TesterSerializationCleaningCode
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::TesterSerializationCleaningCode( const AGR_Member& member ) const
{
    if( bRequiresCleaning_ )
        return "    ASN_Tools::Delete( asnMission." + member.ASNName()+ " );\n";
    else
        return "";    
}

// -----------------------------------------------------------------------------
// Name: agr_type_abc::DIAType
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::DIAType( const AGR_Member& member ) const
{
    return strDIAType_ + " " + member.CPPName() + ";\n";
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::AsnType
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
const std::string& AGR_Type_ABC::ASNType() const
{
    return strAsnType_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::RemoveNamespace
// Created: AGE 2004-09-14
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::RemoveNamespace( const std::string& strName ) const
{
    std::string::size_type nIdx = strName.find_last_of( ':' );
    if( nIdx != std::string::npos && nIdx + 1 < strName.size() )
        return strName.substr( nIdx + 1 );
    return strName;
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::GetDIAType
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
const std::string& AGR_Type_ABC::GetDIAType() const
{
    return strDIAType_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::MOS2RCCode
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::MOS2RCCode( const std::string& strAsnParam ) const
{
    return strAsnParam;
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::MOSRCCode
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_Type_ABC::MOSRCCode( const std::string& strAsnParam ) const
{
    return strAsnParam;
}

// -----------------------------------------------------------------------------
// Name: AGR_Type_ABC::GetHumanName
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
const std::string& AGR_Type_ABC::GetHumanName() const
{
    return strHumanName_;
}
