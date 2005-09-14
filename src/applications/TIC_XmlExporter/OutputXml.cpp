// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-19 $
// $Archive: /MVW_v10/Build/SDK/TIC_XmlExporter/src/OutputXml.cpp $
// $Author: Sbo $
// $Modtime: 22/07/05 10:18 $
// $Revision: 27 $
// $Workfile: OutputXml.cpp $
//
// *****************************************************************************

#include "Types.h"

#include "OutputXml.h"
#include "Config.h"

#include "TIC_Export/Workspace.h"
#include "TIC_Export/Callback_ABC.h"
#include "TIC_Export/Automat.h"
#include "TIC_Export/Pawn.h"
#include "TIC_Export/Platform.h"
#include "TIC_Export/Communication.h"
#include "TIC_Export/CommunicationLink.h"

// MT
#include "MT/MT_Tools/MT_Tools_lib.h"
#include "MT/MT_IO/MT_IO_lib.h"
#include "MT/MT_Archive/MT_Archive_lib.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "MT/MT_XmlTools/MT_XXmlOutputArchive.h"

using namespace TICExport;

//-----------------------------------------------------------------------------
// Name: OutputXml::OutputXml
// Created: SBO 2005-05-20
//-----------------------------------------------------------------------------
OutputXml::OutputXml( const Config& config )
    : Callback_ABC          ( )
    , archive_              ( *new MT_XXmlOutputArchive() )
    , bIsIncremental_       ( config.IsIncrementalExport     () )
    , bExportCommunications_( config.MustExportCommunications() )
    , pWorkspace_           ( 0 )
{
    MT_ExtractFilePath  ( config.GetExportFilename(), strDirectory_  );
    MT_ExtractFileName  ( config.GetExportFilename(), strFilename_   );

    const std::string   strCurrentDir = MT_GetCurrentDir();

    if( bIsIncremental_ )
    {
        if( strFilename_ == "" )
        {
            MT_LOG_INFO_MSG( "No output file specified, using default: 'tic_%iso-time%.xml'" );
            strFilename_ = "tic.xml";
        }
        if( strDirectory_ == "" )
        {
            MT_LOG_INFO_MSG( "No output directory specified, using default: 'tic'" );
            strDirectory_ = "tic";
        }
        if( MT_ChangeDir( strDirectory_ ) != eErrNo_OK )
        {
            MT_LOG_INFO_MSG( "Directory: " << strDirectory_ << " does not exists, creating..." );
            T_MT_Dir_ErrNo eOut = MT_MakeDir( strDirectory_ );
            if( eOut != eErrNo_OK && eOut != eErrNo_EEXIST )
            {
                MT_LOG_ERROR_MSG( "Creating output directory for incremental export: " << MT_Dir_GetErrorMsg( eOut ) );
                MT_LOG_WARNING_MSG( "Going to non incremental mode " );
                bIsIncremental_ = false;
            }
        }
        MT_ChangeDir( strCurrentDir );
    }
    else
    {
        if( MT_ChangeDir( strDirectory_ ) != eErrNo_OK )
        {
            MT_LOG_INFO_MSG( "Directory: " << strDirectory_ << " does not exists, creating..." );
            T_MT_Dir_ErrNo eOut = MT_MakeDir( strDirectory_ );
            if( eOut != eErrNo_OK && eOut != eErrNo_EEXIST )
                MT_LOG_ERROR_MSG( "Creating output directory for incremental export: " << MT_Dir_GetErrorMsg( eOut ) );
        }
        MT_ChangeDir( strCurrentDir );
    }
}

//-----------------------------------------------------------------------------
// Name: OutputXml::~OutputXml
// Created: SBO 2005-05-20
//-----------------------------------------------------------------------------
OutputXml::~OutputXml()
{
    delete &archive_;
}

// -----------------------------------------------------------------------------
// Name: OutputXml::SetWorkspace
// Created: NLD 2005-06-08
// -----------------------------------------------------------------------------
void OutputXml::SetWorkspace( const Workspace& workspace )
{
    pWorkspace_ = &workspace;
}

// -----------------------------------------------------------------------------
// Name: OutputXml::OnReceiveEndTick
// Created: NLD 2005-06-08
// -----------------------------------------------------------------------------
void OutputXml::OnReceiveEndTick()
{
    Save();
}

//-----------------------------------------------------------------------------
// Name: OutputXml::GetNextIncrementalFilename
// Created: SBO 2005-05-20
//-----------------------------------------------------------------------------
const std::string OutputXml::GetNextIncrementalFilename( std::string& strCurrentTime )
{
    std::stringstream   ss;

    std::string         strBase;
    MT_ExtractBaseName  ( strFilename_, strBase );

    for( uint i = 0; i < strCurrentTime.size(); i++ )
        if( strCurrentTime[ i ] == ':' )
            strCurrentTime[ i ] = '.';
    ss << strDirectory_ << "/" << strBase << "_" << strCurrentTime << ".xml";
    return ss.str();
}

