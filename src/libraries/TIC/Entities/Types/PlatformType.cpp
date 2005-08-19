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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/PlatformType.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:07 $
// $Revision: 9 $
// $Workfile: PlatformType.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"

#include "PlatformType.h"
#include "Entities/Natures/Platform/Device.h"
#include "Entities/Natures/Platform/DeviceCategory.h"
#include "Entities/Natures/Platform/DeviceSubCategory.h"

using namespace TIC;

PlatformType::T_PlatformTypeMap PlatformType::platformTypes_;

// -----------------------------------------------------------------------------
// Name: PlatformType constructor
// Created: SBO 2005-05-26
// -----------------------------------------------------------------------------
PlatformType::PlatformType( const std::string& strName, XmlInputArchive& archive )
    : strName_                ( strName )
    , pDevice_                ( &Device::notDefined )
    , pDeviceCategory_        ( &DeviceCategory::notDefined )
    , pDeviceSubCategory_     ( &DeviceSubCategory::notDefined )
    , bCanBeDisaggregated_    ( true ) // if not otherwise specified, every platform type can be disaggregated
{
    try   
    {
        archive.ReadField    ( "MosID"     , nId_           );
        archive.ReadField    ( "CodeEMAT6" , strEMAT6_      );
        archive.ReadField    ( "CodeEMAT8" , strEMAT8_      );
        archive.ReadField    ( "CodeLFRIL" , strLFRIL_      );
        archive.ReadField    ( "CodeNNO"   , strNNO_        );
        archive.ReadField    ( "Protection", strProtection_ );
        archive.ReadField    ( "Volume"    , strVolume_     );

        archive.Section      ( "Vitesses" );
        archive.ReadAttribute( "maximum", nMaxSpeed_ );
        archive.EndSection   (); // Vitesses
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( "Parse error " << exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: PlatformType destructor
// Created: SBO 2005-05-26
// -----------------------------------------------------------------------------
PlatformType::~PlatformType()
{
}

// -----------------------------------------------------------------------------
// Name: PlatformType::Initialize
// Created: SBO 2005-05-26
// -----------------------------------------------------------------------------
void PlatformType::Initialize( const std::string& strPlatformFile )
{
    try
    {
        LoadPlatforms( strPlatformFile  );
    }
    catch( ... )
    {
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: PlatformType::Terminate
// Created: SBO 2005-05-26
// -----------------------------------------------------------------------------
void PlatformType::Terminate()
{
    for( CIT_PlatformTypeMap it = platformTypes_.begin(); it != platformTypes_.end(); ++it )
        delete it->second;
    platformTypes_.clear();
}

// -----------------------------------------------------------------------------
// Name: PlatformType::LoadPlatforms
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void PlatformType::LoadPlatforms( const std::string& strFile )
{
    try
    {
        std::string         strCurrentDir = MT_GetCurrentDir();
        std::string         strDirname;
        std::string         strFilename;
        MT_ExtractFilePath  ( strFile, strDirname  );
        MT_ExtractFileName  ( strFile, strFilename );
        MT_ChangeDir        ( strDirname );

        XmlInputArchive    archive;
        
        uint16             nPlatformTypesLoaded = 0;

        archive.Open       ( strFilename );

        archive.BeginList  ( "Composantes" );
        while( archive.NextListElement() )
        {
            archive.Section( "Composante" );

            std::string strName;
            archive.ReadAttribute( "nom", strName );

            PlatformType* pPlatformType = new PlatformType( strName, archive );
            platformTypes_[ pPlatformType->GetName() ] = pPlatformType;

            archive.EndSection(); // Composante

            nPlatformTypesLoaded++;
        }
        archive.EndList    (); // Composantes

        archive.Close      ();
        MT_ChangeDir       ( strCurrentDir );

        MT_LOG_INFO_MSG( nPlatformTypesLoaded << " Platform types loaded" );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: PlatformType::LoadNatures
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void PlatformType::LoadNatures( const std::string& strFile )
{
    try
    {
        std::string         strCurrentDir = MT_GetCurrentDir();
        std::string         strDirname;
        std::string         strFilename;
        MT_ExtractFilePath  ( strFile, strDirname  );
        MT_ExtractFileName  ( strFile, strFilename );
        MT_ChangeDir        ( strDirname );

        XmlInputArchive    archive;
        
        archive.Open       ( strFilename );

        archive.BeginList  ( "Plateformes" );
        while( archive.NextListElement() )
        {
            archive.Section( "Plateforme" );

            std::string strTmp;
            archive.ReadAttribute( "nom", strTmp );
            
            IT_PlatformTypeMap it = platformTypes_.find( strTmp );
            if( it != platformTypes_.end() )
            {
                PlatformType* pPlatformType = const_cast< PlatformType * >( it->second );

                archive.Section( "NatureEquipement" );
                archive.ReadAttribute( "type", strTmp );
                pPlatformType->pDevice_ = Device::Find( strTmp );
                if( pPlatformType->pDevice_ == 0 )
                    pPlatformType->pDevice_ = &Device::notDefined;

                archive.ReadField( "Categorie", strTmp );
                pPlatformType->pDeviceCategory_ = DeviceCategory::Find( strTmp );
                if( pPlatformType->pDeviceCategory_ == 0 )
                    pPlatformType->pDeviceCategory_ = &DeviceCategory::notDefined;

                if( archive.ReadField( "SousCategorie", strTmp, XmlInputArchive::eNothing ) )
                {
                    pPlatformType->pDeviceSubCategory_ = DeviceSubCategory::Find( strTmp );
                    if( pPlatformType->pDeviceSubCategory_ == 0 )
                        pPlatformType->pDeviceSubCategory_ = &DeviceSubCategory::notDefined;
                }
                else
                    pPlatformType->pDeviceSubCategory_ = &DeviceSubCategory::notDefined;

                archive.EndSection(); // Equipement
            }
            else
            {
                MT_LOG_WARNING_MSG( "Nature specified for '" << strTmp << "' which is not an existing platform type" );
            }
            archive.EndSection(); // Plateforme
        }
        archive.EndList    (); // Plateformes

        archive.Close      ();
        MT_ChangeDir       ( strCurrentDir );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: PlatformType::LoadBlackList
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void PlatformType::SetBlackList( const std::string& strFile )
{
    try
    {
        std::string         strCurrentDir = MT_GetCurrentDir();
        std::string         strDirname;
        std::string         strFilename;
        MT_ExtractFilePath  ( strFile, strDirname  );
        MT_ExtractFileName  ( strFile, strFilename );
        MT_ChangeDir        ( strDirname );

        XmlInputArchive    archive;
        
        archive.Open       ( strFilename );

        archive.BeginList  ( "NatureEquipements" );
        while( archive.NextListElement() )
            FilterDevices  ( archive );
        archive.EndList    (); // NatureEquipements

        archive.BeginList  ( "TypePlateformes" );
        while( archive.NextListElement() )
            FilterPlatform ( archive );
        archive.EndList    (); // TypePlateformes

        archive.Close      ();
        MT_ChangeDir       ( strCurrentDir );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() );
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: PlatformType::FilterDevices
// Created: SBO 2005-07-01
// -----------------------------------------------------------------------------
void PlatformType::FilterDevices( XmlInputArchive& archive )
{
    std::string strTmp;
    archive.Section      ( "NatureEquipement" );
    archive.ReadAttribute( "type", strTmp );
    const Device* pDevice = Device::Find( strTmp );
    if( pDevice != 0 )
    {
        archive.BeginList        ( "Categories" );
        for( uint nCount = 0; archive.NextListElement(); nCount++ )
        {
            std::string strCat;
            archive.Section      ( "Categorie" );
            archive.ReadAttribute( "type", strCat );

            uint nSubCount = 0;
            if( archive.BeginList( "SousCategories", XmlInputArchive::eNothing ) )
            {
                for( ; archive.NextListElement(); nSubCount++ )
                {
                    std::string strSubCat;
                    archive.Section   ( "SousCategorie" );
                    archive.Read      ( strSubCat );
                    archive.EndSection(); // SousCategorie

                    Filter( *pDevice, strCat, strSubCat );
                }
                archive.EndList      (); // SousCategories
            }

            archive.EndSection   (); // Categorie
            
            // if no subcategory was specified, filter the whole category
            if( nSubCount == 0 )
                Filter( *pDevice, strCat );
        }
        archive.EndList(); // Categories

        // if no category was specified, filter the whole type
        if( nCount == 0 )
            Filter( *pDevice );
    }
    else
    {
        MT_LOG_WARNING_MSG( "Reading blacklist file: device type '" << strTmp << "' does not exists" );
    }
    archive.EndSection   (); // NatureEquipement
}

// -----------------------------------------------------------------------------
// Name: PlatformType::Filter
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void PlatformType::Filter( const Device& device )
{
    for( CIT_PlatformTypeMap it = platformTypes_.begin(); it != platformTypes_.end(); ++it )
        if( it->second->pDevice_ == &device )
            it->second->bCanBeDisaggregated_ = false;
}

// -----------------------------------------------------------------------------
// Name: PlatformType::Filter
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void PlatformType::Filter( const Device& device, const std::string& strCat )
{
    const DeviceCategory* pDeviceCat = DeviceCategory::Find( strCat );
    if( pDeviceCat != 0 )
    {
        for( CIT_PlatformTypeMap it = platformTypes_.begin(); it != platformTypes_.end(); ++it )
            if( it->second->pDevice_ == &device && it->second->pDeviceCategory_ == pDeviceCat )
                it->second->bCanBeDisaggregated_ = false;
    }
    else
        MT_LOG_WARNING_MSG( "Reading blacklist file: device category '" << strCat << "' does not exists" );
}

// -----------------------------------------------------------------------------
// Name: PlatformType::Filter
// Created: SBO 2005-07-04
// -----------------------------------------------------------------------------
void PlatformType::Filter( const Device& device, const std::string& strCat, const std::string& strSubCat )
{
    const DeviceCategory* pDeviceCat = DeviceCategory::Find( strCat );
    if( pDeviceCat != 0 )
    {
        const DeviceSubCategory* pDeviceSubCat = DeviceSubCategory::Find( strSubCat );
        if( pDeviceSubCat != 0 )
        {
            for( CIT_PlatformTypeMap it = platformTypes_.begin(); it != platformTypes_.end(); ++it )
                if( it->second->pDevice_ == &device && it->second->pDeviceCategory_ == pDeviceCat )
                    it->second->bCanBeDisaggregated_ = false;
        }
        else
            MT_LOG_WARNING_MSG( "Reading blacklist file: device subcategory '" << strSubCat << "' does not exists" );
    }
    else
        MT_LOG_WARNING_MSG( "Reading blacklist file: device category '" << strCat << "' does not exists" );
}

// -----------------------------------------------------------------------------
// Name: PlatformType::FilterPlatform
// Created: SBO 2005-07-07
// -----------------------------------------------------------------------------
void PlatformType::FilterPlatform( XmlInputArchive& archive )
{
    std::string       strTmp;

    archive.Section   ( "TypePlateforme" );
    archive.Read      ( strTmp );
    const PlatformType* pPlatformType = PlatformType::Find( strTmp );
    if( pPlatformType != 0 )
        pPlatformType->bCanBeDisaggregated_ = false;
    else
        MT_LOG_WARNING_MSG( "Reading blacklist file: platform type '" << strTmp << "' does not exists" );
    archive.EndSection(); // TypePlateforme
}
