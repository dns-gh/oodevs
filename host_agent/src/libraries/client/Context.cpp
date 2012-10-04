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

#include <boost/assign/list_of.hpp>
#include <boost/make_shared.hpp>

#include <QCoreApplication>
#include <QSettings>
#include <QStringList>
#include <QtConcurrentRun>

using namespace gui;
using namespace property_tree;
using runtime::FileSystem_ABC;
using runtime::Pool_ABC;
using runtime::Runtime_ABC;
using runtime::Utf8;
using host::Package;

namespace
{
    const std::string install_dir = "_"; // use short directory names until packages use small paths
    const std::string tmp_dir     = "0";
    const std::string trash_dir   = "1";
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
    async_.Join();
    io_.Join();

    const Path next = fs_.MakeAnyPath( root_ / trash_dir );
    fs_.Rename( root_ / tmp_dir, next / "_" );
    fs_.Remove( root_ / trash_dir );
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
    fs_.MakePaths( root_ / trash_dir );
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
    emit Show();
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
        install_ = boost::make_shared< Package >( pool_, fs_, root_ / install_dir, true );
        install_->Parse();
        items_.Fill( install_->GetProperties() );
    }
    if( cmd_ != CMD_RUN )
    {
        items_.SetEditable( true );
        emit EnableEdition();
        emit ClearProgress();
        emit ClearMessage();
        return;
    }
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
    install_->Uninstall( io_, root_ / trash_dir, removed );
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

        case HTTP_CMD_DOWNLOAD_ITEM:
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
    rpy->deleteLater();
    if( rpy->error() != QNetworkReply::NoError )
    {
        emit ClearProgress();
        emit StatusMessage( "Unable to download session (" + rpy->errorString() + ")" );
        return;
    }

    emit StatusMessage( "Downloading package(s)..." );
    async_.Register( QtConcurrent::run( this, &Context::ParseSession, rpy->readAll() ) );
}

// -----------------------------------------------------------------------------
// Name: Head::ParseSession
// Created: BAX 2012-09-20
// -----------------------------------------------------------------------------
void Context::ParseSession( const QByteArray& body )
{
    session_ = FromJson( QUtf8( body ) );
    QWriteLocker lock( &access_ );
    ApplySession();
}

