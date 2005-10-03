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
#include "AGR_TesterMissionGenerator.h"
#include "AGR_Mission.h"
#include "AGR_Workspace.h"

// -----------------------------------------------------------------------------
// Name: AGR_TesterMissionGenerator constructor
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
AGR_TesterMissionGenerator::AGR_TesterMissionGenerator()
{
    //NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: AGR_TesterMissionGenerator destructor
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
AGR_TesterMissionGenerator::~AGR_TesterMissionGenerator()
{
    //NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: AGR_TesterMissionGenerator::Generate
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void AGR_TesterMissionGenerator::Generate( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    MT_MakeDir( strOutputPath + "/TesterMissions" );
    MT_MakeDir( strOutputPath + "/TesterMissions/Pawn" );
    MT_MakeDir( strOutputPath + "/TesterMissions/Automat" );
    MT_MakeDir( strOutputPath + "/TesterMissions/Population" );
    MT_MakeDir( strOutputPath + "/TesterMissions/Order" );

    std::cout << "Generating TESTER mission files" << std::endl;
    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
        mission.GenerateTesterClassHeader( workspace, strOutputPath );
        mission.GenerateTesterClassCpp   ( workspace, strOutputPath );
    }
}
