// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-09-21 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MosGenerator.cpp $
// $Author: Nld $
// $Modtime: 18/10/04 13:03 $
// $Revision: 2 $
// $Workfile: AGR_MosGenerator.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_MosGenerator.h"
#include "AGR_Workspace.h"
#include "AGR_Mission.h"
#include "AGR_RC.h"

// -----------------------------------------------------------------------------
// Name: AGR_MosGenerator constructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_MosGenerator::AGR_MosGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_MosGenerator destructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_MosGenerator::~AGR_MosGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_MosGenerator::Generate
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_MosGenerator::Generate( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    MT_MakeDir( strOutputPath + "/MOS_Light" );
    std::cout << "Generating MOS_Light files" << std::endl;
    GenerateMosMissionHeaderFiles( workspace, strOutputPath );
    GenerateMosMissionCppFiles   ( workspace, strOutputPath );
    GenerateMosMissionConverter  ( workspace, strOutputPath );
    GenerateMosRcs               ( workspace, strOutputPath );
}

// -----------------------------------------------------------------------------
// Name: AGR_MosGenerator::GenerateMosMissionHeaderFiles
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_MosGenerator::GenerateMosMissionHeaderFiles( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strPionFunctionDeclaration;
    std::string strAutomateFunctionDeclaration;
    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
        std::string strDeclaration = "    void CreateMission_" + mission.BaseName() + "();\n";
        if( mission.IsMissionForAutomate() )
            strAutomateFunctionDeclaration += strDeclaration;
        else
            strPionFunctionDeclaration += strDeclaration;
    }

    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_MOS_MissionPion_Skeleton.h", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$MissionCreatorDeclarations$", strPionFunctionDeclaration );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/MOS_Light/MOS_MissionPion.h" );

    strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_MOS_MissionAutomate_Skeleton.h", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$MissionCreatorDeclarations$", strAutomateFunctionDeclaration );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent,  strOutputPath + "MOS_Light/MOS_MissionAutomate.h" );
}

// -----------------------------------------------------------------------------
// Name: AGR_MosGenerator::GenerateMosMissionCppFiles
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_MosGenerator::GenerateMosMissionCppFiles( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strAsnDeletionPion;
    std::string strAsnDeletionAutomate;
    std::string strMissionCreationPion;
    std::string strMissionCreationAutomate;
    std::string strMissionFunctionsPion;
    std::string strMissionFunctionsAutomate;

    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
//        if( mission.Name().find( "Test" ) != std::string::npos )
//        continue;
        std::string strAsnDeletion = "        case " + mission.ASNTypeName() + " : delete pASNMsgOrder_->GetAsnMsg().mission.u." + mission.LowName() + "; break;\n";
        std::string strCreation    = "        case " + mission.EnumName() + " : CreateMission_" + mission.BaseName() + "(); break;\n";
        std::string strFunctions   = mission.GenerateMosImplementation() + "\n";
        if( mission.IsMissionForAutomate() )
        {
            strAsnDeletionAutomate += strAsnDeletion;
            strMissionCreationAutomate += strCreation;
            strMissionFunctionsAutomate += strFunctions;
        }
        else
        {
            strAsnDeletionPion += strAsnDeletion;
            strMissionCreationPion += strCreation;
            strMissionFunctionsPion += strFunctions;
        }
    }

    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_MOS_MissionPion_Skeleton.cpp", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$AsnMissionDeletion$", strAsnDeletionPion );
    workspace.ReplaceInString( strBaseContent, "$MosMissionCreation$", strMissionCreationPion );
    workspace.ReplaceInString( strBaseContent, "$MissionCreatorImplementations$", strMissionFunctionsPion );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent, strOutputPath + "MOS_Light/MOS_MissionPion.cpp" );

    strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_MOS_MissionAutomate_Skeleton.cpp", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$AsnMissionDeletion$", strAsnDeletionAutomate );
    workspace.ReplaceInString( strBaseContent, "$MosMissionCreation$", strMissionCreationAutomate );
    workspace.ReplaceInString( strBaseContent, "$MissionCreatorImplementations$", strMissionFunctionsAutomate );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent, strOutputPath + "MOS_Light/MOS_MissionAutomate.cpp" );
}
    
// -----------------------------------------------------------------------------
// Name: AGR_MosGenerator::GenerateMosMissionConverter
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_MosGenerator::GenerateMosMissionConverter( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strConverterList;
    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
        strConverterList += "    T_MissionConverter(\"";
        std::string strConverterInput = mission.Name().substr( std::string( "Mission_" ).size() );
        workspace.ReplaceInString( strConverterInput, "_", " " );
        strConverterList += strConverterInput + "\", \"";
        strConverterList += mission.HumanName();
        strConverterList += "\", " + mission.EnumName() + " ),\n";
    }

    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_MOS_MissionConverter_Skeleton.cpp", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$MissionConverterList$", strConverterList );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent,  strOutputPath + "MOS_Light/MOS_MissionConverter.cpp" );
}


// -----------------------------------------------------------------------------
// Name: AGR_MosGenerator::GenerateMosRcs
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
void AGR_MosGenerator::GenerateMosRcs( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "MOS_RC.cpp.skel", strBaseContent );

    std::stringstream strTmp;

    const AGR_Workspace::T_RCVector& rcs = workspace.GetRCs();
    for( AGR_Workspace::CIT_RCVector it = rcs.begin(); it != rcs.end(); ++it )
    {
        const AGR_RC& rc = **it;

        strTmp << "        ";
        strTmp << rc.GenerateMOS();
    }

    workspace.ReplaceInString( strBaseContent, "$AGR_MOS_RC$", strTmp.str() );
    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/MOS_Light/MOS_RC.cpp" );
}
