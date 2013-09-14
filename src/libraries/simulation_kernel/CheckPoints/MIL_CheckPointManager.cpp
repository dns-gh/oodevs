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
#include "tools/FileWrapper.h"
#include "Tools/MIL_Config.h"
#include "Tools/MIL_Tools.h"
#include "Tools/MIL_IDManager.h"
#include "tools/XmlStreamOperators.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>
#pragma warning ( push )
#pragma warning ( disable : 4244 4245 )
#include <boost/CRC.hpp>
#pragma warning ( pop )
#include <boost/date_time/posix_time/posix_time.hpp>
#include <direct.h>

namespace bpt = boost::posix_time;

namespace
{

void CreateMetaData( const tools::Path& strFileName, const tools::Path& name,
        const boost::crc_32_type::value_type& nDataCRC,
        const boost::crc_32_type::value_type& nCRCCRC )
{
    try
    {
        const bpt::ptime realTime( bpt::from_time_t( MIL_Time_ABC::GetTime().GetRealTime() ) );
        tools::Xofstream xos( strFileName );
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
    catch( const std::exception& )
    {
        throw MASA_EXCEPTION( "Cannot create file '" + strFileName.ToUTF8() + "'" );
    }
}

boost::crc_32_type::value_type CreateData( const tools::Path& strFileName )
{
    tools::Ofstream file( strFileName, std::ios::out | std::ios::binary );
    if( !file || !file.is_open() )
        throw MASA_EXCEPTION( "Cannot open file '" + strFileName.ToUTF8() + "'" );
    MIL_CheckPointOutArchive archive( file );
    MIL_AgentServer::GetWorkspace().save( archive );
    file.close();
    return MIL_Tools::ComputeCRC( strFileName );
}

void ReadCrc( xml::xistream& xis )
{
    tools::Path strFileName;
    boost::crc_32_type::value_type nCRC;
    xis >> xml::attribute( "name", strFileName )
        >> xml::attribute( "crc", nCRC );
    if( MIL_Tools::ComputeCRC( strFileName ) != nCRC )
        MT_LOG_ERROR_MSG( "Problem loading checkpoint - File '" + strFileName.ToUTF8() + "' has changed since the checkpoint creation" );
}

void CheckFilesCRC( const MIL_Config& config )
{
    tools::Xifstream xis( config.BuildCheckpointChildFile( "CRCs.xml" ) );
    xis >> xml::start( "files" )
        >> xml::list( "file", &ReadCrc );
}

void CheckCRC( const MIL_Config& config )
{
    tools::Xifstream xis( config.BuildCheckpointChildFile( "MetaData.xml" ) );

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

}  // namespace

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
    , checkpointName_       ( config.GetCheckpointNameTestMode() )
{
    config.BuildSessionChildFile( "checkpoints" ).CreateDirectories();
    MT_LOG_INFO_MSG( MT_FormatString( "Automatic checkpoint every %d seconds", nCheckPointsFrequency_ ) );
    MT_LOG_INFO_MSG( MT_FormatString( "Automatic checkpoint max number is %d", nMaxCheckPointNbr_ ) );
    if( nMaxCheckPointNbr_ == 0 )
        MT_LOG_INFO_MSG( "No Checkpoints kept. No automatic checkpoint will be saved");
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
void MIL_CheckPointManager::LoadCheckPoint( const MIL_Config& config, const ObjectTypeResolver_ABC& resolver )
{
    MT_LOG_STARTUP_MESSAGE( "------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "----  Loading Checkpoint  ----" );
    MT_LOG_STARTUP_MESSAGE( "------------------------------" );
    MT_LOG_INFO_MSG( "Loading from checkpoint '" << config.BuildCheckpointChildFile( "" ) << "'" );
    if( config.UseCheckPointCRC() )
        CheckCRC( config );
    tools::Ifstream file( config.BuildCheckpointChildFile( "data" ), std::ios::in | std::ios::binary );
    if( !file || !file.is_open() )
        throw MASA_EXCEPTION( "Cannot open file '" + config.BuildCheckpointChildFile( "data" ).ToUTF8() + "'" );
    MIL_CheckPointInArchive archive( file, resolver );
    MIL_AgentServer::GetWorkspace().load( archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::Update
// Created: NLD 2005-12-27
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::Update()
{
    if( MIL_Time_ABC::GetTime().GetCurrentTimeStep() < nNextCheckPointTick_ )
        return;
    const tools::Path name = BuildCheckPointName();
    if( SaveCheckPoint( name ) )
        RotateCheckPoints( name );
    nLastCheckPointTick_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    UpdateNextCheckPointTick();
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::UpdateNextCheckPointTick
// Created: NLD 2005-12-27
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::UpdateNextCheckPointTick()
{
    unsigned int nTick = nLastCheckPointTick_ + (unsigned int)MIL_Tools::ConvertSecondsToSim( nCheckPointsFrequency_ );
    if( nTick < MIL_Time_ABC::GetTime().GetCurrentTimeStep() )
        nNextCheckPointTick_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    else
        nNextCheckPointTick_ = nTick;
    MT_LOG_INFO_MSG( MT_FormatString( "Next automatic checkpoint at tick #%d", nNextCheckPointTick_ ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::BuildCheckPointName
// Created: JVT 2005-02-28
// -----------------------------------------------------------------------------
tools::Path MIL_CheckPointManager::BuildCheckPointName() const
{
    if( !checkpointName_.IsEmpty() )
        return checkpointName_;
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
    return name.str().c_str();
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::RotateCheckPoints
// Created: JVT 2005-04-13
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::RotateCheckPoints( const tools::Path& newName )
{
    if( nMaxCheckPointNbr_ == 0 )
        return;
    if( currentCheckPoints_.size() == nMaxCheckPointNbr_ )
    {
        try
        {
            tools::Path oldName = currentCheckPoints_.front();
            tools::Path oldFile = MIL_AgentServer::GetWorkspace().GetConfig().BuildCheckpointChildFile( "", oldName );
            oldFile.RemoveAll();
            client::ControlCheckPointSaveDelete message;
            message().set_name( oldName.ToUTF8() );
            message.Send( NET_Publisher_ABC::Publisher() );
        }
        catch( const std::exception& e )
        {
            MT_LOG_ERROR_MSG( "Error while removing old checkpoint ( '" << tools::GetExceptionMsg( e ) << "' )" );
        }
        currentCheckPoints_.pop();
    }
    currentCheckPoints_.push( newName );
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::SaveOrbatCheckPoint
// Created: NLD 2007-01-11
// -----------------------------------------------------------------------------
bool MIL_CheckPointManager::SaveOrbatCheckPoint( const tools::Path& name )
{
    try
    {
        const MIL_Config& config = MIL_AgentServer::GetWorkspace().GetConfig();
        tools::Path filename = config.BuildCheckpointChildFile( config.GetOrbatFileName(), name );
        {
            tools::Xofstream xos( filename );
            MIL_AgentServer::GetWorkspace().WriteODB( xos );
        }
        filename = config.BuildCheckpointChildFile( config.GetKnowledgesFileName(), name );
        {
            tools::Xofstream xosKnowledge( filename );
            MIL_AgentServer::GetWorkspace().WriteKnowledges( xosKnowledge );
        }
        filename = config.BuildCheckpointChildFile( config.GetWeatherFileName(), name );
        {
            tools::Xofstream xosWeather( filename );
            MIL_AgentServer::GetWorkspace().WriteWeather( xosWeather );
        }
        filename = config.BuildCheckpointChildFile( config.GetUrbanFileName(), name );
        {
            tools::Xofstream xosUrban( filename );
            MIL_AgentServer::GetWorkspace().WriteUrban( xosUrban );
        }
    }
    catch( const xml::exception& e )
    {
        _clearfp();
        MT_LOG_ERROR_MSG( "Can't save backup checkpoint ( " << tools::GetExceptionMsg( e ) << " )" );
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
bool MIL_CheckPointManager::SaveFullCheckPoint( const tools::Path& name, const tools::Path& userName /*= ""*/ )
{
    const MIL_Config& config = MIL_AgentServer::GetWorkspace().GetConfig();
    try
    {
        const boost::crc_32_type::value_type nDataFileCRC = CreateData( config.BuildCheckpointChildFile( "data", name ) );
        const boost::crc_32_type::value_type nCRCFileCRC  = config.serialize( config.BuildCheckpointChildFile( "CRCs.xml" , name ) );
        CreateMetaData( config.BuildCheckpointChildFile( "MetaData.xml", name ), userName, nDataFileCRC, nCRCFileCRC );
    }
    catch( const tools::Exception& e )
    {
        MT_LOG_ERROR_MSG( e.CreateLoggerMsg() );
        return false;
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Can't save checkpoint ( '" << tools::GetExceptionMsg( e ) << "' )" );
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
bool MIL_CheckPointManager::SaveCheckPoint( const tools::Path& name, const tools::Path& userName /*= ""*/ )
{
    tools::Path checkpointName = userName.IsEmpty()? name : userName;
    MT_LOG_INFO_MSG( "Begin save checkpoint " << checkpointName );
    client::ControlCheckPointSaveBegin().Send( NET_Publisher_ABC::Publisher() );
    MIL_AgentServer::GetWorkspace().GetConfig().BuildCheckpointChildFile( "", checkpointName ).CreateDirectories();
    bool bNotOk = !SaveOrbatCheckPoint( checkpointName );
    MT_LOG_INFO_MSG( "End save orbat" );
    bNotOk |= !SaveFullCheckPoint( checkpointName, userName );
    MT_LOG_INFO_MSG( "End save checkpoint" );
    client::ControlCheckPointSaveEnd msg;
    msg().set_name( checkpointName.ToUTF8() );
    msg.Send( NET_Publisher_ABC::Publisher() );
    return ! bNotOk;
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::OnReceiveMsgCheckPointSaveNow
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::OnReceiveMsgCheckPointSaveNow( const sword::ControlCheckPointSaveNow& msg )
{
    tools::Path name;
    if( msg.has_name() )
        name = tools::Path::FromUTF8( msg.name() );
    client::ControlCheckPointSaveNowAck asnReplyMsg;
    asnReplyMsg.Send( NET_Publisher_ABC::Publisher() );
    SaveCheckPoint( BuildCheckPointName(), name );
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
    nLastCheckPointTick_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
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
