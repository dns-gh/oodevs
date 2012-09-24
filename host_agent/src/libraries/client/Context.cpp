// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Context.h"

#include "Helpers.h"
#include "ItemModel.h"
#include "QAsync.h"

#include "runtime/FileSystem_ABC.h"
#include "runtime/Io.h"
#include "runtime/PropertyTree.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Scoper.h"
#include "runtime/Utf8.h"

#include "host/Package.h"

#include <boost/make_shared.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

#include <QCoreApplication>
#include <QSettings>
#include <QStringList>
#include <QtConcurrentRun>

using namespace gui;
using namespace property_tree;
using namespace runtime;
using host::Package;

namespace
{
    const std::string install_dir = "_";
    const std::string tmp_dir     = "tmp";
    const size_t      buffer_size = 1<<20;
}

struct gui::Download : public io::Reader_ABC
{
    // -----------------------------------------------------------------------------
    // Name: Download::Download
    // Created: BAX 2012-09-21
    // -----------------------------------------------------------------------------
    Download( const FileSystem_ABC& fs, const Path& root, const QString& name )
        : buffer_( buffer_size )
        , root_  ( fs.MakeAnyPath( root / tmp_dir ) )
        , name_  ( name )
        , eof_   ( false )
        , write_ ( 0 )
        , read_  ( 0 )
        , reply_ ( 0 )
    {
        // NOTHING
    }

    // -----------------------------------------------------------------------------
    // Name: Download::~Download
    // Created: BAX 2012-09-21
    // -----------------------------------------------------------------------------
    ~Download()
    {
        // NOTHING
    }

    // -----------------------------------------------------------------------------
    // Name: Download::Write
    // Created: BAX 2012-09-21
    // -----------------------------------------------------------------------------
    void Write( QNetworkReply* rpy )
    {
        boost::lock_guard< boost::mutex > lock( access_ );
        if( eof_ )
            return;
        const qint64 avail = rpy->bytesAvailable();
        reply_ = rpy;
        const qint64 left = buffer_size - write_;
        const size_t next = std::min( left, avail );
        const qint64 len  = next ? rpy->read( &buffer_[write_], next ) : 0;
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
        if( read_ == write_ )
        {
            read_  = 0;
            write_ = 0;
            QMetaObject::invokeMethod( reply_, "readyRead" );
        }
        return static_cast< int >( next );
    }

    // -----------------------------------------------------------------------------
    // Name: Download::GetRoot
    // Created: BAX 2012-09-21
    // -----------------------------------------------------------------------------
    const Path& GetRoot() const
    {
        return root_;
    }

    // -----------------------------------------------------------------------------
    // Name: Download::GetName
    // Created: BAX 2012-09-21
    // -----------------------------------------------------------------------------
    const QString& GetName() const
    {
        return name_;
    }

    // -----------------------------------------------------------------------------
    // Name: Download::GetReply
    // Created: BAX 2012-09-21
    // -----------------------------------------------------------------------------
    const QPointer< QNetworkReply > GetReply() const
    {
        return reply_;
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
    // Name: Download::Finish
    // Created: BAX 2012-09-24
    // -----------------------------------------------------------------------------
    bool Finish( QNetworkReply* rpy )
    {
        if( rpy->bytesAvailable() <= 0 )
            return true;
        boost::lock_guard< boost::mutex > lock( access_ );
        return eof_;
    }

private:
    const Path root_;
    const QString name_;
    boost::mutex access_;
    std::vector< char > buffer_;
    size_t write_;
    size_t read_;
    bool eof_;
    boost::condition_variable gate_;
    QPointer< QNetworkReply > reply_;
};

// -----------------------------------------------------------------------------
// Name: Context::Context
// Created: BAX 2012-09-20
// -----------------------------------------------------------------------------
Context::Context( const Runtime_ABC& runtime, const FileSystem_ABC& fs, Pool_ABC& pool,
                  QAsync& async, ItemModel& items )
    : runtime_  ( runtime )
    , fs_       ( fs )
    , pool_     ( pool )
    , items_    ( items )
    , cmd_      ( CMD_DISPLAY )
    , root_     ( runtime.GetModuleFilename().remove_filename() )
    , url_      ()
    , async_    ( async )
    , io_       ( pool )
    , install_  ()
    , downloads_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Context::~Context
// Created: BAX 2012-09-20
// -----------------------------------------------------------------------------
Context::~Context()
{
    io_.Join();
    fs_.Remove( root_ / tmp_dir );
}

// -----------------------------------------------------------------------------
// Name: Context::Start
// Created: BAX 2012-09-20
// -----------------------------------------------------------------------------
void Context::Start()
{
    ParseArguments();
    fs_.MakePaths( root_ / install_dir );
    fs_.MakePaths( root_ / tmp_dir );
    ProcessCommand();
}

#define RETURN_STATUS( ... ) do {\
    emit StatusMessage( __VA_ARGS__ );\
    return;\
} while( 0 )

