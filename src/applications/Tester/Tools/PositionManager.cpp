// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Tools/PositionManager.cpp $
// $Author: Sbo $
// $Modtime: 18/07/05 15:07 $
// $Revision: 8 $
// $Workfile: PositionManager.cpp $
//
// *****************************************************************************

#include "Tester_Pch.h"
#include "Types.h"
#include "Tools/PositionManager.h"
#include "Tools/Position.h"

#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

using namespace TEST;

//-----------------------------------------------------------------------------
// Name: PositionManager::PositionManager
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
PositionManager::PositionManager()
{
}

//-----------------------------------------------------------------------------
// Name: PositionManager::~PositionManager
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
PositionManager::~PositionManager()
{
}

//-----------------------------------------------------------------------------
// Name: PositionManager::Initialize
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
void PositionManager::Initialize( const std::string& strWorldConfigFile )
{
    try
    {
        std::string         strCurrentDir = MT_GetCurrentDir();
        std::string         strDir;
        std::string         strFile;
        MT_ExtractFilePath  ( strWorldConfigFile, strDir  );
        MT_ExtractFileName  ( strWorldConfigFile, strFile );
        MT_ChangeDir        ( strDir );

        XmlInputArchive     archive;

        std::string         strTmp;
        archive.Open        ( strFile );
        archive.ReadField   ( "World", strTmp );
        archive.Close       ();

        Position::Initialize( strTmp );

        MT_ChangeDir        ( strCurrentDir );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        throw exception;
    }
}

//-----------------------------------------------------------------------------
// Name: PositionManager::Terminate
// Created: SBO 2005-05-23
//-----------------------------------------------------------------------------
void PositionManager::Terminate()
{
    Position::Terminate();
}
