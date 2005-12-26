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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Mos2Generator.cpp $
// $Author: Age $
// $Modtime: 25/10/04 11:55 $
// $Revision: 3 $
// $Workfile: AGR_Mos2Generator.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_Mos2Generator.h"
#include "AGR_Workspace.h"
#include "AGR_Mission.h"
#include "AGR_FragOrder.h"
#include "AGR_RC.h"

// -----------------------------------------------------------------------------
// Name: AGR_Mos2Generator constructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_Mos2Generator::AGR_Mos2Generator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_Mos2Generator destructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_Mos2Generator::~AGR_Mos2Generator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_Mos2Generator::Generate
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_Mos2Generator::Generate( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    std::cout << "Generating MOS_Light2 files" << std::endl;
    MT_MakeDir( strOutputPath + "/src" );
    MT_MakeDir( strOutputPath + "/src/applications" );
    MT_MakeDir( strOutputPath + "/src/applications/MOS_Light2" );
    GenerateMos2MissionInterfaceHeaderFiles( workspace, strOutputPath );
    GenerateMos2MissionInterfaceCppFiles   ( workspace, strOutputPath );
    GenerateMos2Rcs                        ( workspace, strOutputPath );
}

// -----------------------------------------------------------------------------
// Name: AGR_Mos2Generator::GenerateMos2MissionInterfaceHeaderFiles
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_Mos2Generator::GenerateMos2MissionInterfaceHeaderFiles( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strPionFunctionDeclaration;
    std::string strAutomateFunctionDeclaration;
    std::string strPopulationFunctionDeclaration;
    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
        std::string strDeclaration = "    void CreateMission_" + mission.BaseName() + "();\n";
        if( mission.IsOfMissionType( AGR_Mission::eMissionAutomate ) )
            strAutomateFunctionDeclaration += strDeclaration;
        else if( mission.IsOfMissionType( AGR_Mission::eMissionPopulation ) )
            strPopulationFunctionDeclaration += strDeclaration;
        else
            strPionFunctionDeclaration += strDeclaration;
    }

    // Pour les pions:
    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_MOS_UnitMissionInterface_Gen_Skeleton.h", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$MissionCreatorDeclarations$", strPionFunctionDeclaration );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/src/applications/MOS_Light2/MOS_UnitMissionInterface_Gen.h" );

    // Pour les automates:
    strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_MOS_AutomateMissionInterface_Gen_Skeleton.h", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$MissionCreatorDeclarations$", strAutomateFunctionDeclaration );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/src/applications/MOS_Light2/MOS_AutomateMissionInterface_Gen.h" );

    // Pour les populations:
    strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_MOS_PopulationMissionInterface_Gen_Skeleton.h", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$MissionCreatorDeclarations$", strPopulationFunctionDeclaration );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/src/applications/MOS_Light2/MOS_PopulationMissionInterface_Gen.h" );

    // For fragmentary orders
    std::string strFragOrderDeclaration;
    for( AGR_Workspace::CIT_FragOrder_Vector it = workspace.FragOrders().begin(); it != workspace.FragOrders().end(); ++it )
    {
        const AGR_FragOrder& order = **it;
//        if( order.Name().find( "Test" ) != std::string::npos )
//            continue;
        strFragOrderDeclaration += "    void CreateOrder_"  + order.BaseName() + "();\n";
    }

    strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_MOS_FragmentaryOrderInterface_Gen_Skeleton.h", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$OrderCreatorDeclarations$", strFragOrderDeclaration );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/src/applications/MOS_Light2/MOS_FragmentaryOrderInterface_Gen.h" );
}