//-----------------------------------------------------------------------------
// Name: OutputXml::Save
// Created: SBO 2005-05-20
//-----------------------------------------------------------------------------
void OutputXml::Save()
{
    assert( pWorkspace_ );
    archive_.Start();
    if( !archive_.Section( "Tic" ) )
    {
        MT_LOG_ERROR_MSG( "Error creating archive: " << archive_.RetrieveLastError()->GetInfo().c_str() );
        throw std::exception();
    }
    archive_.WriteAttribute( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" );
    archive_.WriteAttribute( "xsi:noNamespaceSchemaLocation", "tic.xsd" );

    std::string strCurrentTime = pWorkspace_->GetSimIsoTime();
    archive_.WriteField( "HeureSIM", strCurrentTime );

    uint nElementCount = pWorkspace_->GetAutomatCount();

    archive_.BeginList( "Automates", nElementCount );
    for( uint i = 0; i < nElementCount; i++ )
        OutputAutomat( pWorkspace_->GetAutomatAt( i ) );
    archive_.EndList(); // Automates

    if( bExportCommunications_ )
    {
        archive_.BeginList( "Communications", nElementCount );

        nElementCount = pWorkspace_->GetCommunicationLinkCount();
        for( uint i = 0; i < nElementCount; i++ )
            OutputCommunicationLink( pWorkspace_->GetCommunicationLinkAt( i ) );
        archive_.EndList(); // Automates
    }

    archive_.EndSection(); // Tic

    bool bOut;
    if( bIsIncremental_ )
        bOut = archive_.WriteToFile( GetNextIncrementalFilename( strCurrentTime ) );
    else
        bOut = archive_.WriteToFile( strDirectory_ + strFilename_ );
    if( !bOut )
        MT_LOG_ERROR_MSG( "Cannot write file: " << archive_.RetrieveLastError()->GetInfo().c_str() );
    MT_LOG_INFO_MSG( "Export done: " << strCurrentTime );
}

//-----------------------------------------------------------------------------
// Name: OutputXml::OutputAutomat
// Created: SBO 2005-05-20
//-----------------------------------------------------------------------------
void OutputXml::OutputAutomat( const Automat& automat )
{
    archive_.Section( "Automate" );
    archive_.WriteAttribute( "id", automat.GetId() );
    archive_.WriteField( "Camp", automat.GetTeam() );
    archive_.WriteField( "GroupeConnaissance", automat.GetKnowledgeGroup() );
    
    OutputPawn( automat.GetPc() );

    uint nNbrPawns = automat.GetPawnCount();
    if( nNbrPawns > 0 )
    {
        archive_.BeginList( "Pions", nNbrPawns );
        for( uint i = 0; i < nNbrPawns; i++ )
        {
            const Pawn& pawn = automat.GetPawnAt( i );
            if( pawn.IsPc() )
                continue;
            OutputPawn( pawn );
        }
        archive_.EndList(); // Pions
    }
    archive_.EndSection(); // Automate
}

//-----------------------------------------------------------------------------
// Name: OutputXml::OutputPawn
// Created: SBO 2005-05-20
//-----------------------------------------------------------------------------
void OutputXml::OutputPawn( const Pawn& pawn )
{
    archive_.Section( pawn.IsPc() ? "PC" : "Pion" );

    archive_.WriteAttribute( "id", pawn.GetId() );
    archive_.WriteAttribute( "type", ( pawn.IsAggregated() ? "agrege" : "desagrege" ) );
    //archive_.WriteAttribute( "type", "agrege" );

    archive_.Section( "Nature" );
    archive_.WriteField( "Nom", pawn.GetNatureName() );
    archive_.WriteField( "Niveau", pawn.GetNatureLevel() );
    archive_.WriteField( "Arme", pawn.GetNatureWeapon() );
    archive_.WriteField( "Specialisation", pawn.GetNatureSpecialization() );
    archive_.WriteField( "Categorie", pawn.GetNatureCategory() );
    archive_.WriteField( "Qualificatif", pawn.GetNatureQualifier() );
    archive_.WriteField( "TypeMobilite", pawn.GetNatureMobility() );
    archive_.WriteField( "CapaciteMission", pawn.GetNatureCapacity() );
    archive_.EndSection(); // Nature
    
    archive_.WriteField( "Direction", pawn.GetDirection() );
    archive_.WriteField( "Vitesse"  , pawn.GetSpeed()     );
    
    archive_.Section( "Position" );
    std::stringstream ss;
    ss.precision( 6 );
    ss.setf( std::ios::showpos );
    ss << pawn.GetLatitude();
    archive_.WriteField( "Latitude" , ss.str()  );
    ss.seekp( 0, std::ios::beg );
    ss << pawn.GetLongitude();
    archive_.WriteField( "Longitude", ss.str() );
    archive_.WriteField( "Hauteur" , pawn.GetHeight()  );
    archive_.EndSection(); // Position

    std::string strState;
    switch( pawn.GetState() )
    {
    case Pawn::eStateOperational:
        strState = "Operationnel";
        break;
    case Pawn::eStateFixable:
        strState = "Reparable";
        break;
    case Pawn::eStateDestroyed:
        strState = "Detruit";
        break;
    default:
        break;
    }
    archive_.WriteField( "Etat", strState );

    uint nNbrPlatforms = pawn.GetPlatformCount();
    if( nNbrPlatforms > 0 )
    {
        archive_.BeginList( "Plateformes", nNbrPlatforms );
        for( uint i = 0; i < nNbrPlatforms; i++ )
            OutputPlatform( pawn.GetPlatformAt( i ) );
        archive_.EndList(); // Plateforms
    }

    archive_.EndSection(); // Pion/PC
}

//-----------------------------------------------------------------------------
// Name: OutputXml::OutputPlatform
// Created: SBO 2005-05-20
//-----------------------------------------------------------------------------
void OutputXml::OutputPlatform( const Platform& platform )
{
    archive_.Section( "Plateforme" );
    
    archive_.WriteAttribute( "id", platform.GetId() );

    archive_.WriteField( "Type"      , platform.GetTypeName()       );
    archive_.WriteField( "CodeEMAT6" , platform.GetTypeEMAT6()      );
    archive_.WriteField( "CodeEMAT8" , platform.GetTypeEMAT8()      );
    archive_.WriteField( "CodeLFRIL" , platform.GetTypeLFRIL()      );
    archive_.WriteField( "CodeNNO"   , platform.GetTypeNNO()        );
    archive_.WriteField( "Protection", platform.GetTypeProtection() );
    archive_.WriteField( "Volume"    , platform.GetTypeVolume()     );
    archive_.WriteField( "Direction" , platform.GetDirection()      );
    archive_.WriteField( "Vitesse"   , platform.GetSpeed()          );

    archive_.Section( "Position" );

    std::stringstream  ss;
    ss.precision( 6 );
    ss.setf( std::ios::showpos );
    ss << platform.GetLatitude();
    archive_.WriteField( "Latitude" , ss.str()  );
    ss.seekp( 0, std::ios::beg );
    ss << platform.GetLongitude();
    archive_.WriteField( "Longitude", ss.str() );
    archive_.WriteField( "Hauteur" , platform.GetHeight()  );
    archive_.EndSection(); // Position

    std::string strState;
    switch( platform.GetState() )
    {
    case Platform::ePlatformStatePrisoner:
        strState = "Prisonnier";
        break;
    case Platform::ePlatformStateDestroyed:
        strState = "Detruit";
        break;
    case Platform::ePlatformStateFixing:
        strState = "EnReparation";
        break;
    case Platform::ePlatformStateFixable:
        strState = "Reparable";
        break;
    case Platform::ePlatformStateOk:
        strState = "Operationnel";
        break;
    default:
        break;
    }
    archive_.WriteField( "Etat", strState );

    archive_.Section   ( "NatureEquipement" );
    std::string toto = platform.GetDevice();
    archive_.WriteAttribute( "type" , platform.GetDevice() );
    archive_.WriteField( "Categorie", platform.GetDeviceCategory() );
    archive_.WriteField( "SousCategorie", platform.GetDeviceSubCategory() );
    archive_.EndSection(); // Equipement

    archive_.EndSection(); // Plateforme
}

//-----------------------------------------------------------------------------
// Name: OutputXml::OutputCommunicationLink
// Created: SBO 2005-07-12
//-----------------------------------------------------------------------------
void OutputXml::OutputCommunicationLink( const CommunicationLink& link )
{
    archive_.Section( "Liaison" );
    archive_.WriteAttribute( "automate", link.GetAutomatId() );
    archive_.WriteAttribute( "pion"    , link.GetPawnId   () );
    
    uint nComNbr = link.GetCommunicationCount( CommunicationLink::eSourcePawn );
    archive_.Section   ( "PionVersAutomate" );
    for( uint i = 0; i < nComNbr; ++i )
        OutputCommunication( link.GetCommunicationAt( CommunicationLink::eSourcePawn, i ) );
    archive_.EndSection();

    nComNbr = link.GetCommunicationCount( CommunicationLink::eSourceAutomat );
    archive_.Section   ( "AutomateVersPion" );
    for( uint i = 0; i < nComNbr; ++i )
        OutputCommunication( link.GetCommunicationAt( CommunicationLink::eSourceAutomat, i ) );
    archive_.EndSection();

    archive_.EndSection();
}

//-----------------------------------------------------------------------------
// Name: OutputXml::OutputCommunication
// Created: SBO 2005-05-25
//-----------------------------------------------------------------------------
void OutputXml::OutputCommunication( const Communication& communication )
{
    if( communication.GetArity() <= 0 )
        return;

    archive_.Section( "Communication" );
    archive_.WriteAttribute( "type"       , communication.GetType   () );
    archive_.WriteAttribute( "soustype"   , communication.GetSubType() );
    archive_.WriteAttribute( "nombre"     , communication.GetArity  () );
    if( communication.IsReport() )
        archive_.WriteAttribute( "message", communication.GetMessage() );
    archive_.EndSection();
}
