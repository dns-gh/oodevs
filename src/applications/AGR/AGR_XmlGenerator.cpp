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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_XmlGenerator.cpp $
// $Author: Age $
// $Modtime: 21/09/04 12:00 $
// $Revision: 1 $
// $Workfile: AGR_XmlGenerator.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_XmlGenerator.h"
#include "AGR_Mission.h"
#include "AGR_FragOrder.h"
#include "AGR_Workspace.h"
#include "AGR_RC.h"

// -----------------------------------------------------------------------------
// Name: AGR_XmlGenerator constructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_XmlGenerator::AGR_XmlGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_XmlGenerator destructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_XmlGenerator::~AGR_XmlGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_XmlGenerator::GenerateMissions
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
void AGR_XmlGenerator::GenerateMissions( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    std::cout << "Generating xml mission file" << std::endl;

    uint nIdx = 1;

    std::stringstream strPionXMLMissions;
    std::stringstream strAutomateXMLMissions;
    std::stringstream strPopulationXMLMissions;
    strPionXMLMissions       << "<units>" << std::endl;
    strAutomateXMLMissions   << "<automats>" << std::endl;
    strPopulationXMLMissions << "<populations>" << std::endl;
    for( AGR_Workspace::CIT_Mission_Vector it = workspace.Missions().begin(); it != workspace.Missions().end(); ++it )
    {
        const AGR_Mission& mission = **it;
        if( mission.IsOfMissionType( AGR_Mission::eMissionPion ) )
            strPionXMLMissions << mission.GenerateMissionXML( nIdx++ );
        else if( mission.IsOfMissionType( AGR_Mission::eMissionAutomate ) )
            strAutomateXMLMissions << mission.GenerateMissionXML( nIdx ++ );
        else if( mission.IsOfMissionType( AGR_Mission::eMissionPopulation ) )
            strPopulationXMLMissions << mission.GenerateMissionXML( nIdx ++ );
    }
    strPionXMLMissions       << "</units>" << std::endl;
    strAutomateXMLMissions   << "</automats>" << std::endl;
    strPopulationXMLMissions << "</populations>" << std::endl;

    std::stringstream strXMLFragOrders;
    strXMLFragOrders << "<fragorders>" << std::endl;
    for( AGR_Workspace::CIT_FragOrder_Vector it = workspace.FragOrders().begin(); it != workspace.FragOrders().end(); ++it )
    {
        const AGR_FragOrder& order = **it;
        strXMLFragOrders << order.GenerateMissionXML( nIdx ++ );
    }
    strXMLFragOrders << "</fragorders>" << std::endl;


    std::stringstream strOutput;
    strOutput << strPionXMLMissions.str() << std::endl
              << strAutomateXMLMissions.str() << std::endl
              << strPopulationXMLMissions.str() << std::endl
              << strXMLFragOrders.str();
   
    workspace.WriteStringInFile( strOutput.str(), strOutputPath + "/xml/missions.xml");
}

// -----------------------------------------------------------------------------
// Name: AGR_XmlGenerator::GenerateRCs
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
void AGR_XmlGenerator::GenerateRCs( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    std::cout << "Generating xml rcs file" << std::endl;

    std::stringstream strXML;
    strXML << "<reports>" << std::endl;

    uint nIdx = 0;
    const AGR_Workspace::T_RCVector& rcs = workspace.GetRCs();
    for( AGR_Workspace::CIT_RCVector it = rcs.begin(); it != rcs.end(); ++it )
    {
        const AGR_RC& rc = **it;
        strXML << rc.GenerateXML( nIdx ++ );
    }
    
    strXML << "</reports>" << std::endl;

    workspace.WriteStringInFile( strXML.str(), strOutputPath + "/xml/reports.xml");
}

// -----------------------------------------------------------------------------
// Name: AGR_XmlGenerator::Generate
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_XmlGenerator::Generate( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    MT_MakeDir( strOutputPath + "/xml" );
   
    GenerateMissions( workspace, strOutputPath );
    GenerateRCs     ( workspace, strOutputPath );    
}
