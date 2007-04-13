// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Loader.h"
#include "SimulationDispatcher.h"
#include "Publisher_ABC.h"
#include "pathfind/InputBinaryStream.h"
#include "boost/filesystem/operations.hpp"
#include "tools/AsnMessageDecoder.h"

namespace bfs = boost::filesystem;

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Loader constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Loader::Loader( SimulationDispatcher& simulation, const std::string& directory )
    : simulation_( simulation )
    , currentFrame_( 0 )
{
    const bfs::path dir( directory, bfs::native );
    LoadIndex   ( ( dir / "index" ).string() );
    LoadKeyIndex( ( dir / "keyindex" ).string() );
    updates_  .open( ( dir / "update" ).string().c_str(), std::ios_base::binary | std::ios_base::in );
    keys_     .open( ( dir / "key"    ).string().c_str(), std::ios_base::binary | std::ios_base::in );

    SkipToFrame( 0 );
}

// -----------------------------------------------------------------------------
// Name: Loader destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Loader::~Loader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Loader::LoadIndex
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void Loader::LoadIndex( const std::string& file )
{
    frames_.reserve( 100 );
    InputBinaryStream input( file );
    while( ! input.EndOfFile() )
    {
        frames_.push_back( Frame() );
        input >> frames_.back();
    }
}

// -----------------------------------------------------------------------------
// Name: Loader::LoadKeyIndex
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void Loader::LoadKeyIndex( const std::string& file )
{
    keyFrames_.reserve( 100 );
    InputBinaryStream input( file );
    while( ! input.EndOfFile() )
    {
        keyFrames_.push_back( KeyFrame() );
        input >> keyFrames_.back();
    }
}

// -----------------------------------------------------------------------------
// Name: Loader::SkipToFrame
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void Loader::SkipToFrame( unsigned frame )
{
    if( keyFrames_.empty() )
        return;

    // $$$$ AGE 2007-04-11: 
    unsigned key = frame / 100;
    if( key >= keyFrames_.size() )
        key = keyFrames_.size() - 1;
    const KeyFrame& keyFrame = keyFrames_[ key ];
    keys_.seekg( keyFrame.offset_ );

    simulation_.StartSynchronisation();

    InputBinaryWrapper input( keys_ );
    while( keys_.tellg() < keyFrame.offset_ + keyFrame.size_ )
        LoadInClientMessage( input ); 

    currentFrame_ = keyFrame.frameNumber_;
    while( currentFrame_+1 < frame && Tick() )
        ;

    simulation_.EndSynchronisation();

    if( currentFrame_ < frame )
        Tick();
}

// -----------------------------------------------------------------------------
// Name: Loader::Tick
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
bool Loader::Tick()
{
    if( currentFrame_ >= frames_.size() )
        return false;

    const Frame& current = frames_[ currentFrame_++ ];
    if( current.count_ )
    {
        updates_.seekg( current.offset_ );
        LoadOutSimMessage( updates_, current.count_ );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: Loader::LoadOutSimMessage
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void Loader::LoadOutSimMessage( std::ifstream& inputStream, unsigned count )
{
    InputBinaryWrapper input( inputStream );
    while( count-- )
        LoadOutSimMessage( input );
}

// -----------------------------------------------------------------------------
// Name: Loader::LoadOutSimMessage
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void Loader::LoadOutSimMessage( InputBinaryWrapper& input )
{
    unsigned size;
    input >> size;
    input.Read( (char*)buffer_, size );

    ASN1T_MsgsOutSim message;
    ASN1PERDecodeBuffer decoder( buffer_, sizeof( buffer_ ), TRUE );
    ASN1C_MsgsOutSim ctrl( decoder, message );
    if( ctrl.Decode() != ASN_OK )
    {
        decoder.PrintErrorInfo();
        throw std::runtime_error( "ASN fussé" );
    }
    simulation_.OnReceive( message );
}

// -----------------------------------------------------------------------------
// Name: Loader::LoadInClientMessage
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void Loader::LoadInClientMessage( InputBinaryWrapper& input )
{
    unsigned size;
    input >> size;
    input.Read( (char*)buffer_, size );

    ASN1T_MsgsInClient message;
    ASN1PERDecodeBuffer decoder( buffer_, sizeof( buffer_ ), TRUE );
    ASN1C_MsgsInClient ctrl( decoder, message );
    if( ctrl.Decode() != ASN_OK )
    {
        decoder.PrintErrorInfo();
        throw std::runtime_error( "ASN fussé" );
    }

    simulation_.OnReceive( message );
}

// -----------------------------------------------------------------------------
// Name: Loader::GetCurrentTick
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
unsigned Loader::GetCurrentTick() const
{
    return currentFrame_;
}

// -----------------------------------------------------------------------------
// Name: Loader::GetTickNumber
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
unsigned Loader::GetTickNumber() const
{
    return frames_.size();
}
