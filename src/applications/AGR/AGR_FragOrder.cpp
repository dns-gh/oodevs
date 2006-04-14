// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-15 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_FragOrder.cpp $
// $Author: Nld $
// $Modtime: 4/05/05 15:17 $
// $Revision: 12 $
// $Workfile: AGR_FragOrder.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_FragOrder.h"
#include "AGR_Workspace.h"
#include "AGR_Member.h"

#include <boost/algorithm/string.hpp>

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder constructor
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
AGR_FragOrder::AGR_FragOrder( const std::string& strAsnName, const std::string& strModuleName )
    : AGR_Class                 ( strAsnName, strModuleName )
    , strDemandLowName_         ( "" )
    , bAvailableForAllMissions_ ( false )
    , bAvailableWithoutMissions_( false )
{
    if( strAsnName.substr( 14, 8 ) == "Automate" )
        eFragOrderTarget_ = eFragOrderAutomate;
    else if( strAsnName.substr( 14, 10 ) == "Population" )
    {
        eFragOrderTarget_ = eFragOrderPopulation;
        bAvailableForAllMissions_  = true; //$$$ à voir
        bAvailableWithoutMissions_ = true;
    }
    else
        eFragOrderTarget_ = eFragOrderPion;

    ////
    if(     strAsnName == "OrderConduite_AttendreSePoster" 
        ||  strAsnName == "OrderConduite_Poursuivre" 
        ||  strAsnName == "OrderConduite_Interrompre" 
        ||  strAsnName == "OrderConduite_ChangerAmbiance" )
    {
        bAvailableForAllMissions_  = true;
        bAvailableWithoutMissions_ = false;
    }

    else if(    strAsnName == "OrderConduite_MettreTenueNBC"      
             || strAsnName == "OrderConduite_EnleverTenueNBC"     
             || strAsnName == "OrderConduite_PasserEnSilenceRadio"
             || strAsnName == "OrderConduite_ArreterSilenceRadio" 
             || strAsnName == "OrderConduite_PasserEnSilenceRadar"
             || strAsnName == "OrderConduite_ArreterSilenceRadar" 
             || strAsnName == "OrderConduite_ChangerReglesEngagement" 
             || strAsnName == "OrderConduite_RecupererTransporteurs"
             || strAsnName == "OrderConduite_ChangerReglesEngagementPopulation" )
    {
        bAvailableForAllMissions_  = true;
        bAvailableWithoutMissions_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder destructor
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
AGR_FragOrder::~AGR_FragOrder()
{
    //NOTHING
}


// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::Read
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
void AGR_FragOrder::Read( MT_XXmlInputArchive& input, const AGR_Workspace& workspace )
{
    std::string strSection = input.GetCurrentElementName();
    if( strSection == "xsd:restriction" )
    {
        // This is a fragmentary order with a single member (the order is the member).
        nFragOrderType_ = eRestriction;
        input.Section( "xsd:restriction" );

        std::string strTypeBase;
        input.ReadAttribute( "base", strTypeBase );

        AGR_Member* pMember = workspace.CreateMember( "", strTypeBase, *this, false );
        if( !pMember )
            throw std::exception( ( "Unable to determine type of " + strTypeBase + " in mission " + Name() ).c_str() );
        RegisterMember( *pMember );

        input.EndSection(); // "xsd:restriction"
        return;
    }


    if( strSection == "xsd:complexContent" )
    {
        nFragOrderType_ = eComplexContent;
        // This is a complex fragmentary order with a single member.
        input.Section( "xsd:complexContent" );
        input.Section( "xsd:extension" );

        std::string strTypeBase;
        input.ReadAttribute( "base", strTypeBase );

        AGR_Member* pMember = workspace.CreateMember( "", strTypeBase, *this, false );
        if( !pMember )
            throw std::exception( ( "Unable to determine member type of " + strTypeBase + " in mission " + Name() ).c_str() );
        RegisterMember( *pMember );

        input.EndSection(); // "xsd:extension"
        input.EndSection(); // "xsd:complexContent"
        return;
    }

    if( strSection == "xsd:sequence" )
    {
        nFragOrderType_ = eSequence;
        input.BeginList( "xsd:sequence" );
        while( input.NextListElement() )
        {
            input.BeginList( "xsd:element" );
            std::string strMemberName;
            input.ReadAttribute( "name", strMemberName );

            // Check if this is an optional member
            int nTmp;
            input.EnableExceptions( false );
            bool bOptional = input.ReadAttribute( "minOccurs", nTmp );
            if( !bOptional )
                input.ClearLastError();

            AGR_Member* pMember = 0;

            std::string strMemberType;
            if( input.ReadAttribute( "type", strMemberType ) )
            {
                // the type of the member is an existing type
                pMember = workspace.CreateMember( strMemberName, strMemberType, *this, bOptional );
                if( ! pMember )
                    throw std::exception( ("Unable to determine member type " + strMemberType).c_str() );
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
            RegisterMember( *pMember );

            input.EndList(); // xsd:element
        }

        input.EndList(); // xsd:sequence
    }
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::GenerateMilClassHeader
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
void AGR_FragOrder::GenerateMilClassHeader( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    const std::string strOrderBaseName = BaseName();
    std::string strBaseHeaderFile;
    if( eFragOrderTarget_ == eFragOrderAutomate )
        strBaseHeaderFile = AGR_SKEL_DIR "AGR_OrderConduite_Automate_Skeleton.h";
    else if( eFragOrderTarget_ == eFragOrderPopulation )
        strBaseHeaderFile = AGR_SKEL_DIR "AGR_OrderConduite_Population_Skeleton.h";
    else
        strBaseHeaderFile = AGR_SKEL_DIR "AGR_OrderConduite_Pion_Skeleton.h";

    // get the skeleton file
    std::string strBaseContent;
    workspace.ReadStringFile( strBaseHeaderFile, strBaseContent );

    // replace the order name
    workspace.ReplaceInString( strBaseContent, "$OrderName$", strOrderBaseName );

    // generate the static member --> index of DirectIA members
    std::string strStaticMembers;
    for( CIT_MemberVector it = MemberList().begin(); it != MemberList().end(); ++it )
    {
        const AGR_Member& member = **it;
        strStaticMembers += member.DIAIndexDeclaration();
    }
    workspace.ReplaceInString( strBaseContent, "$StaticOrderMember$", strStaticMembers );

    // generate the non-static member --> UserType pointers
//    std::string strDeclarations;
//    for( CIT_MemberVector it = MemberList().begin(); it != MemberList().end(); ++it )
//    {
//        const AGR_Member& member = **it;
//        strDeclarations += member.Declaration();
//    }
//    workspace.ReplaceInString( strBaseContent, "$OrderMember$", strDeclarations );
    //workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    std::string strResultFileName( "/src/libraries/Missions/" + MilFilePathName() + ".h" );
    workspace.WriteStringInFile( strBaseContent, strOutputPath + strResultFileName );    
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::GenerateMilClassCpp
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
void AGR_FragOrder::GenerateMilClassCpp( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strBaseCppFile;
    if( eFragOrderTarget_ == eFragOrderAutomate )
        strBaseCppFile = AGR_SKEL_DIR "AGR_OrderConduite_Automate_Skeleton.cpp";
    else if( eFragOrderTarget_ == eFragOrderPopulation )
        strBaseCppFile = AGR_SKEL_DIR "AGR_OrderConduite_Population_Skeleton.cpp";
    else
        strBaseCppFile = AGR_SKEL_DIR "AGR_OrderConduite_Pion_Skeleton.cpp";

    // get the skeleton file
    std::string strBaseContent;
    workspace.ReadStringFile( strBaseCppFile, strBaseContent );

    // replace the mission name    
    workspace.ReplaceInString( strBaseContent, "$OrderName$", BaseName() );
    workspace.ReplaceInString( strBaseContent, "$LowerOrderName$", LowName() );

    std::string strStaticMemberInit;
    std::string strStaticMemberScriptInit;
    std::string strAsnMemberInit;
    std::string strDIAMemberInit;

    if( ! MemberList().empty() )
    {
        strStaticMemberScriptInit += "    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );\n";
        strAsnMemberInit          += "    const ASN1T_OrderConduite_" + BaseName() + "& asnMission = ";
        if( nFragOrderType_ == eRestriction )
            strAsnMemberInit += "asnMsg.u." + LowName() + ";\n";
        else
            strAsnMemberInit += "*asnMsg.u." + LowName() + ";\n";
    }
    else
        strStaticMemberScriptInit += "    (void)DEC_Tools::GetDIAType( type.GetConduiteDIATypeName() );\n";

    // generate the static member --> index of DirectIA members
    std::string strStaticMembers;
    for( CIT_MemberVector it = MemberList().begin(); it != MemberList().end(); ++it )
    {
        const AGR_Member& member = **it;
        strStaticMemberInit += "int MIL_OrderConduite_" + BaseName() + "::" + member.DIAIndexName() + " = 0 ;\n";
        strStaticMemberScriptInit += member.DIAInitialisationCode();
        strAsnMemberInit += member.ASNInitialisationCode();
        if( eFragOrderTarget_ != eFragOrderAutomate ) //$$$ population?
            strDIAMemberInit        += member.DIAParametersInitialisationCode();
    }

    workspace.ReplaceInString( strBaseContent, "$StaticMemberInit$", strStaticMemberInit );
    workspace.ReplaceInString( strBaseContent, "$StaticMemberScriptInit$", strStaticMemberScriptInit );
    workspace.ReplaceInString( strBaseContent, "$InitMembersFromAsn$", strAsnMemberInit );
    workspace.ReplaceInString( strBaseContent, "$InitMemberFromDIA$", strDIAMemberInit );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    std::string strResultFileName( "/src/libraries/Missions/" + MilFilePathName() + ".cpp" );
    workspace.WriteStringInFile( strBaseContent, strOutputPath + strResultFileName );        
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::MosLight2CreationCode
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
std::string AGR_FragOrder::MosLight2CreationCode() const
{
    return "        case eOrdreConduite_" + BaseName() + " :\n"
         + "             CreateOrder_" + BaseName() + "();\n"
         + "             break;\n";
}
    
// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::MosLight2DeletionCode
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
std::string AGR_FragOrder::MosLight2DeletionCode() const
{
    if( NeedsDeleting() )
        return "        case T_MsgOrderConduite_order_conduite_" + LowName() + " :\n"
             + "             delete pASNMsgOrder_->GetAsnMsg().order_conduite.u." + LowName() + ";\n"
             + "             break;\n";
    return "";
}
    
// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::MosLight2ImplementationCode
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
std::string AGR_FragOrder::MosLight2ImplementationCode() const
{
    std::stringstream strResult;

    const std::string strFunctionName = "FragmentaryOrderInterface::CreateOrder_" + BaseName();

    strResult << "// -----------------------------------------------------------------------------\n"
              << "// Name: " << strFunctionName << "\n"
              << "// Created: AGR\n"
              << "// -----------------------------------------------------------------------------\n";
    

    // model : void FragmentaryOrderInterface::CreateOrder_AutomateMouvementZoneImplantation() {
    strResult << "void " << strFunctionName << "()\n"
              << "{\n";

    if( MemberList().empty() )
    {
        strResult << "    // NOTHING\n}\n";
        return strResult.str();
    }

    if( nFragOrderType_ == eRestriction )
    {
        // model: pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_automate_mouvement_zone_implantation;
        strResult << "    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_" << LowName() << ";\n";
        strResult << "#define asnMission pASNMsgOrder_->GetAsnMsg().order_conduite.u\n";
    }
    else
    {
        // model: ASN1T_OrderConduite_Pion_RenforcerTC2EnVSRAM& asnMission = *new ASN1T_OrderConduite_Pion_RenforcerTC2EnVSRAM();
        //        pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_order_conduite_pion_renforcer_tc2_en_vs_ram;
        //        pASNMsgOrder_->GetAsnMsg().order_conduite.u.order_conduite_pion_renforcer_tc2_en_vs_ram = &asnMission;
        strResult << "    ASN1T_" << Name() << "& asnMission = *new ASN1T_" << Name() << "();\n";
        strResult << "    pASNMsgOrder_->GetAsnMsg().order_conduite.t = T_MsgOrderConduite_order_conduite_" << LowName() << ";\n";
        strResult << "    pASNMsgOrder_->GetAsnMsg().order_conduite.u." << LowName() << " = &asnMission;\n";
    }

    // Interface creation
    for( CIT_MemberVector it = MemberList().begin(); it != MemberList().end(); ++it )
    {
        const AGR_Member& member = **it;
        std::string strVarName;
        strResult << member.Mos2InitialisationCode();
    }
    if( nFragOrderType_ == eRestriction )
        strResult << "#undef asnMission\n";
    strResult << "}\n\n";

    return strResult.str();    
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::GenerateDiaDefinition
// Created: AGE 2004-09-23
// -----------------------------------------------------------------------------
std::string AGR_FragOrder::GenerateDiaDefinition() const
{
    std::string strResult( "typedef struct " );
    strResult += DIATypeName();
    strResult += " : Rep_OrdreConduite\n{\n";

    size_t n = 0;
    for( CIT_MemberVector it = MemberList().begin(); it != MemberList().end(); ++it, ++n )
    {
        const AGR_Member& member = **it;
        strResult += member.DIAType();
    }

    strResult += "};\n";
    return strResult;
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::CanBeDemanded
// Created: AGE 2004-09-23
// -----------------------------------------------------------------------------
bool AGR_FragOrder::CanBeDemanded() const
{
    return ! strDemandLowName_.empty();
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::ASNDemandName
// Created: AGE 2004-09-23
// -----------------------------------------------------------------------------
std::string AGR_FragOrder::ASNDemandName() const
{
    if( CanBeDemanded() )
        return "T_MsgAttenteOrdreConduite_ordres_conduite_element_" + strDemandLowName_;
    else
        return "";
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::DIATypeName
// Created: AGE 2004-09-23
// -----------------------------------------------------------------------------
std::string AGR_FragOrder::DIATypeName() const
{
    return "Rep_" + Name();
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::KnowledgeAccessor
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
std::string AGR_FragOrder::KnowledgeAccessor() const
{
    return "*pKnowledgeGroup_";
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::KnowledgeAccessorCheck
// Created: SBO 2005-12-01
// -----------------------------------------------------------------------------
std::string AGR_FragOrder::KnowledgeAccessorCheck() const
{
    std::string strResult( "    if( !pKnowledgeGroup_ )\n" );
    strResult += "        return EnumOrderErrorCode::error_invalid_mission_parameters;\n";
    return strResult;
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::ASNPrefixedName
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
std::string AGR_FragOrder::ASNPrefixedName( const AGR_Member& member ) const
{
    if( nFragOrderType_ == eComplexContent || nFragOrderType_ == eRestriction )
        return "asnMission";
    else
        return "asnMission." + member.ASNName();
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::MilFilePathName
// Created: AGE 2004-09-16
// -----------------------------------------------------------------------------
std::string AGR_FragOrder::MilFilePathName() const
{
    return "Conduite\\" + MilFileName();
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::HumanName
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_FragOrder::HumanName() const
{
 /*   std::string strResult = BaseName();
    for( unsigned int i = 0; i < strResult.size(); ++i )
        if( strResult.at(i) == '_' )
            strResult.at(i) = ' ';
    return strResult;*/

    
    std::string strResult = BaseName();
    
    int nPos = strResult.find( '_' );
    if( nPos != strResult.npos )
    {
        std::string strTarget = strResult.substr( 0, nPos );
        if(    strTarget == "Automate" 
            || strTarget == "Pion"
            || strTarget == "Population" )
            strResult.insert( nPos, ":" );
    }

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
// Name: AGR_FragOrder::ASNTypeName
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_FragOrder::ASNTypeName() const
{
    return "T_MsgOrderConduite_order_conduite_" + LowName();
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::Mos2ASNPrefixedName
// Created: AGE 2004-09-17
// -----------------------------------------------------------------------------
std::string AGR_FragOrder::Mos2ASNPrefixedName( const AGR_Member& member ) const
{
    if( nFragOrderType_ == eComplexContent )
        return "asnMission";
    else if( nFragOrderType_ == eRestriction )
        return "pASNMsgOrder_->GetAsnMsg().order_conduite.u." + member.ASNName();
    else
        return "asnMission." + member.ASNName();    
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::BaseName
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_FragOrder::BaseName() const
{
    return Name().substr( std::string( "OrderConduite_" ).size() );
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::EnumName
// Created: AGE 2004-09-20
// -----------------------------------------------------------------------------
std::string AGR_FragOrder::EnumName() const
{
    return "eOrdreConduite_" + BaseName();
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::SetDemandLowName
// Created: AGE 2004-09-23
// -----------------------------------------------------------------------------
void AGR_FragOrder::SetDemandLowName( const std::string& str )
{
    strDemandLowName_ = str;
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::SetLowName
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
void AGR_FragOrder::SetLowName( const std::string& strLowName )
{
    AGR_Class::SetLowName( strLowName );
    if( ( nFragOrderType_ == eRestriction || nFragOrderType_ == eComplexContent ) && ! MemberList().empty() )
        MemberList().front()->ChangeName( strLowName );
}


// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::NeedsDeleting
// Created: APE 2004-05-14
// -----------------------------------------------------------------------------
bool AGR_FragOrder::NeedsDeleting() const
{
    return MemberList().size() >= 1 && nFragOrderType_ == eSequence; 
}


// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::IsAvailableForAllMissions
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
bool AGR_FragOrder::IsAvailableForAllMissions() const
{
    return bAvailableForAllMissions_;
}

// -----------------------------------------------------------------------------
// Name: AGR_FragOrder::IsAvailableWithoutMissions
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
bool AGR_FragOrder::IsAvailableWithoutMissions() const
{
    return bAvailableWithoutMissions_;
}
