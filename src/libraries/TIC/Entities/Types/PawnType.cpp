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

#include "TIC_pch.h"

#include "PawnType.h"
#include "PawnPlatform.h"
#include "Entities/Natures/Pawn/NatureCategory.h"
#include "Entities/Natures/Pawn/NatureLevel.h"
#include "Entities/Natures/Pawn/NatureSpecialization.h"
#include "Entities/Natures/Pawn/NatureWeapon.h"
#include "Entities/Natures/Pawn/NatureMobility.h"
#include "Entities/Natures/Pawn/NatureQualifier.h"
#include "Entities/Natures/Pawn/NatureCapacity.h"


using namespace TIC;

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

        archive.Section      ( "Nature" );

        archive.Section      ( "Niveau" );
        archive.ReadAttribute( "type", strValue );
        pLevel_              = NatureLevel::Find( strValue );
        archive.EndSection   (); // Niveau

        archive.Section      ( "Arme" );
        archive.ReadAttribute( "type", strValue );
        pWeapon_             = NatureWeapon::Find( strValue );
        archive.EndSection   (); // Arme

        archive.Section      ( "Specialisation" );
        archive.ReadAttribute( "type", strValue );
        pSpecialization_     = NatureSpecialization::Find( strValue );
        archive.EndSection   (); // Specialisation

        archive.Section      ( "Categorie" );
        archive.ReadAttribute( "type", strValue );
        pCategory_           = NatureCategory::Find( strValue );
        archive.EndSection   (); // Categorie

        archive.Section      ( "TypeMobilite" );
        archive.ReadAttribute( "type", strValue );
        pMobility_           = NatureMobility::Find( strValue );
        archive.EndSection   (); // TypeMobilite

        archive.Section      ( "Qualificatif" );
        archive.ReadAttribute( "type", strValue );
        pQualifier_          = NatureQualifier::Find( strValue );
        archive.EndSection   (); // Qualifier

        archive.Section      ( "CapaciteMission" );
        archive.ReadAttribute( "type", strValue );
        pCapacity_           = NatureCapacity::Find( strValue );
        archive.EndSection   (); // CapaciteMission

        archive.EndSection   (); // Nature

        uint                 nArity;
        std::string          strTmp;
        const PlatformType*  pPlatformType;
        archive.BeginList    ( "Equipements" );
        while( archive.NextListElement() )
        {
            archive.Section      ( "Equipement" );
            
            archive.ReadAttribute( "nom", strTmp );
            pPlatformType = PlatformType::Find( strTmp );
            if( pPlatformType == 0 )
            {
                MT_LOG_ERROR_MSG( "Unknown platform type " << strTmp );
                throw std::exception();
            }

            bool bIsEmbarquable = false;
            archive.ReadAttribute( "embarquable", bIsEmbarquable, XmlInputArchive::eNothing );

            archive.Read( nArity );
            for( uint i = 0; i < nArity; i++ )
            {
                PawnPlatform* pPawnPlatform = new PawnPlatform( *pPlatformType, bIsEmbarquable );
                platformTypes_.insert( pPawnPlatform );
            }
            archive.EndSection   (); // Equipement
        }
        archive.EndList      (); // Equipements
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
    for( CIT_PawnPlatformMultiSet it = platformTypes_.begin(); it != platformTypes_.end(); ++it )
        delete *it;
    platformTypes_.clear();
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
