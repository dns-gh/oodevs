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
#include "tools/AsnMessageDecoder.h"
#include "boost/filesystem/operations.hpp"
namespace bfs = boost::filesystem;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MessageLoader constructor
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
MessageLoader::MessageLoader( const Config& config, const std::string& records )
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
    if( current.count_ )
    {
        updates_.seekg( current.offset_ );
        LoadSimToClientMessage( updates_, current.count_, handler );
    }
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
    keys_.seekg( keyFrame.offset_ );

    tools::InputBinaryWrapper input( keys_ );
    while( keys_.tellg() < keyFrame.offset_ + keyFrame.size_ )
        LoadSimToClientMessage( input, handler ); 

    return keyFrame.frameNumber_;
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
// Name: MessageLoader::LoadSimToClientMessage
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
void MessageLoader::LoadSimToClientMessage( std::ifstream& inputStream, unsigned count, MessageHandler_ABC& handler )
{
    tools::InputBinaryWrapper input( inputStream );
    while( count-- )
        LoadSimToClientMessage( input, handler );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::LoadSimToClientMessage
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
void MessageLoader::LoadSimToClientMessage( tools::InputBinaryWrapper& input, MessageHandler_ABC& handler )
{
    unsigned size;
    input >> size;
    input.Read( (char*)buffer_, size );

    ASN1T_MsgsSimToClient message;
    ASN1PERDecodeBuffer decoder( buffer_, sizeof( buffer_ ), TRUE );
    ASN1C_MsgsSimToClient ctrl( decoder, message );
    if( ctrl.Decode() != ASN_OK )
    {
        decoder.PrintErrorInfo();
        throw std::runtime_error( "ASN fussé" );
    }
    handler.Receive( message );
}

// -----------------------------------------------------------------------------
// Name: MessageLoader::GetTickNumber
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
unsigned int MessageLoader::GetTickNumber() const
{
    return frames_.size();
}
