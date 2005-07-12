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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MilFragOrderGenerator.cpp $
// $Author: Age $
// $Modtime: 21/09/04 11:58 $
// $Revision: 1 $
// $Workfile: AGR_MilFragOrderGenerator.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "AGR_pch.h"
#include "AGR_MilFragOrderGenerator.h"
#include "AGR_FragOrder.h"
#include "AGR_Workspace.h"

// -----------------------------------------------------------------------------
// Name: AGR_MilFragOrderGenerator constructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_MilFragOrderGenerator::AGR_MilFragOrderGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_MilFragOrderGenerator destructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_MilFragOrderGenerator::~AGR_MilFragOrderGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_MilFragOrderGenerator::Generate
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_MilFragOrderGenerator::Generate( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    MT_MakeDir( strOutputPath + "/Missions" );
    MT_MakeDir( strOutputPath + "/Missions/Pion" );
    MT_MakeDir( strOutputPath + "/Missions/Automate" );
    MT_MakeDir( strOutputPath + "/Missions/Conduite" );

    std::cout << "Generating MIL frag orders files" << std::endl;
    for( AGR_Workspace::CIT_FragOrder_Vector it = workspace.FragOrders().begin(); it != workspace.FragOrders().end(); ++it )
    {
        const AGR_FragOrder& order = **it;
        order.GenerateMilClassHeader( workspace, strOutputPath );
        order.GenerateMilClassCpp( workspace, strOutputPath );
    }
}
