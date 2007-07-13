// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MessageLoader.h"
#include "Config.h"
#include "MessageHandler_ABC.h"
#include "Config.h"
#include "tools/InputBinaryStream.h"
#include "tools/AsnMessageDecoder.h"
#include "tools/thread/BarrierCommand.h"
#include <boost/filesystem/operations.hpp>
#include <boost/bind.hpp>
namespace bfs = boost::filesystem;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MessageLoader constructor
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
MessageLoader::MessageLoader( const Config& config, const std::string& records )
//    : disk_( 1 )
//    , cpu_ ( 1 )
{
    const bfs::path dir( config.GetRecorderDirectory( records ), bfs::native );

    LoadIndex   ( ( dir / "index" ).string() );
    LoadKeyIndex( ( dir / "keyindex" ).string() );
    updates_  .open( ( dir / "update" ).string().c_str(), std::ios_base::binary | std::ios_base::in );
    keys_     .open( ( dir / "key"    ).string().c_str(), std::ios_base::binary | std::ios_base::in );
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
bool MessageLoader::LoadFrame( unsigned int frameNumber, MessageHandler_ABC& handler )
{
    if( frameNumber >= frames_.size() )
        return false;
    const Frame& current = frames_[ frameNumber ];
    Load( updates_, current.offset_, current.size_, handler );
//    disk_.Enqueue( boost::bind( &MessageLoader::Load, this, boost::ref( updates_ ), current.offset_, current.size_, boost::ref( handler ) ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadKeyFrame
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
unsigned int MessageLoader::LoadKeyFrame( unsigned int frameNumber, MessageHandler_ABC& handler )
{
    unsigned key = frameNumber / 100;
    if( key >= keyFrames_.size() )
        key = keyFrames_.size() - 1;
    const KeyFrame& keyFrame = keyFrames_[ key ];

    Load( keys_, keyFrame.offset_, keyFrame.size_, handler );

    return keyFrame.frameNumber_;
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::Synchronize
// Created: AGE 2007-07-13
// -----------------------------------------------------------------------------
void MessageLoader::Synchronize()
{
//    {
//        tools::thread::BarrierCommand barrier;
//        disk_.Enqueue( barrier.command() );
//        barrier.wait();
//    }
//
//    {
//        tools::thread::BarrierCommand barrier;
//        cpu_.Enqueue( barrier.command() );
//        barrier.wait();
//    }
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
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::Load
// Created: AGE 2007-07-13
// -----------------------------------------------------------------------------
void MessageLoader::Load( std::ifstream& in, unsigned from, unsigned size, MessageHandler_ABC& handler )
{
    in.seekg( from );
    boost::shared_ptr< Buffer > buffer( new Buffer( size, in ) );
    LoadBuffer( buffer, handler );
//    cpu_.Enqueue( boost::bind( &MessageLoader::LoadBuffer, this, buffer, boost::ref( handler ) ) );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadBuffer
// Created: AGE 2007-07-13
// -----------------------------------------------------------------------------
void MessageLoader::LoadBuffer( const boost::shared_ptr< Buffer >& buffer, MessageHandler_ABC& handler )
{
    unsigned char* current = buffer->data_;
    while( current < buffer->data_ + buffer->size_ )
        LoadSimToClientMessage( current, handler );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadSimToClientMessage
// Created: AGE 2007-07-13
// -----------------------------------------------------------------------------
void MessageLoader::LoadSimToClientMessage( const unsigned char*& input, MessageHandler_ABC& handler )
{
    unsigned messageSize = *reinterpret_cast< const unsigned* >( input );
    input += sizeof( unsigned );

    ASN1T_MsgsSimToClient message;
    ASN1PERDecodeBuffer decoder( input, messageSize, TRUE );
    ASN1C_MsgsSimToClient ctrl( decoder, message );
    if( ctrl.Decode() != ASN_OK )
    {
        decoder.PrintErrorInfo();
        throw std::runtime_error( "ASN fuss�" );
    }
    handler.Receive( message );

    input += messageSize;
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadIndex
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
void MessageLoader::LoadIndex( const std::string& file )
{
    frames_.reserve( 100 );
    tools::InputBinaryStream input( file );
    while( ! input.EndOfFile() )
    {
        frames_.push_back( Frame() );
        input >> frames_.back();
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadKeyIndex
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
void MessageLoader::LoadKeyIndex( const std::string& file )
{
    keyFrames_.reserve( 100 );
    tools::InputBinaryStream input( file );
    while( ! input.EndOfFile() )
    {
        keyFrames_.push_back( KeyFrame() );
        input >> keyFrames_.back();
    }
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::GetTickNumber
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
unsigned int MessageLoader::GetTickNumber() const
{
    return frames_.size();
}
