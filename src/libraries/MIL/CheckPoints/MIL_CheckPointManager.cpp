// *****************************************************************************
//
// $Created: NLD 2003-08-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/CheckPoints/MIL_CheckPointManager.cpp $
// $Author: Nld $
// $Modtime: 18/06/05 0:35 $
// $Revision: 8 $
// $Workfile: MIL_CheckPointManager.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include <direct.h>

#include "MIL_CheckPointManager.h"
#include "Tools/MIL_Tools.h"
#include "Tools/MIL_IDManager.h"
#include "Network/NET_ASN_Messages.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

static const std::string strCheckPointsDirectory( "CheckPoints/" );

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager constructor
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
MIL_CheckPointManager::MIL_CheckPointManager()
    : rCheckPointsFrequency_( 0 )
    , nMaxCheckPointNbr_    ( 0 )
    , currentCheckPoints_   ()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager constructor
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
MIL_CheckPointManager::MIL_CheckPointManager( MIL_InputArchive& archive )
    : rCheckPointsFrequency_( 0 )
    , nMaxCheckPointNbr_    ( 0 )
    , currentCheckPoints_   ()
{
    archive.Section( "SauvegardesAutomatiques" );
    archive.ReadField( "TempsEntreSauvegardes", rCheckPointsFrequency_, CheckValueGreater( 0. ) );
    archive.ReadField( "NombreMax", nMaxCheckPointNbr_, CheckValueGreater( 0 ) );
    archive.EndSection(); // SauvegardesAutomatiques

    MT_LOG_INFO_MSG( MT_FormatString( "Automatic checkpoint every %f minutes", rCheckPointsFrequency_ ) );
    MT_LOG_INFO_MSG( MT_FormatString( "Automatic checkpoint max number is %d", nMaxCheckPointNbr_ ) );
    MT_Timer_ABC::Start( MT_TimeSpan( (int)( 1000 * 60 * rCheckPointsFrequency_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager destructor
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
MIL_CheckPointManager::~MIL_CheckPointManager()
{
    MT_Timer_ABC::Stop();
}

// =============================================================================
// TOOLS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::CreateCheckPointDirectory
// Created: JVT 2005-02-28
// -----------------------------------------------------------------------------
const std::string MIL_CheckPointManager::CreateCheckPointDirectory()
{
    // récupération du temps courrant
    time_t t;
    time( &t );
    struct tm * time;
    time = localtime( &t );

    // Création du nom ( IDEX__YYYY_MM_DD__HHhMMmSEC )
    std::stringstream name;
    name << strCheckPointsDirectory;
    name << std::setfill( '0' ) << std::setw( 2 ) <<  MIL_AgentServer::GetWorkspace().GetExerciceID() << "__";
    name << std::setfill( '0' ) << std::setw( 4 ) << ( 1900 + time->tm_year ) << '_';
    name << std::setfill( '0' ) << std::setw( 2 ) << ( 1 + time->tm_mon ) << '_';
    name << std::setfill( '0' ) << std::setw( 2 ) << time->tm_mday << "__";
    name << std::setfill( '0' ) << std::setw( 2 ) << time->tm_hour << 'h';
    name << std::setfill( '0' ) << std::setw( 2 ) << time->tm_min << 'm';
    name << std::setfill( '0' ) << std::setw( 2 ) << time->tm_sec;
    name << '/';

    ::_mkdir( name.str().c_str() );
    
    return name.str();
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::CreateMetaData
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::CreateMetaData( const std::string& strFileName, const std::string& strCheckPointName, const boost::crc_32_type::value_type& nDataCRC, const boost::crc_32_type::value_type& nCRCCRC )
{
    MT_XXmlOutputArchive metaDataArchive;
    metaDataArchive.Section( "CheckPoint" );
    metaDataArchive.WriteField( "Nom", strCheckPointName );
    metaDataArchive.Section( "CRC" );
    metaDataArchive.WriteField( "Configuration", nCRCCRC );
    metaDataArchive.WriteField( "Sauvegarde", nDataCRC );
    metaDataArchive.EndSection(); // CRC
    metaDataArchive.EndSection(); // CheckPoint

    if ( !metaDataArchive.WriteToFile( strFileName ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Can't create file '%s'", strFileName.c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::CreateData
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
boost::crc_32_type::value_type MIL_CheckPointManager::CreateData( const std::string& strFileName )
{
    std::ofstream file( strFileName.c_str(), std::ios::out | std::ios::binary );

    if ( !file || !file.is_open() )
        throw MT_ScipioException( __FILE__, __FUNCTION__, __LINE__, MT_FormatString( "Can't open file '%s'", strFileName.c_str() ) );

    MIL_CheckPointOutArchive archive( file );
    MIL_IDManager  ::serialize( archive );
    MIL_AgentServer::GetWorkspace().save( archive );
    file.close();
    
    return MIL_Tools::ComputeCRC( strFileName );
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::CheckFilesCRC
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::CheckFilesCRC( const std::string& strPath )
{
    MIL_InputArchive archive;
    
    archive.Open( strPath + "/CRCs.xml" );
    archive.BeginList( "Fichiers" );

    while ( archive.NextListElement() )
    {
        std::string                    strFileName;
        boost::crc_32_type::value_type nCRC;
        
        archive.Section( "Fichier" );
        archive.ReadAttribute( "nom", strFileName );
        archive.Read( nCRC );
        archive.EndSection();
        
        if ( MIL_Tools::ComputeCRC( strFileName ) != nCRC )
            throw MT_ScipioException( __FILE__, __FUNCTION__, __LINE__ , MT_FormatString( "Checkpoint '%s' can not be loaded - File '%s' has changed since the checkpoint creation", strPath.c_str(), strFileName.c_str() ) );
    }      
    archive.EndList();    
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::CheckCRCFile
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::CheckCRC( const std::string& strPath )
{
    MIL_InputArchive                metaDatas;
    boost::crc_32_type::value_type  nCRC;
    
    metaDatas.Open( strPath + "/MetaData.xml" );
    metaDatas.Section( "CheckPoint" );
    metaDatas.Section( "CRC" );
    metaDatas.ReadField( "Configuration", nCRC );
    
    if ( MIL_Tools::ComputeCRC( strPath + "/CRCs.xml" ) != nCRC )
        throw MT_ScipioException( __FILE__, __FUNCTION__, __LINE__ , MT_FormatString( "Checkpoint '%s' can not be loaded - File 'CRCs.xml' has changed since the checkpoint creation", strPath.c_str() ) );

    CheckFilesCRC( strPath );

    metaDatas.ReadField( "Sauvegarde", nCRC );
    
    if ( MIL_Tools::ComputeCRC( strPath + "/data" ) != nCRC )
        throw MT_ScipioException( __FILE__, __FUNCTION__, __LINE__ , MT_FormatString( "Checkpoint '%s' can not be loaded - File 'data' has changed since the checkpoint creation", strPath.c_str() ) );
}


// =============================================================================
// MAIN
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::LoadCheckPoint
// Created: JVT 03-07-23
//-----------------------------------------------------------------------------
void MIL_CheckPointManager::LoadCheckPoint( std::string strCheckPointPath )
{
    MT_LOG_STARTUP_MESSAGE( "------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "----  Loading Checkpoint  ----" );
    MT_LOG_STARTUP_MESSAGE( "------------------------------" );

    strCheckPointPath.insert( strCheckPointPath.begin(), strCheckPointsDirectory.begin(), strCheckPointsDirectory.end() );
    strCheckPointPath += "/";

    MT_LOG_INFO_MSG( MT_FormatString( "Loading SIM state from checkpoint '%s'", strCheckPointPath.c_str() ) );

    NET_ASN_MsgCtrlCheckPointLoadBegin asnLoadBeginMsg;
    asnLoadBeginMsg.Send();
    
    if ( MIL_AgentServer::GetWorkspace().GetConfig().UseCheckPointCRC() )
        CheckCRC( strCheckPointPath );

    std::ifstream file( ( strCheckPointPath + "data" ).c_str(), std::ios::in | std::ios::binary );

    if ( !file || !file.is_open() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Can't open file '%s'", ( strCheckPointPath + "data" ).c_str() ) );


    MIL_CheckPointInArchive archive( file );
    
    MIL_IDManager  ::serialize( archive );
    MIL_AgentServer::GetWorkspace().load( archive );
    
    file.close();
 
    NET_ASN_MsgCtrlCheckPointLoadEnd asnLoadEndMsg;
    asnLoadEndMsg.Send();
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::ManageOldCheckPoints
// Created: JVT 2005-04-13
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::ManageOldCheckPoints( const std::string& strNewName )
{
    using namespace boost::filesystem; 

    assert( currentCheckPoints_.size() <= nMaxCheckPointNbr_ );
     
    if( currentCheckPoints_.size() == nMaxCheckPointNbr_ )
    {
        const path oldPath( currentCheckPoints_.front() );
     
        try
        {
            remove_all( oldPath );
        }
        catch( std::exception& exception )
        {
            MT_LOG_ERROR_MSG( MT_FormatString( "Error while removing old checkpoing ( '%s' )", exception.what() ) );
        }

        currentCheckPoints_.pop();        
    }    
    currentCheckPoints_.push( strNewName );
}

// =============================================================================
// SERIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::SaveCheckPoint
// Created: JVT 2005-04-13
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::SaveCheckPoint( const std::string& strName, const std::string& strPath )
{
    // création du fichier
    MT_LOG_INFO_MSG( "Begin save checkpoint" );
    NET_ASN_MsgCtrlCheckPointSaveBegin asnSaveBeginMsg;
    asnSaveBeginMsg.Send();

    try
    {        
        const boost::crc_32_type::value_type nDataFileCRC = CreateData( strPath + "data" );
        const boost::crc_32_type::value_type nCRCFileCRC  = MIL_AgentServer::GetWorkspace().GetConfig().serialize( strPath + "CRCs.xml" );
        
        CreateMetaData( strPath + "MetaData.xml", strName, nDataFileCRC, nCRCFileCRC );
    }
    catch( MT_ScipioException& exception )
    {
        exception.SendToLogger();
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( MT_FormatString( "Can't save checkpoint ( '%s' )", exception.what() ) );
    }
    catch( ... )
    {
        _clearfp();
        MT_LOG_ERROR_MSG( "Can't save checkpoint ( Unknown error )" );
    }

    MT_LOG_INFO_MSG( "End save checkpoint" );
    NET_ASN_MsgCtrlCheckPointSaveEnd asnSaveEndMsg;
    asnSaveEndMsg.Send();
}

//-----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::SaveCheckPoint
// Created: JVT 03-07-23
//-----------------------------------------------------------------------------
void MIL_CheckPointManager::SaveCheckPoint( const std::string& strName )
{
    const std::string strPath = CreateCheckPointDirectory();
    
    SaveCheckPoint( strName, strPath );
}

// =============================================================================
// TIMER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::OnTimer
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::OnTimer()
{
    const std::string strPath = CreateCheckPointDirectory();

    ManageOldCheckPoints( strPath );
    SaveCheckPoint( std::string(), strPath );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::OnReceiveMsgCheckPointSaveNow
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::OnReceiveMsgCheckPointSaveNow( const ASN1T_MsgCtrlCheckPointSaveNow& asnMsg )
{
    std::string strCheckPointName;
    if( asnMsg.m.nomPresent )
        strCheckPointName = asnMsg.nom;

    SaveCheckPoint( strCheckPointName );
    
    NET_ASN_MsgCtrlCheckPointSaveNowAck asnReplyMsg;
    asnReplyMsg.Send();
}
          
// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::OnReceiveMsgCheckPointSetFrequency
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::OnReceiveMsgCheckPointSetFrequency( const ASN1T_MsgCtrlCheckPointSetFrequency& asnMsg )
{
    rCheckPointsFrequency_ = asnMsg;
    MT_Timer_ABC::Start( MT_TimeSpan( (int)( 1000 * 60 * rCheckPointsFrequency_ ) ) );

    NET_ASN_MsgCtrlCheckPointSetFrequencyAck asnReplyMsg;
    asnReplyMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::load
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> rCheckPointsFrequency_
         >> nMaxCheckPointNbr_;
   
    MT_LOG_INFO_MSG( MT_FormatString( "Automatic checkpoint every %f minutes", rCheckPointsFrequency_ ) );
    MT_LOG_INFO_MSG( MT_FormatString( "Automatic checkpoint max number is %d", nMaxCheckPointNbr_ ) );
    MT_Timer_ABC::Start( MT_TimeSpan( (int)( 1000 * 60 * rCheckPointsFrequency_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::save
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << rCheckPointsFrequency_
         << nMaxCheckPointNbr_;
}
