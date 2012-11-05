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
#include <tools/XmlCrc32Signature.h>

namespace bpt = boost::posix_time;
namespace bfs = boost::filesystem;

MIL_CheckPointManager::T_CheckPointsQueue MIL_CheckPointManager::currentCheckPoints_;

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
    bfs::create_directories( config.BuildSessionChildFile( "checkpoints" ) );
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
    MT_LOG_INFO_MSG( MT_FormatString( "Loading SIM state from checkpoint '%s'", config.BuildCheckpointChildFile( "" ).c_str() ) );

    try
    {
        const bfs::path checkpointsRoot = bfs::path( config.GetCheckpointsDirectory(), bfs::native );
        if( bfs::exists( checkpointsRoot ) )
        {
            bfs::recursive_directory_iterator end;
            for( bfs::recursive_directory_iterator it( checkpointsRoot ); it != end; ++it )
            {
                const bfs::path child = *it;
                if( bfs::is_directory( child ) )
                {
                    bfs::path autoPath( child / "auto" );
                    if( bfs::exists( autoPath ) )
                        currentCheckPoints_.push( child.leaf().c_str() );
                }
            }
        }
    }
    catch( ... )
    {
        // NOTHING
    }

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
void MIL_CheckPointManager::SaveCheckPointDirectory( const std::string& name, const std::string& userName )
{
    client::ControlCheckPointSaveNowAck asnReplyMsg;
    asnReplyMsg.Send( NET_Publisher_ABC::Publisher() );
    SaveCheckPoint( false, name, userName );
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
    if( SaveCheckPoint( true, name ) )
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
//#ifndef _DEBUG //$$$$ boost + nedmalloc + binary_ioarchive + std::locale = crash // $$$$ LDC better avoided by defining BOOST_NO_STD_LOCALE
    delete pArchive;
//#endif
    return MIL_Tools::ComputeCRC( strFileName );
}

