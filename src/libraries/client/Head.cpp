// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Head.h"
#include "moc_Head.cpp"

#include "Context.h"
#include "ui_head.h"

#include <QMessageBox>
#include <QMetaType>
#include <QSettings>

#include <boost/function.hpp>

using namespace gui;
using namespace runtime;

// -----------------------------------------------------------------------------
// Name: Head::Head
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
Head::Head( const Runtime_ABC& runtime, const FileSystem_ABC& fs, Pool_ABC& pool )
    : QMainWindow( 0 )
    , ui_        ( new Ui::Head() )
    , async_     ( new runtime::Async( pool ) )
{
    qRegisterMetaType< size_t >( "size_t" );
    qRegisterMetaType< HttpCommand >( "HttpCommand" );

    ui_->setupUi( this );
    ui_->status->addPermanentWidget( &progress_ );
    progress_.setVisible( false );
    progress_.setTextVisible( false );
    ui_->status->addPermanentWidget( &count_ );
    items_.Setup( ui_->items );
    connect( &items_, SIGNAL( modelReset() ), this, SLOT( OnModifiedItems() ) );
    connect( &items_, SIGNAL( rowsInserted( const QModelIndex&, int, int ) ), this, SLOT( OnModifiedItems() ) );
    connect( &items_, SIGNAL( rowsRemoved( const QModelIndex&, int, int ) ), this, SLOT( OnModifiedItems() ) );
    connect( ui_->filter, SIGNAL( textChanged( const QString& ) ), items_.GetModel(), SLOT( setFilterFixedString( const QString& ) ) );
    connect( ui_->clear_filter, SIGNAL( clicked ( bool ) ), ui_->filter, SLOT( clear() ) );
    connect( ui_->toggle_items, SIGNAL( clicked( bool ) ), &items_, SLOT( Toggle() ) );
    OnModifiedItems();

    LoadSettings();
    ctx_.reset( new Context( runtime, fs, pool, *async_, items_ ) );
    connect( ctx_.get(), SIGNAL( StatusMessage( const QString& ) ), ui_->status, SLOT( showMessage( const QString& ) ) );
    connect( ctx_.get(), SIGNAL( ClearMessage() ), ui_->status, SLOT( clearMessage() ) );
    connect( ctx_.get(), SIGNAL( ShowProgress( int, int ) ), this, SLOT( OnShowProgress( int, int ) ) );
    connect( ctx_.get(), SIGNAL( ClearProgress() ), &progress_, SLOT( hide() ) );
    connect( ui_->remove_items, SIGNAL( clicked( bool ) ), ctx_.get(), SLOT( OnRemove() ) );
    connect( ctx_.get(), SIGNAL( Exit() ), this, SLOT( close() ) );
    connect( ctx_.get(), SIGNAL( Show() ), this, SLOT( show() ) );
    connect( ctx_.get(), SIGNAL( EnableEdition() ), this, SLOT( OnEnableEdition() ) );
    connect( ctx_.get(), SIGNAL( SingleInstanceError() ), this, SLOT( OnSingleInstanceError() ) );
    async_->Post( [&]{
        ctx_->Start();
    } );
}

// -----------------------------------------------------------------------------
// Name: Head::~Head
// Created: BAX 2012-09-06
// -----------------------------------------------------------------------------
Head::~Head()
{
    SaveSettings();
    async_->Join();
    ctx_.reset();
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
    ui_->items->header()->restoreState( opt.value( "items_header_state" ).toByteArray() );
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
    opt.setValue( "items_header_state", ui_->items->header()->saveState() );
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
// Name: Head::OnProgress
// Created: BAX 2012-09-20
// -----------------------------------------------------------------------------
void Head::OnShowProgress( int min, int max )
{
    progress_.setRange( min, max );
    progress_.show();
}

// -----------------------------------------------------------------------------
// Name: Head::OnEnableEdition
// Created: BAX 2012-10-04
// -----------------------------------------------------------------------------
void Head::OnEnableEdition()
{
    ui_->edit_controls->setEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: Head::OnSingleInstanceError
// Created: BAX 2012-10-04
// -----------------------------------------------------------------------------
void Head::OnSingleInstanceError()
{
    QMessageBox::critical( NULL, "Critical failure", "Another client is already running" );
}