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
#include "tools/thread/BarrierCommand.h"
#include <boost/filesystem/operations.hpp>
#include <boost/bind.hpp>
#include "protocol/protocol.h"

namespace bfs = boost::filesystem;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MessageLoader constructor
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
MessageLoader::MessageLoader( const Config& config, bool threaded )
{
    if( threaded )
    {
        disk_.reset( new tools::thread::ThreadPool( 1 ) );
        cpu_.reset ( new tools::thread::ThreadPool( 1 ) );
    }
    const bfs::path dir( config.GetRecordDirectory(), bfs::native );

    index_    .open( ( dir / "index" ).string().c_str(),    std::ios_base::binary | std::ios_base::in );
    keyIndex_ .open( ( dir / "keyindex" ).string().c_str(), std::ios_base::binary | std::ios_base::in );
    updates_  .open( ( dir / "update" ).string().c_str(),   std::ios_base::binary | std::ios_base::in );
    keys_     .open( ( dir / "key"    ).string().c_str(),   std::ios_base::binary | std::ios_base::in );
    LoadIndices();
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
    if( frameNumber >= frames_.size() )
        return false;
    const Frame& current = frames_[ frameNumber ];
    if( disk_.get() )
        disk_->Enqueue( boost::bind( &MessageLoader::Load, this, boost::ref( updates_ ), current.offset_, current.size_, boost::ref( handler ), callback ) );
    else
        Load( updates_, current.offset_, current.size_, handler, callback );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadKeyFrame
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
unsigned int MessageLoader::LoadKeyFrame( unsigned int frameNumber, MessageHandler_ABC& handler, const T_Callback& callback /*= T_Callback()*/ )
{
    synchronisation_ = frameNumber != 0;
    unsigned key = frameNumber / 100;
    if( key >= keyFrames_.size() )
        key = keyFrames_.size() - 1;
    const KeyFrame& keyFrame = keyFrames_[ key ];

    if( disk_.get() )
        disk_->Enqueue( boost::bind( &MessageLoader::Load, this, boost::ref( keys_ ), keyFrame.offset_, keyFrame.size_, boost::ref( handler ), callback ) );
    else
        Load( keys_, keyFrame.offset_, keyFrame.size_, handler, callback );

    return keyFrame.frameNumber_;
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::Synchronize
// Created: AGE 2007-07-13
// -----------------------------------------------------------------------------
void MessageLoader::Synchronize()
{
    if( disk_.get() )
    {
        tools::thread::BarrierCommand barrier;
        disk_->Enqueue( barrier.command() );
        barrier.wait();
    }

    if( cpu_.get() )
    {
        tools::thread::BarrierCommand barrier;
        cpu_->Enqueue( barrier.command() );
        barrier.wait();
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::ReloadIndices
// Created: AGE 2007-10-31
// -----------------------------------------------------------------------------
void MessageLoader::ReloadIndices()
{
    if( disk_.get() )
        disk_->Enqueue( boost::bind( &MessageLoader::LoadIndices, this ) );
    else
        LoadIndices();
}

namespace dispatcher
{
    struct Buffer
    {
         Buffer( unsigned size, std::ifstream& in )
            : data_( new unsigned char[size] )
            , size_( size )
        {
            in.read( (char*)data_, size );
        }
        ~Buffer()
        {
            delete[] data_;
        }
        unsigned char* data_;
        unsigned size_;
    };
    void YieldMessages( unsigned pendingMessages )
    {
        if( pendingMessages > 10 )
            tools::thread::Thread::Sleep( pendingMessages / 10 );
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
    if( cpu_.get() )
    {
        YieldMessages( cpu_->PendingMessages() );
        cpu_->Enqueue( boost::bind( &MessageLoader::LoadBuffer, this, buffer, boost::ref( handler ), callback ) );
    }
    else
        LoadBuffer( buffer, handler, callback );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadBuffer
// Created: AGE 2007-07-13
// -----------------------------------------------------------------------------
void MessageLoader::LoadBuffer( const boost::shared_ptr< Buffer >& buffer, MessageHandler_ABC& handler, const T_Callback& callback )
{
    unsigned char* current = buffer->data_;
    while( current < buffer->data_ + buffer->size_ )
        LoadSimToClientMessage( current, handler );
    if( callback )
        callback();
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadSimToClientMessage
// Created: AGE 2007-07-13
// -----------------------------------------------------------------------------
void MessageLoader::LoadSimToClientMessage( unsigned char*& input, MessageHandler_ABC& handler )
{
    unsigned long messageSize = *reinterpret_cast< const unsigned long* >( input );
    input += sizeof( unsigned long );

    MsgsSimToClient::MsgSimToClient message;
    if( ! message.ParseFromArray( input, messageSize ) )
        throw std::runtime_error( __FUNCTION__ ": message deserialization failed." );

    // $$$$ JSR 2010-07-01: In synchronisation mode, we must not send order messages, as they were already sent,
    // to avoid them to be displayed several times in timeline (mantis 3725)
    if( synchronisation_ )
    {
        if( message.has_message() )
        {
            if( message.message().has_unit_order() )
                message.mutable_message()->mutable_unit_order()->mutable_type()->set_id( 0 );
            else if( message.message().has_automat_order() )
                message.mutable_message()->mutable_automat_order()->mutable_type()->set_id( 0 );
            else if( message.message().has_crowd_order() )
                message.mutable_message()->mutable_crowd_order()->mutable_type()->set_id( 0 );
        }
    }

    handler.Receive( message );

    input += messageSize;
}

namespace
{
    template< typename C >
    void Load( C& frames, std::ifstream& file )
    {
        typedef typename C::value_type T;
        tools::InputBinaryWrapper input( file );
        T frame;
        input >> frame;
        while( file )
        {
            frames.push_back( frame );
            input >> frame;
        }
        file.clear();
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadIndices
// Created: AGE 2007-10-31
// -----------------------------------------------------------------------------
void MessageLoader::LoadIndices()
{
    ::Load( frames_, index_ );
    ::Load( keyFrames_, keyIndex_ );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::GetTickNumber
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
unsigned int MessageLoader::GetTickNumber() const
{
    return frames_.size();
}