namespace
{
    struct CrcChecker
    {
        void ReadCrc( xml::xistream& xis, const MIL_Config& config )
        {
            std::string strFileName;
            boost::crc_32_type::value_type nCRC;
            xis >> xml::attribute( "name", strFileName )
                >> xml::attribute( "crc", nCRC );
            if( !bfs::exists( strFileName ) && strFileName.find( config.GetExercisesDir() ) != std::string::npos )
            {
                bfs::path fnPath( strFileName );
                strFileName = config.BuildExerciseChildFile( fnPath.filename() );
            }
            if( MIL_Tools::ComputeCRC( strFileName ) != nCRC )
                MT_LOG_ERROR_MSG( "Problem loading checkpoint - File '" + strFileName + "' has changed since the checkpoint creation" );
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
        >> xml::list( "file", checker, &CrcChecker::ReadCrc, config );
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
        MT_LOG_ERROR_MSG( "Problem loading checkpoint - File 'CRCs.xml' has changed since the checkpoint creation" );
    CheckFilesCRC( config );
    xis >> xml::start( "save" )
            >> xml::attribute( "crc", nCRC );
    if( MIL_Tools::ComputeCRC( config.BuildCheckpointChildFile( "data" ) ) != nCRC )
        MT_LOG_ERROR_MSG( "Problem loading checkpoint - File 'data' has changed since the checkpoint creation" );
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
            std::string oldName = currentCheckPoints_.front();
            std::string oldFile = MIL_AgentServer::GetWorkspace().GetConfig().BuildCheckpointChildFile( "", oldName );
            const bfs::path oldPath( oldFile, bfs::native );
            if( bfs::exists( oldPath ) )
            {
                bfs::remove_all( oldPath );
                client::ControlCheckPointSaveDelete message;
                message().set_name( oldName );
                message.Send( NET_Publisher_ABC::Publisher() );
            }
        }
        catch( std::exception& exception )
        {
            MT_LOG_ERROR_MSG( MT_FormatString( "Error while removing old checkpoint ( '%s' )", exception.what() ) );
        }
        currentCheckPoints_.pop();
    }
    currentCheckPoints_.push( newName );
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::SaveOrbatCheckPoint
// Created: NLD 2007-01-11
// -----------------------------------------------------------------------------
bool MIL_CheckPointManager::SaveOrbatCheckPoint( const std::string& name )
{
    try
    {
        std::string filename = MIL_AgentServer::GetWorkspace().GetConfig().BuildCheckpointChildFile( "orbat.xml", name );
        {
            xml::xofstream xos( filename );
            MIL_AgentServer::GetWorkspace().WriteODB( xos );
        }
        tools::WriteXmlCrc32Signature( filename );

        filename = MIL_AgentServer::GetWorkspace().GetConfig().BuildCheckpointChildFile( "weather.xml", name );
        {
            xml::xofstream xosWeather( filename );
            MIL_AgentServer::GetWorkspace().WriteWeather( xosWeather );
        }
        tools::WriteXmlCrc32Signature( filename );

        filename = MIL_AgentServer::GetWorkspace().GetConfig().BuildCheckpointChildFile( "urbanstate.xml", name );
        {
            xml::xofstream xosUrban( filename );
            MIL_AgentServer::GetWorkspace().WriteUrban( xosUrban );
        }
        tools::WriteXmlCrc32Signature( filename );
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
bool MIL_CheckPointManager::SaveFullCheckPoint( const std::string& name, const std::string& userName /*= ""*/ )
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
bool MIL_CheckPointManager::SaveCheckPoint( bool automatic, const std::string& name, const std::string& userName /*= ""*/ )
{
    std::string checkpointName = userName.empty()? name : userName;
    MT_LOG_INFO_MSG( "Begin save checkpoint " << checkpointName );
    client::ControlCheckPointSaveBegin().Send( NET_Publisher_ABC::Publisher() );
    ::_mkdir( MIL_AgentServer::GetWorkspace().GetConfig().BuildCheckpointChildFile( "", checkpointName ).c_str() );

    if( automatic )
    {
        const MIL_Config& config = MIL_AgentServer::GetWorkspace().GetConfig();
        const std::string autoFileName = config.BuildCheckpointChildFile( "auto", checkpointName );
        std::ofstream file( autoFileName.c_str(), std::ios::out | std::ios::binary );
        file.close();
    }

    bool bNotOk = !SaveOrbatCheckPoint( checkpointName );
    MT_LOG_INFO_MSG( "End save orbat" );
    bNotOk |= !SaveFullCheckPoint( checkpointName, userName );
    MT_LOG_INFO_MSG( "End save checkpoint" );
    client::ControlCheckPointSaveEnd msg;
    msg().set_name( checkpointName );
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
// Name: MIL_CheckPointManager::OnReceiveMsgControlExportRequest
// Created: LDC 2012-02-15
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::OnReceiveMsgControlExportRequest( const sword::ControlExportRequest msg )
{
    const std::string& name = msg.directory_name();
    client::ControlExportRequestAck asnReplyMsg;
    bool success = SaveCheckPoint( false, name, name );
    asnReplyMsg().set_error_code( success ? sword::ControlExportRequestAck::success : sword::ControlExportRequestAck::failure );
    asnReplyMsg().set_directory_name( name );
    asnReplyMsg.Send( NET_Publisher_ABC::Publisher() );
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
// Name: MIL_CheckPointManager::OnReceiveMsgCheckPointDeleteRequest
// Created: JSR 2012-01-02
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::OnReceiveMsgCheckPointDeleteRequest( const sword::ControlCheckPointDeleteRequest& msg )
{
    try
    {
        std::string path = MIL_AgentServer::GetWorkspace().GetConfig().BuildCheckpointChildFile( "", msg.checkpoint() );
        const bfs::path bfsPath( path, bfs::native );
        bfs::remove_all( bfsPath );
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( MT_FormatString( "Error while removing checkpoint ( '%s' )", exception.what() ) );
    }
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
