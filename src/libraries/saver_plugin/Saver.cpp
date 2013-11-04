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
#include "MT_Tools/MT_Logger.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ReplaySenders.h"
#include "tools/InputBinaryWrapper.h"
#include <tools/Exception.h>
#include "tools/OutputBinaryWrapper.h"
#include <boost/algorithm/string.hpp>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )
#include <boost/lexical_cast.hpp>

using namespace plugins::saver;

const tools::Path Saver::currentFolderName_( "current" );

// -----------------------------------------------------------------------------
// Name: Saver constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Saver::Saver( dispatcher::ClientPublisher_ABC& client, const dispatcher::Config& config )
    : client_            ( client )
    , recorderDirectory_ ( config.GetRecordDirectory() )
    , frameCount_        ( 0 )
    , fragmentFirstFrame_( 1 )
    , currentFolder_     ( 0 )
    , hasCheckpoint_     ( config.HasCheckpoint() )
{
    MT_LOG_INFO_MSG( "Recorder enabled - data stored in " << recorderDirectory_.ToUTF8() );
}

// -----------------------------------------------------------------------------
// Name: Saver destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Saver::~Saver()
{
    Flush();
    index_.close();
    keyIndex_.close();
    key_.close();
    update_.close();
    CopyFromCurrentToFolder();
}

namespace
{
    tools::Path CreateFolderName( unsigned int frame )
    {
        std::string foldername;
        std::string number = boost::lexical_cast< std::string >( frame );
        foldername.assign( 8 - number.size(), '0' );
        foldername.append( number );
        return foldername.c_str();
    }
}

namespace
{
    void DeleteFilesRecursive( const tools::Path& path )
    {
        if( !path.Exists() )
            return;

        for( auto it = path.begin(); it != path.end(); ++it )
            if( it->IsDirectory() )
                DeleteFilesRecursive( *it );
            else
                try
                {
                    it->Remove();
                }
                catch( ... )
                {
                    // NOTHING
                }
    }
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
        if( recorderDirectory_.Exists() )
            try
            {
                recorderDirectory_.RemoveAll();
            }
            catch( ... )
            {
                DeleteFilesRecursive( recorderDirectory_ );
            }
    }
    CreateNewFragment( true );
}

// -----------------------------------------------------------------------------
// Name: Saver::CreateNewFragment
// Created: JSR 2010-10-26
// -----------------------------------------------------------------------------
void Saver::CreateNewFragment( bool first /*= false*/ )
{
    const tools::Path currentDirectory = recorderDirectory_ / currentFolderName_;
    bool exists = false;
    try
    {
        exists = currentDirectory.Exists();
    }
    catch( const std::exception& )
    {
        exists = false;
    }
    if( !exists )
        currentDirectory.CreateDirectories();
    if( !first )
        TerminateFragment();
    index_   .open( currentDirectory / "index"   , std::ios_base::binary | std::ios_base::out );
    keyIndex_.open( currentDirectory / "keyindex", std::ios_base::binary | std::ios_base::out );
    key_     .open( currentDirectory / "key"     , std::ios_base::binary | std::ios_base::out );
    update_  .open( currentDirectory / "update"  , std::ios_base::binary | std::ios_base::out );
    current_.Reset( static_cast< unsigned >( update_.tellp() ) );
    tools::Ofstream info;
    info.open( currentDirectory / "info", std::ios_base::binary | std::ios_base::out | std::ios_base::trunc );
    info.close();
}

// -----------------------------------------------------------------------------
// Name: Saver::StartFrame
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void Saver::StartFrame( const Savable_ABC& message, const sword::SimToClient& msg )
{
    SaveUpdateMessage( message );
    if( msg.message().has_control_begin_tick() )
    {
        const sword::ControlBeginTick& beginTickMsg = msg.message().control_begin_tick();
        simTime_ = beginTickMsg.date_time().data();
        if( beginTickMsg.has_real_date_time() )
            realTime_ = beginTickMsg.real_date_time().data();
    }
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
    current_.Reset( static_cast< unsigned >( update_.tellp() ) );
}

// -----------------------------------------------------------------------------
// Name: Saver::SaveKeyFrame
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void Saver::SaveKeyFrame( const Savable_ABC& message )
{
    try
    {
        dispatcher::KeyFrame frame;
        frame.offset_ = static_cast< unsigned >( key_.tellp() );
        frame.frameNumber_ = frameCount_;
        {
            tools::OutputBinaryWrapper wrapper( key_ );
            message.Serialize( wrapper );
            frame.size_ = static_cast< unsigned >( key_.tellp() ) - frame.offset_;
        }
        {
            tools::OutputBinaryWrapper wrapper( keyIndex_ );
            wrapper << frame.frameNumber_;
            wrapper << frame.offset_;
            wrapper << frame.size_;
        }
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Saver plugin : " << tools::GetExceptionMsg( e ) << " : " << ( recorderDirectory_ / currentFolderName_ / "key" ) );
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
        current_.size_ = static_cast< unsigned >( update_.tellp() ) - current_.offset_;
        tools::OutputBinaryWrapper wrapper( index_ );
        wrapper << current_.offset_;
        wrapper << current_.size_;
        current_.Reset();
        key_.flush();
        update_.flush();
        keyIndex_.flush();
        index_.flush();
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Saver plugin : " << tools::GetExceptionMsg( e ) << " : " << ( recorderDirectory_ / currentFolderName_ / "index" ) );
    }
    GenerateInfoFile();
}

