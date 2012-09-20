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
#include "runtime/PropertyTree.h"
#include "runtime/Runtime_ABC.h"
#include "runtime/Utf8.h"

#include "host/Package.h"

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
}

// -----------------------------------------------------------------------------
// Name: Context::Context
// Created: BAX 2012-09-20
// -----------------------------------------------------------------------------
Context::Context( const Runtime_ABC& runtime, const FileSystem_ABC& fs, Pool_ABC& pool,
                  QAsync& async, ItemModel& items )
    : runtime_   ( runtime )
    , fs_        ( fs )
    , pool_      ( pool )
    , items_     ( items )
    , cmd_       ( CMD_DISPLAY )
    , root_      ( runtime.GetModuleFilename().remove_filename() )
    , url_       ()
    , async_     ( async )
    , io_        ( pool )
    , install_   ()
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
            root_ = Utf8Convert( Utf8( *it ) );
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
            {
                emit StatusMessage( "Invalid URL scheme" );
                return;
            }
            if( !url_.hasQueryItem( "protocol" ) )
            {
                emit StatusMessage( "Missing protocol parameter" );
                return;
            }
            QString protocol = url_.queryItemValue( "protocol" );
            if( protocol.endsWith( ':' ) )
                protocol.chop( 1 );
            url_.setScheme( protocol );
            if( !url_.hasQueryItem( "sid" ) )
            {
                emit StatusMessage( "Missing SID parameter" );
                return;
            }
            if( !url_.hasQueryItem( "session" ) )
            {
                emit StatusMessage( "Missing session ID parameter" );
                return;
            }
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
    emit Progress( true, 0, 0 );
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
    QString app = Utf8( Utf8Convert( runtime_.GetModuleFilename() ) );
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
        QMutexLocker lock( &access_ );
        install_.reset( new Package( pool_, fs_, root_ / install_dir, true ) );
        install_->Parse();
        items_.Fill( install_->GetProperties() );
    }
    if( cmd_ != CMD_RUN )
        emit Progress( false, 0, 0 );
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
    QMutexLocker lock( &access_ );
    install_->Uninstall( io_, root_ / tmp_dir, removed );
}

// -----------------------------------------------------------------------------
// Name: Context::GetSession
// Created: BAX 2012-09-20
// -----------------------------------------------------------------------------
void Context::GetSession()
{
    emit StatusMessage( "Downloading session..." );
    emit Progress( true, 0, 0 );

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
        emit Progress( false, 0, 0 );
        emit StatusMessage( "Unable to download session (" + rpy->errorString() + ")" );
        return;
    }

    emit Progress( false , 0, 0 );
    emit StatusMessage( QString() );
    async_.Register( QtConcurrent::run( this, &Context::ParseSession, rpy ) );
}

// -----------------------------------------------------------------------------
// Name: Head::ParseSession
// Created: BAX 2012-09-20
// -----------------------------------------------------------------------------
void Context::ParseSession( QNetworkReply* rpy )
{
    const Tree session = FromJson( Utf8( rpy->readAll() ) );
    qDebug() << Utf8( ToJson( session, true ) );
}