// -----------------------------------------------------------------------------
// Name: Context::ParseArguments
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void Context::ParseArguments()
{
    const QStringList list = QCoreApplication::arguments();
    for( QStringList::const_iterator it = list.begin() + 1; it != list.end(); ++it )
        if( *it == "--root" && ++it != list.end() )
        {
            root_ = Utf8( QUtf8( *it ) );
        }
        else if( *it == "--register" )
        {
            cmd_ = CMD_REGISTER;
        }
        else if( *it == "--unregister" )
        {
            cmd_ = CMD_UNREGISTER;
        }
        else if( *it == "--run" && ++it != list.end() )
        {
            cmd_ = CMD_RUN;
            url_ = *it;
            if( url_.scheme() != "sword" )
                RETURN_STATUS( "Invalid URL scheme" );
            if( !url_.hasQueryItem( "protocol" ) )
                RETURN_STATUS( "Missing protocol parameter" );
            QString protocol = url_.queryItemValue( "protocol" );
            if( protocol.endsWith( ':' ) )
                protocol.chop( 1 );
            url_.setScheme( protocol );
            if( !url_.hasQueryItem( "sid" ) )
                RETURN_STATUS( "Missing SID parameter" );
            if( !url_.hasQueryItem( "session" ) )
                RETURN_STATUS( "Missing session ID parameter" );
        }
        else
        {
            RETURN_STATUS( "Unknown parameter " + *it );
        }
}

// -----------------------------------------------------------------------------
// Name: Context::ProcessCommand
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void Context::ProcessCommand()
{
    switch( cmd_ )
    {
        case CMD_REGISTER:
            Register();
            emit Exit();
            return;

        case CMD_UNREGISTER:
            Unregister();
            emit Exit();
            return;
    }
    emit StatusMessage( "Loading cache..." );
    emit ShowProgress( 0, 0 );
    ParseRoot();
}

namespace
{
// -----------------------------------------------------------------------------
// Name: RegisterProtocolHandler
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void RegisterProtocolHandler( const QString& protocol, const QString& cmd )
{
    QSettings opt( "HKEY_CLASSES_ROOT\\" + protocol, QSettings::NativeFormat );
    opt.setValue( ".", "URL:" + protocol + " Protocol" );
    opt.setValue( "URL Protocol", "" );
    QSettings open( "HKEY_CLASSES_ROOT\\" + protocol + "\\shell\\open\\command", QSettings::NativeFormat );
    open.setValue( ".", cmd );
}
}

// -----------------------------------------------------------------------------
// Name: Context::Register
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void Context::Register()
{
    QString app = QUtf8( Utf8( runtime_.GetModuleFilename() ) );
    QString cmd = QString( "\"%1\" --run \"%2\"" ).arg( app ).arg( "%1" );
    RegisterProtocolHandler( "sword", cmd );
}

namespace
{
// -----------------------------------------------------------------------------
// Name: UnregisterProtocolHandler
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void UnregisterProtocolHandler( const QString& protocol )
{
    QSettings opt( "HKEY_CLASSES_ROOT\\" + protocol, QSettings::NativeFormat );
    opt.remove( "" );
}
}

// -----------------------------------------------------------------------------
// Name: Context::Unregister
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void Context::Unregister()
{
    UnregisterProtocolHandler( "sword" );
}

