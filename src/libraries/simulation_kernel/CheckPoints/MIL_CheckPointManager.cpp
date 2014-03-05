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
#include <boost/date_time/posix_time/posix_time.hpp>
#include <direct.h>

namespace bpt = boost::posix_time;

namespace
{

bool IsValidCheckpointName( const std::wstring& name )
{
    for( auto it = name.cbegin(); it != name.cend(); ++it )
        if( !std::iswalnum( *it ) && !std::iswspace( *it ) && *it != L'_' )
            return false;
    return true;
}

void CreateMetaData( const tools::Path& filename, const tools::Path& name )
{
    try
    {
        const bpt::ptime realTime( bpt::from_time_t( MIL_Time_ABC::GetTime().GetRealTime() ) );
        tools::Xofstream xos( filename );
        xos << xml::start( "checkpoint" )
                << xml::content( "name", name )
                << xml::content( "date", bpt::to_iso_string( realTime ) );
    }
    catch( const std::exception& )
    {
        throw MASA_EXCEPTION( "Cannot create file '" + filename.ToUTF8() + "'" );
    }
}

void CreateData( const tools::Path& filename, const boost::shared_ptr< TER_World >& world )
{
    tools::Ofstream file( filename, std::ios::out | std::ios::binary );
    if( !file || !file.is_open() )
        throw MASA_EXCEPTION( "Cannot open file '" + filename.ToUTF8() + "'" );
    MIL_CheckPointOutArchive archive( file, world );
    MIL_AgentServer::GetWorkspace().save( archive );
    file.close();
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
MIL_CheckPointManager::MIL_CheckPointManager( const MIL_Config& config,
       const boost::shared_ptr< TER_World >& world )
    : nMaxCheckPointNbr_    ( config.GetCheckPointsKept() )
    , nCheckPointsFrequency_( config.GetCheckPointsFrequency() )
    , nLastCheckPointTick_  ( 0 )
    , nNextCheckPointTick_  ( 0 )
    , checkpointName_       ( config.GetCheckpointNameTestMode() )
    , world_( world )
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
void MIL_CheckPointManager::LoadCheckPoint( const MIL_Config& config,
        const ObjectTypeResolver_ABC& resolver,
        const boost::shared_ptr< TER_World >& world )
{
    MT_LOG_STARTUP_MESSAGE( "------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "----  Loading Checkpoint  ----" );
    MT_LOG_STARTUP_MESSAGE( "------------------------------" );
    MT_LOG_INFO_MSG( "Loading from checkpoint '" << config.BuildCheckpointChildFile( "" ) << "'" );
    tools::Ifstream file( config.BuildCheckpointChildFile( "data" ), std::ios::in | std::ios::binary );
    if( !file || !file.is_open() )
        throw MASA_EXCEPTION( "Cannot open file '" + config.BuildCheckpointChildFile( "data" ).ToUTF8() + "'" );
    MIL_CheckPointInArchive archive( file, resolver, world );
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
    if( SaveCheckPoint( name, "", false, 0, 0 ).empty() )
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
std::string MIL_CheckPointManager::SaveOrbatCheckPoint( const tools::Path& name )
{
    std::string err;
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
        err = tools::GetExceptionMsg( e );
        MT_LOG_ERROR_MSG( "Can't save backup checkpoint ( " << err << " )" );
    }
    return err;
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::SaveFullCheckPoint
// Created: NLD 2007-01-11
// -----------------------------------------------------------------------------
std::string MIL_CheckPointManager::SaveFullCheckPoint( const tools::Path& name,
        const tools::Path& userName )
{
    std::string err;
    const MIL_Config& config = MIL_AgentServer::GetWorkspace().GetConfig();
    try
    {
        CreateData( config.BuildCheckpointChildFile( "data", name ), world_ );
        CreateMetaData( config.BuildCheckpointChildFile( "MetaData.xml", name ), userName );
    }
    catch( const tools::Exception& e )
    {
        err = tools::GetExceptionMsg( e );
        MT_LOG_ERROR_MSG( "cannot save checkpoint: " << e.CreateLoggerMsg() );
    }
    catch( const std::exception& e )
    {
        err = tools::GetExceptionMsg( e );
        MT_LOG_ERROR_MSG( "cannot save checkpoint: " << err );
    }
    return err;
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::SaveCheckPoint
// Created: JVT 2005-04-13
// -----------------------------------------------------------------------------
namespace
{

struct CheckpointGuard: boost::noncopyable
{
    CheckpointGuard( const tools::Path& name )
        : name_( name )
    {
        MT_LOG_INFO_MSG( "Begin save checkpoint " << name_ );
        client::ControlCheckPointSaveBegin begin;
        begin().set_name( name_.ToUTF8() );
        begin.Send( NET_Publisher_ABC::Publisher() );
    }

    ~CheckpointGuard()
    {
        MT_LOG_INFO_MSG( "End save checkpoint " << name_ );
        client::ControlCheckPointSaveEnd end;
        end().set_name( name_.ToUTF8() );
        try
        {
            end.Send( NET_Publisher_ABC::Publisher() );
        }
        catch( const std::exception& )
        {
            // If the connection is lost, it does not matter we cannot send
            // the end message.
        }
    }

    const tools::Path name_;
};

} // namespace

std::string MIL_CheckPointManager::SaveCheckPoint( const tools::Path& checkpointName,
        const tools::Path& userName, bool sendState, unsigned int clientId,
        unsigned int ctx )
{
    CheckpointGuard guard( checkpointName );
    if( sendState )
    {
        MT_LOG_INFO_MSG( "Sending simulation state" );
        client::ControlSendCurrentStateBegin()
            .Send( NET_Publisher_ABC::Publisher(), ctx, clientId );
        MIL_AgentServer::GetWorkspace().SendStateToNewClient();
        client::ControlSendCurrentStateEnd()
            .Send( NET_Publisher_ABC::Publisher(), ctx, clientId );
        MT_LOG_INFO_MSG( "Simulation state sent" );
    }
    MIL_Config& cfg = MIL_AgentServer::GetWorkspace().GetConfig();
    cfg.BuildCheckpointChildFile( "", checkpointName ).CreateDirectories();
    const std::string orbatErr = SaveOrbatCheckPoint( checkpointName );
    MT_LOG_INFO_MSG( "End save orbat" );
    const std::string checkpointErr = SaveFullCheckPoint( checkpointName, userName );
    return !orbatErr.empty() ? orbatErr : checkpointErr;
}

// -----------------------------------------------------------------------------
// Name: MIL_CheckPointManager::OnReceiveMsgCheckPointSaveNow
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MIL_CheckPointManager::OnReceiveMsgCheckPointSaveNow(
        const sword::ControlCheckPointSaveNow& msg, unsigned int clientId, unsigned int ctx )
{
    client::ControlCheckPointSaveNowAck ack;
    try
    {
        tools::Path userName;
        if( msg.has_name() )
            userName = tools::Path::FromUTF8( msg.name() );
        tools::Path checkpointName = BuildCheckPointName();
        if( !userName.IsEmpty() )
        {
            if( !IsValidCheckpointName( userName.ToUnicode() ))
                throw MASA_EXCEPTION( "invalid checkpoint name, only alphanumeric"
                    ", spaces and underscores are allowed" );
            checkpointName = userName;
        }
        const std::string err = SaveCheckPoint( checkpointName, userName,
                msg.send_state(), clientId, ctx );
        if( !err.empty() )
            ack().set_error_msg( err );
        ack().set_name( checkpointName.ToUTF8() );
    }
    catch( const std::exception& e )
    {
        ack().set_error_msg( tools::GetExceptionMsg( e ) );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), ctx, clientId );
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
    const_cast< boost::shared_ptr< TER_World >& >( world_ ) = file.GetWorld();
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
