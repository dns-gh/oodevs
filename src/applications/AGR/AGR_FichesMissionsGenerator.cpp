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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_FichesMissionsGenerator.cpp $
// $Author: Nld $
// $Modtime: 29/04/05 14:59 $
// $Revision: 3 $
// $Workfile: AGR_FichesMissionsGenerator.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_FichesMissionsGenerator.h"
#include "AGR_Workspace.h"
#include "AGR_Mission.h"

#include "MT_Tools/MT_Tools_Types.h"

// -----------------------------------------------------------------------------
// Name: AGR_FichesMissionsGenerator constructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_FichesMissionsGenerator::AGR_FichesMissionsGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_FichesMissionsGenerator destructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_FichesMissionsGenerator::~AGR_FichesMissionsGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_FichesMissionsGenerator::Generate
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_FichesMissionsGenerator::Generate( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    MT_MakeDir( strOutputPath + "/FichesMissions" );
    MT_MakeDir( strOutputPath + "/FichesMissions/Missions/" );

    std::cout << "Generating Fiches Missions files" << std::endl;

    GenerateFichesMissions( workspace, strOutputPath );
    GenerateIndex         ( workspace, strOutputPath );
}

// -----------------------------------------------------------------------------
// Name: AGR_FichesMissionsGenerator::GenerateFichesMissions
// Created: NLD 2005-04-29
// -----------------------------------------------------------------------------
void AGR_FichesMissionsGenerator::GenerateFichesMissions( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;

        if( sCaseInsensitiveEqual()( "TEST", mission.WeaponName() ) )
            continue;

        std::string strBaseContent = "";
        workspace.ReadStringFile( AGR_SKEL_DIR "FicheMission_skeleton.html", strBaseContent );
    
        if( mission.IsOfMissionType( AGR_Mission::eMissionAutomate ) )
            workspace.ReplaceInString( strBaseContent, "$AGR_NIVEAU$", "Automate" );
        else if( mission.IsOfMissionType( AGR_Mission::eMissionPopulation ) )
            workspace.ReplaceInString( strBaseContent, "$AGR_NIVEAU$", "Population" );
        else
            workspace.ReplaceInString( strBaseContent, "$AGR_NIVEAU$", "Pion" );

        workspace.ReplaceInString( strBaseContent, "$AGR_ARME$"   , mission.WeaponName      () );
        workspace.ReplaceInString( strBaseContent, "$AGR_MISSION$", mission.HumanMissionName() );
        workspace.ReplaceInString( strBaseContent, "$AGR_IMAGE$"  , mission.Name() + ".png" );

        std::string strParameters;
        strParameters = mission.GenerateFicheMission();
        workspace.ReplaceInString( strBaseContent, "$AGR_MISSIONPARAMETRES$", strParameters );
   
        MT_MakeDir( strOutputPath + "/FichesMissions/Missions/" + mission.WeaponName() );
        workspace.WriteStringInFile( strBaseContent, strOutputPath + "/FichesMissions/Missions/" + mission.WeaponName() + "/" + mission.Name() + ".html" );
    }    
}



// -----------------------------------------------------------------------------
// Name: AGR_FichesMissionsGenerator::GenerateIndex
// Created: NLD 2005-04-29
// -----------------------------------------------------------------------------
void AGR_FichesMissionsGenerator::GenerateIndex( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    typedef std::map< std::string, const AGR_Mission* >                               T_MissionMap;
    typedef T_MissionMap::const_iterator                                              CIT_MissionMap;
    typedef std::map< std::string /*automate/pion*/, T_MissionMap  /*mission name*/ > T_MissionByTypeMap;
    typedef T_MissionByTypeMap::const_iterator                                        CIT_MissionByTypeMap;
    typedef std::map< std::string /*arme*/, T_MissionByTypeMap >                      T_MissionByWeaponMap;
    typedef T_MissionByWeaponMap::const_iterator                                      CIT_MissionByWeaponMap;
    
    T_MissionByWeaponMap missionByWeapons;
    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;

        if( mission.IsOfMissionType( AGR_Mission::eMissionAutomate ) )
            missionByWeapons[ mission.WeaponName() ][ "Automate" ] [ mission.HumanMissionName() ] = &mission;
        else if( mission.IsOfMissionType( AGR_Mission::eMissionPopulation ) )
            missionByWeapons[ mission.WeaponName() ][ "Population" ] [ mission.HumanMissionName() ] = &mission;
        else
            missionByWeapons[ mission.WeaponName() ][ "Pion" ] [ mission.HumanMissionName() ] = &mission;
    }

    std::stringstream strIndexResult;
    for( CIT_MissionByWeaponMap itMissionByWeapon = missionByWeapons.begin(); itMissionByWeapon != missionByWeapons.end(); ++itMissionByWeapon )
    {
        const std::string& strWeapon = itMissionByWeapon->first;

        if( sCaseInsensitiveEqual()( "TEST", strWeapon ) )
            continue;

        std::stringstream strAutomateWeaponIndexResult;
        std::stringstream strPionWeaponIndexResult;

        strIndexResult << "<a href=\"index_missions_" << strWeapon << ".html\">" << strWeapon << "</a><br>" << std::endl;

        std::string strBaseContent = "";
        workspace.ReadStringFile( AGR_SKEL_DIR "FicheMission_index_arme_skeleton.html", strBaseContent );
        workspace.ReplaceInString( strBaseContent, "$AGR_ARME$", strWeapon );

        const T_MissionMap& automateMissions   = missionByWeapons[ strWeapon ][ "Automate"   ];
        const T_MissionMap& pionMissions       = missionByWeapons[ strWeapon ][ "Pion"       ];
        const T_MissionMap& populationMissions = missionByWeapons[ strWeapon ][ "Population" ];

        std::stringstream strTmp;
        for( CIT_MissionMap itMission = automateMissions.begin(); itMission != automateMissions.end(); ++itMission )
            strTmp << "<a href=\"Missions/" << strWeapon << "/" << itMission->second->Name() << ".html\">" << (itMission->first) << "</a></br>" << std::endl;
        workspace.ReplaceInString( strBaseContent, "$AGR_MISSIONS_AUTOMATE$", strTmp.str() );

        std::stringstream strTmp2;
        for( CIT_MissionMap itMission = pionMissions.begin(); itMission != pionMissions.end(); ++itMission )
            strTmp2 << "<a href=\"Missions/" << strWeapon << "/" << itMission->second->Name() << ".html\">" << (itMission->first) << "</a></br>" << std::endl;
        workspace.ReplaceInString( strBaseContent, "$AGR_MISSIONS_PION$", strTmp2.str() );

        std::stringstream strTmp3;
        for( CIT_MissionMap itMission = populationMissions.begin(); itMission != populationMissions.end(); ++itMission )
            strTmp2 << "<a href=\"Missions/" << strWeapon << "/" << itMission->second->Name() << ".html\">" << (itMission->first) << "</a></br>" << std::endl;
        workspace.ReplaceInString( strBaseContent, "$AGR_MISSIONS_POPULATION$", strTmp2.str() );

        workspace.WriteStringInFile( strBaseContent, strOutputPath + "/FichesMissions/index_missions_" + strWeapon + ".html" );
    }

    MT_MakeDir( strOutputPath + "/FichesMissions/Missions" );

    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "FicheMission_index_skeleton.html", strBaseContent );
    workspace.ReplaceInString( strBaseContent, "$AGR_MISSIONS_ARMES$", strIndexResult.str() );
    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/FichesMissions/index.html" );
}
       

