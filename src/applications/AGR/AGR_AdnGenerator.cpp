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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_AdnGenerator.cpp $
// $Author: Age $
// $Modtime: 21/09/04 11:53 $
// $Revision: 1 $
// $Workfile: AGR_AdnGenerator.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_AdnGenerator.h"
#include "AGR_Workspace.h"
#include "AGR_Mission.h"

// -----------------------------------------------------------------------------
// Name: AGR_AdnGenerator constructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_AdnGenerator::AGR_AdnGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_AdnGenerator destructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_AdnGenerator::~AGR_AdnGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_AdnGenerator::Generate
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_AdnGenerator::Generate( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    MT_MakeDir( strOutputPath + "/ADN" );
    std::cout << "Generating Adn files" << std::endl;
    GenerateAdnEnumFile        ( workspace, strOutputPath );
    GenerateAdnMissionConverter( workspace, strOutputPath );
}

// -----------------------------------------------------------------------------
// Name: AGR_AdnGenerator::GenerateAdnEnumFile
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_AdnGenerator::GenerateAdnEnumFile( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strCommomMissionPion( "\n" );
    std::string strAbcMissionPion( "\n" );
    std::string strInfMissionPion( "\n" );
    std::string strGenMissionPion( "\n" );
    std::string str3dMissionPion( "\n" );
    std::string strLogMissionPion( "\n" );
    std::string strAlatMissionPion( "\n" );
    std::string strRensMissionPion( "\n" );
    std::string strNbcMissionPion( "\n" );

    std::string strCommomMissionAutomate( "\n" );
    std::string strAbcMissionAutomate( "\n" );
    std::string strInfMissionAutomate( "\n" );
    std::string strGenMissionAutomate( "\n" );
    std::string str3dMissionAutomate( "\n" );
    std::string strLogMissionAutomate( "\n" );
    std::string strAlatMissionAutomate( "\n" );
    std::string strRensMissionAutomate( "\n" );
    std::string strNbcMissionAutomate( "\n" );
    
    std::string strMissionTest;

    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
        std::string strMissionEnum = "    e" + mission.Name() + ",\n";
        if( mission.Name().find( "_Test_" ) != std::string::npos )
        {
            strMissionTest += strMissionEnum;
            continue;
        }

        if( mission.IsMissionForAutomate() )
        {
            if( IsOfType( mission, "ABC" ) )
                strAbcMissionAutomate += strMissionEnum;
            else if( IsOfType( mission, "INF" ) )
                strInfMissionAutomate += strMissionEnum;
            else if( IsOfType( mission, "GEN" ) )
                strGenMissionAutomate += strMissionEnum;
            else if( IsOfType( mission, "ASS" ) )
                str3dMissionAutomate += strMissionEnum;
            else if( IsOfType( mission, "ASA" ) )
                str3dMissionAutomate += strMissionEnum;
            else if( IsOfType( mission, "BSD" ) )
                strLogMissionAutomate += strMissionEnum;
            else if( IsOfType( mission, "TC2" ) )
                strLogMissionAutomate += strMissionEnum;
            else if( IsOfType( mission, "ALAT" ) )
                strAlatMissionAutomate += strMissionEnum;
            else if( IsOfType( mission, "RENS" ) )
                strRensMissionAutomate += strMissionEnum;
            else if( IsOfType( mission, "NBC" ) )
                strNbcMissionAutomate += strMissionEnum;
            else 
                strCommomMissionAutomate += strMissionEnum;
        }
        else
        {
            if( IsOfType( mission, "ABC" ) )
                strAbcMissionPion += strMissionEnum;
            else if( IsOfType( mission, "INF" ) )
                strInfMissionPion += strMissionEnum;
            else if( IsOfType( mission, "GEN" ) )
                strGenMissionPion += strMissionEnum;
            else if( IsOfType( mission, "ASS" ) )
                str3dMissionPion += strMissionEnum;
            else if( IsOfType( mission, "ASA" ) )
                str3dMissionPion += strMissionEnum;
            else if( IsOfType( mission, "BSD" ) )
                strLogMissionPion += strMissionEnum;
            else if( IsOfType( mission, "TC2" ) )
                strLogMissionPion += strMissionEnum;
            else if( IsOfType( mission, "ALAT" ) )
                strAlatMissionPion += strMissionEnum;
            else if( IsOfType( mission, "RENS" ) )
                strRensMissionPion += strMissionEnum;
            else if( IsOfType( mission, "NBC" ) )
                strNbcMissionPion += strMissionEnum;
            else
                strCommomMissionPion += strMissionEnum;
        }
    }

    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_ADN_Generated_Enums_Skeleton.h", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$CommonMissionPion$", strCommomMissionPion );
    workspace.ReplaceInString( strBaseContent, "$AbcMissionPion$", strAbcMissionPion );
    workspace.ReplaceInString( strBaseContent, "$InfMissionPion$", strInfMissionPion );
    workspace.ReplaceInString( strBaseContent, "$GenMissionPion$", strGenMissionPion );
    workspace.ReplaceInString( strBaseContent, "$3dMissionPion$", str3dMissionPion );
    workspace.ReplaceInString( strBaseContent, "$AlatMissionPion$", strAlatMissionPion );
    workspace.ReplaceInString( strBaseContent, "$LogMisssionPion$", strLogMissionPion );
    workspace.ReplaceInString( strBaseContent, "$RensMisssionPion$", strRensMissionPion );
    workspace.ReplaceInString( strBaseContent, "$NbcMisssionPion$", strNbcMissionPion );
    workspace.ReplaceInString( strBaseContent, "$CommonMissionAutomate$", strCommomMissionAutomate );
    workspace.ReplaceInString( strBaseContent, "$AbcMissionAutomate$", strAbcMissionAutomate );
    workspace.ReplaceInString( strBaseContent, "$InfMissionAutomate$", strInfMissionAutomate );
    workspace.ReplaceInString( strBaseContent, "$GenMissionAutomate$", strGenMissionAutomate );
    workspace.ReplaceInString( strBaseContent, "$3dMissionAutomate$", str3dMissionAutomate );
    workspace.ReplaceInString( strBaseContent, "$AlatMissionAutomate$", strAlatMissionAutomate );
    workspace.ReplaceInString( strBaseContent, "$LogMissionAutomate$", strLogMissionAutomate );
    workspace.ReplaceInString( strBaseContent, "$RensMisssionAutomate$", strRensMissionAutomate );
    workspace.ReplaceInString( strBaseContent, "$NbcMisssionAutomate$", strNbcMissionAutomate );
    workspace.ReplaceInString( strBaseContent, "$TestMission$", strMissionTest );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent, strOutputPath + "ADN/ADN_Generated_Enums.h" );
}
    
