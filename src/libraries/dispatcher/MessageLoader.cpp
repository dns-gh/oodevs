// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MessageLoader.h"
#include "Config.h"
#include "MessageHandler_ABC.h"
#include "Config.h"
#include "tools/InputBinaryStream.h"
#include "protocol/Protocol.h"
#include <boost/filesystem/operations.hpp>
#include <boost/bind.hpp>

namespace bfs = boost::filesystem;
using namespace dispatcher;

const std::string MessageLoader::infoFileName_( "info" );
const std::string MessageLoader::indexFileName_( "index" );
const std::string MessageLoader::keyIndexFileName_( "keyindex" );
const std::string MessageLoader::keyFileName_( "key" );
const std::string MessageLoader::updateFileName_( "update" );
boost::mutex MessageLoader::filesAccessMutex_;

// -----------------------------------------------------------------------------
// Name: MessageLoader constructor
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
MessageLoader::MessageLoader( const Config& config, bool threaded )
    : config_   ( config )
    , tickCount_( 0 )
    , initReady_( false )
{
    if( threaded )
    {
        disk_.reset( new tools::thread::ThreadPool( 1 ) );
        cpu_.reset ( new tools::thread::ThreadPool( 1 ) );
    }
    folderObserver_.reset( new boost::thread( boost::bind( &MessageLoader::ScanData, this ) ) );
    boost::unique_lock< boost::mutex > lock( initMutex_ );
    while( !initReady_ )
        initCondition_.wait(lock);
}

