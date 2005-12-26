// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-04-22 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Mission.cpp $
// $Author: Nld $
// $Modtime: 29/04/05 14:09 $
// $Revision: 22 $
// $Workfile: AGR_Mission.cpp $
//
// *****************************************************************************

#include "AGR_pch.h"
#include "AGR_Mission.h"

#include "AGR_Workspace.h"
#include "AGR_Enumeration.h"
#include "AGR_Member.h"

#include <boost/algorithm/string.hpp>
#include <cstdio>
#include <cassert>

// -----------------------------------------------------------------------------
// Name: AGR_Mission constructor
/** @param  strAsnMissionName
*/
// Created: AGN 2004-04-22
// -----------------------------------------------------------------------------
AGR_Mission::AGR_Mission( const std::string& strAsnMissionName, const std::string& strModuleName )
    : AGR_Class    ( strAsnMissionName, strModuleName )
    , eMissionType_( eMissionPion )
{
    if( Name().substr( 8, 8 ) == "Automate" )
        eMissionType_ = eMissionAutomate;
    else if( Name().substr( 8, 10 ) == "Population" )
        eMissionType_ = eMissionPopulation;
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission destructor
// Created: AGN 2004-04-22
// -----------------------------------------------------------------------------
AGR_Mission::~AGR_Mission()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: AGR_Mission::Read
/** @param  input
    @param  enumerationSet
*/
// Created: AGN 2004-04-22
// -----------------------------------------------------------------------------
void AGR_Mission::Read( MT_XXmlInputArchive& input, const AGR_Workspace& workspace )
{
    std::string strSection = input.GetCurrentElementName();
 
    if( strSection == "xsd:sequence" )
    {
        input.BeginList( "xsd:sequence" );
        while( input.NextListElement() )
        {
            input.BeginList( "xsd:element" );
            std::string strMemberName;
            input.ReadAttribute( "name", strMemberName );

            // Check if this is an optional mission member
            int nTmp;
            input.EnableExceptions( false );
            bool bOptional = input.ReadAttribute( "minOccurs", nTmp );
            if( !bOptional )
                input.ClearLastError();


            AGR_Member* pMember = 0;
            const AGR_Enumeration* pEnumeration = 0;

            std::string strMemberType;
            if( input.ReadAttribute( "type", strMemberType ) )
            {
                // the type of the member is an existing type
                pMember = workspace.CreateMember( strMemberName, strMemberType, *this, bOptional );
                if( ! pMember )
                    throw std::exception( ("Unable to determine member type " + strMemberType).c_str() );

                pEnumeration = dynamic_cast< const AGR_Enumeration* >( &pMember->Type() );
            }
            else
            {
                input.ClearLastError();
                // the type of the member is a local defined type (mostly an enumeration)
                pMember = workspace.CreateLocalTypeMember( Name(), strMemberName, input, *this, bOptional );
                if( !pMember )
                    throw std::exception( "Unable to determine local type " );
            }
            input.EnableExceptions( true );

            if( ! pMember )
                throw std::exception( "Unable to determine member type" );

            if( pEnumeration != 0 )
                memberIdEnumMap_.insert( std::make_pair( MemberList().size(), pEnumeration ) );
            RegisterMember( *pMember );

            input.EndList(); // xsd:element
        }

        input.EndList(); // xsd:sequence
    }

    // If it was none of that, it must be a fragmentary order with no parameter...
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::GenerateMilClassHeader
// Created: AGN 2004-04-22
// -----------------------------------------------------------------------------
void AGR_Mission::GenerateMilClassHeader( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strMissionBaseName( BaseName() );
    std::string strBaseHeaderFile;

    if( eMissionType_ == eMissionAutomate )
        strBaseHeaderFile = AGR_SKEL_DIR "AGR_MissionAutomate_Skeleton.h";
    else if( eMissionType_ == eMissionPopulation )
        strBaseHeaderFile = AGR_SKEL_DIR "AGR_MissionPopulation_Skeleton.h";
    else
        strBaseHeaderFile = AGR_SKEL_DIR "AGR_MissionPion_Skeleton.h";

    // get the skeleton file
    std::string strBaseContent;
    workspace.ReadStringFile( strBaseHeaderFile, strBaseContent );

    // replace the mission name
    workspace.ReplaceInString( strBaseContent, "$MissionName$", strMissionBaseName );

    // generate the static member --> index of DirectIA members
    std::string strStaticMembers;
    for( CIT_MemberVector it = MemberList().begin(); it != MemberList().end(); ++it )
    {
        assert( *it );
        const AGR_Member& member = **it;
        strStaticMembers += member.DIAIndexDeclaration();
    }
    workspace.ReplaceInString( strBaseContent, "$StaticMissionMember$", strStaticMembers );

    // generate the non-static member --> UserType pointers
    std::string strDeclarations;
    for( CIT_MemberVector it = MemberList().begin(); it != MemberList().end(); ++it )
    {
        assert( *it );
        const AGR_Member& member = **it;
        strDeclarations += member.Declaration();
    }
    workspace.ReplaceInString( strBaseContent, "$MissionMember$", strDeclarations );

    std::string strResultFileName =  "/src/libraries/Missions/" + MilFilePathName() + ".h";
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent, strOutputPath + strResultFileName );
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::GenerateMilClassCpp
// Created: AGN 2004-04-23
// -----------------------------------------------------------------------------
void AGR_Mission::GenerateMilClassCpp( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strMissionBaseName( BaseName() );
    std::string strBaseCppFile;
    std::string strUnitName;

    if( eMissionType_ == eMissionAutomate )
    {
        strBaseCppFile = AGR_SKEL_DIR "/AGR_MissionAutomate_Skeleton.cpp";
        strUnitName = "Automate";
    }
    else if( eMissionType_ == eMissionPopulation )
    {
        strBaseCppFile = AGR_SKEL_DIR "/AGR_MissionPopulation_Skeleton.cpp";
        strUnitName = "Population";	
    }
    else
    {
        strBaseCppFile = AGR_SKEL_DIR "/AGR_MissionPion_Skeleton.cpp";
        strUnitName = "Pion";
    }

    // get the skeleton file
    std::string strBaseContent;
    workspace.ReadStringFile( strBaseCppFile, strBaseContent );

    // replace the mission name 
    workspace.ReplaceInString( strBaseContent, "$MissionName$", strMissionBaseName );
    workspace.ReplaceInString( strBaseContent, "$LowerMissionName$", LowName() );

    std::string strStaticMemberInit;
    std::string strStaticMemberScriptInit;
    std::string strAsnMemberInit;
    std::string strMemberInit;
    std::string strMissionMemberInit;
    std::string strMemberReset;
    std::string strMemberSerialization;
    std::string strMemberCleanSerialization;
    // generate the static member --> index of DirectIA members
    std::string strStaticMembers;

    if( ! MemberList().empty() )
    {
        strStaticMemberScriptInit += "    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );\n";
        if( eMissionType_ == eMissionAutomate )
            strAsnMemberInit += "    const ASN1T_Mission_Automate_" + strMissionBaseName + "& asnMission = *asnMsg.mission.u." + LowName() + ";\n";
        else if( eMissionType_ == eMissionPopulation )
            strAsnMemberInit += "    const ASN1T_Mission_Population_" + strMissionBaseName + "& asnMission = *asnMsg.mission.u." + LowName() + ";\n";
        else
            strAsnMemberInit += "    const ASN1T_Mission_Pion_" + strMissionBaseName + "& asnMission = *asnMsg.mission.u." + LowName() + ";\n";
    }
    else
        strStaticMemberScriptInit += "    (void)DEC_Tools::GetDIAType( type.GetDIATypeName() );\n";

    for( CIT_MemberVector it = MemberList().begin(); it != MemberList().end(); ++it )
    {
        assert( *it );
        const AGR_Member& member = **it;

        strMemberReset = member.ResetCode();
        strStaticMemberInit += "int MIL_" + strUnitName + "Mission_" + strMissionBaseName + "::" + member.DIAIndexName() + " = 0 ;\n";
        strStaticMemberScriptInit += member.DIAInitialisationCode();
        strAsnMemberInit += member.ASNInitialisationCode();
        strMemberSerialization      += member.SerializationCode();
        strMemberCleanSerialization += member.SerializationCleaningCode();
        if( eMissionType_ != eMissionAutomate )
        {
            strMemberInit        += member.MemberInitialisationCode();
            strMissionMemberInit += member.MissionInitialisationCode();
        }
    }

    workspace.ReplaceInString( strBaseContent, "$StaticMemberInit$", strStaticMemberInit );
    workspace.ReplaceInString( strBaseContent, "$StaticMemberScriptInit$", strStaticMemberScriptInit );
    workspace.ReplaceInString( strBaseContent, "$InitMembersFromAsn$", strAsnMemberInit );
    workspace.ReplaceInString( strBaseContent, "$InitMembers$", strMemberInit );
    workspace.ReplaceInString( strBaseContent, "$InitMemberFromMission$", strMissionMemberInit );
    workspace.ReplaceInString( strBaseContent, "$ResetMembers$", strMemberReset );
    workspace.ReplaceInString( strBaseContent, "$SerializeMembers$", strMemberSerialization );
    workspace.ReplaceInString( strBaseContent, "$CleanSerializedMembers$", strMemberCleanSerialization );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    std::string strResultFileName =  "/src/libraries/Missions/" + MilFilePathName() + ".cpp";
    workspace.WriteStringInFile( strBaseContent, strOutputPath + strResultFileName ); 
}


// -----------------------------------------------------------------------------
// TESTER
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: AGR_Mission::GenerateTesterClassHeader
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void AGR_Mission::GenerateTesterClassHeader( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strMissionBaseName( BaseName() );
    std::string strBaseHeaderFile;
    std::string strUnitName;

    if( eMissionType_ == eMissionAutomate )
    {
        strBaseHeaderFile = AGR_SKEL_DIR "AGR_TesterMissionAutomat_Skeleton.h";
        strUnitName = "Automat";
    }
    else if( eMissionType_ == eMissionPopulation )
    {
        strBaseHeaderFile = AGR_SKEL_DIR "AGR_TesterMissionPopulation_Skeleton.h";
        strUnitName = "Population";
    }
    else
    {
        strBaseHeaderFile = AGR_SKEL_DIR "AGR_TesterMissionPawn_Skeleton.h";
        strUnitName = "Pawn";
    }

    // get the skeleton file
    std::string strBaseContent;
    workspace.ReadStringFile( strBaseHeaderFile, strBaseContent );

    // replace the mission name
    workspace.ReplaceInString( strBaseContent, "$MissionName$", strMissionBaseName );

    std::string strResultFileName =  "/src/applications/Tester/Actions/Missions/" + strUnitName + "/Mission_" + strUnitName + "_" + strMissionBaseName + ".h";
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent, strOutputPath + strResultFileName );
}


// -----------------------------------------------------------------------------
// Name: AGR_Mission::GenerateTesterClassCpp
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void AGR_Mission::GenerateTesterClassCpp( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strMissionBaseName( BaseName() );
    std::string strBaseCppFile;
    std::string strUnitName;

    if( eMissionType_ == eMissionAutomate )
    {
        strBaseCppFile = AGR_SKEL_DIR "/AGR_TesterMissionAutomat_Skeleton.cpp";
        strUnitName = "Automat";
    }
    else if( eMissionType_ == eMissionPopulation )
    {
        strBaseCppFile = AGR_SKEL_DIR "/AGR_TesterMissionPopulation_Skeleton.cpp";
        strUnitName = "Population";
    }
    else
    {
        strBaseCppFile = AGR_SKEL_DIR "/AGR_TesterMissionPawn_Skeleton.cpp";
        strUnitName = "Pawn";
    }

    // get the skeleton file
    std::string strBaseContent;
    workspace.ReadStringFile( strBaseCppFile, strBaseContent );

    // replace the mission name
    workspace.ReplaceInString( strBaseContent, "$MissionName$", strMissionBaseName );
    workspace.ReplaceInString( strBaseContent, "$LowerMissionName$", LowName() );

    std::string strMemberSerialization;
    std::string strMemberCleanSerialization;
    std::string strDynamicParamAllocation;
    std::string strDynamicParamCleaning;

    for( CIT_MemberVector it = MemberList().begin(); it != MemberList().end(); ++it )
    {
        assert( *it );
        const AGR_Member& member = **it;

        strMemberSerialization      += member.TesterSerializationCode();
        strMemberCleanSerialization += member.TesterSerializationCleaningCode();
        strDynamicParamAllocation   += member.TesterParamAllocationCode();
        strDynamicParamCleaning     += member.TesterParamCleaningCode();
    }

    workspace.ReplaceInString( strBaseContent, "$DynamicParamAllocation$", strDynamicParamAllocation );
    workspace.ReplaceInString( strBaseContent, "$SerializeMembers$", strMemberSerialization );
    workspace.ReplaceInString( strBaseContent, "$DynamicParamCleaning$", strDynamicParamCleaning );
    workspace.ReplaceInString( strBaseContent, "$CleanSerializedMembers$", strMemberCleanSerialization );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    std::string strResultFileName =  "/src/applications/Tester/Actions/Missions/" + strUnitName + "/Mission_" + strUnitName + "_" + strMissionBaseName + ".cpp";
    workspace.WriteStringInFile( strBaseContent, strOutputPath + strResultFileName );
}






// -----------------------------------------------------------------------------
// Name: AGR_Mission::GenerateDiaDefinition
/** @return
*/
// Created: AGN 2004-04-23
// -----------------------------------------------------------------------------
std::string AGR_Mission::GenerateDiaDefinition() const
{
    std::string strResult( "typedef struct T_" );
    strResult += Name();

    if( eMissionType_ == eMissionAutomate )
        strResult += " : T_Mission_Automate\n{\n";
    else if( eMissionType_ == eMissionPopulation )
        strResult += " : T_Mission_Population\n{\n";
    else
        strResult += " : T_Mission_Pion\n{\n";

    size_t n = 0;
    for( CIT_MemberVector it = MemberList().begin(); it != MemberList().end(); ++it, ++n )
    {
        assert( *it );
        const AGR_Member& member = **it;
        CIT_Int_Enumeration_Map foundEnum;

        // $$$$ AGE 2004-09-21: Temporary
//        if( (foundEnum = memberIdEnumMap_.find( n )) != memberIdEnumMap_.end() )
//        {
//            std::string strEnumName = foundEnum->second->GetName();
//            strEnumName.replace( 0, 4, "E_" );
//            strResult += "    " + strEnumName + " " + member.CPPName() + ";\n";
//        }
//        else
            strResult += member.DIAType();

    }
    strResult += "};\n";
    return strResult;
}



// -----------------------------------------------------------------------------
// Name: AGR_Mission::GenerateMosImplementation
/** @return
*/
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
std::string AGR_Mission::GenerateMosImplementation() const
{
    std::string strResult;

    // model : void MOS_MissionPion::CreateMission_Test_DestroyObject()
    strResult += "void ";
    if( eMissionType_ == eMissionAutomate )
        strResult += "MOS_MissionAutomate::CreateMission_" + BaseName() + "()\n";
    else if( eMissionType_ == eMissionPopulation )
        strResult += "MOS_MissionPopulation::CreateMission_" + BaseName() + "()\n";
    else
        strResult += "MOS_MissionPion::CreateMission_" + BaseName() + "()\n";

    strResult += "{\n";

    // model : ASN1T_Mission_Pion_Test_DestroyObject& asnMission = *new ASN1T_Mission_Pion_Test_DestroyObject();
    strResult += "    ASN1T_" + Name() + "& asnMission = *new ASN1T_" + Name() + "();\n";

    // model : pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_destroy_object;
    strResult += "    pASNMsgOrder_->GetAsnMsg().mission.t = ";
    if( eMissionType_ == eMissionAutomate )
        strResult += "T_Mission_Automate_" + LowName() + ";\n";
    else if( eMissionType_ == eMissionPopulation )
        strResult += "T_Mission_Population_" + LowName() + ";\n";
    else
        strResult += "T_Mission_Pion_" + LowName() + ";\n";

    // model : pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_destroy_object = &asnMission;
    strResult += "    pASNMsgOrder_->GetAsnMsg().mission.u." + LowName() + " = &asnMission;\n";

    for( CIT_MemberVector it = MemberList().begin(); it != MemberList().end(); ++it )
    {
        assert( *it );
        const AGR_Member& member = **it;
        strResult += member.MosInitialisationCode();
    }

    strResult += "}\n";

    return strResult;
}


// -----------------------------------------------------------------------------
// Name: AGR_Mission::GenerateMos2Implementation
/** @return
*/
// Created: APE 2004-04-28
// -----------------------------------------------------------------------------
std::string AGR_Mission::GenerateMos2Implementation() const
{
    std::stringstream strResult;
    std::string strFunctionName;
    if( eMissionType_ == eMissionAutomate )
        strFunctionName = "MOS_AutomateMissionInterface::CreateMission_" + BaseName();
    else if( eMissionType_ == eMissionPopulation )
        strFunctionName = "MOS_PopulationMissionInterface::CreateMission_" + BaseName();
    else
        strFunctionName = "MOS_UnitMissionInterface::CreateMission_" + BaseName();

    strResult << "// -----------------------------------------------------------------------------\n"
              << "// Name: " << strFunctionName << "\n"
              << "// Created: AGR\n"
              << "// -----------------------------------------------------------------------------\n";


    // model : void MOS_MissionPion::CreateMission_DestroyObject() {
    strResult << "void " << strFunctionName << "()\n"
              << "{\n";

    // model : ASN1T_Mission_Pion_Test_DestroyObject& asnMission = *new ASN1T_Mission_Pion_Test_DestroyObject();
    strResult << "    ASN1T_" << Name() << "& asnMission = *new ASN1T_" << Name() << "();\n";

    // model : pASNMsgOrder_->GetAsnMsg().mission.t = T_Mission_Pion_mission_pion_test_destroy_object;
    strResult << "    pASNMsgOrder_->GetAsnMsg().mission.t = ";
    if( eMissionType_ == eMissionAutomate )
        strResult << "T_Mission_Automate_" << LowName() << ";\n";
    else if( eMissionType_ == eMissionPopulation )
        strResult << "T_Mission_Population_" << LowName() << ";\n";
    else
        strResult << "T_Mission_Pion_" << LowName() << ";\n";

    // model : pASNMsgOrder_->GetAsnMsg().mission.u.mission_pion_test_destroy_object = &asnMission;
    strResult << "    pASNMsgOrder_->GetAsnMsg().mission.u." << LowName() << " = &asnMission;\n";

    // Interface creation
    for( CIT_MemberVector it = MemberList().begin(); it != MemberList().end(); ++it )
    {
        assert( *it );
        const AGR_Member& member = **it;
        strResult << member.Mos2InitialisationCode();
    }
    strResult << "}\n";

    return strResult.str();
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::GenerateFicheMission
// Created: NLD 2005-04-29
// -----------------------------------------------------------------------------
std::string AGR_Mission::GenerateFicheMission() const
{
    std::stringstream strResult;

    for( CIT_MemberVector it = MemberList().begin(); it != MemberList().end(); ++it )
    {
        assert( *it );
        const AGR_Member& member = **it;
        strResult << "<span class=\"parametre param\" style=\"font-weight: normal;\">"
                  << member.HumanName()
                  << "</span><br>"
                  << std::endl;

        strResult << "<span style=\"font-weight: normal;\" class=\"normal\">BLABLA</span></br></br>" << std::endl;
    }
    return strResult.str();
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::KnowledgeAccessor
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
std::string AGR_Mission::KnowledgeAccessor() const
{
    std::string strAccessor;
    if( eMissionType_ == eMissionAutomate )
        strAccessor = "automate_";
    else if( eMissionType_ == eMissionPopulation )
        strAccessor = "population_";
    else
        strAccessor = "pion_";
    strAccessor += ".GetKnowledgeGroup().GetKSQuerier()";
    return strAccessor;
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::KnowledgeAccessorCheck
// Created: SBO 2005-12-01
// -----------------------------------------------------------------------------
std::string AGR_Mission::KnowledgeAccessorCheck() const
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: AGR_Mission::MilFileName
// Created: AGE 2004-09-16
// -----------------------------------------------------------------------------
std::string AGR_Mission::MilFileName() const
{
    if( eMissionType_ == eMissionAutomate )
        return "MIL_AutomateMission_" + BaseName();
    else if( eMissionType_ == eMissionPopulation )
        return "MIL_PopulationMission_" + BaseName();
    else
        return "MIL_PionMission_" + BaseName();
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::MilFilePathName
// Created: AGE 2004-09-16
// -----------------------------------------------------------------------------
std::string AGR_Mission::MilFilePathName() const
{
    if( eMissionType_ == eMissionAutomate )
        return "Automate\\" + MilFileName();
    else if( eMissionType_ == eMissionPopulation )
        return "Population\\" + MilFileName();
    else
        return "Pion\\" + MilFileName();
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::HumanName
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_Mission::HumanName() const
{
    std::string strResult;
    if( eMissionType_ == eMissionAutomate )
        strResult = "Automate " + BaseName();
    else if( eMissionType_ == eMissionPopulation )
        strResult = "Population " + BaseName();
    else
        strResult = "Pion " + BaseName();
    for( unsigned int i = 0; i < strResult.size(); ++i )
        if( strResult.at(i) == '_' )
            strResult.at(i) = ' ';
    return strResult;
}


// -----------------------------------------------------------------------------
// Name: AGR_Mission::ASNTypeName
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_Mission::ASNTypeName() const
{
    if( eMissionType_ == eMissionAutomate )
        return "T_Mission_Automate_" + LowName();
    else if( eMissionType_ == eMissionPopulation )
        return "T_Mission_Population_" + LowName();
    else
        return "T_Mission_Pion_" + LowName();
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::BaseName
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_Mission::BaseName() const
{
    if( eMissionType_ == eMissionAutomate )
        return Name().substr( std::string( "Mission_Automate_" ).size() );
    else if( eMissionType_ == eMissionPopulation )
        return Name().substr( std::string( "Mission_Population_" ).size() );
    else
        return Name().substr( std::string( "Mission_Pion_" ).size() );
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::WeaponName
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
std::string AGR_Mission::WeaponName() const
{
    std::string strName = BaseName();

    int nPos = strName.find( '_' );
    if( nPos == strName.npos )
        return "COMMUNES";
    return strName.erase( nPos );
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::HumanMissionName
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
std::string AGR_Mission::HumanMissionName() const
{
    std::string strResult = BaseName();

    int nPos = strResult.find( '_' );
    if( nPos != strResult.npos )
        strResult.erase( 0, nPos + 1 );

    boost::replace_all( strResult, "_", " " );

    std::string::iterator prevIt = strResult.begin();
    for( std::string::iterator it = strResult.begin(); it != strResult.end(); )
    {
        if( boost::is_upper()( *it ) && boost::is_lower()( *prevIt ) )
            it = strResult.insert( it, ' ' );

        prevIt = it;
        ++it;
    }
    boost::trim( strResult );
    return strResult;
}


// -----------------------------------------------------------------------------
// Name: AGR_Mission::DIATypeName
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_Mission::DIATypeName() const
{
    return "T_" + Name();
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::MRTBehavior
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_Mission::MRTBehavior() const
{
    if( eMissionType_ == eMissionAutomate )
        return "MIS_Automate_MRT_" + BaseName();
    else if( eMissionType_ == eMissionPopulation )
        return "MIS_Population_" + BaseName();
    else
        return "MIS_Pion_" + BaseName();
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::ConduiteBehavior
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_Mission::ConduiteBehavior() const
{
    if( eMissionType_ == eMissionAutomate )
        return "MIS_Automate_CDT_" + BaseName();
    else if( eMissionType_ == eMissionPopulation )
        return "MIS_Population_" + BaseName();
    else
        return "MIS_Pion_" + BaseName();
}

// -----------------------------------------------------------------------------
// Name: AGR_Mission::IsOfMissionType
// Created: SBO 2005-09-29
// -----------------------------------------------------------------------------
bool AGR_Mission::IsOfMissionType( E_MissionType eType ) const
{
    return eMissionType_ == eType;
}
