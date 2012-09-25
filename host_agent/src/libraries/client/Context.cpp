// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Context.h"

#include "Download.h"
#include "Helpers.h"
#include "ItemModel.h"
#include "QAsync.h"

#include "runtime/FileSystem_ABC.h"
#include "runtime/PropertyTree.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Scoper.h"
#include "runtime/Utf8.h"

#include "host/Package.h"

#include <boost/make_shared.hpp>

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
    const QNetworkRequest::Attribute id_attribute = QNetworkRequest::Attribute( QNetworkRequest::User + 0 );
}

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
    connect( this, SIGNAL( NetworkRequest( HttpCommand, const QNetworkRequest& ) ), this, SLOT( OnNetworkRequest( HttpCommand, const QNetworkRequest& ) ) );
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
// Name: Head::OnNetworkRequest
// Created: BAX 2012-09-20
// -----------------------------------------------------------------------------
void Context::OnNetworkRequest( HttpCommand cmd, const QNetworkRequest& req )
{
    QNetworkReply* rpy = net_.get( req );
    switch( cmd )
    {
        case HTTP_CMD_GET_SESSION:
            connect( rpy, SIGNAL( finished() ), this, SLOT( OnGetSession() ) );
            break;

        case HTTP_CMD_DOWNLOAD_INSTALL:
            OpenDownload( rpy );
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
    size_t idx = size_t( ~0 );
    //AddItem( session, "binary" );
    AddItem( session, "model", idx );
    AddItem( session, "terrain", idx );
    AddItem( session, "exercise", idx );
    if( idx != -1 )
        return;

    emit ClearMessage();
    emit ClearProgress();
}

// -----------------------------------------------------------------------------
// Name: Head::AddItem
// Created: BAX 2012-09-20
// -----------------------------------------------------------------------------
void Context::AddItem( const Tree& src, const std::string& type, size_t& idx )
{
    const std::string name = Get< std::string >( src, type + ".name" );
    const std::string checksum = Get< std::string >( src, type + ".checksum" );
    const std::string node = Get< std::string >( src, "node" );
    Package::T_Item pkg = install_->Find( type, name, checksum );
    if( pkg )
        return;

    const int id = static_cast< int >( ++idx + (1<<30) );
    const QString qtype = QUtf8( type );
    const QString qname = QUtf8( name );
    const QString qchecksum = QUtf8( checksum );

    boost::shared_ptr< Item > item = boost::make_shared< Item >( static_cast< size_t >( id ), qtype, qname, qchecksum );
    items_.Append( item );

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
    req.setAttribute( id_attribute, id );
    emit NetworkRequest( HTTP_CMD_DOWNLOAD_INSTALL, req );
}

// -----------------------------------------------------------------------------
// Name: Context::OpenDownload
// Created: BAX 2012-09-24
// -----------------------------------------------------------------------------
void Context::OpenDownload( QNetworkReply* rpy )
{
    const size_t id = static_cast< size_t >( rpy->request().attribute( id_attribute ).toULongLong() );
    T_Download down = MakeDownload( id, rpy, fs_, root_ / tmp_dir );
    connect( down.get(), SIGNAL( Progress( size_t, size_t, int ) ), this, SLOT( OnDownloadProgress( size_t, size_t, int ) ) );
    connect( down.get(), SIGNAL( End( size_t ) ), this, SLOT( OnCloseDownload( size_t ) ) );
    QWriteLocker lock( &access_ );
    downloads_.insert( id, down );
}

// -----------------------------------------------------------------------------
// Name: Context::OnDownloadProgress
// Created: BAX 2012-09-25
// -----------------------------------------------------------------------------
void Context::OnDownloadProgress( size_t id, size_t current, int progress )
{
    QWriteLocker write( &access_ );
    QModelIndex idx = items_.Find( id );
    if( !idx.isValid() )
        return;
    items_.setData( idx.sibling( idx.row(), ITEM_COL_SIZE ), current, Qt::UserRole );
    items_.setData( idx.sibling( idx.row(), ITEM_COL_STATUS ), progress, Qt::UserRole );
}

// -----------------------------------------------------------------------------
// Name: Context::OnCloseDownload
// Created: BAX 2012-09-24
// -----------------------------------------------------------------------------
void Context::OnCloseDownload( size_t id )
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