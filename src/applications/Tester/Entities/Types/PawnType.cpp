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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/PawnType.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:06 $
// $Revision: 7 $
// $Workfile: PawnType.cpp $
//
// *****************************************************************************

#include "Tester_Pch.h"

#include "PawnType.h"

using namespace TEST;

PawnType::T_PawnTypeMap PawnType::pawnTypes_;

// -----------------------------------------------------------------------------
// Name: PawnType constructor
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
PawnType::PawnType( const std::string& strName, XmlInputArchive& archive )
    : strName_ ( strName )
{
    try   
    {
        std::string strValue;

        archive.ReadField    ( "MosID", nId_ );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( "Parse error " << exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: PawnType destructor
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
PawnType::~PawnType()
{
}

// -----------------------------------------------------------------------------
// Name: PawnType::Initialize
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
void PawnType::Initialize( const std::string& strScipioPawnConfigFile )
{
    try
    {
        std::string         strCurrentDir = MT_GetCurrentDir();
        std::string         strDir;
        std::string         strFile;
        MT_ExtractFilePath  ( strScipioPawnConfigFile, strDir  );
        MT_ExtractFileName  ( strScipioPawnConfigFile, strFile );
        MT_ChangeDir        ( strDir );

        XmlInputArchive    archive;
        
        uint16             nPawnTypesLoaded = 0;

        archive.Open       ( strFile );

        archive.BeginList  ( "Pions" );
        while( archive.NextListElement() )
        {
            archive.Section( "Unite" );

            std::string strName;
            archive.ReadAttribute( "nom", strName );

            PawnType* pPawnType = new PawnType( strName, archive );
            pawnTypes_[ pPawnType->GetName() ] = pPawnType;

            archive.EndSection(); // Unite

            nPawnTypesLoaded++;
        }
        archive.EndList    (); // Pions

        archive.Close      ();
        MT_ChangeDir        ( strCurrentDir );

        MT_LOG_INFO_MSG( nPawnTypesLoaded << " Pawn types loaded" );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: PawnType::Terminate
// Created: SBO 2005-05-24
// -----------------------------------------------------------------------------
void PawnType::Terminate()
{
    for( CIT_PawnTypeMap it = pawnTypes_.begin(); it != pawnTypes_.end(); ++it )
        delete it->second;
    pawnTypes_.clear();
}
