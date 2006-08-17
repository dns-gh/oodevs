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
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MilRCGenerator.cpp $
// $Author: Nld $
// $Modtime: 29/04/05 14:59 $
// $Revision: 3 $
// $Workfile: AGR_MilRCGenerator.cpp $
//
// *****************************************************************************

#include "AGR_pch.h"

#include "AGR_MilRCGenerator.h"
#include "AGR_Types.h"
#include "AGR_Workspace.h"
#include "AGR_RC.h"


// -----------------------------------------------------------------------------
// Name: AGR_MilRCGenerator constructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_MilRCGenerator::AGR_MilRCGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_MilRCGenerator destructor
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
AGR_MilRCGenerator::~AGR_MilRCGenerator()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AGR_MilRCGenerator::Generate
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_MilRCGenerator::Generate( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    std::cout << "Generating simulation RCs" << std::endl;
    MT_MakeDir( strOutputPath + "/src" );
    MT_MakeDir( strOutputPath + "/src/libraries" );
    MT_MakeDir( strOutputPath + "/src/libraries/simulation_kernel" );
    MT_MakeDir( strOutputPath + "/src/libraries/simulation_kernel/Entities" );
    MT_MakeDir( strOutputPath + "/src/libraries/simulation_kernel/Entities/RC" );
    GenerateMILRCClassHeader( workspace, strOutputPath );
    GenerateMILRCClassBody  ( workspace, strOutputPath );
}

// -----------------------------------------------------------------------------
// Name: AGR_MilRCGenerator::GenerateMILRCClassHeader
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
void AGR_MilRCGenerator::GenerateMILRCClassHeader( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    // MIL_RC.h
    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "MIL_RC.h.skel", strBaseContent );

    std::stringstream strTmp;

    const AGR_Workspace::T_RCVector& rcs = workspace.GetRCs();
    for( AGR_Workspace::CIT_RCVector it = rcs.begin(); it != rcs.end(); ++it )
    {
        const AGR_RC& rc = **it;

        strTmp << "        ";
        strTmp << rc.GenerateMILRCClassHeader();
    }   
    strTmp << "        eNbr" << std::endl;
    workspace.ReplaceInString( strBaseContent, "$AGR_MIL_EnumRcs$", strTmp.str() );
    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/src/libraries/simulation_kernel/Entities/RC/MIL_RC.h" );
}

// -----------------------------------------------------------------------------
// Name: AGR_MilRCGenerator::GenerateMILRCClassBodyInitialize
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
void AGR_MilRCGenerator::GenerateMILRCClassBody( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    // MIL_RC.cpp
    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "MIL_RC.cpp.skel", strBaseContent );

    std::stringstream strCodeInitialize;
    std::stringstream strCodeIncludes;

    const AGR_Workspace::T_RCVector& rcs = workspace.GetRCs();
    for( AGR_Workspace::CIT_RCVector it = rcs.begin(); it != rcs.end(); ++it )
    {
        const AGR_RC& rc = **it;

        strCodeInitialize << rc.GenerateMILRCClassBodyInitialize();
        strCodeInitialize << "\t";

        strCodeIncludes << rc.GenerateMILRCClassBodyIncludes();
    }  

    workspace.ReplaceInString( strBaseContent, "$AGR_MIL_RCs_Initialize$", strCodeInitialize.str() );
    workspace.ReplaceInString( strBaseContent, "$AGR_MIL_RCs_Includes$"  , strCodeIncludes.str() );
    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/src/libraries/simulation_kernel/Entities/RC/MIL_RC.cpp" );
}