// -----------------------------------------------------------------------------
// Name: AGR_AdnGenerator::GenerateAdnMissionConverter
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_AdnGenerator::GenerateAdnMissionConverter( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::string strConverterList;
    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        strConverterList += "    T_ConverterMission(\"";
        std::string strConverterInput = (*it)->Name().substr( std::string( "Mission_" ).size() );
        workspace.ReplaceInString( strConverterInput, "_", " " );
        strConverterList += strConverterInput + "\", QT_TRANSLATE_NOOP( \"ADN_Enums\", \"";

        strConverterList += strConverterInput;
        strConverterList += "\"), e" + (*it)->Name() + " ),\n";
    }

    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_ADN_MissionConverter_Skeleton.cpp", strBaseContent );

    workspace.ReplaceInString( strBaseContent, "$MissionConverterList$", strConverterList );
    workspace.ReplaceInString( strBaseContent, "$TIME$", MT_GetCurrentDate() + " - " + MT_GetCurrentTime() );

    workspace.WriteStringInFile( strBaseContent, strOutputPath + "ADN/ADN_MissionConverter.cpp" );
}

// -----------------------------------------------------------------------------
// Name: AGR_AdnGenerator::IsOfType
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
bool AGR_AdnGenerator::IsOfType( const AGR_Mission& mission, const std::string& strType ) const
{
    return mission.Name().find( "_" + strType + "_" ) != std::string::npos;
}
