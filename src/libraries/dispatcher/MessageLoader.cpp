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
#include "MessageHandler_ABC.h"
#include "WaitEvent.h"
#include "tools/InputBinaryStream.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/Protocol.h"
#include "protocol/ReplaySenders.h"
#include "tools/ThreadPool.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/filesystem/operations.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

namespace bfs = boost::filesystem;
using namespace dispatcher;

namespace
{
    const std::string infoFileName( "info" );
    const std::string indexFileName( "index" );
    const std::string keyIndexFileName( "keyindex" );
    const std::string keyFileName( "key" );
    const std::string updateFileName( "update" );
    const std::string currentFolderName( "current" );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader constructor
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
MessageLoader::MessageLoader( const bfs::path& records, bool threaded,
                              ClientPublisher_ABC* clients )
    : records_  ( records )
    , clients_  ( clients )
    , firstTick_( std::numeric_limits< unsigned int >::max() )
    , tickCount_( 0 )
    , init_     ( new WaitEvent() )
    , quit_     ( new WaitEvent() )
{
    if( threaded )
    {
        disk_.reset( new tools::ThreadPool( 1 ) );
        cpu_.reset ( new tools::ThreadPool( 1 ) );
    }
    folderObserver_.reset( new boost::thread( boost::bind( &MessageLoader::ScanData, this ) ) );
    init_->Wait();
}

// -----------------------------------------------------------------------------
// Name: MessageLoader destructor
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
MessageLoader::~MessageLoader()
{
    quit_->Signal();
    folderObserver_->join();
    if( disk_.get() )
        disk_->Join( tools::ThreadPool::ProcessPending );
    if( cpu_.get() )
        cpu_->Join( tools::ThreadPool::ProcessPending );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadFrame
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
bool MessageLoader::LoadFrame( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback /*= T_Callback()*/ )
{
    boost::mutex::scoped_lock lock( access_ );
    if( disk_.get() )
    {
        for( CIT_FragmentsInfos it = fragmentsInfos_.begin(); it != fragmentsInfos_.end(); ++it )
            if( frameNumber >= it->second.first && frameNumber <= it->second.second )
            {
                disk_->Post( boost::bind( &MessageLoader::LoadFrameInThread, this, it->first, frameNumber, boost::ref( handler ), callback ) );
                break;
            }
    }
    else
    {
        if( !SwitchToFragment( frameNumber ) )
            return false;
        const size_t next = frameNumber - fragmentsInfos_[ currentOpenFolder_ ].first;
        if( next >= frames_.size() )
            return false;
        const Frame& current = frames_[ next ];
        Load( updates_, current.offset_, current.size_, handler, callback,
              bfs::path( currentOpenFolder_ ) / updateFileName );
    }
    return true;
}

namespace
{
    const KeyFrame* FindKeyFrame( const MessageLoader::T_KeyFrames& keyFrames, unsigned frame )
    {
        const KeyFrame* next = 0;
        for( MessageLoader::CIT_KeyFrames it = keyFrames.begin(); it != keyFrames.end(); ++it )
            if( it->frameNumber_ <= frame )
                next = &*it;
        return next;
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadKeyFrame
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
void MessageLoader::LoadKeyFrame( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback /*= T_Callback()*/ )
{
    boost::mutex::scoped_lock lock( access_ );
    synchronisation_ = frameNumber != 0;
    if( disk_.get() )
    {
        for( CIT_FragmentsInfos it = fragmentsInfos_.begin(); it != fragmentsInfos_.end(); ++it )
            if( frameNumber >= it->second.first && frameNumber <= it->second.second )
            {
                disk_->Post( boost::bind( &MessageLoader::LoadKeyFrameInThread, this, it->first, frameNumber, boost::ref( handler ), callback ) );
                return;
            }
    }
    else
    {
        if( !SwitchToFragment( frameNumber ) )
            return;
        const KeyFrame* next = ::FindKeyFrame( keyFrames_, frameNumber );
        if( next )
            Load( keys_, next->offset_, next->size_, handler, callback,
                  bfs::path( currentOpenFolder_ ) / keyFileName );
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::GetTickNumber
// Created: JSR 2010-10-28
// -----------------------------------------------------------------------------
unsigned int MessageLoader::GetTickNumber() const
{
    boost::mutex::scoped_lock lock( access_ );
    return tickCount_;
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::GetFirstTick
// Created: JSR 2011-11-03
// -----------------------------------------------------------------------------
unsigned int MessageLoader::GetFirstTick() const
{
    boost::mutex::scoped_lock lock( access_ );
    return firstTick_;
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::FindKeyFrame
// Created: JSR 2010-10-28
// -----------------------------------------------------------------------------
unsigned int MessageLoader::FindKeyFrame( unsigned int frameNumber )
{
    boost::mutex::scoped_lock lock( access_ );
    if( !SwitchToFragment( frameNumber ) )
        return 0;
    const KeyFrame* next = ::FindKeyFrame( keyFrames_, frameNumber );
    return next ? next->frameNumber_ : 0;
}

namespace
{
    bool OpenFile( std::ifstream& stream, const bfs::path& file )
    {
        if( !bfs::exists( file ) )
            return false;
        // istream open()/close() are not supposed to clear the file state,
        // it must be done manually, nice move C++.
        stream.clear();
        stream.open( file.string().c_str(), std::ios_base::binary | std::ios_base::in );
        return stream.good();
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::FillTimeTable
// Created: JSR 2011-07-25
// -----------------------------------------------------------------------------
void MessageLoader::FillTimeTable( sword::TimeTable& msg, unsigned int beginTick, unsigned int endTick ) const
{
    try
    {
        unsigned int tick = beginTick;
        while( tick <= endTick )
        {
            bool incremented = false;
            boost::mutex::scoped_lock lock( access_ );
            for( CIT_FragmentsInfos it = fragmentsInfos_.begin(); it != fragmentsInfos_.end(); ++it )
            {
                if( tick >= it->second.first && tick <= it->second.second )
                {
                    std::ifstream infoFile;
                    unsigned int start;
                    unsigned int end;
                    std::string simTime;
                    std::string realTime;
                    if( OpenFile( infoFile, records_ / it->first / infoFileName ) )
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
        MT_LOG_ERROR_MSG( "Exception caught while filling time table." );
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::ScanData
// Created: JSR 2010-10-27
// -----------------------------------------------------------------------------
void MessageLoader::ScanData()
{
    while( !quit_->IsSignaled() )
    {
        ScanDataFolders( false );
        init_->Signal();
        quit_->Wait( boost::posix_time::seconds( 10 ) );
    }
}

namespace
{
    bool IsValidRecordDir( const bfs::directory_iterator& it )
    {
        if( !bfs::is_directory( it->status() ) )
            return false;
        const bfs::path& root = it->path();
        return bfs::is_regular_file( root / infoFileName )
            && bfs::is_regular_file( root / indexFileName )
            && bfs::is_regular_file( root / keyIndexFileName )
            && bfs::is_regular_file( root / keyFileName )
            && bfs::is_regular_file( root / updateFileName );
    }

    bfs::directory_iterator IterateDirectory( const bfs::path& path )
    {
        if( !bfs::exists( path ) )
            return bfs::directory_iterator();
        return bfs::directory_iterator( path );
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::ScanDataFolders
// Created: JSR 2010-10-27
// -----------------------------------------------------------------------------
void MessageLoader::ScanDataFolders( bool forceAdd )
{
    boost::mutex::scoped_lock lock( access_ );
    for( bfs::directory_iterator it = IterateDirectory( records_ ); it !=  bfs::directory_iterator(); ++it )
        try
        {
            if( !IsValidRecordDir( it ) )
                continue;
            const std::string dir = it->path().filename().string();
            bool doAdd = false;
            if( !forceAdd )
            {
                const bool mainLoop = !disk_.get() && init_->IsSignaled();
                const bool skipCurrent = mainLoop && dir == currentFolderName;
                doAdd = !skipCurrent && fragmentsInfos_.find( dir ) == fragmentsInfos_.end();
                if( mainLoop && doAdd  )
                    fragmentsInfos_.erase( currentFolderName );
            }
            if( doAdd || forceAdd )
                AddFolder( dir );
        }
        catch( const std::exception & )
        {
            // NOTHING
        }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::AddFolder
// Created: JSR 2010-10-28
// -----------------------------------------------------------------------------
void MessageLoader::AddFolder( const std::string& folderName )
{
    std::ifstream infoFile;
    unsigned int start;
    unsigned int end;
    if( !OpenFile( infoFile, records_ / folderName / infoFileName ) )
        return;
    tools::InputBinaryWrapper wrapper( infoFile );
    wrapper >> start;
    wrapper >> end;
    infoFile.close();
    fragmentsInfos_[ folderName ] = std::make_pair< unsigned int, unsigned int >( start, end );
    tickCount_ = std::max( tickCount_, end );
    firstTick_ = std::min( firstTick_, start );
    if( clients_ )
    {
        replay::NewDataChunkNotification msg;
        msg().set_last_tick( tickCount_ );
        msg.Send( *clients_ );
    }
}

namespace
{
    bool IsEof( std::ifstream& file )
    {
        file.peek();
        return file.eof();
    }

    void LoadIndexes( MessageLoader::T_Frames& frames, std::ifstream& file )
    {
        try
        {
            tools::InputBinaryWrapper input( file );
            while( !IsEof( file ) )
            {
                Frame frame;
                input >> frame.offset_;
                input >> frame.size_;
                frames.push_back( frame );
            }
        }
        catch( ... )
        {
            MT_LOG_WARNING_MSG( "Unable to read index file completely" );
        }
    }

    void LoadIndexes( MessageLoader::T_KeyFrames& frames, std::ifstream& file )
    {
        try
        {
            tools::InputBinaryWrapper input( file );
            while( !IsEof( file ) )
            {
                KeyFrame frame;
                input >> frame.frameNumber_;
                input >> frame.offset_;
                input >> frame.size_;
                frames.push_back( frame );
            }
        }
        catch( ... )
        {
            MT_LOG_WARNING_MSG( "Unable to read key index file completely" );
        }
    }

    template< typename T >
    T LoadIndexesFrom( const bfs::path& filename )
    {
        T reply;
        std::ifstream stream;
        const bool valid = OpenFile( stream, filename );
        if( valid )
            LoadIndexes( reply, stream );
        return reply;
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::SwitchToFragment
// Created: JSR 2010-10-28
// -----------------------------------------------------------------------------
bool MessageLoader::SwitchToFragment( unsigned int& frameNumber )
{
    frameNumber = std::max( firstTick_, std::min( frameNumber, tickCount_ ) );
    const bool doSwitch = currentOpenFolder_.empty()
                       || frameNumber < fragmentsInfos_[ currentOpenFolder_ ].first
                       || frameNumber > fragmentsInfos_[ currentOpenFolder_ ].second;
    if( !doSwitch )
        return true;

    frames_.clear();
    keyFrames_.clear();
    updates_.close();
    keys_.close();
    currentOpenFolder_.clear();

    for( CIT_FragmentsInfos it = fragmentsInfos_.begin(); it != fragmentsInfos_.end(); ++it )
        if( frameNumber >= it->second.first && frameNumber <= it->second.second )
        {
            currentOpenFolder_ = it->first;
            const bfs::path dir = records_ / currentOpenFolder_;
            OpenFile( updates_, dir / updateFileName );
            OpenFile( keys_,    dir / keyFileName );
            frames_    = LoadIndexesFrom< T_Frames >   ( dir / indexFileName );
            keyFrames_ = LoadIndexesFrom< T_KeyFrames >( dir / keyIndexFileName );
            return true;
        }
    return false;
}

struct dispatcher::Buffer
{
    Buffer( size_t size )
        : data_( size )
    {
        // NOTHING
    }
    bool Read( std::ifstream& src )
    {
        size_t len = 0;
        const size_t size = data_.size();
        while( !IsEof( src ) && src.good() && len < size )
        {
            src.read( &data_[len], size - len );
            len += static_cast< size_t >( src.gcount() );
        }
        data_.resize( len );
        return len == size;
    }
    std::vector< char > data_;
};

namespace
{
    typedef boost::shared_ptr< dispatcher::Buffer > BufPtr;

    BufPtr MakeBuffer( std::ifstream& src, size_t size, const bfs::path& filename )
    {
        BufPtr next( boost::make_shared< Buffer >( size ) );
        const bool valid = next->Read( src );
        if( !valid )
            MT_LOG_WARNING_MSG( "[dispatcher] Skipping invalid file " + filename.string() );
        return valid ? next : BufPtr();
    }

    BufPtr MakeBufferFrom( const bfs::path& filename, unsigned offset, unsigned size )
    {
        std::ifstream stream;
        const bool valid = OpenFile( stream, filename );
        if( !valid )
            return BufPtr();
        stream.seekg( offset );
        return MakeBuffer( stream, size, filename );
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::Load
// Created: AGE 2007-07-13
// -----------------------------------------------------------------------------
void MessageLoader::Load( std::ifstream& in, unsigned from, unsigned size, MessageHandler_ABC& handler, const T_Callback& callback, const bfs::path& filename )
{
    in.clear();
    in.seekg( from );
    BufPtr buf = MakeBuffer( in, size, filename );
    if( buf )
        LoadBuffer( buf, handler, callback, synchronisation_ );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadFrameInThread
// Created: JSR 2010-10-29
// -----------------------------------------------------------------------------
void MessageLoader::LoadFrameInThread( const std::string& folder, unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback )
{
    const T_Frames frames = LoadIndexesFrom< T_Frames >( records_ / folder / indexFileName );

    boost::mutex::scoped_lock dataLock( access_ );
    T_FragmentsInfos::const_iterator it = fragmentsInfos_.find( folder );
    if( it == fragmentsInfos_.end() )
        return;

    const size_t next = frameNumber - it->second.first;
    if( next >= frames.size() )
        return;

    const Frame& current = frames[ next ];
    BufPtr buf = MakeBufferFrom( records_ / folder / updateFileName, current.offset_, current.size_ );
    if( buf )
        cpu_->Post( boost::bind( &MessageLoader::LoadBuffer, this, buf, boost::ref( handler ), callback, synchronisation_ ) );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadKeyFrameInThread
// Created: JSR 2010-10-29
// -----------------------------------------------------------------------------
void MessageLoader::LoadKeyFrameInThread( const std::string& folder, unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback )
{
    const T_KeyFrames keyFrames = LoadIndexesFrom< T_KeyFrames >( records_ / folder / keyIndexFileName );
    const KeyFrame* next = ::FindKeyFrame( keyFrames, frameNumber );
    if( !next )
        return;

    BufPtr buf = MakeBufferFrom( records_ / folder / keyFileName, next->offset_, next->size_ );
    if( buf )
        cpu_->Post( boost::bind( &MessageLoader::LoadBuffer, this, buf, boost::ref( handler ), callback, synchronisation_ ) );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadBuffer
// Created: AGE 2007-07-13
// -----------------------------------------------------------------------------
void MessageLoader::LoadBuffer( const BufPtr& buffer, MessageHandler_ABC& handler,
                                const T_Callback& callback, bool synchronized )
{
    char* current = &buffer->data_[0];
    char* end = current + buffer->data_.size();
    while( current < end )
        current += LoadSimToClientMessage( current, end - current, handler, synchronized );
    if( callback )
        callback();
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadSimToClientMessage
// Created: AGE 2007-07-13
// -----------------------------------------------------------------------------
size_t MessageLoader::LoadSimToClientMessage( const char* input, size_t size,
                                              MessageHandler_ABC& handler,
                                              bool synchronized )
{
    unsigned msgSize = 0;
    if( size < sizeof msgSize )
        throw std::runtime_error( __FUNCTION__" not enough data for message size" );
    msgSize = *reinterpret_cast< const unsigned* >( input );
    if( msgSize + sizeof msgSize > size )
        throw std::runtime_error( __FUNCTION__" not enough data for message payload" );
    sword::SimToClient message;
    const bool valid = message.ParseFromArray( input + sizeof msgSize, msgSize );
    if( !valid )
        throw std::runtime_error( __FUNCTION__" invalid message" );
    // $$$$ JSR 2010-07-01: In synchronisation mode, we must not send order messages, as they were already sent,
    // to avoid them to be displayed several times in timeline (mantis 3725)
    if( synchronized && message.has_message() )
    {
        if( message.message().has_unit_order() )
            message.mutable_message()->mutable_unit_order()->mutable_type()->set_id( 0 );
        else if( message.message().has_automat_order() )
            message.mutable_message()->mutable_automat_order()->mutable_type()->set_id( 0 );
        else if( message.message().has_crowd_order() )
            message.mutable_message()->mutable_crowd_order()->mutable_type()->set_id( 0 );
    }
    handler.Receive( message );
    return msgSize + sizeof msgSize;
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::ReloadAllFragmentsInfos
// Created: MMC 2012-09-14
// -----------------------------------------------------------------------------
void MessageLoader::ReloadAllFragmentsInfos()
{
    {
        boost::mutex::scoped_lock lock( access_ );
        fragmentsInfos_.clear();
        currentOpenFolder_.clear();
    }
    ScanDataFolders( true );
}
