// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Saver.h"
#include "Savable_ABC.h"
#include "pathfind/OutputBinaryWrapper.h"
#include "boost/filesystem/operations.hpp"
namespace bfs = boost::filesystem;

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Saver constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Saver::Saver( const std::string& directory )
    : frameCount_( 0 )
{
    const bfs::path dir( directory, bfs::native );
    bfs::create_directory( dir );

    index_   .open( ( dir / "index"    ).string().c_str(), std::ios_base::binary | std::ios_base::out );
    keyIndex_.open( ( dir / "keyindex" ).string().c_str(), std::ios_base::binary | std::ios_base::out );
    key_     .open( ( dir / "key"      ).string().c_str(), std::ios_base::binary | std::ios_base::out );
    update_  .open( ( dir / "update"   ).string().c_str(), std::ios_base::binary | std::ios_base::out );
}

// -----------------------------------------------------------------------------
// Name: Saver destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Saver::~Saver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Saver::StartFrame
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void Saver::StartFrame( const Savable_ABC& message )
{
    current_.Reset( update_.tellp() );
    SaveUpdateMessage( message );
}

// -----------------------------------------------------------------------------
// Name: Saver::EndFrame
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void Saver::EndFrame( const Savable_ABC& message )
{
    ++frameCount_;
    SaveUpdateMessage( message );
    Flush();
}

// -----------------------------------------------------------------------------
// Name: Saver::SaveKeyFrame
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void Saver::SaveKeyFrame( const Savable_ABC& message )
{
    KeyFrame frame;
    frame.offset_ = key_.tellp();
    frame.frameNumber_ = frameCount_;
    {
        OutputBinaryWrapper wrapper( key_ );
        message.Serialize( wrapper );
        frame.size_ = long( key_.tellp() ) - frame.offset_;
    }
    {
        OutputBinaryWrapper wrapper( keyIndex_ );
        wrapper << frame;
    }
}

// -----------------------------------------------------------------------------
// Name: Saver::Flush
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void Saver::Flush()
{
    OutputBinaryWrapper wrapper( index_ );
    wrapper << current_;
    current_.Reset();
}

// -----------------------------------------------------------------------------
// Name: Saver::SaveUpdateMessage
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void Saver::SaveUpdateMessage( const Savable_ABC& message )
{
    ++current_.count_;
    OutputBinaryWrapper wrapper( update_ );
    message.Serialize( wrapper );
}
