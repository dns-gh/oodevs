// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-17 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/AutomatType.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:06 $
// $Revision: 2 $
// $Workfile: AutomatType.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"

#include "AutomatType.h"
#include "PawnType.h"


using namespace TIC;

AutomatType::T_AutomatTypeMap AutomatType::automatTypes_;

// -----------------------------------------------------------------------------
// Name: AutomatType constructor
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
AutomatType::AutomatType( const std::string& strName, XmlInputArchive& archive )
    : strName_ ( strName )
{
    try   
    {
        std::string strValue;

        archive.ReadField    ( "MosID", nId_ );

        archive.Section      ( "PionPC" );
        archive.ReadAttribute( "type", strValue );
        archive.EndSection   (); // PionPC

        pPcPawnType_ = PawnType::Find( strValue );
        assert( pPcPawnType_ );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( "Parse error " << exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatType destructor
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
AutomatType::~AutomatType()
{
}

// -----------------------------------------------------------------------------
// Name: AutomatType::Initialize
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
void AutomatType::Initialize( const std::string& strScipioAutomatConfigFile )
{
    try
    {
        std::string         strCurrentDir = MT_GetCurrentDir();
        std::string         strDir;
        std::string         strFile;
        MT_ExtractFilePath  ( strScipioAutomatConfigFile, strDir  );
        MT_ExtractFileName  ( strScipioAutomatConfigFile, strFile );
        MT_ChangeDir        ( strDir );

        XmlInputArchive    archive;
        
        uint16             nAutomatTypesLoaded = 0;

        archive.Open       ( strFile );

        archive.BeginList  ( "Automates" );
        while( archive.NextListElement() )
        {
            archive.Section( "Unite" );

            std::string strName;
            archive.ReadAttribute( "nom", strName );

            AutomatType* pAutomatType = new AutomatType( strName, archive );
            automatTypes_[ pAutomatType->GetName() ] = pAutomatType;

            archive.EndSection(); // Unite

            nAutomatTypesLoaded++;
        }
        archive.EndList    (); // Automates

        archive.Close      ();
        MT_ChangeDir        ( strCurrentDir );

        MT_LOG_INFO_MSG( nAutomatTypesLoaded << " Automat types loaded" );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatType::Terminate
// Created: SBO 2005-05-24
// -----------------------------------------------------------------------------
void AutomatType::Terminate()
{
    for( CIT_AutomatTypeMap it = automatTypes_.begin(); it != automatTypes_.end(); ++it )
        delete it->second;
    automatTypes_.clear();
}
