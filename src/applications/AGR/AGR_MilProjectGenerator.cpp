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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MilProjectGenerator.cpp $
// $Author: Nld $
// $Modtime: 3/03/05 17:32 $
// $Revision: 6 $
// $Workfile: AGR_MilProjectGenerator.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_MilProjectGenerator.h"
#include "AGR_FragOrder.h"
#include "AGR_Workspace.h"
#include "AGR_Mission.h"

// -----------------------------------------------------------------------------
// Name: AGR_MilProjectGenerator constructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_MilProjectGenerator::AGR_MilProjectGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_MilProjectGenerator destructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_MilProjectGenerator::~AGR_MilProjectGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_MilProjectGenerator::Generate
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_MilProjectGenerator::Generate( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    MT_MakeDir( strOutputPath + "/Missions" );
    MT_MakeDir( strOutputPath + "/Missions/Pion" );
    MT_MakeDir( strOutputPath + "/Missions/Automate" );
    MT_MakeDir( strOutputPath + "/Missions/Conduite" );

    std::cout << "Generating MIL project files" << std::endl;
    GenerateMissionsProjectFile( workspace, strOutputPath );
    GenerateMissionsCPPFile    ( workspace, strOutputPath );
    GenerateMissionsHeaderFile ( workspace, strOutputPath );
}

// -----------------------------------------------------------------------------
// Name: AGR_MilProjectGenerator::GenerateMissionsProjectFile
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_MilProjectGenerator::GenerateMissionsProjectFile( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    std::string strAutomateFileList, strPionFileList, strConduiteFileList, strBaseFileList;
    std::string strIncludeFileList;
    const std::string strSpacings( "                            " );
    const std::string strExcludeFromBuild = strSpacings + "    <FileConfiguration\n"
                                          + strSpacings + "         Name=\"Release|Win32\"\n"
                                          + strSpacings + "         ExcludedFromBuild=\"TRUE\">\n"
                                          + strSpacings + "         <Tool Name=\"VCCLCompilerTool\"/>\n"
                                          + strSpacings + "    </FileConfiguration>\n"
                                          + strSpacings + "    <FileConfiguration\n"
                                          + strSpacings + "         Name=\"Debug|Win32\"\n"
                                          + strSpacings + "         ExcludedFromBuild=\"TRUE\">\n"
                                          + strSpacings + "         <Tool Name=\"VCCLCompilerTool\"/>\n"
                                          + strSpacings + "    </FileConfiguration>\n";


    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
        std::string strFileEntry = strSpacings + "<File\n"
                                 + strSpacings + "    RelativePath=\"..\\..\\src\\libraries\\Missions\\" + mission.MilFilePathName() + ".h\">\n"
                                 + strSpacings + "</File>\n"
                                 + strSpacings + "<File\n"
                                 + strSpacings + "    RelativePath=\"..\\..\\src\\libraries\\Missions\\" + mission.MilFilePathName() + ".cpp\">\n"
                                 + strExcludeFromBuild
                                 + strSpacings + "</File>\n";
        if( mission.IsMissionForAutomate() )
            strAutomateFileList += strFileEntry;
        else
            strPionFileList += strFileEntry;
        strIncludeFileList += "#include \"" + mission.MilFilePathName() + ".cpp\"\n";
    }

    for( AGR_Workspace::CIT_FragOrder_Vector it = workspace.FragOrders().begin(); it != workspace.FragOrders().end(); ++it )
    {
        const AGR_FragOrder& order = **it;
        std::string strFileEntry = strSpacings + "<File\n"
                                 + strSpacings + "    RelativePath=\"..\\..\\src\\libraries\\Missions\\" + order.MilFilePathName() + ".h\">\n"
                                 + strSpacings + "</File>\n"
                                 + strSpacings + "<File\n"
                                 + strSpacings + "    RelativePath=\"..\\..\\src\\libraries\\Missions\\" + order.MilFilePathName() + ".cpp\">\n"
                                 + strExcludeFromBuild
                                 + strSpacings + "</File>\n";
        strConduiteFileList += strFileEntry;
        strIncludeFileList += "#include \"" + order.MilFilePathName() + ".cpp\"\n";
    }


    std::string strBaseContent;
    workspace.ReadStringFile( AGR_SKEL_DIR "Missions.vcproj", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$AutomateFileList$", strAutomateFileList );
    workspace.ReplaceInString( strBaseContent, "$PionFileList$", strPionFileList );
    workspace.ReplaceInString( strBaseContent, "$ConduiteFileList$", strConduiteFileList );

    workspace.WriteStringInFile( strBaseContent,  strOutputPath + "/Missions/Missions.vcproj" );
}

