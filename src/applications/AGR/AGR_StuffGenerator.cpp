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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_StuffGenerator.cpp $
// $Author: Age $
// $Modtime: 21/09/04 14:59 $
// $Revision: 1 $
// $Workfile: AGR_StuffGenerator.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_StuffGenerator.h"
#include "AGR_Mission.h"
#include "AGR_Workspace.h"

// -----------------------------------------------------------------------------
// Name: AGR_StuffGenerator constructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_StuffGenerator::AGR_StuffGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_StuffGenerator destructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_StuffGenerator::~AGR_StuffGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_StuffGenerator::Generate
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_StuffGenerator::Generate( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    std::cout << "Generating stuff" << std::endl;
    GenerateMissionToolFile        ( workspace, strOutputPath );
    GenerateMilMissionConverterFile( workspace, strOutputPath );
}

// -----------------------------------------------------------------------------
// Name: AGR_StuffGenerator::GenerateMissionToolFile
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_StuffGenerator::GenerateMissionToolFile( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strBaseContent;
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_OrderTools_Skeleton.cpp", strBaseContent );

    std::string strMissionPionInclude;
    std::string strMissionAutomateInclude;
    std::string strMissionPopulationInclude;
    std::string strMissionUnitCreation;
    std::string strMissionAutomatCreation;
    std::string strMissionPopulationCreation;
    std::string strMissionDiaInit;
    std::string strMissionPionIdConversion;
    std::string strMissionAutomateIdConversion;
    std::string strMissionPopulationIdConversion;

    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
        std::string strClassName; 

        if( mission.IsOfMissionType( AGR_Mission::eMissionAutomate ) )
        {
            strClassName = "MIL_AutomateMission_" + mission.BaseName();
            strMissionAutomateInclude += "#include \"Automate/" + strClassName + ".h\"\n";
            strMissionAutomatCreation += "        case " + mission.EnumName() + ": return new " + strClassName + "();\n";
            strMissionAutomateIdConversion += "        case T_Mission_Automate_" + mission.LowName() + ": return " + mission.EnumName() + ";\n";
        }
        else if( mission.IsOfMissionType( AGR_Mission::eMissionPopulation ) )
        {
            strClassName = "MIL_PopulationMission_" + mission.BaseName();
            strMissionPopulationInclude += "#include \"Population/" + strClassName + ".h\"\n";
            strMissionPopulationCreation += "        case " + mission.EnumName() + ": return new " + strClassName + "();\n";
            strMissionPopulationIdConversion += "        case T_Mission_Population_" + mission.LowName() + ": return " + mission.EnumName() + ";\n";
        }
        else
        {
            strClassName = "MIL_PionMission_" + mission.BaseName();
            strMissionPionInclude += "#include \"Pion/" + strClassName + ".h\"\n";
            strMissionUnitCreation += "        case " + mission.EnumName() + ": return new " + strClassName + "();\n";
            strMissionPionIdConversion += "        case T_Mission_Pion_" + mission.LowName() + ": return " + mission.EnumName() + ";\n";
        }
        strMissionDiaInit += "    " + strClassName + "::InitializeDIA();\n";
    }

    workspace.ReplaceInString( strBaseContent, "$IncludePionMissionHeaders$", strMissionPionInclude );
    workspace.ReplaceInString( strBaseContent, "$IncludeAutomateMissionHeaders$", strMissionAutomateInclude );
    workspace.ReplaceInString( strBaseContent, "$IncludePopulationMissionHeaders$", strMissionPopulationInclude );
    workspace.ReplaceInString( strBaseContent, "$CreatePionMission$", strMissionUnitCreation );
    workspace.ReplaceInString( strBaseContent, "$CreateAutomateMission$", strMissionAutomatCreation );
    workspace.ReplaceInString( strBaseContent, "$CreatePopulationMission$", strMissionPopulationCreation );
    workspace.ReplaceInString( strBaseContent, "$InitMissionDiaIdx$", strMissionDiaInit );
    workspace.ReplaceInString( strBaseContent, "$ConvertPionIdAsnToSim$", strMissionPionIdConversion );
    workspace.ReplaceInString( strBaseContent, "$ConvertAutomateIdAsnToSim$", strMissionAutomateIdConversion );
    workspace.ReplaceInString( strBaseContent, "$ConvertPopulationIdAsnToSim$", strMissionPopulationIdConversion );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent,  strOutputPath + "/src/libraries/simulation_missions/MIL_OrderTools.cpp" );
}
    
// -----------------------------------------------------------------------------
// Name: AGR_StuffGenerator::GenerateMilMissionConverterFile
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_StuffGenerator::GenerateMilMissionConverterFile( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strConverterList;
    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
        strConverterList += "    converter< E_MissionID >(\"";
        std::string strConverterInput = mission.BaseName();
        workspace.ReplaceInString( strConverterInput, "_", " " );
        strConverterList += strConverterInput + "\", " + mission.EnumName() + " ),\n";
    }

    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_MissionConverter_Skeleton.cpp", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$MissionConverterList$", strConverterList );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent,  strOutputPath + "/src/libraries/simulation_missions/MIL_MissionConverter.cpp" );
}

