// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "client_pch.h"
#include "Download.h"
#include "moc_Download.cpp"

#include "Helpers.h"

#include "runtime/Async.h"
#include "runtime/FileSystem_ABC.h"

#include <boost/make_shared.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

using namespace gui;
using runtime::FileSystem_ABC;

namespace
{
    const size_t buffer_size = 1<<20;
}

namespace
{
struct Download : public gui::Download_ABC, public io::Writer_ABC
{
    // -----------------------------------------------------------------------------
    // Name: Download::Download
    // Created: BAX 2012-09-21
    // -----------------------------------------------------------------------------
    Download( size_t id, QNetworkReply* rpy,
              const FileSystem_ABC& fs,
              runtime::Pool_ABC& pool,
              const Path& root )
        : fs_      ( fs )
        , buffer_  ( buffer_size )
        , id_      ( id )
        , root_    ( fs.MakeAnyPath( root ) )
        , rpy_     ( rpy )
        , write_   ( 0 )
        , read_    ( 0 )
        , current_ ( 0 )
        , total_   ( 0 )
        , progress_( 0 )
        , eof_     ( false )
        , headers_ ( false )
        , finished_( false )
        , async_   ( pool )
    {
        rpy->setReadBufferSize( buffer_size );
        connect( this, SIGNAL( ReadyWrite() ), rpy, SIGNAL( readyRead() ) );
        connect( this, SIGNAL( Abort() ), this, SLOT( OnAbort() ) );
        connect( rpy, SIGNAL( readyRead() ), this, SLOT( OnReadyRead() ) );
        connect( rpy, SIGNAL( error( QNetworkReply::NetworkError ) ), this, SLOT( OnFinished() ) );
        connect( rpy, SIGNAL( finished() ), this, SLOT( OnFinished() ) );
        async_.Post( [=]{
            Unpack();
        } );
    }

    // -----------------------------------------------------------------------------
    // Name: Download::~Download
    // Created: BAX 2012-09-21
    // -----------------------------------------------------------------------------
    ~Download()
    {
        Close();
        async_.Join();
        rpy_->disconnect( this );
        rpy_->abort();
        rpy_->deleteLater();
    }

    // -----------------------------------------------------------------------------
    // Name: Download::ReadHeaders
    // Created: BAX 2012-09-21
    // -----------------------------------------------------------------------------
    void ReadHeaders( QNetworkReply* rpy )
    {
        if( headers_ )
            return;
        headers_ = true;
        bool valid = false;
        const int total = rpy->rawHeader( QString( "Original-Content-Length" ).toAscii() ).toInt( &valid );
        if( valid )
            total_ = total;
    }

    // -----------------------------------------------------------------------------
    // Name: Download::OnReadyRead
    // Created: BAX 2012-09-21
    // -----------------------------------------------------------------------------
    void OnReadyRead()
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        ReadHeaders( rpy_ );
        const qint64 left = buffer_size - write_;
        const size_t next = std::min( left, rpy_->bytesAvailable() );
        const qint64 len  = next ? rpy_->read( &buffer_[write_], next ) : 0;
        write_           += len;
        gate_.notify_one();
    }

    // -----------------------------------------------------------------------------
    // Name: Download::Read
    // Created: BAX 2012-09-21
    // -----------------------------------------------------------------------------
    int Read( void* data, size_t size )
    {
        boost::unique_lock< boost::mutex > lock( access_ );
        while( !eof_ && read_ == write_ )
            gate_.wait( lock );
        if( eof_ && read_ == write_ )
            return 0;
        const size_t next = std::min( write_ - read_, size );
        memcpy( data, &buffer_[read_], next );
        read_ += next;
        Rewind();
        return static_cast< int >( next );
    }

    // -----------------------------------------------------------------------------
    // Name: Download::Rewind
    // Created: BAX 2012-09-24
    // -----------------------------------------------------------------------------
    void Rewind()
    {
        if( read_ != write_ )
            return;
        read_  = 0;
        write_ = 0;
        emit ReadyWrite();
    }

