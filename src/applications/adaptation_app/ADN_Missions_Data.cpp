// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-14 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Missions_Data.cpp $
// $Author: Ape $
// $Modtime: 22/04/05 15:54 $
// $Revision: 3 $
// $Workfile: ADN_Missions_Data.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_Xml_Exception.h"
#include "ADN_Tools.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ENT/ENT_Tr.h"


// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data constructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Missions_Data::ADN_Missions_Data()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data destructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Missions_Data::~ADN_Missions_Data()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::FilesNeeded
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szMissions_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::Reset
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::Reset()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Missions" );
    
    input.Section( "MissionReddition" );
    input.ReadAttribute( "nom", nSurrenderMission_, ENT_Tr::ConvertToAutomataMission, ADN_XmlInput_Helper::eThrow );
    input.EndSection(); // MissionReddition

    input.Section( "MissionSeRendreVersCampRefugies" );
    input.ReadAttribute( "nom", nGoToRefugeeCampMission_, ENT_Tr::ConvertToAutomataMission, ADN_XmlInput_Helper::eThrow );
    input.EndSection(); // MissionSeRendreVersCampRefugies

    input.EndSection(); // Missions
}


// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Missions_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Missions" );

    output.Section( "MissionReddition" );
    output.WriteAttribute( "nom", ENT_Tr::ConvertFromAutomataMission( nSurrenderMission_.GetData() ) );
    output.EndSection(); // MissionReddition

    output.Section( "MissionSeRendreVersCampRefugies" );
    output.WriteAttribute( "nom", ENT_Tr::ConvertFromAutomataMission( nGoToRefugeeCampMission_.GetData() ) );
    output.EndSection(); // MissionSeRendreVersCampRefugies

    output.EndSection(); // Missions
}
