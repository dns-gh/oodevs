// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Download.h"

#include "QAsync.h"

#include "runtime/FileSystem_ABC.h"

#include <boost/make_shared.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

#include <QCoreApplication>
#include <QPointer>
#include <QtConcurrentRun>

using namespace gui;
using runtime::FileSystem_ABC;

namespace
{
    const size_t buffer_size = 1<<20;
}

namespace
{
struct Download : public gui::Download_ABC
{
    // -----------------------------------------------------------------------------
    // Name: Download::Download
    // Created: BAX 2012-09-21
    // -----------------------------------------------------------------------------
    Download( QNetworkReply* rpy, const FileSystem_ABC& fs, const Path& root )
        : fs_    ( fs )
        , buffer_( buffer_size )
        , root_  ( fs.MakeAnyPath( root ) )
        , eof_   ( false )
        , write_ ( 0 )
        , read_  ( 0 )
        , total_ ( 0 )
        , rpy_   ( rpy )
        , async_ ()
    {
        rpy->setReadBufferSize( buffer_size );
        connect( this, SIGNAL( ReadyWrite() ), rpy, SIGNAL( readyRead() ) );
        connect( this, SIGNAL( Abort() ), this, SLOT( OnAbort() ) );
        connect( rpy, SIGNAL( readyRead() ), this, SLOT( OnReadyRead() ) );
        connect( rpy, SIGNAL( error( QNetworkReply::NetworkError ) ), this, SLOT( OnFinished() ) );
        connect( rpy, SIGNAL( finished() ), this, SLOT( OnFinished() ) );
        async_.Register( QtConcurrent::run( this, &Download::Unpack ) );
    }

    // -----------------------------------------------------------------------------
    // Name: Download::~Download
    // Created: BAX 2012-09-21
    // -----------------------------------------------------------------------------
    ~Download()
    {
        async_.Join();
    }

    // -----------------------------------------------------------------------------
    // Name: Download::ReadHeaders
    // Created: BAX 2012-09-21
    // -----------------------------------------------------------------------------
    void ReadHeaders( QNetworkReply* rpy )
    {
        if( has_headers_ )
            return;
        has_headers_ = true;
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
            FileSystem_ABC::T_Unpacker unpacker = fs_.Unpack( root_, *this );
            unpacker->Unpack();
        }
        catch( ... )
        {
            // log me
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
            QThread::yieldCurrentThread();
        }
        Close();
    }

    // -----------------------------------------------------------------------------
    // Name: Download::OnFinished
    // Created: BAX 2012-09-24
    // -----------------------------------------------------------------------------
    void OnFinished()
    {
        QNetworkReply::NetworkError err = rpy_->error();
        if( err != QNetworkReply::NoError && err != QNetworkReply::OperationCanceledError )
        {
            // find a way to report errors
        }
        Finish();
        emit End( rpy_ );
    }

private:
    const FileSystem_ABC& fs_;
    const Path root_;
    mutable boost::mutex access_;
    boost::condition_variable gate_;
    std::vector< char > buffer_;
    QPointer< QNetworkReply > rpy_;
    size_t write_;
    size_t read_;
    size_t total_;
    bool has_headers_;
    bool eof_;
    QAsync async_;
};
}

// -----------------------------------------------------------------------------
// Name: gui::MakeDownload
// Created: BAX 2012-09-24
// -----------------------------------------------------------------------------
boost::shared_ptr< gui::Download_ABC > gui::MakeDownload( QNetworkReply* rpy, const runtime::FileSystem_ABC& fs, const Path& root )
{
    return boost::make_shared< Download >( rpy, fs, root );
}