    // -----------------------------------------------------------------------------
    // Name: Download::Write
    // Created: BAX 2012-09-24
    // -----------------------------------------------------------------------------
    int Write( const void* /*data*/, size_t size )
    {
        const int rpy = static_cast< int >( size );
        boost::lock_guard< boost::mutex > lock( access_ );
        current_ += size;
        if( !total_ )
            return rpy;
        const qint64 next = ( current_ * 100 ) / total_;
        if( next <= progress_ )
            return rpy;
        progress_ = next;
        emit Progress( id_, current_, static_cast< int >( next ) );
        return rpy;
    }

    // -----------------------------------------------------------------------------
    // Name: Download::Close
    // Created: BAX 2012-09-21
    // -----------------------------------------------------------------------------
    void Close()
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        eof_ = true;
        gate_.notify_one();
    }

    // -----------------------------------------------------------------------------
    // Name: Download::Unpack
    // Created: BAX 2012-09-24
    // -----------------------------------------------------------------------------
    void Unpack()
    {
        try
        {
            FileSystem_ABC::T_Unpacker unpacker = fs_.Unpack( root_, *this, this );
            unpacker->Unpack();
        }
        catch( const std::exception& err )
        {
            emit Error( id_, QUtf8( std::string( err.what() ) ) );
        }
        Close();
        emit Abort();
    }

    // -----------------------------------------------------------------------------
    // Name: Download::OnAbort
    // Created: BAX 2012-09-24
    // -----------------------------------------------------------------------------
    void OnAbort()
    {
        if( rpy_.isNull() )
            return;
        const qint64 len = rpy_->bytesAvailable();
        if( len > 0 )
            rpy_->abort();
    }

    // -----------------------------------------------------------------------------
    // Name: Download::IsFinished
    // Created: BAX 2012-09-24
    // -----------------------------------------------------------------------------
    bool IsFinished() const
    {
        if( rpy_->bytesAvailable() <= 0 )
            return true;
        boost::lock_guard< boost::mutex > lock( access_ );
        return eof_;
    }

    // -----------------------------------------------------------------------------
    // Name: Download::Finish
    // Created: BAX 2012-09-24
    // -----------------------------------------------------------------------------
    void Finish()
    {
        while( !IsFinished() )
        {
            OnReadyRead();
            QCoreApplication::processEvents();
        }
        Close();
    }

    // -----------------------------------------------------------------------------
    // Name: Download::OnFinished
    // Created: BAX 2012-09-24
    // -----------------------------------------------------------------------------
    void OnFinished()
    {
        if( finished_ )
            return;
        finished_ = true;
        QNetworkReply::NetworkError err = rpy_->error();
        if( err != QNetworkReply::NoError && err != QNetworkReply::OperationCanceledError )
            emit Error( id_, rpy_->errorString() );
        Finish();
        async_.Join();
        emit Progress( id_, current_, 100 );
        emit End( id_, fs_.IsFile( root_ / "signature" ) );
    }

private:
    const FileSystem_ABC& fs_;
    const size_t id_;
    const Path root_;
    mutable boost::mutex access_;
    boost::condition_variable gate_;
    std::vector< char > buffer_;
    QPointer< QNetworkReply > rpy_;
    size_t write_;
    size_t read_;
    qint64 current_;
    size_t total_;
    qint64 progress_;
    bool eof_;
    bool headers_;
    bool finished_;
    runtime::Async async_;
};
}

// -----------------------------------------------------------------------------
// Name: gui::MakeDownload
// Created: BAX 2012-09-24
// -----------------------------------------------------------------------------
boost::shared_ptr< gui::Download_ABC > gui::MakeDownload( size_t id, QNetworkReply* rpy,
                                                          const runtime::FileSystem_ABC& fs,
                                                          runtime::Pool_ABC& pool,
                                                          const Path& root )
{
    return boost::make_shared< Download >( id, rpy, fs, pool, root );
}