// -----------------------------------------------------------------------------
// Name: AGR_MilProjectGenerator::GenerateMissionsCPPFile
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_MilProjectGenerator::GenerateMissionsCPPFile( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    std::string strIncludeList, strPionRegistrationList, strAutomateRegistrationList, strOrderRegistrationList;
    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
        std::string strType = mission.IsMissionForAutomate() ? "MIL_AutomateMissionType" : "MIL_PionMissionType";
        strIncludeList     += "#include \"" + mission.MilFilePathName() + ".cpp\"\n";
        std::string strRegistration = "    " + strType + "::RegisterMission< " + mission.MilFileName() + ">( \""
            + mission.HumanName() + "\", "
            + mission.EnumName() + ", "
            + mission.ASNTypeName() + ", "
            + "\"" + mission.DIATypeName() + "\", "
            + "\"" + mission.MRTBehavior() + "\""
            + ( mission.IsMissionForAutomate() ? ", \"" + mission.ConduiteBehavior() + "\"" : "" )
            + ");\n";
        if( mission.IsMissionForAutomate() )
            strAutomateRegistrationList += strRegistration;
        else
            strPionRegistrationList += strRegistration;
    }

    for( AGR_Workspace::CIT_FragOrder_Vector it = workspace.FragOrders().begin(); it != workspace.FragOrders().end(); ++it )
    {
        const AGR_FragOrder& order = **it;
        strIncludeList     += "#include \"" + order.MilFilePathName() + ".cpp\"\n";
        strOrderRegistrationList += "    MIL_OrderConduiteType::RegisterOrderConduite< " + order.MilFileName() + ">"
                                  + "( \"" + order.HumanName() + "\""
                                  + ", " + order.EnumName()
                                  + ", " + order.ASNTypeName()
                                  + ", \"" + order.DIATypeName() + "\""
                                  + ", " + ( order.IsAvailableForAllMissions () ? "true" : "false" )
                                  + ", " + ( order.IsAvailableWithoutMissions() ? "true" : "false" )
                                  + ( order.CanBeDemanded() ? ", " + order.ASNDemandName() : "" )
                                  + " );\n";
    }

    std::string strBaseContent;
    workspace.ReadStringFile( AGR_SKEL_DIR "Missions_Skeleton.cpp", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$IncludeList$", strIncludeList );
    workspace.ReplaceInString( strBaseContent, "$AutomateMissionsRegistration$", strAutomateRegistrationList );
    workspace.ReplaceInString( strBaseContent, "$PionMissionsRegistration$", strPionRegistrationList );
    workspace.ReplaceInString( strBaseContent, "$OrderConduiteRegistration$", strOrderRegistrationList );

    workspace.WriteStringInFile( strBaseContent,  strOutputPath + "/Missions/Missions.cpp" );
}

// -----------------------------------------------------------------------------
// Name: AGR_MilProjectGenerator::GenerateMissionsHeaderFile
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_MilProjectGenerator::GenerateMissionsHeaderFile( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    std::string strPionMissionEnumList, strAutomateMissionEnumList, strOrderEnumList;
    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
        const std::string strEnum = "        e" + mission.Name() + ",\n";
        if( mission.IsMissionForAutomate() )
            strAutomateMissionEnumList += strEnum;
        else
            strPionMissionEnumList += strEnum;
    }

    for( AGR_Workspace::CIT_FragOrder_Vector it = workspace.FragOrders().begin(); it != workspace.FragOrders().end(); ++it )
    {
        const AGR_FragOrder& order = **it;
        strOrderEnumList += "        " + order.EnumName() + ",\n";
    }

    std::string strBaseContent;
    workspace.ReadStringFile( AGR_SKEL_DIR "Missions_Skeleton.h", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$AutomateMissionEnumList$", strAutomateMissionEnumList );
    workspace.ReplaceInString( strBaseContent, "$PionMissionEnumList$", strPionMissionEnumList );
    workspace.ReplaceInString( strBaseContent, "$OrderConduiteEnumList$", strOrderEnumList );

    workspace.WriteStringInFile( strBaseContent,  strOutputPath + "/Missions/Missions.h" );
}
