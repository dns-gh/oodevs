// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Saver.h"
#include "Savable_ABC.h"
#include "dispatcher/Config.h"
#include "tools/OutputBinaryWrapper.h"
#include "tools/InputBinaryWrapper.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/Protocol.h"
#pragma warning( push )
#pragma warning( disable: 4127 )
#include <boost/filesystem/convenience.hpp>
#pragma warning( pop )
#include <boost/lexical_cast.hpp>

namespace bfs = boost::filesystem;

using namespace plugins::saver;

// -----------------------------------------------------------------------------
// Name: Saver constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Saver::Saver( const dispatcher::Config& config )
    : recorderDirectory_ ( config.GetRecordDirectory() )
    , frameCount_        ( 0 )
    , fragmentFirstFrame_( 0 )
    , currentFolder_     ( 0 )
    , hasCheckpoint_     ( config.HasCheckpoint() )
{
    MT_LOG_INFO_MSG( "Recorder enabled - data stored in " << bfs::path( recorderDirectory_, bfs::native ).native_file_string() );
}

// -----------------------------------------------------------------------------
// Name: Saver destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Saver::~Saver()
{
    Flush();
}

// -----------------------------------------------------------------------------
// Name: Saver::ControlInformation
// Created: JSR 2010-11-03
// -----------------------------------------------------------------------------
void Saver::ControlInformation( const sword::ControlInformation& controlInformation )
{
    frameCount_ = controlInformation.current_tick() - 1;
    fragmentFirstFrame_ = frameCount_;
    if( hasCheckpoint_ )
        UpdateFragments();
    else
    {
        const bfs::path recorderDirectory( recorderDirectory_, bfs::native );
        if( bfs::exists( recorderDirectory ) )
            bfs::remove_all( recorderDirectory );
    }
    CreateNewFragment( true );
}

// -----------------------------------------------------------------------------
// Name: Saver::CreateNewFragment
// Created: JSR 2010-10-26
// -----------------------------------------------------------------------------
void Saver::CreateNewFragment( bool first /*= false*/ )
{
    if( !first )
        TerminateFragment();
    std::string number = boost::lexical_cast< std::string >( currentFolder_++ );
    currentFolderName_.assign( 8 - number.size(), '0' );
    currentFolderName_.append( number );
    const bfs::path currentDirectory = bfs::path( recorderDirectory_, bfs::native ) / currentFolderName_;
    bfs::create_directories( currentDirectory );
    index_   .open( ( currentDirectory / "index"    ).string().c_str(), std::ios_base::binary | std::ios_base::out );
    keyIndex_.open( ( currentDirectory / "keyindex" ).string().c_str(), std::ios_base::binary | std::ios_base::out );
    key_     .open( ( currentDirectory / "key"      ).string().c_str(), std::ios_base::binary | std::ios_base::out );
    update_  .open( ( currentDirectory / "update"   ).string().c_str(), std::ios_base::binary | std::ios_base::out );
    current_.Reset( update_.tellp() );
}

// -----------------------------------------------------------------------------
// Name: Saver::StartFrame
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void Saver::StartFrame( const Savable_ABC& message )
{
    SaveUpdateMessage( message );
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
    dispatcher::KeyFrame frame;
    frame.offset_ = key_.tellp();
    frame.frameNumber_ = frameCount_;
    {
        tools::OutputBinaryWrapper wrapper( key_ );
        message.Serialize( wrapper );
        frame.size_ = static_cast< long >( key_.tellp() ) - frame.offset_;
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
    try
    {
        current_.size_ = static_cast< long >( update_.tellp() ) - current_.offset_;
        tools::OutputBinaryWrapper wrapper( index_ );
        wrapper << current_;
        current_.Reset();
        index_.flush(); keyIndex_.flush(); key_.flush(); update_.flush();
        GenerateInfoFile();
    }
    catch( std::exception& exception )
    {
      MT_LOG_ERROR_MSG( exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: Saver::TerminateFragment
// Created: JSR 2010-10-27
// -----------------------------------------------------------------------------
void Saver::TerminateFragment()
{
    const bfs::path currentDirectory = bfs::path( recorderDirectory_, bfs::native ) / currentFolderName_;
    index_.close();
    keyIndex_.close();
    key_.close();
    update_.close();
    fragmentFirstFrame_ = frameCount_;
}

// -----------------------------------------------------------------------------
// Name: Saver::GenerateInfoFile
// Created: JSR 2010-10-27
// -----------------------------------------------------------------------------
void Saver::GenerateInfoFile() const
{
    const bfs::path currentDirectory = bfs::path( recorderDirectory_, bfs::native ) / currentFolderName_;
    std::ofstream info;
    info.open( ( currentDirectory / "info" ).string().c_str(), std::ios_base::binary | std::ios_base::out | std::ios_base::trunc );
    tools::OutputBinaryWrapper wrapper( info );
    wrapper << fragmentFirstFrame_;
    wrapper << frameCount_ - 1;
    info.close();
}

// -----------------------------------------------------------------------------
// Name: Saver::UpdateFragments
// Created: JSR 2010-11-16
// -----------------------------------------------------------------------------
void Saver::UpdateFragments()
{
    const bfs::path recorderDirectory( recorderDirectory_, bfs::native );
    if( !bfs::exists( recorderDirectory ) )
        return;
    currentFolder_ = 0;
    for( bfs::directory_iterator it( recorderDirectory ); it !=  bfs::directory_iterator(); ++it )
        if( bfs::is_directory( it->status() ) )
        {
            const bfs::path infoFile = it->path() / "info";
            if( bfs::exists( infoFile ) )
            {
                try
                {
                    std::ifstream stream;
                    stream.open( infoFile.string().c_str(), std::ios_base::binary | std::ios_base::in );
                    tools::InputBinaryWrapper wrapper( stream );
                    unsigned int start;
                    unsigned int end;
                    wrapper >> start;
                    wrapper >> end;
                    stream.close();
                    if( start >= frameCount_ )
                        bfs::remove_all( it->path() );
                    else if( frameCount_ > start && frameCount_ <= end )
                    {
                        std::ofstream stream;
                        stream.open( infoFile.string().c_str(), std::ios_base::binary | std::ios_base::out | std::ios_base::trunc );
                        tools::OutputBinaryWrapper wrapper( stream );
                        wrapper << start;
                        wrapper << frameCount_ - 1;
                        stream.close();
                        currentFolder_ = boost::lexical_cast< unsigned int >( it->path().leaf() ) + 1;
                    }
                    else
                        currentFolder_ = std::max( currentFolder_, boost::lexical_cast< unsigned int >( it->path().leaf() ) + 1 );
                }
                catch( const std::exception & )
                {
                    // NOTHING
                }
            }
        }
}
