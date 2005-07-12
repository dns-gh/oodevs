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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MilMissionGenerator.cpp $
// $Author: Age $
// $Modtime: 21/09/04 12:00 $
// $Revision: 1 $
// $Workfile: AGR_MilMissionGenerator.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_MilMissionGenerator.h"
#include "AGR_Mission.h"
#include "AGR_Workspace.h"

// -----------------------------------------------------------------------------
// Name: AGR_MilMissionGenerator constructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_MilMissionGenerator::AGR_MilMissionGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_MilMissionGenerator destructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_MilMissionGenerator::~AGR_MilMissionGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_MilMissionGenerator::Generate
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_MilMissionGenerator::Generate( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    MT_MakeDir( strOutputPath + "/Missions" );
    MT_MakeDir( strOutputPath + "/Missions/Pion" );
    MT_MakeDir( strOutputPath + "/Missions/Automate" );
    MT_MakeDir( strOutputPath + "/Missions/Conduite" );

    std::cout << "Generating MIL mission files" << std::endl;
    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
        mission.GenerateMilClassHeader( workspace, strOutputPath );
        mission.GenerateMilClassCpp( workspace, strOutputPath );
    }
}
