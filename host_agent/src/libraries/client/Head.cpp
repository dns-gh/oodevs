// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Head.h"

#include "runtime/FileSystem_ABC.h"
#include "runtime/Runtime_ABC.h"
#include "host/Package.h"

#include <boost/filesystem/path.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include <QSettings>
#include <QtConcurrentRun>

using namespace gui;
using namespace runtime;
using host::Package;

namespace
{
    const std::wstring install_dir = L"_";
    const std::wstring tmp_dir     = L"tmp";

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

// -----------------------------------------------------------------------------
// Name: UnregisterProtocolHandler
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void UnregisterProtocolHandler( const QString& protocol )
{
    QSettings opt( "HKEY_CLASSES_ROOT\\" + protocol, QSettings::NativeFormat );
    opt.remove( "" );
}

Path MakePath( const QFileInfo& fi, const std::wstring& suffix )
{
    return fi.absoluteFilePath().toStdWString() + L"/" + suffix;
}
}

// -----------------------------------------------------------------------------
// Name: Head::Head
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
Head::Head( const Runtime_ABC& runtime, const FileSystem_ABC& fs, Pool_ABC& pool )
    : QMainWindow( 0 )
    , runtime_   ( runtime )
    , fs_        ( fs )
    , pool_      ( pool )
    , root_      ( QString::fromStdWString( runtime.GetModuleFilename().remove_filename().wstring().c_str() ) )
    , cmd_       ( CMD_DISPLAY )
    , items_     ()
    , async_     ()
    , io_async_  ( pool )
{
    ui_.setupUi( this );
    ui_.status->addPermanentWidget( &progress_ );
    ui_.status->addPermanentWidget( &count_ );
    ui_.items->setModel( &proxy_ );
    proxy_.setSourceModel( &items_ );
    proxy_.setDynamicSortFilter( true );
    proxy_.setFilterKeyColumn( -1 );
    proxy_.setFilterCaseSensitivity( Qt::CaseInsensitive );
    proxy_.setSortCaseSensitivity( Qt::CaseInsensitive );
    QObject::connect( this, SIGNAL( ProgressVisible( bool ) ), this, SLOT( OnProgressVisible( bool ) ) );
    QObject::connect( &items_, SIGNAL( modelReset() ), this, SLOT( OnModifiedItems() ) );
    QObject::connect( &items_, SIGNAL( rowsInserted( const QModelIndex&, int, int ) ), this, SLOT( OnModifiedItems() ) );
    QObject::connect( &items_, SIGNAL( rowsRemoved( const QModelIndex&, int, int ) ), this, SLOT( OnModifiedItems() ) );
    QObject::connect( ui_.filter, SIGNAL( textChanged( const QString& ) ), &proxy_, SLOT( setFilterFixedString( const QString& ) ) );
    QObject::connect( ui_.clear_filter, SIGNAL( clicked ( bool ) ), ui_.filter, SLOT( clear() ) );
    QObject::connect( ui_.toggle_items, SIGNAL( clicked( bool ) ), &items_, SLOT( Toggle() ) );
    QObject::connect( ui_.remove_items, SIGNAL( clicked( bool ) ), this, SLOT( OnRemove() ) );
    OnModifiedItems();

    LoadSettings();
    ParseArguments();

    fs_.MakePaths( MakePath( root_, install_dir ) );
    fs_.MakePaths( MakePath( root_, tmp_dir ) );
}

// -----------------------------------------------------------------------------
// Name: Head::~Head
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
Head::~Head()
{
    SaveSettings();
    async_.Join();
    io_async_.Join();
    fs_.Remove( MakePath( root_, tmp_dir ) );
}

// -----------------------------------------------------------------------------
// Name: Head::LoadSettings
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void Head::LoadSettings()
{
    QSettings opt;
    restoreGeometry( opt.value( "main_geometry" ).toByteArray() );
    restoreState( opt.value( "main_state" ).toByteArray() );
    ui_.items->header()->restoreState( opt.value( "items_header_state" ).toByteArray() );
}

// -----------------------------------------------------------------------------
// Name: Head::SaveSettings
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void Head::SaveSettings()
{
    QSettings opt;
    opt.setValue( "main_geometry", saveGeometry() );
    opt.setValue( "main_state", saveState() );
    opt.setValue( "items_header_state", ui_.items->header()->saveState() );
}

// -----------------------------------------------------------------------------
// Name: Head::ParseArguments
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void Head::ParseArguments()
{
    const QStringList list = QCoreApplication::arguments();
    for( QStringList::const_iterator it = list.begin() + 1; it != list.end(); ++it )
        if( *it == "--root" && ++it != list.end() )
        {
            root_ = *it;
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
        }
        else
        {
            throw std::runtime_error( "unknown parameter " + it->toStdString() );
        }
}

// -----------------------------------------------------------------------------
// Name: Head::ProcessCommand
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
bool Head::ProcessCommand()
{
    switch( cmd_ )
    {
        case CMD_REGISTER:
            Register();
            return true;

        case CMD_UNREGISTER:
            Unregister();
            return true;

        case CMD_RUN:
            throw std::runtime_error( "run not implemented" );
    }
    ui_.status->showMessage( "Loading package(s)..." );
    progress_.setRange( 0, 0 );
    progress_.setTextVisible( false );
    progress_.setVisible( true );
    async_.Register( QtConcurrent::run( this, &Head::ParseRoot ) );
    return false;
}

// -----------------------------------------------------------------------------
// Name: Head::Register
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void Head::Register()
{
    QString app = QString::fromStdWString( runtime_.GetModuleFilename().wstring() );
    QString cmd = QString( "\"%1\" \"%2\"" ).arg( app ).arg( "%1" );
    RegisterProtocolHandler( "sword", cmd );
}

// -----------------------------------------------------------------------------
// Name: Head::Unregister
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void Head::Unregister()
{
    UnregisterProtocolHandler( "sword" );
}

// -----------------------------------------------------------------------------
// Name: Head::ParseRoot
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void Head::ParseRoot()
{
    install_ = boost::make_shared< Package >( pool_, fs_, MakePath( root_, install_dir ), true );
    install_->Parse();
    items_.Fill( install_->GetProperties() );
    emit ProgressVisible( false );
}

// -----------------------------------------------------------------------------
// Name: Head::OnProgressVisible
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void Head::OnProgressVisible( bool visible )
{
    progress_.setVisible( visible );
    ui_.status->clearMessage();
}

// -----------------------------------------------------------------------------
// Name: Head::OnModifiedItems
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void Head::OnModifiedItems()
{
    const size_t count = items_.rowCount();
    count_.setText( QString( "%1 item%2" ).arg( count ).arg( count != 1 ? "s" : "" ) );
}

// -----------------------------------------------------------------------------
// Name: Head::OnRemove
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
void Head::OnRemove()
{
    const std::vector< size_t > removed = items_.Remove();
    install_->Uninstall( io_async_, MakePath( root_, tmp_dir ), removed );
}