// -----------------------------------------------------------------------------
// Name: Head::Apply
// Created: BAX 2012-10-02
// -----------------------------------------------------------------------------
void Context::ApplySession()
{
    size_t idx = size_t( ~0 );
    AddItem( session_, "client", idx );
    AddItem( session_, "model", idx );
    AddItem( session_, "terrain", idx );
    AddItem( session_, "exercise", idx );
    if( idx != -1 )
        return;

    StartClient();
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
    const QString qtype = QUtf8( type );
    Package::T_Item pkg = install_->Find( type, name, checksum );
    if( pkg )
    {
        links_.insert( qtype, pkg );
        return;
    }

    const int id = static_cast< int >( ++idx + (1<<30) );
    const QString qname = QUtf8( name );
    const QString qchecksum = QUtf8( checksum );

    boost::shared_ptr< Item > item = boost::make_shared< Item >( static_cast< size_t >( id ), qtype, qname, qchecksum );
    items_.Append( item );

    QUrl next = url_;
    next.setPath( "/api/download_client" );
    QList< QPair< QString, QString > > list;
    list.append( qMakePair( QString( "sid" ), url_.queryItemValue( "sid" ) ) );
    if( type != "client" )
    {
        next.setPath( "/api/download_install" );
        list.append( qMakePair( QString( "id" ),  QUtf8( node ) ) );
        list.append( qMakePair( QString( "type" ), qtype ) );
        list.append( qMakePair( QString( "name" ),  qname ) );
        list.append( qMakePair( QString( "checksum" ),  qchecksum ) );
    }
    next.setQueryItems( list );

    QNetworkRequest req( next );
    req.setAttribute( id_attribute, id );
    emit NetworkRequest( HTTP_CMD_DOWNLOAD_ITEM, req );
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

namespace
{
// -----------------------------------------------------------------------------
// Name: GetValue
// Created: BAX 2012-09-25
// -----------------------------------------------------------------------------
QVariant GetValue( const QModelIndex& idx, int col )
{
    return idx.sibling( idx.row(), col ).data( Qt::UserRole );
}

// -----------------------------------------------------------------------------
// Name: SetValue
// Created: BAX 2012-09-25
// -----------------------------------------------------------------------------
void SetValue( ItemModel& model, const QModelIndex& idx, int col, const QVariant& value )
{
    model.setData( idx.sibling( idx.row(), col ), value, Qt::UserRole );
}
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
    SetValue( items_, idx, ITEM_COL_SIZE, current );
    SetValue( items_, idx, ITEM_COL_STATUS, progress );
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
    async_.Register( QtConcurrent::run( this, &Context::ParsePackages ) );
    emit StatusMessage( "Parsing package(s)..." );
}

// -----------------------------------------------------------------------------
// Name: Context::ParsePackages
// Created: BAX 2012-09-25
// -----------------------------------------------------------------------------
void Context::ParsePackages()
{
    T_Package next = boost::make_shared< Package >( pool_, fs_, root_ / tmp_dir, true );
    const bool valid = next->Parse();
    if( !valid )
    {
        emit ClearProgress();
        emit StatusMessage( "Unable to parse downloaded package(s)" );
        return;
    }

    Package::T_Items targets;
    for( int row = 0; row < items_.rowCount(); ++row )
    {
        QModelIndex idx = items_.index( row, 0 );
        if( !items_.IsRequired( idx ) )
            continue;
        const QString type = GetValue( idx, ITEM_COL_TYPE ).toString();
        const QString name = GetValue( idx, ITEM_COL_NAME ).toString();
        const QString sum  = GetValue( idx, ITEM_COL_CHECKSUM ).toString();
        const Package::T_Item it = next->Find( QUtf8( type ), QUtf8( name ), QUtf8( sum ) );
        if( !it )
        {
            emit ClearProgress();
            emit StatusMessage( QString( "Missing download %1 %2 0x%3" ).arg( type ).arg( name ).arg( sum ) );
            return;
        }
        targets.push_back( it );
        const Tree src = next->GetPropertiesFrom( *it );
        items_.Replace( idx.row(), boost::make_shared< Item >( src, 100 ) );
    }

    emit StatusMessage( "Installing package(s)" );
    QWriteLocker write( &access_ );
    install_->InstallWith( io_, root_, targets, true );
    ApplySession();
}

namespace
{
// -----------------------------------------------------------------------------
// Name: MakeOption
// Created: BAX 2012-10-02
// -----------------------------------------------------------------------------
template< typename T >
std::string MakeOption( const std::string& option, const T& value )
{
    return "--" + option + " \"" + boost::lexical_cast< std::string >( value ) + "\"";
}

// -----------------------------------------------------------------------------
// Name: WriteConfiguration
// Created: BAX 2012-10-02
// -----------------------------------------------------------------------------
void WriteConfiguration( const FileSystem_ABC& fs, Path root,
                         const std::string& host, int port )
{
    root /= "sessions";
    fs.MakePaths( root );
    Tree data;
    data.put( "session.config.gaming.network.<xmlattr>.server", host + ":" + boost::lexical_cast< std::string >( port ) );
    fs.WriteFile( root / "session.xml", ToXml( data ) );
}
}

// -----------------------------------------------------------------------------
// Name: GetPath
// Created: BAX 2012-10-02
// -----------------------------------------------------------------------------
Path Context::GetPath( const QString& type )
{
    T_Links::const_iterator it = links_.find( type );
    if( it == links_.end() )
    {
        emit StatusMessage( "Missing package of type " + type );
        emit ClearProgress();
        return Path();
    }
    return install_->GetRoot( *it.value() );
}

// -----------------------------------------------------------------------------
// Name: Context::StartClient
// Created: BAX 2012-10-02
// -----------------------------------------------------------------------------
void Context::StartClient()
{
    const Path client = GetPath( "client" );
    const Path model = GetPath( "model" );
    const Path terrain = GetPath( "terrain" );
    const Path exercise = GetPath( "exercise" );
    if( client.empty() || model.empty() || terrain.empty() || exercise.empty() )
        return;
    const Path name = Utf8( Get< std::string >( session_, "exercise.name" ) );
    WriteConfiguration( fs_, exercise / "exercises" / name, QUtf8( url_.host() ), Get< int >( session_, "port" ) );
    std::vector< std::string > args = boost::assign::list_of
        ( MakeOption( "models-dir", Utf8( model / "data/models" ) ) )
        ( MakeOption( "terrains-dir", Utf8( terrain / "data/terrains" ) ) )
        ( MakeOption( "exercises-dir", Utf8( exercise / "exercises" ) ) )
        ( MakeOption( "exercise", Utf8( name ) ) );
    try
    {
        runtime_.Start( Utf8( client / "gaming_app.exe" ), args, Utf8( client ), std::string() );
        emit Exit();
    }
    catch( const std::exception& err )
    {
        emit StatusMessage( QString( "Unable to start client (%1)" ).arg( err.what() ) );
    }
}