// -----------------------------------------------------------------------------
// Name: Context::ParseRoot
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void Context::ParseRoot()
{
    {
        QWriteLocker lock( &access_ );
        install_.reset( new Package( pool_, fs_, root_ / install_dir, true ) );
        install_->Parse();
        items_.Fill( install_->GetProperties() );
    }
    if( cmd_ != CMD_RUN )
        emit ClearProgress();
    else
        GetSession();
}

// -----------------------------------------------------------------------------
// Name: Context::OnRemove
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void Context::OnRemove()
{
    const std::vector< size_t > removed = items_.Remove();
    QWriteLocker lock( &access_ );
    install_->Uninstall( io_, root_ / tmp_dir, removed );
}

// -----------------------------------------------------------------------------
// Name: Context::GetSession
// Created: BAX 2012-09-20
// -----------------------------------------------------------------------------
void Context::GetSession()
{
    emit StatusMessage( "Downloading session..." );

    QUrl next = url_;
    next.setPath( "/api/get_session" );
    QList< QPair< QString, QString > > list;
    list.append( qMakePair( QString( "sid" ), url_.queryItemValue( "sid" ) ) );
    list.append( qMakePair( QString( "id" ),  url_.queryItemValue( "session" ) ) );
    next.setQueryItems( list );
    emit NetworkRequest( HTTP_CMD_GET_SESSION, QNetworkRequest( next ) );
}

// -----------------------------------------------------------------------------
// Name: Context::SetNetworkReply
// Created: BAX 2012-09-20
// -----------------------------------------------------------------------------
void Context::SetNetworkReply( HttpCommand cmd, QNetworkReply* rpy )
{
    switch( cmd )
    {
        case HTTP_CMD_GET_SESSION:
            connect( rpy, SIGNAL( finished() ), this, SLOT( OnGetSession() ) );
            break;

        case HTTP_CMD_DOWNLOAD_INSTALL:
            rpy->setReadBufferSize( buffer_size );
            connect( rpy, SIGNAL( readyRead() ), this, SLOT( OnDownloadRead() ) );
            connect( rpy, SIGNAL( error( QNetworkReply::NetworkError ) ), this, SLOT( OnDownloadEnd() ) );
            connect( rpy, SIGNAL( finished() ), this, SLOT( OnDownloadEnd() ) );
            break;
    }
}

// -----------------------------------------------------------------------------
// Name: Context::OnGetSession
// Created: BAX 2012-09-20
// -----------------------------------------------------------------------------
void Context::OnGetSession()
{
    QNetworkReply* rpy = qobject_cast< QNetworkReply* >( sender() );
    if( rpy->error() != QNetworkReply::NoError )
    {
        emit ClearProgress();
        emit StatusMessage( "Unable to download session (" + rpy->errorString() + ")" );
        return;
    }

    emit StatusMessage( "Downloading package(s)..." );
    async_.Register( QtConcurrent::run( this, &Context::ParseSession, rpy ) );
}

// -----------------------------------------------------------------------------
// Name: Head::ParseSession
// Created: BAX 2012-09-20
// -----------------------------------------------------------------------------
void Context::ParseSession( QNetworkReply* rpy )
{
    const Tree session = FromJson( QUtf8( rpy->readAll() ) );
    QWriteLocker lock( &access_ );
    //qDebug() << QUtf8( ToJson( session, true ) );
    //AddItem( session, "binary" );
    AddItem( session, "model" );
    AddItem( session, "terrain" );
    AddItem( session, "exercise" );
    if( !downloads_.empty() )
        return;

    emit ClearMessage();
    emit ClearProgress();
}

