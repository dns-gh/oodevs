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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_HalGenerator.cpp $
// $Author: Nld $
// $Modtime: 28/10/04 17:01 $
// $Revision: 5 $
// $Workfile: AGR_HalGenerator.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_HalGenerator.h"
#include "AGR_Workspace.h"
#include "AGR_Mission.h"
#include "AGR_FragOrder.h"
#include "AGR_RC.h"

// -----------------------------------------------------------------------------
// Name: AGR_HalGenerator constructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_HalGenerator::AGR_HalGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_HalGenerator destructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_HalGenerator::~AGR_HalGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_HalGenerator::Generate
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_HalGenerator::Generate( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    //$$$$
    MT_MakeDir( strOutputPath + "/data" );
    MT_MakeDir( strOutputPath + "/data/test" );
    MT_MakeDir( strOutputPath + "/data/test/Data" );
    MT_MakeDir( strOutputPath + "/data/test/Data/Modeles" );
    MT_MakeDir( strOutputPath + "/data/test/Data/Modeles/Sources" );
    MT_MakeDir( strOutputPath + "/data/test/Data/Modeles/Sources/Common" );
    MT_MakeDir( strOutputPath + "/data/test/Data/Modeles/Sources/Common/Moteur" );
    std::cout << "Generating script file" << std::endl;
    GenerateMissionEnumFile  ( workspace, strOutputPath );
    GenerateMissionScriptFile( workspace, strOutputPath );
    GenerateOrderConduiteScriptFile( workspace, strOutputPath );
    GenerateRCEnumFile( workspace, strOutputPath );
}

// -----------------------------------------------------------------------------
// Name: AGR_HalGenerator::GenerateMissionEnumFile
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_HalGenerator::GenerateMissionEnumFile( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strMissionPionEnumeration;
    std::string strMissionAutomateEnumeration;
    std::string strMissionPopulationEnumeration;
    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
        std::string strEntry = "    " + mission.EnumName() + ",\n";
        if( mission.IsOfMissionType( AGR_Mission::eMissionAutomate ) )
            strMissionAutomateEnumeration += strEntry;
        else if( mission.IsOfMissionType( AGR_Mission::eMissionPopulation ) )
            strMissionPopulationEnumeration += strEntry;
        else
            strMissionPionEnumeration += strEntry;
    }

    std::string strOrderEnumeration;
    for( AGR_Workspace::CIT_FragOrder_Vector it = workspace.FragOrders().begin(); it != workspace.FragOrders().end(); ++it )
        strOrderEnumeration += "    " + (*it)->EnumName() + ",\n";


    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "Types_Enum_Missions_Skeleton.hal", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$MissionAutomateEnumeration$", strMissionAutomateEnumeration );
    workspace.ReplaceInString( strBaseContent, "$MissionPionEnumeration$", strMissionPionEnumeration );
    workspace.ReplaceInString( strBaseContent, "$MissionPopulationEnumeration$", strMissionPopulationEnumeration );
    workspace.ReplaceInString( strBaseContent, "$OrderConduiteEnumeration$", strOrderEnumeration );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent,  strOutputPath + "/data/test/Data/Modeles/Sources/Common/Moteur/Types_Enums_Generated.hal" );
}

// -----------------------------------------------------------------------------
// Name: AGR_HalGenerator::GenerateMissionScriptFile
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_HalGenerator::GenerateMissionScriptFile( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strMissionPion;
    std::string strMissionAutomate;
    std::string strMissionPopulation;

    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
        if( mission.IsOfMissionType( AGR_Mission::eMissionAutomate ) )
            strMissionAutomate += mission.GenerateDiaDefinition() + "\n";
        else if( mission.IsOfMissionType( AGR_Mission::eMissionPopulation ) )
            strMissionPopulation += mission.GenerateDiaDefinition() + "\n";
        else
            strMissionPion += mission.GenerateDiaDefinition() + "\n";
    }

    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "Types_Missions_Pion_Skeleton.hal", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$Missions$", strMissionPion );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/data/test/Data/Modeles/Sources/Common/Moteur/Types_Missions_Pion.hal" );

    strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "Types_Missions_Automate_Skeleton.hal", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$Missions$", strMissionAutomate );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent,  strOutputPath + "/data/test/Data/Modeles/Sources/Common/Moteur/Types_Missions_Automate.hal" );

    strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "Types_Missions_Population_Skeleton.hal", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$Missions$", strMissionPopulation );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent,  strOutputPath + "/data/test/Data/Modeles/Sources/Common/Moteur/Types_Missions_Population.hal" );
}

// -----------------------------------------------------------------------------
// Name: AGR_HalGenerator::GenerateOrderConduiteScriptFile
// Created: AGE 2004-09-23
// -----------------------------------------------------------------------------
void AGR_HalGenerator::GenerateOrderConduiteScriptFile( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strFragOrder;
    for( AGR_Workspace::CIT_FragOrder_Vector it = workspace.FragOrders().begin(); it != workspace.FragOrders().end(); ++it )
    {
        const AGR_FragOrder& order = **it;
        strFragOrder += order.GenerateDiaDefinition() + "\n";
    }

    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "Types_OrderConduites_Skeleton.hal", strBaseContent );
    workspace.ReplaceInString( strBaseContent, "$Orders$", strFragOrder );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );
    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/data/test/Data/Modeles/Sources/Common/Moteur/Types_OrdresConduite.hal" );
}


// -----------------------------------------------------------------------------
// Name: AGR_HalGenerator::GenerateRCEnumFile
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
void AGR_HalGenerator::GenerateRCEnumFile( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    // MIL_RC.h
    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "Types_RC.hal.skel", strBaseContent );

    std::stringstream strTmp;

    const AGR_Workspace::T_RCVector& rcs = workspace.GetRCs();
    for( AGR_Workspace::CIT_RCVector it = rcs.begin(); it != rcs.end(); ++it )
    {
        const AGR_RC& rc = **it;

        strTmp << "    ";
        strTmp << rc.GenerateHALEnum();
        
    }   
    strTmp << "    eNbr" << std::endl;
    workspace.ReplaceInString( strBaseContent, "$AGR_HAL_EnumRcs$", strTmp.str() );
    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/data/test/Data/Modeles/Sources/Common/Moteur/Types_RC.hal" );
}
