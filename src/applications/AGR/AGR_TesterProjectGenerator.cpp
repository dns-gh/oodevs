// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-10 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_TesterProjectGenerator.h"
#include "AGR_Workspace.h"
#include "AGR_Mission.h"

// -----------------------------------------------------------------------------
// Name: AGR_TesterProjectGenerator constructor
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
AGR_TesterProjectGenerator::AGR_TesterProjectGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_TesterProjectGenerator destructor
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
AGR_TesterProjectGenerator::~AGR_TesterProjectGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_TesterProjectGenerator::Generate
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void AGR_TesterProjectGenerator::Generate( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    MT_MakeDir( strOutputPath + "/src" );
    MT_MakeDir( strOutputPath + "/src/applications" );
    MT_MakeDir( strOutputPath + "/src/applications/simulation_tester" );
    MT_MakeDir( strOutputPath + "/src/applications/simulation_tester/Actions" );
    MT_MakeDir( strOutputPath + "/src/applications/simulation_tester/Actions/Missions" );
    MT_MakeDir( strOutputPath + "/src/applications/simulation_tester/Actions/Missions/Pawn" );
    MT_MakeDir( strOutputPath + "/src/applications/simulation_tester/Actions/Missions/Automat" );
    MT_MakeDir( strOutputPath + "/src/applications/simulation_tester/Actions/Missions/Population" );
    MT_MakeDir( strOutputPath + "/src/applications/simulation_tester/Actions/Missions/Order" );

    std::cout << "Generating simulation tester project files" << std::endl;
    GenerateMissionsCPPFile    ( workspace, strOutputPath );
    GenerateMissionsHeaderFile ( workspace, strOutputPath );
}

// -----------------------------------------------------------------------------
// Name: AGR_TesterProjectGenerator::GenerateMissionsCPPFile
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void AGR_TesterProjectGenerator::GenerateMissionsCPPFile( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    std::string strPawnIncludeList, strPawnRegistrationList;
    std::string strAutomatIncludeList, strAutomatRegistrationList;
    std::string strPopulationIncludeList, strPopulationRegistrationList;

    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
        if( mission.IsOfMissionType( AGR_Mission::eMissionAutomate ) )
        {
            strAutomatIncludeList      += "#include \"Automat/Mission_Automat_" + mission.BaseName() + ".cpp\"\n";
            strAutomatRegistrationList += "    Register< Mission_Automat_" + mission.BaseName() + " >( \"" + mission.HumanName() + "\" );\n";;
        }
        else if( mission.IsOfMissionType( AGR_Mission::eMissionPopulation ) )
        {
            strPopulationIncludeList      += "#include \"Population/Mission_Population_" + mission.BaseName() + ".cpp\"\n";
            strPopulationRegistrationList += "    Register< Mission_Population_" + mission.BaseName() + " >( \"" + mission.HumanName() + "\" );\n";;
        }
        else
        {
            strPawnIncludeList      += "#include \"Pawn/Mission_Pawn_" + mission.BaseName() + ".cpp\"\n";
            strPawnRegistrationList += "    Register< Mission_Pawn_" + mission.BaseName() + " >( \"" + mission.HumanName() + "\" );\n";;
        }
    }

    // pawn types
    std::string strBaseContent;
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_TesterMission_Pawn_Type_Skeleton.cpp", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$IncludeList$", strPawnIncludeList );
    workspace.ReplaceInString( strBaseContent, "$PionMissionsRegistration$", strPawnRegistrationList );

    workspace.WriteStringInFile( strBaseContent,  strOutputPath + "/src/applications/simulation_tester/Actions/Missions/Mission_Pawn_Type.cpp" );

    // automat types
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_TesterMission_Automat_Type_Skeleton.cpp", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$IncludeList$", strAutomatIncludeList );
    workspace.ReplaceInString( strBaseContent, "$AutomateMissionsRegistration$", strAutomatRegistrationList );

    workspace.WriteStringInFile( strBaseContent,  strOutputPath + "/src/applications/simulation_tester/Actions/Missions/Mission_Automat_Type.cpp" );

    // population types
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_TesterMission_Population_Type_Skeleton.cpp", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$IncludeList$", strPopulationIncludeList );
    workspace.ReplaceInString( strBaseContent, "$PopulationMissionsRegistration$", strPopulationRegistrationList );

    workspace.WriteStringInFile( strBaseContent,  strOutputPath + "/src/applications/simulation_tester/Actions/Missions/Mission_Population_Type.cpp" );
}

// -----------------------------------------------------------------------------
// Name: AGR_TesterProjectGenerator::GenerateMissionsHeaderFile
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void AGR_TesterProjectGenerator::GenerateMissionsHeaderFile( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    /*
    std::string strBaseContent;
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_TesterMissions_Pawn_Type_Skeleton.h", strBaseContent );

    workspace.WriteStringInFile( strBaseContent,  strOutputPath + "/src/applications/simulation_tester/Actions/Missions/Mission_Pawn_Type.h" );
    */
}


