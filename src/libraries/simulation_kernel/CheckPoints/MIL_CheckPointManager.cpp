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

#include "simulation_kernel_pch.h"
#include "MIL_CheckPointManager.h"
#include "MIL_AgentServer.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_Config.h"
#include "Tools/MIL_Tools.h"
#include "Tools/MIL_IDManager.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <direct.h>

namespace bpt = boost::posix_time;

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager constructor
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
MIL_CheckPointManager::MIL_CheckPointManager()
    : nMaxCheckPointNbr_    ( 0 )
    , nCheckPointsFrequency_( 0 )
    , nLastCheckPointTick_  ( 0 )
    , nNextCheckPointTick_  ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager constructor
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
MIL_CheckPointManager::MIL_CheckPointManager( const MIL_Config& config )
    : nMaxCheckPointNbr_    ( config.GetCheckPointsKept() )
    , nCheckPointsFrequency_( config.GetCheckPointsFrequency() )
    , nLastCheckPointTick_  ( 0 )
    , nNextCheckPointTick_  ( 0 )
{
    boost::filesystem::create_directories( config.BuildSessionChildFile( "checkpoints" ) );
    MT_LOG_INFO_MSG( MT_FormatString( "Automatic checkpoint every %d seconds", nCheckPointsFrequency_ ) );
    MT_LOG_INFO_MSG( MT_FormatString( "Automatic checkpoint max number is %d", nMaxCheckPointNbr_ ) );
    UpdateNextCheckPointTick();
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager destructor
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
MIL_CheckPointManager::~MIL_CheckPointManager()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::LoadCheckPoint
// Created: JVT 03-07-23
//-----------------------------------------------------------------------------
void MIL_CheckPointManager::LoadCheckPoint( const MIL_Config& config )
{
    MT_LOG_STARTUP_MESSAGE( "------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "----  Loading Checkpoint  ----" );
    MT_LOG_STARTUP_MESSAGE( "------------------------------" );
    MT_LOG_INFO_MSG( MT_FormatString( "Loading SIM state from checkpoint '%s'", config.BuildCheckpointChildFile( "" ).c_str() ) )
    if( config.UseCheckPointCRC() )
        CheckCRC( config );
    std::ifstream file( config.BuildCheckpointChildFile( "data" ).c_str(), std::ios::in | std::ios::binary );
    if( !file || !file.is_open() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Cannot open file '%s'", config.BuildCheckpointChildFile( "data" ).c_str() ) );
    MIL_CheckPointInArchive* pArchive = new MIL_CheckPointInArchive( file );
    MIL_AgentServer::GetWorkspace().load( *pArchive );
    file.close();
#ifndef _DEBUG //$$$$ boost + nedmalloc + binary_ioarchive + std::locale = crash
    delete pArchive;
#endif

}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::SaveCheckPointDirectory
// Created: JSR 2010-03-10
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::SaveCheckPointDirectory( const std::string& name, const std::string userName )
{
    client::ControlCheckPointSaveNowAck asnReplyMsg;
    asnReplyMsg.Send( NET_Publisher_ABC::Publisher() );
    SaveCheckPoint( name, userName );
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::Update
// Created: NLD 2005-12-27
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::Update()
{
    if( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() < nNextCheckPointTick_ )
        return;
    const std::string name = BuildCheckPointName();
    if( SaveCheckPoint( name ) )
        RotateCheckPoints( name );
    nLastCheckPointTick_ = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    UpdateNextCheckPointTick();
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::UpdateNextCheckPointTick
// Created: NLD 2005-12-27
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::UpdateNextCheckPointTick()
{
    unsigned int nTick = nLastCheckPointTick_ + (unsigned int)MIL_Tools::ConvertSecondsToSim( nCheckPointsFrequency_ );
    if( nTick < MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
        nNextCheckPointTick_ = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    else
        nNextCheckPointTick_ = nTick;
    MT_LOG_INFO_MSG( MT_FormatString( "Next automatic checkpoint at tick #%d", nNextCheckPointTick_ ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::BuildCheckPointName
// Created: JVT 2005-02-28
// -----------------------------------------------------------------------------
const std::string MIL_CheckPointManager::BuildCheckPointName()
{
    // récupération du temps courrant
    time_t t;
    time( &t );
    struct tm * time;
    time = localtime( &t );
    // Création du nom ( IDEX__YYYY_MM_DD__HHhMMmSEC )
    std::stringstream name;
    name << std::setfill( '0' ) << std::setw( 4 ) << ( 1900 + time->tm_year ) << '_';
    name << std::setfill( '0' ) << std::setw( 2 ) << ( 1 + time->tm_mon ) << '_';
    name << std::setfill( '0' ) << std::setw( 2 ) << time->tm_mday << "__";
    name << std::setfill( '0' ) << std::setw( 2 ) << time->tm_hour << 'h';
    name << std::setfill( '0' ) << std::setw( 2 ) << time->tm_min << 'm';
    name << std::setfill( '0' ) << std::setw( 2 ) << time->tm_sec;
    return name.str();
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::CreateMetaData
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::CreateMetaData( const std::string& strFileName, const std::string& name, const boost::crc_32_type::value_type& nDataCRC, const boost::crc_32_type::value_type& nCRCCRC )
{
    try
    {
        const bpt::ptime realTime( bpt::from_time_t( MIL_AgentServer::GetWorkspace().GetRealTime() ) );
        xml::xofstream xos( strFileName );
        xos << xml::start( "checkpoint" )
                << xml::content( "name", name )
                << xml::content( "date", bpt::to_iso_string( realTime ) )
                << xml::start( "crc" )
                    << xml::start( "configuration" )
                        << xml::attribute( "crc", nCRCCRC )
                    << xml::end
                    << xml::start( "save" )
                        << xml::attribute( "crc", nDataCRC );
    }
    catch( ... )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Cannot create file '%s'", strFileName.c_str() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::CreateData
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
boost::crc_32_type::value_type MIL_CheckPointManager::CreateData( const std::string& strFileName )
{
    std::ofstream file( strFileName.c_str(), std::ios::out | std::ios::binary );
    if( !file || !file.is_open() )
        throw MT_ScipioException( __FILE__, __FUNCTION__, __LINE__, MT_FormatString( "Cannot open file '%s'", strFileName.c_str() ) );
    MIL_CheckPointOutArchive* pArchive = new MIL_CheckPointOutArchive( file );
    MIL_AgentServer::GetWorkspace().save( *pArchive );
    file.close();
#ifndef _DEBUG //$$$$ boost + nedmalloc + binary_ioarchive + std::locale = crash
    delete pArchive;
#endif
    return MIL_Tools::ComputeCRC( strFileName );
}

namespace
{
    struct CrcChecker
    {
        void ReadCrc( xml::xistream& xis )
        {
            std::string strFileName;
            boost::crc_32_type::value_type nCRC;
            xis >> xml::attribute( "name", strFileName )
                >> xml::attribute( "crc", nCRC );
            if( MIL_Tools::ComputeCRC( strFileName ) != nCRC )
                throw MT_ScipioException( __FILE__, __FUNCTION__, __LINE__ ,
                    "Cannot load checkpoint - File '" + strFileName + "' has changed since the checkpoint creation" );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::CheckFilesCRC
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::CheckFilesCRC( const MIL_Config& config )
{
    CrcChecker checker;
    xml::xifstream xis( config.BuildCheckpointChildFile( "CRCs.xml" ) );
    xis >> xml::start( "files" )
        >> xml::list( "file", checker, &CrcChecker::ReadCrc );
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::CheckCRC
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::CheckCRC( const MIL_Config& config )
{
    xml::xifstream xis( config.BuildCheckpointChildFile( "MetaData.xml" ) );

    boost::crc_32_type::value_type  nCRC;
    xis >> xml::start( "checkpoint" )
        >> xml::start( "crc" )
        >> xml::start( "configuration" )
            >> xml::attribute( "crc", nCRC )
        >> xml::end;
    if( MIL_Tools::ComputeCRC( config.BuildCheckpointChildFile( "CRCs.xml" ) ) != nCRC )
        throw MT_ScipioException( __FILE__, __FUNCTION__, __LINE__ , "Cannot load checkpoint - File 'CRCs.xml' has changed since the checkpoint creation" );
    CheckFilesCRC( config );
    xis >> xml::start( "save" )
            >> xml::attribute( "crc", nCRC );
    if( MIL_Tools::ComputeCRC( config.BuildCheckpointChildFile( "data" ) ) != nCRC )
        throw MT_ScipioException( __FILE__, __FUNCTION__, __LINE__ , "Cannot load checkpoint - File 'data' has changed since the checkpoint creation" );
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::RotateCheckPoints
// Created: JVT 2005-04-13
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::RotateCheckPoints( const std::string& newName )
{
    if( currentCheckPoints_.size() == nMaxCheckPointNbr_ )
    {
        try
        {
            const boost::filesystem::path oldPath( currentCheckPoints_.front(), boost::filesystem::native );
            boost::filesystem::remove_all( oldPath );
        }
        catch( std::exception& exception )
        {
            MT_LOG_ERROR_MSG( MT_FormatString( "Error while removing old checkpoint ( '%s' )", exception.what() ) );
        }
        currentCheckPoints_.pop();
    }
    currentCheckPoints_.push( MIL_AgentServer::GetWorkspace().GetConfig().BuildCheckpointChildFile( "", newName ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::SaveOrbatCheckPoint
// Created: NLD 2007-01-11
// -----------------------------------------------------------------------------
bool MIL_CheckPointManager::SaveOrbatCheckPoint( const std::string& name )
{
    try
    {
        xml::xofstream xos( MIL_AgentServer::GetWorkspace().GetConfig().BuildCheckpointChildFile( "orbat.xml", name ) );
        MIL_AgentServer::GetWorkspace().WriteODB( xos );
    }
    catch( xml::exception& e )
    {
        _clearfp();
        MT_LOG_ERROR_MSG( MT_FormatString( "Can't save backup checkpoint ( %s )", e.what() ) );
        return false;
    }
    catch( ... )
    {
        _clearfp();
        MT_LOG_ERROR_MSG( "Can't save backup checkpoint ( Unknown error )" );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::SaveFullCheckPoint
// Created: NLD 2007-01-11
// -----------------------------------------------------------------------------
bool MIL_CheckPointManager::SaveFullCheckPoint( const std::string& name, const std::string userName /*= ""*/ )
{
    const MIL_Config& config = MIL_AgentServer::GetWorkspace().GetConfig();
    try
    {
        const boost::crc_32_type::value_type nDataFileCRC = CreateData( config.BuildCheckpointChildFile( "data", name ) );
        const boost::crc_32_type::value_type nCRCFileCRC  = config.serialize( config.BuildCheckpointChildFile( "CRCs.xml" , name ) );
        CreateMetaData( config.BuildCheckpointChildFile( "MetaData.xml", name ), userName, nDataFileCRC, nCRCFileCRC );
    }
    catch( MT_ScipioException& exception )
    {
        exception.SendToLogger();
        return false;
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( MT_FormatString( "Can't save checkpoint ( '%s' )", exception.what() ) );
        return false;
    }
    catch( ... )
    {
        _clearfp();
        MT_LOG_ERROR_MSG( "Can't save checkpoint ( Unknown error )" );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::SaveCheckPoint
// Created: JVT 2005-04-13
// -----------------------------------------------------------------------------
bool MIL_CheckPointManager::SaveCheckPoint( const std::string& name, const std::string userName /*= ""*/ )
{
    MT_LOG_INFO_MSG( "Begin save checkpoint " << name );
    client::ControlCheckPointSaveBegin().Send( NET_Publisher_ABC::Publisher() );
    ::_mkdir( MIL_AgentServer::GetWorkspace().GetConfig().BuildCheckpointChildFile( "", name ).c_str() );
    const bool bNotOk = !SaveOrbatCheckPoint( name ) || !SaveFullCheckPoint( name, userName );
    MT_LOG_INFO_MSG( "End save checkpoint" );
    client::ControlCheckPointSaveEnd msg;
    msg().set_name( name );
    msg.Send( NET_Publisher_ABC::Publisher() );
    return ! bNotOk;
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::OnReceiveMsgCheckPointSaveNow
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::OnReceiveMsgCheckPointSaveNow( const sword::ControlCheckPointSaveNow& msg )
{
    std::string name;
    if( msg.has_name() )
        name = msg.name();
    SaveCheckPointDirectory( BuildCheckPointName(), name );
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::OnReceiveMsgCheckPointSetFrequency
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::OnReceiveMsgCheckPointSetFrequency( const sword::ControlCheckPointSetFrequency& msg )
{
    nCheckPointsFrequency_ = msg.frequency() * 60; // $$$$ NLD 2007-01-11: beeeeeeaaaaah
    client::ControlCheckPointSetFrequencyAck().Send( NET_Publisher_ABC::Publisher() );
    UpdateNextCheckPointTick();
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::load
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> nCheckPointsFrequency_
         >> nMaxCheckPointNbr_;
    MT_LOG_INFO_MSG( MT_FormatString( "Automatic checkpoint every %d seconds", nCheckPointsFrequency_ ) );
    MT_LOG_INFO_MSG( MT_FormatString( "Automatic checkpoint max number is %d", nMaxCheckPointNbr_ ) );
    nLastCheckPointTick_ = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    UpdateNextCheckPointTick();
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::save
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << nCheckPointsFrequency_
         << nMaxCheckPointNbr_;
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::GetCheckPointFrequency
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
unsigned int MIL_CheckPointManager::GetCheckPointFrequency() const
{
    return nCheckPointsFrequency_;
}
