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
#include "tools/WaitEvent.h"
#include "tools/InputBinaryWrapper.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/Protocol.h"
#include "protocol/ReplaySenders.h"
#include "tools/ThreadPool.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;
using namespace dispatcher;

namespace
{
    const tools::Path infoFileName( "info" );
    const tools::Path indexFileName( "index" );
    const tools::Path keyIndexFileName( "keyindex" );
    const tools::Path keyFileName( "key" );
    const tools::Path updateFileName( "update" );
    const tools::Path currentFolderName( "current" );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader constructor
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
MessageLoader::MessageLoader( const tools::Path& records, bool threaded,
                              ClientPublisher_ABC* clients )
    : records_  ( records )
    , clients_  ( clients )
    , firstTick_( std::numeric_limits< unsigned int >::max() )
    , tickCount_( 0 )
    , init_     ( new tools::WaitEvent() )
    , quit_     ( new tools::WaitEvent() )
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
        for( auto it = fragmentsInfos_.begin(); it != fragmentsInfos_.end(); ++it )
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
        Load( updates_, current.offset_, current.size_, handler, callback, currentOpenFolder_ / updateFileName );
    }
    return true;
}

namespace
{
    const KeyFrame* FindKeyFrame( const MessageLoader::T_KeyFrames& keyFrames, unsigned frame )
    {
        const KeyFrame* next = 0;
        for( auto it = keyFrames.begin(); it != keyFrames.end(); ++it )
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
    if( disk_.get() )
    {
        for( auto it = fragmentsInfos_.begin(); it != fragmentsInfos_.end(); ++it )
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
            Load( keys_, next->offset_, next->size_, handler, callback, currentOpenFolder_ / keyFileName );
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
        return UINT_MAX;
    const KeyFrame* next = ::FindKeyFrame( keyFrames_, frameNumber );
    return next ? next->frameNumber_ : UINT_MAX;
}

namespace
{
    bool OpenFile( tools::Ifstream& stream, const tools::Path& file )
    {
        if( !file.Exists() )
            return false;
        // istream open()/close() are not supposed to clear the file state,
        // it must be done manually, nice move C++.
        stream.clear();
        stream.open( file, std::ios_base::binary | std::ios_base::in );
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
            for( auto it = fragmentsInfos_.begin(); it != fragmentsInfos_.end(); ++it )
            {
                if( tick >= it->second.first && tick <= it->second.second )
                {
                    tools::Ifstream infoFile;
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
                                item->set_tick( tick + 1 );
                                item->mutable_simulation_time()->set_data( simTime );
                                item->mutable_real_time()->set_data( realTime );
                                int size = msg.ByteSize();
                                if( size > 512 * 1024 ) //512 kB
                                {
                                    msg.set_partial( true );
                                    return;
                                }
                                ++tick;
                            }
                            if( tick > endTick )
                                return;
                        }
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

namespace
{
    unsigned int ConvertFromGDHToSeconds( const std::string& GDHDate )
    {
        bpt::ptime time = bpt::from_iso_string( GDHDate );
        return ( time - bpt::from_time_t( 0 ) ).total_seconds();
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::FindTickForDate
// Created: JSR 2013-03-12
// -----------------------------------------------------------------------------
unsigned int MessageLoader::FindTickForDate( const std::string& GDHDate ) const
{
    const unsigned int seconds = ConvertFromGDHToSeconds( GDHDate );
    unsigned int ret = GetFirstTick();
    try
    {
        boost::mutex::scoped_lock lock( access_ );
        bool found = false;
        for( auto it = fragmentsInfos_.begin(); it != fragmentsInfos_.end() && !found; ++it )
        {
            tools::Ifstream infoFile;
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
                    if( ConvertFromGDHToSeconds( simTime ) > seconds )
                    {
                        found = true;
                        break;
                    }
                    else
                        ret = i;
                }
                infoFile.close();
            }
        }
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Exception caught while finding tick for date." );
    }
    return std::min( ret, GetTickNumber() - 1 );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::GetEndDateTime
// Created: JSR 2013-11-06
// -----------------------------------------------------------------------------
const std::string& MessageLoader::GetEndDateTime() const
{
    return endDateTime_;
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
    bool IsValidRecordDir( const tools::Path& root )
    {
        if( !root.IsDirectory() )
            return false;
        return ( root / infoFileName ).IsRegularFile()
            && ( root / indexFileName ).IsRegularFile()
            && ( root / keyIndexFileName ).IsRegularFile()
            && ( root / keyFileName ).IsRegularFile()
            && ( root / updateFileName ).IsRegularFile();
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::ScanFolder
// Created: ABR 2013-03-14
// -----------------------------------------------------------------------------
bool MessageLoader::ScanFolder( const tools::Path& path, bool forceAdd )
{
    try
    {
        if( !IsValidRecordDir( path ) )
            return false;

        const tools::Path filename = path.FileName();
        bool doAdd = false;
        if( !forceAdd )
        {
            const bool mainLoop = !disk_.get() && init_->IsSignaled();
            const bool skipCurrent = mainLoop && filename == currentFolderName;
            doAdd = !skipCurrent && fragmentsInfos_.find( filename ) == fragmentsInfos_.end();
            if( mainLoop && doAdd  )
                fragmentsInfos_.erase( currentFolderName );
        }
        if( doAdd || forceAdd )
            AddFolder( filename );
    }
    catch( const std::exception& )
    {
        // NOTHING
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::ScanDataFolders
// Created: JSR 2010-10-27
// -----------------------------------------------------------------------------
void MessageLoader::ScanDataFolders( bool forceAdd )
{
    boost::mutex::scoped_lock lock( access_ );
    records_.Apply( boost::bind( &MessageLoader::ScanFolder, this, _1, forceAdd ), false );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::AddFolder
// Created: JSR 2010-10-28
// -----------------------------------------------------------------------------
void MessageLoader::AddFolder( const tools::Path& folderName )
{
    tools::Ifstream infoFile;
    unsigned int start;
    unsigned int end;
    if( !OpenFile( infoFile, records_ / folderName / infoFileName ) )
        return;
    tools::InputBinaryWrapper wrapper( infoFile );
    wrapper >> start;
    wrapper >> end;
    fragmentsInfos_[ folderName ] = std::make_pair( start, end );
    if( tickCount_ < end )
    {
        tickCount_ = end;
        std::string dummy;
        for( unsigned int i = start; i < end; ++i )
            wrapper >> dummy >> dummy;
        wrapper >> endDateTime_;
    }
    infoFile.close();
    firstTick_ = std::min( firstTick_, start );
    if( clients_ )
    {
        replay::NewDataChunkNotification msg;
        msg().set_last_tick( tickCount_ );
        if( !endDateTime_.empty() )
            msg().mutable_end_date_time()->set_data( endDateTime_ );
        msg.Send( *clients_ );
    }
}

namespace
{
    bool IsEof( tools::Ifstream& file )
    {
        file.peek();
        return file.eof();
    }

    void LoadIndexes( MessageLoader::T_Frames& frames, tools::Ifstream& file )
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

    void LoadIndexes( MessageLoader::T_KeyFrames& frames, tools::Ifstream& file )
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
    T LoadIndexesFrom( const tools::Path& filename )
    {
        T reply;
        tools::Ifstream stream;
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
    const bool doSwitch = currentOpenFolder_.IsEmpty()
                       || frameNumber < fragmentsInfos_[ currentOpenFolder_ ].first
                       || frameNumber > fragmentsInfos_[ currentOpenFolder_ ].second;
    if( !doSwitch )
        return true;

    frames_.clear();
    keyFrames_.clear();
    updates_.close();
    keys_.close();
    currentOpenFolder_.Clear();

    for( auto it = fragmentsInfos_.begin(); it != fragmentsInfos_.end(); ++it )
        if( frameNumber >= it->second.first && frameNumber <= it->second.second )
        {
            currentOpenFolder_ = it->first;
            const tools::Path dir = records_ / currentOpenFolder_;
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
    bool Read( tools::Ifstream& src )
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

    BufPtr MakeBuffer( tools::Ifstream& src, size_t size, const tools::Path& filename )
    {
        BufPtr next( boost::make_shared< Buffer >( size ) );
        const bool valid = next->Read( src );
        if( !valid )
            MT_LOG_WARNING_MSG( "[dispatcher] Skipping invalid file " + filename.ToUTF8() );
        return valid ? next : BufPtr();
    }

    BufPtr MakeBufferFrom( const tools::Path& filename, unsigned offset, unsigned size )
    {
        tools::Ifstream stream;
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
void MessageLoader::Load( tools::Ifstream& in, unsigned from, unsigned size, MessageHandler_ABC& handler, const T_Callback& callback, const tools::Path& filename )
{
    in.clear();
    in.seekg( from );
    BufPtr buf = MakeBuffer( in, size, filename );
    if( buf )
        LoadBuffer( buf, handler, callback );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadFrameInThread
// Created: JSR 2010-10-29
// -----------------------------------------------------------------------------
void MessageLoader::LoadFrameInThread( const tools::Path& folder, unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback )
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
        cpu_->Post( boost::bind( &MessageLoader::LoadBuffer, this, buf, boost::ref( handler ), callback ) );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadKeyFrameInThread
// Created: JSR 2010-10-29
// -----------------------------------------------------------------------------
void MessageLoader::LoadKeyFrameInThread( const tools::Path& folder, unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback )
{
    const T_KeyFrames keyFrames = LoadIndexesFrom< T_KeyFrames >( records_ / folder / keyIndexFileName );
    const KeyFrame* next = ::FindKeyFrame( keyFrames, frameNumber );
    if( !next )
        return;

    BufPtr buf = MakeBufferFrom( records_ / folder / keyFileName, next->offset_, next->size_ );
    if( buf )
        cpu_->Post( boost::bind( &MessageLoader::LoadBuffer, this, buf, boost::ref( handler ), callback ) );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadBuffer
// Created: AGE 2007-07-13
// -----------------------------------------------------------------------------
void MessageLoader::LoadBuffer( const BufPtr& buffer, MessageHandler_ABC& handler, const T_Callback& callback )
{
    char* current = &buffer->data_[0];
    char* end = current + buffer->data_.size();
    while( current < end )
        current += LoadSimToClientMessage( current, end - current, handler );
    if( callback )
        callback();
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadSimToClientMessage
// Created: AGE 2007-07-13
// -----------------------------------------------------------------------------
size_t MessageLoader::LoadSimToClientMessage( const char* input, size_t size, MessageHandler_ABC& handler )
{
    unsigned msgSize = 0;
    const size_t header = sizeof msgSize;
    if( size < header )
    {
        MT_LOG_WARNING_MSG( "[dispatcher] Skipping message due to missing header data" );
        return size;
    }

    msgSize = *reinterpret_cast< const unsigned* >( input );
    size_t payload = std::min( size - header, static_cast< size_t >( msgSize ) );
    if( msgSize > payload )
        MT_LOG_WARNING_MSG( "[dispatcher] Truncating message due to truncated payload or corrupted header" );
    sword::SimToClient message;
    const bool valid = message.ParseFromArray( input + header, static_cast< int >( payload ) );
    if( !valid )
    {
        MT_LOG_WARNING_MSG( "[dispatcher] Skipping message due to corrupted payload" );
        return header + payload;
    }

    handler.Receive( message );
    return header + payload;
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
        currentOpenFolder_.Clear();
    }
    ScanDataFolders( true );
}
