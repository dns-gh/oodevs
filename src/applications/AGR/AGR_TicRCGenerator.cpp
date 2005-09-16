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
#include "AGR_TicRCGenerator.h"
#include "AGR_Workspace.h"
#include "AGR_Mission.h"
#include "AGR_RC.h"
#include "AGR_TypeRC.h"

// -----------------------------------------------------------------------------
// Name: AGR_TicRCGenerator constructor
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
AGR_TicRCGenerator::AGR_TicRCGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_TicRCGenerator destructor
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
AGR_TicRCGenerator::~AGR_TicRCGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_TicRCGenerator::Generate
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void AGR_TicRCGenerator::Generate( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    MT_MakeDir( strOutputPath + "/Tic" );

    std::cout << "Generating TIC project files" << std::endl;
    GenerateMessageRCCPPFile    ( workspace, strOutputPath );
    GenerateMessageRCHeaderFile ( workspace, strOutputPath );
}

// -----------------------------------------------------------------------------
// Name: AGR_TicRCGenerator::GenerateMessageRCCPPFile
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void AGR_TicRCGenerator::GenerateMessageRCCPPFile( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    // MessageRC.cpp
    AGR_TypeRC::Initialize();

    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_TicMessageRC.cpp.skel", strBaseContent );

    std::stringstream strCodeInitialize;

    const AGR_Workspace::T_RCVector& rcs = workspace.GetRCs();
    for( AGR_Workspace::CIT_RCVector it = rcs.begin(); it != rcs.end(); ++it )
    {
        const AGR_RC& rc = **it;

        strCodeInitialize << rc.GenerateTICRCClassBodyInitialize();
        strCodeInitialize << "\t";
    }  

    workspace.ReplaceInString( strBaseContent, "$AGR_TIC_InitializeRcs$", strCodeInitialize.str() );
    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/Tic/MessageReport.cpp" );

    AGR_TypeRC::Terminate();
}

// -----------------------------------------------------------------------------
// Name: AGR_TicRCGenerator::GenerateMessageRCHeaderFile
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
void AGR_TicRCGenerator::GenerateMessageRCHeaderFile( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    // MessageRC.h
    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_TicMessageRC.h.skel", strBaseContent );

    std::stringstream strTmp;

    const AGR_Workspace::T_RCVector& rcs = workspace.GetRCs();
    for( AGR_Workspace::CIT_RCVector it = rcs.begin(); it != rcs.end(); ++it )
    {
        const AGR_RC& rc = **it;

        strTmp << "        ";
        strTmp << rc.GenerateMILRCClassHeader();
    }   
    workspace.ReplaceInString( strBaseContent, "$AGR_TIC_EnumRcs$", strTmp.str() );
    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/Tic/MessageReport.h" );
}