// -----------------------------------------------------------------------------
// Name: Head::AddItem
// Created: BAX 2012-09-20
// -----------------------------------------------------------------------------
void Context::AddItem( const Tree& src, const std::string& type )
{
    const std::string name = Get< std::string >( src, type + ".name" );
    const std::string checksum = Get< std::string >( src, type + ".checksum" );
    const std::string node = Get< std::string >( src, "node" );
    Package::T_Item pkg = install_->Find( type, name, checksum );
    if( pkg )
        return;

    const int id = downloads_.size() + (1<<30);
    const QString qtype = QUtf8( type );
    const QString qname = QUtf8( name );
    const QString qchecksum = QUtf8( checksum );

    boost::shared_ptr< Item > item = boost::make_shared< Item >( static_cast< size_t >( id ), qtype, qname, qchecksum );
    items_.Append( item );
    T_Download down = boost::make_shared< Download >( fs_, root_, qname );
    downloads_.insert( id, down );

    QUrl next = url_;
    next.setPath( "/api/download_install" );
    QList< QPair< QString, QString > > list;
    list.append( qMakePair( QString( "sid" ), url_.queryItemValue( "sid" ) ) );
    list.append( qMakePair( QString( "id" ),  QUtf8( node ) ) );
    list.append( qMakePair( QString( "type" ), qtype ) );
    list.append( qMakePair( QString( "name" ),  qname ) );
    list.append( qMakePair( QString( "checksum" ),  qchecksum ) );
    next.setQueryItems( list );

    QNetworkRequest req( next );
    req.setAttribute( QNetworkRequest::User, id );
    emit NetworkRequest( HTTP_CMD_DOWNLOAD_INSTALL, req );

    async_.Register( QtConcurrent::run( this, &Context::Unpack, down ) );
}

// -----------------------------------------------------------------------------
// Name: Context::OnDownloadRead
// Created: BAX 2012-09-21
// -----------------------------------------------------------------------------
void Context::OnDownloadRead()
{
    QNetworkReply* rpy = qobject_cast< QNetworkReply* >( sender() );
    const int id = rpy->request().attribute( QNetworkRequest::User ).toInt();

    T_Download down;
    {
        QReadLocker read( &access_ );
        T_Downloads::iterator it = downloads_.find( id );
        if( it == downloads_.end() )
            return;
        down = *it;
    }

    down->Write( rpy );
}

// -----------------------------------------------------------------------------
// Name: Context::Unpack
// Created: BAX 2012-09-21
// -----------------------------------------------------------------------------
void Context::Unpack( T_Download down )
{
    try
    {
        FileSystem_ABC::T_Unpacker unpacker = fs_.Unpack( down->GetRoot(), *down );
        unpacker->Unpack();
        down->Close();
    }
    catch( std::exception& err )
    {
        emit StatusMessage( QString( "Unable to download package %1 (%2)" ).arg( down->GetName() ).arg( err.what() ) );
    }
    emit CheckAbort( down->GetReply() );
}

// -----------------------------------------------------------------------------
// Name: Context::OnDownloadInstall
// Created: BAX 2012-09-21
// -----------------------------------------------------------------------------
void Context::OnDownloadEnd()
{
    QNetworkReply* rpy = qobject_cast< QNetworkReply* >( sender() );
    const int id = rpy->request().attribute( QNetworkRequest::User ).toInt();
    rpy->deleteLater();

    T_Download down;
    {
        QReadLocker read( &access_ );
        T_Downloads::iterator it = downloads_.find( id );
        if( it == downloads_.end() )
            return;
        down = *it;
    }

    runtime::Scoper finish = boost::bind( &Context::FinishDownload, this, id );
    QNetworkReply::NetworkError err = rpy->error();
    if( err != QNetworkReply::NoError )
    {
        QString error = err == QNetworkReply::OperationCanceledError ? "Extraction aborted" : rpy->errorString();
        // find another way to report errors...
        emit StatusMessage( QString( "Unable to download package %1 (%2)" ).arg( down->GetName() ).arg( error ) );
        return;
    }

    while( !down->Finish( rpy ) )
    {
        down->Write( rpy );
        QCoreApplication::processEvents();
        QThread::yieldCurrentThread();
    }
    down->Close();
}

// -----------------------------------------------------------------------------
// Name: Context::FinishDownload
// Created: BAX 2012-09-24
// -----------------------------------------------------------------------------
void Context::FinishDownload( int id )
{
    QWriteLocker write( &access_ );
    T_Downloads::iterator it = downloads_.find( id );
    if( it == downloads_.end() )
        return;
    downloads_.erase( it );
    if( !downloads_.empty() )
        return;
    emit ClearProgress();
    emit ClearMessage();
}