// -----------------------------------------------------------------------------
// Name: MessageLoader destructor
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
MessageLoader::~MessageLoader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadFrame
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
bool MessageLoader::LoadFrame( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback /*= T_Callback()*/ )
{
    ++frameNumber;
    if( disk_.get() )
    {
        boost::mutex::scoped_lock lock( dataAccessMutex_ );
        for( CIT_FragmentsInfos it = fragmentsInfos_.begin(); it != fragmentsInfos_.end(); ++it )
            if( frameNumber >= it->second.first && frameNumber <= it->second.second )
            {
                disk_->Enqueue( boost::bind( &MessageLoader::LoadFrameInThread, this, boost::cref( it->first ), frameNumber, boost::ref( handler ), callback ) );
                break;
            }
    }
    else
    {
        if( !SwitchToFragment( frameNumber ) )
            return false;
        boost::mutex::scoped_lock lock( dataAccessMutex_ );
        const Frame& current = frames_[ frameNumber - fragmentsInfos_[ currentOpenFolder_ ].first ];
        Load( updates_, current.offset_, current.size_, handler, callback );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadKeyFrame
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
void MessageLoader::LoadKeyFrame( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback /*= T_Callback()*/ )
{
    synchronisation_ = frameNumber != 0;
    ++frameNumber;
    if( disk_.get() )
    {
        boost::mutex::scoped_lock lock( dataAccessMutex_ );
        for( CIT_FragmentsInfos it = fragmentsInfos_.begin(); it != fragmentsInfos_.end(); ++it )
            if( frameNumber >= it->second.first && frameNumber <= it->second.second )
            {
                disk_->Enqueue( boost::bind( &MessageLoader::LoadKeyFrameInThread, this, boost::cref( it->first ), frameNumber, boost::ref( handler ), callback ) );
                return;
            }
    }
    else
    {
        if( !SwitchToFragment( frameNumber ) )
            return;
        KeyFrame keyFrame = *keyFrames_.begin();
        for( CIT_KeyFrames it = keyFrames_.begin(); it != keyFrames_.end(); ++it )
        {
            if( it->frameNumber_ > frameNumber )
                break;
            keyFrame = *it;
        }
        Load( keys_, keyFrame.offset_, keyFrame.size_, handler, callback );
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::GetTickNumber
// Created: JSR 2010-10-28
// -----------------------------------------------------------------------------
unsigned int MessageLoader::GetTickNumber() const
{
    boost::mutex::scoped_lock lock( dataAccessMutex_ );
    return tickCount_;
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::FindKeyFrame
// Created: JSR 2010-10-28
// -----------------------------------------------------------------------------
unsigned int MessageLoader::FindKeyFrame( unsigned int frameNumber )
{
    ++frameNumber;
    if( !SwitchToFragment( frameNumber ) )
        return 0;
    unsigned int ret = keyFrames_.begin()->frameNumber_;
    for( CIT_KeyFrames it = keyFrames_.begin(); it != keyFrames_.end() && it->frameNumber_ <= frameNumber; ++it )
        ret = it->frameNumber_;
    return ret - 1;
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::FillTimeTable
// Created: JSR 2011-07-25
// -----------------------------------------------------------------------------
void MessageLoader::FillTimeTable( sword::TimeTable& msg, unsigned int beginTick, unsigned int endTick ) const
{
    try
    {
        boost::mutex::scoped_lock lock( filesAccessMutex_ );
        unsigned int tick = beginTick;
        while( tick <= endTick )
        {
            bool incremented = false;
            for( CIT_FragmentsInfos it = fragmentsInfos_.begin(); it != fragmentsInfos_.end(); ++it )
            {
                if( tick >= it->second.first && tick <= it->second.second )
                {
                    std::ifstream infoFile;
                    unsigned int start;
                    unsigned int end;
                    std::string simTime;
                    std::string realTime;
                    if( OpenFile( infoFile, it->first, infoFileName_ ) )
                    {
                        tools::InputBinaryWrapper wrapper( infoFile );
                        wrapper >> start;
                        wrapper >> end;
                        for( unsigned int i = start; i <= end; ++i )
                        {
                            wrapper >> simTime;
                            wrapper >> realTime;
                            if( i == tick )
                            {
                                incremented = true;
                                sword::TimeTable_TimeMapping* item = msg.add_time_table_item();
                                item->set_tick( tick );
                                item->mutable_simulation_time()->set_data( simTime );
                                item->mutable_real_time()->set_data( realTime );
                                ++tick;
                            }

                            if( tick > endTick )
                            {
                                infoFile.close();
                                return;
                            }
                        }

                        infoFile.close();
                    }
                    break;
                }
            }
            if( !incremented )
                ++tick; // juste pour éviter une éventuelle boucle sans fin, au cas où il y aurait eu une erreur.
        }
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::ScanData
// Created: JSR 2010-10-27
// -----------------------------------------------------------------------------
void MessageLoader::ScanData()
{
    const bfs::path dir( config_.GetRecordDirectory(), bfs::native );
    for( ;; )
    {
        if( bfs::exists( dir ) )
            for( bfs::directory_iterator it( dir ); it !=  bfs::directory_iterator(); ++it )
                try
                {
                    bool doAdd = false;
                    {
                        boost::mutex::scoped_lock lock( dataAccessMutex_ );
                        doAdd = bfs::is_directory( it->status() ) && fragmentsInfos_.find( it->path().leaf() ) == fragmentsInfos_.end();
                    }
                    if( doAdd )
                        AddFolder( it->path().leaf() );
                }
                catch( const std::exception & )
                {
                    // NOTHING
                }
        if( !initReady_ )
        {
            boost::lock_guard< boost::mutex > lock( initMutex_ );
            initReady_ = true;
            initCondition_.notify_all();
        }
        boost::this_thread::sleep( boost::posix_time::seconds( 10 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::AddFolder
// Created: JSR 2010-10-28
// -----------------------------------------------------------------------------
void MessageLoader::AddFolder( const std::string& folderName )
{
    boost::mutex::scoped_lock lock( filesAccessMutex_ );
    std::ifstream infoFile;
    unsigned int start;
    unsigned int end;
    if( !OpenFile( infoFile, folderName, infoFileName_ ) )
        return;
    tools::InputBinaryWrapper wrapper( infoFile );
    wrapper >> start;
    wrapper >> end;
    infoFile.close();
    fragmentsInfos_[ folderName ] = std::make_pair< unsigned int, unsigned int >( start, end );
    tickCount_ = std::max( tickCount_, end );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::OpenFile
// Created: JSR 2010-10-29
// -----------------------------------------------------------------------------
bool MessageLoader::OpenFile( std::ifstream& stream, const std::string& folder, const std::string& file ) const
{
    const bfs::path dir = bfs::path( config_.GetRecordDirectory(), bfs::native ) / folder / file;
    if( !bfs::exists( dir ) )
        return false;
    stream.open( dir.string().c_str(), std::ios_base::binary | std::ios_base::in );
    return true;
}

namespace
{
    // $$$$ JSR 2011-07-29: temporaire : tentative fix 5947
    void LoadIndexes( std::vector< Frame >& frames, std::ifstream& file )
    {
        tools::InputBinaryWrapper input( file );
        Frame frame;
        input >> frame.offset_;
        if( file )
        {
            input >> frame.size_;
            while( file )
            {
                frames.push_back( frame );
                input >> frame.offset_;
                if( !file )
                    break;
                input >> frame.size_;
            }
        }
        file.clear();
    }

    void LoadIndexes( std::vector< KeyFrame >& frames, std::ifstream& file )
    {
        tools::InputBinaryWrapper input( file );
        KeyFrame frame;
        input >> frame.frameNumber_;
        if( file )
        {
            input >> frame.offset_;
            if( file )
            {
                input >> frame.size_;
                while( file )
                {
                    frames.push_back( frame );
                    input >> frame.frameNumber_;
                    if( !file )
                        break;
                    input >> frame.offset_;
                    if( !file )
                        break;
                    input >> frame.size_;
                 }
            }
        }
        file.clear();
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::SwitchToFragment
// Created: JSR 2010-10-28
// -----------------------------------------------------------------------------
bool MessageLoader::SwitchToFragment( unsigned int& frameNumber )
{
    if( frameNumber > tickCount_ )
        frameNumber = tickCount_;
    bool doSwitch = false;
    {
        boost::mutex::scoped_lock lock( dataAccessMutex_ );
        doSwitch = currentOpenFolder_.empty() || frameNumber < fragmentsInfos_[ currentOpenFolder_ ].first || frameNumber > fragmentsInfos_[ currentOpenFolder_ ].second;
    }
    if( doSwitch )
    {
        frames_.clear();
        keyFrames_.clear();
        if( !currentOpenFolder_.empty() )
        {
            boost::mutex::scoped_lock lock( filesAccessMutex_ );
            index_.close();
            keyIndex_.close();
            updates_.close();
            keys_.close();
            currentOpenFolder_.clear();
        }
        for( CIT_FragmentsInfos it = fragmentsInfos_.begin(); it != fragmentsInfos_.end(); ++it )
            if( frameNumber >= it->second.first && frameNumber <= it->second.second )
            {
                currentOpenFolder_ = it->first;
                boost::mutex::scoped_lock lock( filesAccessMutex_ );
                OpenFile( index_, currentOpenFolder_, indexFileName_ );
                OpenFile( keyIndex_, currentOpenFolder_, keyIndexFileName_ );
                OpenFile( updates_, currentOpenFolder_, updateFileName_ );
                OpenFile( keys_, currentOpenFolder_, keyFileName_ );
                LoadIndexes( frames_, index_ );
                LoadIndexes( keyFrames_, keyIndex_ );
                return true;
            }
        return false;
    }
    return true;
}

namespace dispatcher
{
    struct Buffer
    {
        Buffer( unsigned size, std::ifstream& in )
            : data_( new char[ size ] )
            , size_( size )
        {
            in.read( data_, size );
        }
        ~Buffer()
        {
            delete [] data_;
        }
        char* data_;
        unsigned int size_;
    };

    void YieldMessages( std::size_t pendingMessages )
    {
        if( pendingMessages > 10 )
            tools::thread::Thread::Sleep( static_cast< unsigned int >( pendingMessages / 10 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::Load
// Created: AGE 2007-07-13
// -----------------------------------------------------------------------------
void MessageLoader::Load( std::ifstream& in, unsigned from, unsigned size, MessageHandler_ABC& handler, const T_Callback& callback )
{
    in.seekg( from );
    boost::shared_ptr< Buffer > buffer( new Buffer( size, in ) );
    LoadBuffer( buffer, handler, callback );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadFrameInThread
// Created: JSR 2010-10-29
// -----------------------------------------------------------------------------
void MessageLoader::LoadFrameInThread( const std::string& folder, unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback )
{
    boost::mutex::scoped_lock fileLock( filesAccessMutex_ );
    std::ifstream index;
    OpenFile( index, folder, indexFileName_ );
    T_Frames frames;
    LoadIndexes( frames, index );
    index.close();
    boost::mutex::scoped_lock dataLock( dataAccessMutex_ );
    const Frame& current = frames[ frameNumber - fragmentsInfos_[ folder ].first ];
    std::ifstream file;
    OpenFile( file, folder, updateFileName_ );
    file.seekg( current.offset_ );
    boost::shared_ptr< Buffer > buffer( new Buffer( current.size_, file ) );
    file.close();
    YieldMessages( cpu_->PendingMessages() );
    cpu_->Enqueue( boost::bind( &MessageLoader::LoadBuffer, this, buffer, boost::ref( handler ), callback ) );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadKeyFrameInThread
// Created: JSR 2010-10-29
// -----------------------------------------------------------------------------
void MessageLoader::LoadKeyFrameInThread( const std::string& folder, unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback )
{
    boost::mutex::scoped_lock lock( filesAccessMutex_ );
    std::ifstream index;
    OpenFile( index, folder, keyIndexFileName_ );
    T_KeyFrames keyFrames;
    LoadIndexes( keyFrames, index );
    index.close();
    KeyFrame& keyFrame = *keyFrames.begin();
    for( CIT_KeyFrames it = keyFrames.begin(); it != keyFrames.end() && it->frameNumber_ <= frameNumber; ++it )
        keyFrame = *it;
    std::ifstream file;
    OpenFile( file, folder, keyFileName_ );
    file.seekg( keyFrame.offset_ );
    boost::shared_ptr< Buffer > buffer( new Buffer( keyFrame.size_, file ) );
    file.close();
    YieldMessages( cpu_->PendingMessages() );
    cpu_->Enqueue( boost::bind( &MessageLoader::LoadBuffer, this, buffer, boost::ref( handler ), callback ) );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadBuffer
// Created: AGE 2007-07-13
// -----------------------------------------------------------------------------
void MessageLoader::LoadBuffer( const boost::shared_ptr< Buffer >& buffer, MessageHandler_ABC& handler, const T_Callback& callback )
{
    char* current = buffer->data_;
    while( current < buffer->data_ + buffer->size_ )
        LoadSimToClientMessage( current, handler );
    if( callback )
        callback();
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadSimToClientMessage
// Created: AGE 2007-07-13
// -----------------------------------------------------------------------------
void MessageLoader::LoadSimToClientMessage( char*& input, MessageHandler_ABC& handler )
{
    unsigned int messageSize = *reinterpret_cast< const unsigned int* >( input );
    input += sizeof( unsigned int );
    sword::SimToClient message;
    if( ! message.ParseFromArray( input, messageSize ) )
        throw std::runtime_error( __FUNCTION__ ": message deserialization failed." );
    // $$$$ JSR 2010-07-01: In synchronisation mode, we must not send order messages, as they were already sent,
    // to avoid them to be displayed several times in timeline (mantis 3725)
    if( synchronisation_ && message.has_message() )
    {
        if( message.message().has_unit_order() )
            message.mutable_message()->mutable_unit_order()->mutable_type()->set_id( 0 );
        else if( message.message().has_automat_order() )
            message.mutable_message()->mutable_automat_order()->mutable_type()->set_id( 0 );
        else if( message.message().has_crowd_order() )
            message.mutable_message()->mutable_crowd_order()->mutable_type()->set_id( 0 );
    }
    handler.Receive( message );
    input += messageSize;
}