// -----------------------------------------------------------------------------
// Name: AGR_Mos2Generator::GenerateMos2MissionInterfaceCppFiles
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_Mos2Generator::GenerateMos2MissionInterfaceCppFiles( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strAsnDeletionPion;
    std::string strAsnDeletionAutomate;
    std::string strAsnDeletionPopulation;
    std::string strMissionCreationPion;
    std::string strMissionCreationAutomate;
    std::string strMissionCreationPopulation;
    std::string strMissionFunctionsPion;
    std::string strMissionFunctionsAutomate;
    std::string strMissionFunctionsPopulation;

    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
//        if( mission.Name().find( "Test" ) != std::string::npos )
//            continue;
        std::string strAsnDeletion = "        case " + mission.ASNTypeName() + " : delete pASNMsgOrder_->GetAsnMsg().mission.u." + mission.LowName() + "; break;\n";
        std::string strCreation    = "        case " + mission.EnumName() + " : CreateMission_" + mission.BaseName() + "(); break;\n";
        std::string strFunctions   = mission.GenerateMos2Implementation() + "\n";

        if( mission.IsOfMissionType( AGR_Mission::eMissionAutomate ) )
        {
            strAsnDeletionAutomate += strAsnDeletion;
            strMissionCreationAutomate += strCreation;
            strMissionFunctionsAutomate += strFunctions;
        }
        else if( mission.IsOfMissionType( AGR_Mission::eMissionPopulation ) )
        {
            strAsnDeletionPopulation += strAsnDeletion;
            strMissionCreationPopulation += strCreation;
            strMissionFunctionsPopulation += strFunctions;
        }
        else
        {
            strAsnDeletionPion += strAsnDeletion;
            strMissionCreationPion += strCreation;
            strMissionFunctionsPion += strFunctions;
        }
    }

    // Pour les pions:
    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_MOS_UnitMissionInterface_Gen_Skeleton.cpp", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$AsnMissionDeletion$", strAsnDeletionPion );
    workspace.ReplaceInString( strBaseContent, "$MosMissionCreation$", strMissionCreationPion );
    workspace.ReplaceInString( strBaseContent, "$MissionCreatorImplementations$", strMissionFunctionsPion );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/src/applications/MOS_Light2/MOS_UnitMissionInterface_Gen.cpp" );

    // Pour les automates:
    strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_MOS_AutomateMissionInterface_Gen_Skeleton.cpp", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$AsnMissionDeletion$", strAsnDeletionAutomate );
    workspace.ReplaceInString( strBaseContent, "$MosMissionCreation$", strMissionCreationAutomate );
    workspace.ReplaceInString( strBaseContent, "$MissionCreatorImplementations$", strMissionFunctionsAutomate );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/src/applications/MOS_Light2/MOS_AutomateMissionInterface_Gen.cpp" );

    // Pour les populations:
    strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_MOS_PopulationMissionInterface_Gen_Skeleton.cpp", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$AsnMissionDeletion$", strAsnDeletionPopulation );
    workspace.ReplaceInString( strBaseContent, "$MosMissionCreation$", strMissionCreationPopulation );
    workspace.ReplaceInString( strBaseContent, "$MissionCreatorImplementations$", strMissionFunctionsPopulation );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/src/applications/MOS_Light2/MOS_PopulationMissionInterface_Gen.cpp" );

    // Fragmentary orders
    std::string strFODeletion;
    std::string strFOCreation;
    std::string strFOFunctions;
    for( AGR_Workspace::CIT_FragOrder_Vector it = workspace.FragOrders().begin(); it != workspace.FragOrders().end(); ++it )
    {
        const AGR_FragOrder& order = **it;
//        if( order.Name().find( "Test" ) != std::string::npos )
//            continue;
        strFOCreation  += order.MosLight2CreationCode();
        strFODeletion  += order.MosLight2DeletionCode();
        strFOFunctions += order.MosLight2ImplementationCode();
    }
    strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_MOS_FragmentaryOrderInterface_Gen_Skeleton.cpp", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$AsnOrderDeletion$", strFODeletion );
    workspace.ReplaceInString( strBaseContent, "$MosOrderCreation$", strFOCreation );
    workspace.ReplaceInString( strBaseContent, "$OrderCreatorImplementations$", strFOFunctions );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/src/applications/MOS_Light2/MOS_FragmentaryOrderInterface_Gen.cpp" );
}

// -----------------------------------------------------------------------------
// Name: AGR_Mos2Generator::GenerateMos2Rcs
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
void AGR_Mos2Generator::GenerateMos2Rcs( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "MOS2_RC.cpp.skel", strBaseContent );

    std::stringstream strTmp;

    const AGR_Workspace::T_RCVector& rcs = workspace.GetRCs();
    for( AGR_Workspace::CIT_RCVector it = rcs.begin(); it != rcs.end(); ++it )
    {
        const AGR_RC& rc = **it;

        strTmp << "        ";
        strTmp << rc.GenerateMOS2();
    }

    workspace.ReplaceInString( strBaseContent, "$AGR_MOS2_RC$", strTmp.str() );
    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/src/applications/MOS_Light2/MOS_RC.cpp" );
}
