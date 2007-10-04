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
#include "Config.h"
#include "PluginConfig.h"
#include "tools/OutputBinaryWrapper.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#pragma warning( push )
#pragma warning( disable: 4127 )
#include <boost/filesystem/convenience.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;
namespace bpt = boost::posix_time;

using namespace dispatcher;

namespace
{
    std::string GenerateOutputDirectory( const Config& config )
    {
        const std::string datetime = bpt::to_iso_string( bpt::second_clock::local_time() );
        return config.BuildRecordsDirectory( datetime );
    }
}

// -----------------------------------------------------------------------------
// Name: Saver constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Saver::Saver( const Config& config )
    : frameCount_( 0 )
{
    const bfs::path recorderDirectory( GenerateOutputDirectory( config ), bfs::native );

    MT_LOG_INFO_MSG( "Recorder enabled - data stored in " << recorderDirectory.native_file_string() );

    bfs::create_directories( recorderDirectory );

    index_   .open( ( recorderDirectory / "index"    ).string().c_str(), std::ios_base::binary | std::ios_base::out );
    keyIndex_.open( ( recorderDirectory / "keyindex" ).string().c_str(), std::ios_base::binary | std::ios_base::out );
    key_     .open( ( recorderDirectory / "key"      ).string().c_str(), std::ios_base::binary | std::ios_base::out );
    update_  .open( ( recorderDirectory / "update"   ).string().c_str(), std::ios_base::binary | std::ios_base::out );
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
    if( ! frameCount_ )
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
    current_.Reset( update_.tellp() );
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
        tools::OutputBinaryWrapper wrapper( key_ );
        message.Serialize( wrapper );
        frame.size_ = long( key_.tellp() ) - frame.offset_;
    }
    {
        tools::OutputBinaryWrapper wrapper( keyIndex_ );
        wrapper << frame;
    }
}

// -----------------------------------------------------------------------------
// Name: Saver::Flush
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void Saver::Flush()
{
    current_.size_ = long( update_.tellp() ) - current_.offset_;
    tools::OutputBinaryWrapper wrapper( index_ );
    wrapper << current_;
    current_.Reset();
}

// -----------------------------------------------------------------------------
// Name: Saver::SaveUpdateMessage
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void Saver::SaveUpdateMessage( const Savable_ABC& message )
{
    tools::OutputBinaryWrapper wrapper( update_ );
    message.Serialize( wrapper );
}