// -----------------------------------------------------------------------------
// Name: Saver::CopyFromCurrentToFolder
// Created: JSR 2011-03-30
// -----------------------------------------------------------------------------
void Saver::CopyFromCurrentToFolder()
{
    const tools::Path currentDirectory = recorderDirectory_ / currentFolderName_;
    try
    {
        if( !currentDirectory.Exists() )
            return;
        const tools::Path newDirectory = recorderDirectory_ / CreateFolderName( currentFolder_++ );
        newDirectory.CreateDirectories();

        for( auto it = currentDirectory.begin(); it != currentDirectory.end(); ++it )
        {
            std::wstring destPath = it->ToUnicode();
            boost::algorithm::replace_first( destPath, currentDirectory.ToUnicode(), newDirectory.ToUnicode() );
            it->Copy( tools::Path::FromUnicode( destPath ) );
            it->Remove();
        }
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Saver plugin : " << tools::GetExceptionMsg( e ) << " : " << ( currentDirectory / "index" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Saver::TerminateFragment
// Created: JSR 2010-10-27
// -----------------------------------------------------------------------------
void Saver::TerminateFragment()
{
    index_.close();
    keyIndex_.close();
    key_.close();
    update_.close();
    CopyFromCurrentToFolder();
    fragmentFirstFrame_ = frameCount_;
    replay::NewDataChunkNotification msg;
    msg().set_last_tick( frameCount_ - 1 );
    msg.Send( client_ );
}

// -----------------------------------------------------------------------------
// Name: Saver::GenerateInfoFile
// Created: JSR 2010-10-27
// -----------------------------------------------------------------------------
void Saver::GenerateInfoFile() const
{
    const tools::Path currentDirectory = recorderDirectory_ / currentFolderName_;
    try
    {
        tools::Ofstream info;
        info.open( currentDirectory / "info", std::ios_base::binary | std::ios_base::in | std::ios_base::ate );
        tools::OutputBinaryWrapper wrapper( info );
        info.seekp( 0, std::ios_base::beg );
        wrapper << fragmentFirstFrame_;
        wrapper << std::max( fragmentFirstFrame_, frameCount_ - 1 );
        info.seekp( 0, std::ios_base::end );
        wrapper << simTime_;
        wrapper << realTime_;
        info.close();
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Saver plugin : " << tools::GetExceptionMsg( e ) << " : " << ( currentDirectory / "info" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Saver::UpdateFragments
// Created: JSR 2010-11-16
// -----------------------------------------------------------------------------
void Saver::UpdateFragments()
{
    if( !recorderDirectory_.Exists() )
        return;
    currentFolder_ = 0;
    for( auto it = recorderDirectory_.begin(); it != recorderDirectory_.end(); ++it )
        if( it->IsDirectory() )
        {
            const tools::Path infoFile = *it / "info";
            if( infoFile.Exists() )
            {
                try
                {
                    tools::Ifstream stream( infoFile, std::ios_base::binary | std::ios_base::in );
                    tools::InputBinaryWrapper wrapper( stream );
                    unsigned int start;
                    unsigned int end;
                    wrapper >> start;
                    wrapper >> end;
                    stream.close();
                    if( start >= frameCount_ )
                        try
                        {
                            it->RemoveAll();
                        }
                        catch( ... )
                        {
                            DeleteFilesRecursive( *it );
                        }
                    else if( frameCount_ > start && frameCount_ <= end )
                    {
                        std::vector< std::string > timetable;
                        {
                            tools::Ifstream stream( infoFile, std::ios_base::binary | std::ios_base::in );
                            tools::InputBinaryWrapper wrapper( stream );
                            unsigned int dummy;
                            wrapper >> dummy; // start
                            wrapper >> dummy; // end
                            std::string tmp;
                            for( unsigned int i = start; i < frameCount_; ++i )
                            {
                                wrapper >> tmp; // sim time
                                timetable.push_back( tmp );
                                wrapper >> tmp; // real time
                                timetable.push_back( tmp );
                            }
                            stream.close();
                        }
                        {
                            tools::Ofstream stream;
                            stream.open( infoFile, std::ios_base::binary | std::ios_base::out | std::ios_base::trunc );
                            tools::OutputBinaryWrapper wrapper( stream );
                            wrapper << start;
                            wrapper << frameCount_ - 1;
                            for( std::vector< std::string >::const_iterator it = timetable.begin(); it != timetable.end(); ++it )
                                wrapper << *it;
                            stream.close();
                        }
                        if( it->FileName() == Saver::currentFolderName_ )
                            CopyFromCurrentToFolder();
                        else
                            currentFolder_ = boost::lexical_cast< unsigned int >( it->FileName() ) + 1;
                    }
                    else
                        currentFolder_ = std::max( currentFolder_, boost::lexical_cast< unsigned int >( it->FileName().ToUnicode() ) + 1 );
                }
                catch( const std::exception & )
                {
                    // NOTHING
                }
            }
        }
}
